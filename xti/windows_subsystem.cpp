// xti keyboard
// Copyright © Jordan Singh
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "windows_subsystem.h"

// 1. Qt framework headers
// 2. System/OS headers
#include <Psapi.h>
#include <shellapi.h>
#include <dwmapi.h>
// 3. C++ standard library headers
#include <stdexcept>
#include <memory>
#include <cctype>
#include <algorithm>
// 4. Project classes

// --- initialize_apply_keyboard_window_style(): Tells windows to apply for keyboard native window styling.
// ----- window: HWND of the Qt app.
// --------------------------------------------------------------------------------------------/
/* public */ void windows_subsystem::initialize_apply_keyboard_window_style(HWND window)
{
    // If the main app window does not call this function, then the keyboard can still take foreground focus away from others
    // despite setting Qt::WindowDoesNotAcceptFocus at startup.
    int64_t r = ::GetWindowLongPtrW(window, GWL_EXSTYLE);
    if (r == 0)
    {
        throw std::runtime_error("Failure on GetWindowLongPtr()");
    }
    r = ::SetWindowLongPtrW(window, GWL_EXSTYLE, r | WS_EX_NOACTIVATE | WS_EX_TOPMOST);
    if (r == 0)
    {
        throw std::runtime_error("Failure on SetWindowLongPtrW()");
    }
}

// --- initialize_force_cursor_visible(): Forces the cursor to be visible even in tablet mode contexts.
// --------------------------------------------------------------------------------------------/
/* public */ void windows_subsystem::initialize_force_cursor_visible()
{
    // TODO - validate this
    ::HKEY hKey;
    wchar_t subKey[] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";
    wchar_t valueName[] = L"EnableCursorSuppression";
    uint32_t valueData = 0; // DWORD value - 0 = Force cursor to stay visible
    int32_t r = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, subKey, 0, KEY_SET_VALUE, &hKey);
    if (r != ERROR_SUCCESS)
    {
        throw std::runtime_error("Failure on RegOpenKeyExW()");
    }
    r = ::RegSetValueExW(hKey, valueName, 0, REG_DWORD, reinterpret_cast<BYTE*>(&valueData), sizeof(valueData));
    if (r != ERROR_SUCCESS)
    {
        throw std::runtime_error("Failure on RegSetValueExW()");
    }
    r = ::RegCloseKey(hKey);
    if (r != ERROR_SUCCESS)
    {
        throw std::runtime_error("Failure on RegCloseKey()");
    }
}

// --- initialize_orientate_main_window(): Moves the main QT window into position.
// ----- window: HWND of the Qt app.
// ------- returns: app dimensions to be used later on for re-positioning other windows.
// ------------------------------------------------------------------------/
/* public */ app_dimensions windows_subsystem::initialize_orientate_main_window(HWND window)
{
    RECT size;
    int32_t r = ::GetWindowRect(window, &size);
    if (r == 0)
    {
        throw std::runtime_error("Failure on GetWindowRect()");
    }
    int32_t baseHeight = size.bottom - size.top;
    r = ::SystemParametersInfoW(SPI_GETWORKAREA, 0, &size, 0);
    if (r == 0)
    {
        throw std::runtime_error("Failure on SystemParametersInfoW()");
    }
    int32_t workingHeight = size.bottom;
    int32_t workingWidth = size.right;
    r = ::SetWindowPos(window, nullptr, 0, (workingHeight / 2) - (baseHeight / 2), workingWidth, baseHeight, 0);
    if (r == 0)
    {
        throw std::runtime_error("Failure on SetWindowPos()");
    }
    app_dimensions out;
    out.dimensionsAvailableScreenWidth = workingWidth;
    out.dimensionsAboveYEnd = (workingHeight / 2) - (baseHeight / 2);
    out.dimensionsBelowYStart = (workingHeight / 2) - (baseHeight / 2) + baseHeight;
    out.dimensionsBelowYEnd = workingHeight;
    return out;
}

// --- initialize_apply_system_super_admin_privilege(): Tells windows to apply for super admin privileges.
// --------------------------------------------------------------------------------------------/
/* public */ void windows_subsystem::initialize_apply_system_super_admin_privilege()
{
    // Even though we run the app as Admin modern windows kernel will not
    // grant certain privileges unless we ask for it directly.
    // Based on how the app currently operates and hooks into the OS
    // this is not needed at this time or called at startup. This was really only used for debugging.
    HANDLE processTokenHandle;
    int32_t r = ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &processTokenHandle);
    if (r == 0)
    {
        throw std::runtime_error("Failure on OpenProcessToken()");
    }

    ::LUID privilegeId;
    r = ::LookupPrivilegeValueW(nullptr, SE_DEBUG_NAME /* Apply for everything */, &privilegeId);
    if (r == 0)
    {
        throw std::runtime_error("Failure on LookupPrivilegeValueW()");
    }

    ::TOKEN_PRIVILEGES tokenPrivilegesRequest;
    tokenPrivilegesRequest.PrivilegeCount = 1;
    tokenPrivilegesRequest.Privileges[0].Luid = privilegeId;
    tokenPrivilegesRequest.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    r = ::AdjustTokenPrivileges(processTokenHandle, false, &tokenPrivilegesRequest, sizeof(TOKEN_PRIVILEGES), nullptr, nullptr);
    if (r == 0)
    {
        throw std::runtime_error("Failure on LookupPrivilegeValueW()");
    }
    // edge case: need to check GetLastError to ensure not ERROR_NOT_ALL_ASSIGNED
    uint32_t errorCode = ::GetLastError();
    if (errorCode != ERROR_SUCCESS)
    {
        throw std::runtime_error("Failure on LookupPrivilegeValueW()");
    }
}

// --- move_active_window(): Moves the current active foreground window above or below the xti keyboard.
// ----- above: True if to open above the xti keyboard, false if move below the xti keyboard.
// ----- appDimensions: Where windows should be placed in the desktop.
// --------------------------------------------------------------------------------------/
/* public */ void windows_subsystem::move_active_window(bool above, const app_dimensions& appDimensions)
{
    HWND window = ::GetForegroundWindow();
    if (window == nullptr) {
        return;
    }
    move_window(window, above, appDimensions);
}

// --- start_process(): Starts a new process and positioning either above or below the xti keyboard.
// ----- exePath: absolute file path of the executable
// ----- workingDirectory: absolute working directory to run it under
// ----- above: True if to open above the xti keyboard, false if move below the xti keyboard.
// ----- appDimensions: Where windows should be placed in the desktop.
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------/
/* public */ void windows_subsystem::start_process(const std::wstring& exePath, const std::wstring& workingDirectory, bool above, const app_dimensions& appDimensions)
{
    HINSTANCE instance = ::ShellExecuteW(nullptr, L"open", exePath.c_str(), nullptr, workingDirectory.c_str(), SW_SHOWNORMAL);
    if (instance == nullptr)
    {
        throw std::runtime_error("Failure on ShellExecuteW()");
    }
    // Wait 500 ms for any other application initialization logic.
    ::Sleep(500);
    size_t find = exePath.find_last_of('\\');
    if (find == std::wstring::npos)
    {
        throw std::runtime_error("exePath is not absolute");
    }
    std::wstring exeOnly = exePath.substr(find + 1);
    HWND window = get_window(exeOnly, L"");
    if (window == nullptr)
    {
        // Either application is reallly slow, or didnt open with an expected GUI window.
        return;
    }
    move_window(window, above, appDimensions);
}

// --- is_process_running(): Determines if a process is running within the system.
// ------- returns: true if found, false if not
// ---------------------------------------------------------------------------------------/
/* public */ bool windows_subsystem::is_process_running(const std::wstring& processName)
{
    uint32_t processesArray[1024];
    uint32_t needed;
    int32_t r;
    r = ::EnumProcesses(reinterpret_cast<::DWORD*>(processesArray), sizeof(processesArray), reinterpret_cast<::DWORD*>(&needed));
    if (r == 0)
    {
        throw std::runtime_error("Failure on EnumProcesses()");
    }
    uint32_t processCount = needed / sizeof(uint32_t);
    if (processCount == 1024)
    {
        throw std::runtime_error("Failure on EnumProcesses() -> overflow");
    }
    for (size_t i = 0; i < processCount; i++)
    {
        if (processesArray[i] == 0) {
            continue; // skip kernel, the get_exe_name_from_process_id fails on 0.
        }
        std::wstring processNameRunning = get_exe_name_from_process_id(processesArray[i]);
        std::wstring processNameCopy = processName;
        std::transform(processNameCopy.begin(), processNameCopy.end(), processNameCopy.begin(), ::toupper);
        if (processNameRunning == processNameCopy)
        {
            return true;
        }
    }
    return false;
}

// --- get_window(): Get a window based on specific underlying exe name and title.
// ----- runningExe: exe name (with extension).
// ----- requiredTitleContains: Text that the window title must contain, empty means any title (only match exe name).
// ------- returns: the found HWND, or nulptr if not found.
// --------------------------------------------------------------------------------------------------------------------/
/* public */ HWND windows_subsystem::get_window(const std::wstring& runningExe, const std::wstring& requiredTitleContains)
{
    enumWindowProcExeName = runningExe;
    std::transform(enumWindowProcExeName.begin(), enumWindowProcExeName.end(), enumWindowProcExeName.begin(), ::toupper);
    enumWindowProcTitleContains = requiredTitleContains;
    enumWindowProcHwndOut = nullptr;
    ::SetLastError(ERROR_SUCCESS);
    ::EnumDesktopWindows(nullptr, enum_window_proc, 0); // throwing away return value here, can return 0 once enumeration stops.
    uint32_t errCode =  ::GetLastError();
    if (errCode != ERROR_SUCCESS)
    {
        throw std::runtime_error("Failure on EnumDesktopWindows()");
    }
    return enumWindowProcHwndOut;
}

/* private */ thread_local std::wstring windows_subsystem::enumWindowProcExeName;
/* private */ thread_local std::wstring windows_subsystem::enumWindowProcTitleContains;
/* private */ thread_local HWND windows_subsystem::enumWindowProcHwndOut;
/* private */ int32_t __stdcall windows_subsystem::enum_window_proc(HWND window, [[maybe_unused]] int64_t param)
{
    uint32_t processId;
    int32_t r = ::GetWindowThreadProcessId(window, reinterpret_cast<::DWORD*>(&processId));
    if (r == 0)
    {
        throw std::runtime_error("Failure on GetWindowThreadProcessId()");
    }
    std::wstring exeName = get_exe_name_from_process_id(processId);
    if (enumWindowProcExeName == exeName)
    {
        // We skip windows that are not visible.
        r = ::IsWindowVisible(window);
        if (r == 0)
        {
            return true;
        }

        int32_t windowTitleLength = ::GetWindowTextLengthW(window);
        // error response only available with GetLastError rather than return of GetWindowTextLengthW here.
        uint32_t errCode = ::GetLastError();
        if (errCode != ERROR_SUCCESS)
        {
            throw std::runtime_error("Failure on GetWindowTextLengthW()");
        }
        std::unique_ptr<wchar_t[]> windowTitle = std::make_unique<wchar_t[]>(windowTitleLength + 1);
        ::SetLastError(ERROR_SUCCESS);
        r = ::GetWindowTextW(window, windowTitle.get(), windowTitleLength + 1);
        if (r == 0)
        {
            errCode = ::GetLastError();
            if (errCode != ERROR_SUCCESS)
            {
                throw std::runtime_error("Failure on GetWindowTextW()");
            }
            // We skip windows with no text or any other reason
            // that prevents us from getting the title when LastError was not set.
            return true;
        }

        if (enumWindowProcTitleContains.empty())
        {
            // No need to check windowTitle here, just return the window we found.
            enumWindowProcHwndOut = window;
            return false;
        }

        std::wstring windowTitleStr = windowTitle.get();
        size_t find = windowTitleStr.find(enumWindowProcTitleContains);
        if (find != std::wstring::npos)
        {
            enumWindowProcHwndOut = window;
            return false;
        }
    }
    return true;
}

// --- move_window(): moves a window either above, or below the xti keyboard.
// ----- window: The window to move
// ----- above: True if move above the xti keyboard, false if move below the xti keyboard.
// ----- appDimensions: Where windows should be placed in the desktop.
/* public */ void windows_subsystem::move_window(HWND window, bool above, const app_dimensions& dimensions) {
    RECT currDimensions;
    int32_t r = ::GetWindowRect(window, &currDimensions);
    if (r == 0)
    {
        throw std::runtime_error("Failure on GetWindowRect()");
    }
    RECT adjDimensions;
    r = ::DwmGetWindowAttribute(window, DWMWA_EXTENDED_FRAME_BOUNDS, &adjDimensions, sizeof(RECT));
    if (r != S_OK)
    {
        throw std::runtime_error("Failure on DwmGetWindowAttribute()");
    }
    int32_t xAdjustment = currDimensions.left - adjDimensions.left;
    int32_t yAdjustment = currDimensions.top - adjDimensions.top;
    int32_t widthAdjustment = (currDimensions.right - currDimensions.left) - (adjDimensions.right - adjDimensions.left);
    int32_t heightAdjustment = (currDimensions.bottom - currDimensions.top) - (adjDimensions.bottom - adjDimensions.top);

    int32_t newX = xAdjustment;
    int32_t newY = (above ? 0 : dimensions.dimensionsBelowYStart) + yAdjustment;
    int32_t newWidth = dimensions.dimensionsAvailableScreenWidth + widthAdjustment;
    int32_t newHeight = (above ? dimensions.dimensionsAboveYEnd : dimensions.dimensionsBelowYEnd - dimensions.dimensionsBelowYStart) + heightAdjustment;

    r = ::SetWindowPos(window, HWND_TOP, newX, newY, newWidth, newHeight, SWP_SHOWWINDOW);
    if (r == 0)
    {
        throw std::runtime_error("Failure on SetWindowPos()");
    }
}

// --- get_exe_name_from_process_id(): get the executable file name (without directory) for a given process ID.
// ----- processId: Native numerical identifier the kernel has assigned to the process.
// ------- returns: the UPPERCASE binary executable file name if found, or empty string if not found.
// -------------------------------------------------------------------------------------------/
/* private */ std::wstring windows_subsystem::get_exe_name_from_process_id(uint32_t processId) {
    ::HANDLE processHandle = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, processId);
    if (processHandle == nullptr)
    {
        if (::GetLastError() == ERROR_ACCESS_DENIED)
        {
            // system processes are off bounds, just skip them
            return L"";
        }
        throw std::runtime_error("Failure on OpenProcess()");
    }
    ::HMODULE moduleHandle;
    uint32_t needed;
    int32_t r = ::EnumProcessModulesEx(processHandle, &moduleHandle, sizeof(moduleHandle), reinterpret_cast<::DWORD*>(&needed), LIST_MODULES_DEFAULT);
    if (r == 0)
    {
        uint32_t errorCode = ::GetLastError();
        if (errorCode == ERROR_NOACCESS || errorCode == ERROR_PARTIAL_COPY)
        {
            // system processes are off bounds, just skip them
            r = ::CloseHandle(processHandle);
            if (r == 0)
            {
                throw std::runtime_error("Failure on CloseHandle()");
            }
            return L"";
        }
        throw std::runtime_error("Failure on EnumProcessModulesEx()");
    }
    wchar_t processName[MAX_PATH];
    r = ::GetModuleBaseNameW(processHandle, moduleHandle, processName, sizeof(processName) / sizeof(wchar_t));
    if (r == 0)
    {
        throw std::runtime_error("Failure on GetModuleBaseNameW()");
    }
    r = ::CloseHandle(processHandle);
    if (r == 0)
    {
        throw std::runtime_error("Failure on CloseHandle()");
    }
    std::wstring out = processName;
    std::transform(out.begin(), out.end(), out.begin(), ::toupper);
    return out;
}
