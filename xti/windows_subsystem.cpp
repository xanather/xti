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
#include <QDebug>
// 2. System/OS headers
#include <Psapi.h>
#include <shellapi.h>
#include <dwmapi.h>
// 3. C++ standard library headers
#include <memory>
#include <cctype>
#include <algorithm>
// 4. Project classes
#include "error_reporter.h"

// --- initialize_apply_keyboard_window_style(): Tells windows to apply for keyboard native window styling.
// ----- window: HWND of the Qt app.
// --------------------------------------------------------------------------------------------/
/* public */ void windows_subsystem::initialize_apply_keyboard_window_style(::HWND window)
{
    // If the main app window does not call this function, then the keyboard can still take foreground
    // focus away from others despite when setting Qt::WindowDoesNotAcceptFocus at startup.
    int64_t r = ::GetWindowLongPtrW(window, GWL_EXSTYLE);
    if (r == 0)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::GetWindowLongPtr() failure.");
    }
    r = ::SetWindowLongPtrW(window, GWL_EXSTYLE, r | WS_EX_NOACTIVATE | WS_EX_TOPMOST);
    if (r == 0)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::SetWindowLongPtrW() failure.");
    }
}

// --- initialize_force_cursor_visible(): Forces the cursor to be visible even in tablet mode contexts.
// --------------------------------------------------------------------------------------------/
/* public */ void windows_subsystem::initialize_force_cursor_visible()
{
    ::HKEY hKey;
    wchar_t subKey[] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";
    wchar_t valueName[] = L"EnableCursorSuppression";
    uint32_t valueData = 0; // DWORD value - 0 = Force cursor to stay visible
    int32_t r = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, subKey, 0, KEY_SET_VALUE, &hKey);
    if (r != ERROR_SUCCESS)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::RegOpenKeyExW() failure.");
    }
    r = ::RegSetValueExW(hKey, valueName, 0, REG_DWORD, reinterpret_cast<BYTE*>(&valueData), sizeof(valueData));
    if (r != ERROR_SUCCESS)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::RegSetValueExW() failure.");
    }
    r = ::RegCloseKey(hKey);
    if (r != ERROR_SUCCESS)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::RegCloseKey() failure.");
    }
}

// --- initialize_orientate_main_window(): Moves the main QT window into position.
// ----- window: HWND of the Qt app.
// ------- returns: app dimensions to be used later on for re-positioning other windows.
// ------------------------------------------------------------------------/
/* public */ app_dimensions windows_subsystem::initialize_orientate_main_window(::HWND window)
{
    ::RECT size;
    int32_t r = ::GetWindowRect(window, &size);
    if (r == 0)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::GetWindowRect() failure.");
    }
    int32_t baseHeight = size.bottom - size.top;
    r = ::SystemParametersInfoW(SPI_GETWORKAREA, 0, &size, 0);
    if (r == 0)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::SystemParametersInfoW() failure.");
    }
    int32_t workingHeight = size.bottom;
    int32_t workingWidth = size.right;
    r = ::SetWindowPos(window, nullptr, 0, (workingHeight / 2) - (baseHeight / 2), workingWidth, baseHeight, 0);
    if (r == 0)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::SetWindowPos() failure.");
    }
    app_dimensions out;
    out.dimensionsAvailableScreenWidth = workingWidth;
    out.dimensionsAboveYEnd = (workingHeight / 2) - (baseHeight / 2);
    out.dimensionsBelowYStart = (workingHeight / 2) - (baseHeight / 2) + baseHeight;
    out.dimensionsBelowYEnd = workingHeight;
    return out;
}

// --- initialize_apply_system_super_admin_privilege(): Tells windows to apply for super admin privileges.
// --------------------------------------------------------------------------------------/
/* public */ void windows_subsystem::initialize_apply_system_super_admin_privilege()
{
    // Even though we run the app as Admin modern windows kernel will not
    // grant certain privileges unless we ask for it directly.
    // Based on how the app currently operates and hooks into the OS
    // this is not needed at this time or called at startup. This was really only used for debugging.
    ::HANDLE processTokenHandle;
    int32_t r = ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &processTokenHandle);
    if (r == 0)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::OpenProcessToken() failure.");
    }
    ::LUID privilegeId;
    r = ::LookupPrivilegeValueW(nullptr, SE_DEBUG_NAME /* Apply for everything */, &privilegeId);
    if (r == 0)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::LookupPrivilegeValueW() failure.");
    }
    ::TOKEN_PRIVILEGES tokenPrivilegesRequest;
    tokenPrivilegesRequest.PrivilegeCount = 1;
    tokenPrivilegesRequest.Privileges[0].Luid = privilegeId;
    tokenPrivilegesRequest.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    r = ::AdjustTokenPrivileges(processTokenHandle, false, &tokenPrivilegesRequest, sizeof(TOKEN_PRIVILEGES), nullptr, nullptr);
    if (r == 0)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::AdjustTokenPrivileges() failure.");
    }
    // edge case: need to check GetLastError to ensure not ERROR_NOT_ALL_ASSIGNED
    uint32_t errCode = ::GetLastError();
    if (errCode != ERROR_SUCCESS)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::AdjustTokenPrivileges() failure.");
    }
}

// --- move_active_window(): Moves the current active foreground window above or below the xti keyboard.
// ----- above: True if to open above the xti keyboard, false if move below the xti keyboard.
// ----- appDimensions: Where windows should be placed in the desktop.
// --------------------------------------------------------------------------------------/
/* public */ void windows_subsystem::move_active_window(bool above, const app_dimensions& appDimensions)
{
    ::HWND window = ::GetForegroundWindow();
    if (window == nullptr)
    {
        return;
    }
    move_window(window, above, appDimensions);
}

// --- initialize_prevent_touch_from_moving_cursor(): Prevents touch input from interfering with the virtual touchpad.
// --------------------------------------------------------------------------------------/
/* public */ void windows_subsystem::initialize_prevent_touch_from_moving_cursor()
{
    ::HMODULE handle = ::GetModuleHandleW(nullptr);
    if (handle == nullptr)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::GetModuleHandleW() failure.");
    }
    llMouseHook = ::SetWindowsHookExW(WH_MOUSE_LL, ll_mouse_proc, handle, 0);
    if (llMouseHook == nullptr)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::SetWindowsHookExW() failure.");
    }
    qDebug() << "[DEBUG]: hook installed";
}
/* public */ void windows_subsystem::cleanup_prevent_touch_from_moving_cursor()
{
    int32_t r = ::UnhookWindowsHookEx(llMouseHook);
    if (r == 0)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::UnhookWindowsHookEx() failure.");
    }
    qDebug() << "[DEBUG]: hook uninstalled";
}
/* private */ ::HHOOK windows_subsystem::llMouseHook;
/* private */ int64_t windows_subsystem::ll_mouse_proc(int32_t code, uint64_t wParam, int64_t lParam)
{
    qDebug() << "[DEBUG]: hook called";
    if (code >= 0 && wParam == WM_MOUSEMOVE)
    {
        qDebug() << "[DEBUG]: WM_MOUSEMOVE called";
        ::MSLLHOOKSTRUCT* hookInfo = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
        uint64_t extraInfo = hookInfo->dwExtraInfo;
        if ((extraInfo & 0xFF515700) == 0xFF515700)
        {
            qDebug() << "[DEBUG]: message skipped called";
            return 1;
        }
    }
    return ::CallNextHookEx(llMouseHook, code, wParam, lParam);
}

// --- start_process(): Starts a new process and positioning either above or below the xti keyboard.
// ----- exePath: absolute file path of the executable.
// ----- params: Additional parameter string to pass at startup.
// ----- workingDirectory: absolute working directory to run it under.
// ----- expectedExeName: The running executable name that this function should eventually produce.
// ----- expectedTitleName: The running window title name that this function should eventually produce. Empty for any.
// ----- above: True if to open above the xti keyboard, false if move below the xti keyboard.
// ----- appDimensions: Where windows should be placed in the desktop.
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------/
/* public */ void windows_subsystem::start_process(const std::wstring& exePath, const std::wstring& params, const std::wstring& workingDirectory,
                                                   const std::wstring& expectedExeName, const std::wstring& expectedTitleName, bool above, const app_dimensions& appDimensions)
{
    // Intentionally don't check the return value of ShellExecuteW.
    // User may have bad config, don't crash the app if we failed to open the process.
    ::ShellExecuteW(nullptr, L"open", exePath.c_str(),
                                           params.empty() ? nullptr : params.c_str(), workingDirectory.c_str(), SW_SHOWNORMAL);
    // Wait 500 ms for any application initialization logic before attempting to re-organize windows.
    ::Sleep(500);
    ::HWND window;
    // Try with title name first.
    if (!expectedTitleName.empty())
    {
        window = get_window(expectedExeName, expectedTitleName);
        if (window != nullptr)
        {
            move_window(window, above, appDimensions);
            return;
        }
    }
    // Try without title name.
    window = get_window(expectedExeName, L"");
    if (window != nullptr)
    {
        move_window(window, above, appDimensions);
    }
}

// --- is_process_running(): Determines if a process is running within the system.
// ----- processName: The process name to check. Not case sensitive.
// ------- returns: true if found, false if not
// ---------------------------------------------------------------------------------------/
/* public */ bool windows_subsystem::is_process_running(const std::wstring& processName)
{
    uint32_t processesArray[1024];
    uint32_t needed;
    int32_t r = ::EnumProcesses(reinterpret_cast<::DWORD*>(processesArray), sizeof(processesArray), reinterpret_cast<::DWORD*>(&needed));
    if (r == 0)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::EnumProcesses() failure.");
    }
    uint32_t processCount = needed / sizeof(uint32_t);
    if (processCount == 1024)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::EnumProcesses() overflow.");
    }
    std::wstring processNameUpper = processName;
    std::transform(processNameUpper.begin(), processNameUpper.end(), processNameUpper.begin(), ::toupper);
    for (size_t i = 0; i < processCount; i++)
    {
        if (processesArray[i] == 0)
        {
            continue; // skip kernel, the get_exe_name_from_process_id fails on 0.
        }
        std::wstring processNameUpperRunning = get_exe_name_from_process_id(processesArray[i]);
        if (processNameUpperRunning == processNameUpper)
        {
            return true;
        }
    }
    return false;
}

// --- get_window(): Get a window based on specific underlying exe name and title.
// ----- runningExe: exe name (with extension). Not case sensitive.
// ----- requiredTitleContains: Text that the window title must contain, empty means any title (only match exe name).
// ------- returns: the found HWND, or nulptr if not found.
// --------------------------------------------------------------------------------------------------------------------/
/* public */ ::HWND windows_subsystem::get_window(const std::wstring& runningExe, const std::wstring& requiredTitleContains)
{
    enumWindowProcExeNameUpper = runningExe;
    std::transform(enumWindowProcExeNameUpper.begin(), enumWindowProcExeNameUpper.end(), enumWindowProcExeNameUpper.begin(), ::toupper);
    enumWindowProcTitleContains = requiredTitleContains;
    enumWindowProcHwndOut = nullptr;
    ::SetLastError(ERROR_SUCCESS);
    // Throwing away return value here, can return 0 if the enumeration stops early. Rely on GetLastError instead as documentation suggests.
    ::EnumDesktopWindows(nullptr, enum_window_proc, 0);
    uint32_t errCode =  ::GetLastError();
    if (errCode != ERROR_SUCCESS)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::EnumDesktopWindows() failure.");
    }
    return enumWindowProcHwndOut;
}

/* private */ thread_local std::wstring windows_subsystem::enumWindowProcExeNameUpper;
/* private */ thread_local std::wstring windows_subsystem::enumWindowProcTitleContains;
/* private */ thread_local ::HWND windows_subsystem::enumWindowProcHwndOut;
/* private */ int32_t __stdcall windows_subsystem::enum_window_proc(::HWND window, [[maybe_unused]] int64_t param)
{
    // In this function we set SetLastError after every return intentionally. See EnumDesktopWindows documentation.
    uint32_t processId;
    int32_t r;
    uint32_t rDword = ::GetWindowThreadProcessId(window, reinterpret_cast<::DWORD*>(&processId));
    if (rDword == 0)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::GetWindowThreadProcessId() failure.");
    }
    std::wstring exeNameUpper = get_exe_name_from_process_id(processId);
    if (enumWindowProcExeNameUpper == exeNameUpper)
    {
        // We skip windows that are not visible.
        r = ::IsWindowVisible(window);
        if (r == 0)
        {
            ::SetLastError(ERROR_SUCCESS);
            return true;
        }

        ::SetLastError(ERROR_SUCCESS); // GetWindowTextLengthW needs SetLastError set first.
        int32_t windowTitleLength = ::GetWindowTextLengthW(window);
        // error response only available with GetLastError rather than return of GetWindowTextLengthW here.
        uint32_t errCode = ::GetLastError();
        if (errCode != ERROR_SUCCESS)
        {
            error_reporter::stop(__FILE__, __LINE__, "Win32::GetWindowTextLengthW() failure.");
        }
        std::unique_ptr<wchar_t[]> windowTitle = std::make_unique<wchar_t[]>(windowTitleLength + 1);
        ::SetLastError(ERROR_SUCCESS);
        r = ::GetWindowTextW(window, windowTitle.get(), windowTitleLength + 1);
        if (r == 0)
        {
            errCode = ::GetLastError();
            if (errCode != ERROR_SUCCESS)
            {
                error_reporter::stop(__FILE__, __LINE__, "Win32::GetWindowTextW() failure.");
            }
            // We skip windows with no text or any other reason that prevents us
            // from getting the title when LastError was not set. Usually its due to one of two senarios:
            // 1. The window hasn't finished initalizing.
            // 2. its a privileged window that really shouldn't be iterated on.
            ::SetLastError(ERROR_SUCCESS);
            return true;
        }

        if (enumWindowProcTitleContains.empty())
        {
            // No need to check windowTitle here, just return the window we found.
            enumWindowProcHwndOut = window;
            ::SetLastError(ERROR_SUCCESS);
            return false;
        }

        std::wstring windowTitleStr = windowTitle.get();
        size_t find = windowTitleStr.find(enumWindowProcTitleContains);
        if (find != std::wstring::npos)
        {
            enumWindowProcHwndOut = window;
            ::SetLastError(ERROR_SUCCESS);
            return false;
        }
    }
    ::SetLastError(ERROR_SUCCESS);
    return true;
}

// --- move_window(): moves a window either above, or below the xti keyboard.
// ----- window: The window to move.
// ----- above: True if move above the xti keyboard, false if move below the xti keyboard.
// ----- appDimensions: Where windows should be placed in the desktop.
/* public */ void windows_subsystem::move_window(::HWND window, bool above, const app_dimensions& dimensions)
{
    RECT currDimensions;
    int32_t r = ::GetWindowRect(window, &currDimensions);
    if (r == 0)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::GetWindowRect() failure.");
    }
    RECT adjDimensions;
    r = ::DwmGetWindowAttribute(window, DWMWA_EXTENDED_FRAME_BOUNDS, &adjDimensions, sizeof(RECT));
    if (r != S_OK)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::DwmGetWindowAttribute() failure.");
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
        error_reporter::stop(__FILE__, __LINE__, "Win32::SetWindowPos() failure.");
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
        error_reporter::stop(__FILE__, __LINE__, "Win32::OpenProcess() failure.");
    }
    ::HMODULE moduleHandle;
    uint32_t needed;
    int32_t r = ::EnumProcessModulesEx(processHandle, &moduleHandle, sizeof(moduleHandle), reinterpret_cast<::DWORD*>(&needed), LIST_MODULES_DEFAULT);
    if (r == 0)
    {
        uint32_t errCode = ::GetLastError();
        if (errCode == ERROR_NOACCESS || errCode == ERROR_PARTIAL_COPY)
        {
            // system processes are off bounds, just skip them
            r = ::CloseHandle(processHandle);
            if (r == 0)
            {
                error_reporter::stop(__FILE__, __LINE__, "Win32::CloseHandle() failure.");
            }
            return L"";
        }
        error_reporter::stop(__FILE__, __LINE__, "Win32::EnumProcessModulesEx() failure.");
    }
    wchar_t processName[MAX_PATH];
    r = ::GetModuleBaseNameW(processHandle, moduleHandle, processName, sizeof(processName) / sizeof(wchar_t));
    if (r == 0)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::GetModuleBaseNameW() failure.");
    }
    r = ::CloseHandle(processHandle);
    if (r == 0)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::CloseHandle() failure.");
    }
    std::wstring out = processName;
    std::transform(out.begin(), out.end(), out.begin(), ::toupper);
    return out;
}

// --- show_exception_to_user(): Shows a message box with given error message.
// ----- error: The message to show.
// -------------------------------------------------------------------------------------------/
/* public */ void windows_subsystem::show_exception_to_user(const std::wstring& error)
{
    // no checking return value here, program is beyond saving at this point - just try show the error.
    ::MessageBoxW(nullptr, error.c_str(), L"XTI Runtime Exception", MB_OK);
}

// --- get_key_modifiers(): Gets the current active key modifiers on the system.
// ------- returns: All key modifier states.
// ---------------------------------------------------------------/
/* public */ key_modifiers windows_subsystem::get_key_modifiers()
{
    key_modifiers modifiers;
    modifiers.capsLock = (::GetKeyState(VK_CAPITAL) & 0x0001) != 0;
    modifiers.scrollLock = (::GetKeyState(VK_SCROLL) & 0x0001) != 0;
    modifiers.numLock = (::GetKeyState(VK_NUMLOCK) & 0x0001) != 0;
    modifiers.control = (::GetKeyState(VK_RCONTROL) & 0x8000) != 0;
    modifiers.shift = (::GetKeyState(VK_RSHIFT) & 0x8000) != 0;
    modifiers.alt = (::GetKeyState(VK_RMENU) & 0x8000) != 0;
    modifiers.windows = (::GetKeyState(VK_RWIN) & 0x8000) != 0;;
    return modifiers;
}

// --- get_focus_window_name(): Gets the current window title that has focus.
// ------- returns: Window title name text.
/* public */ std::wstring windows_subsystem::get_focus_window_name() {
    HWND window = ::GetForegroundWindow();
    if (window == nullptr)
    {
        return L"";
    }
    ::SetLastError(ERROR_SUCCESS); // GetWindowTextLengthW needs SetLastError set first.
    int32_t windowTitleLength = ::GetWindowTextLengthW(window);
    // error response only available with GetLastError rather than return of GetWindowTextLengthW here.
    uint32_t errCode = ::GetLastError();
    if (errCode != ERROR_SUCCESS)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::GetWindowTextLengthW() failure.");
    }
    std::unique_ptr<wchar_t[]> windowTitle = std::make_unique<wchar_t[]>(windowTitleLength + 1);
    ::SetLastError(ERROR_SUCCESS);
    int32_t r = ::GetWindowTextW(window, windowTitle.get(), windowTitleLength + 1);
    if (r == 0)
    {
        errCode = ::GetLastError();
        if (errCode != ERROR_SUCCESS)
        {
            error_reporter::stop(__FILE__, __LINE__, "Win32::GetWindowTextW() failure.");
        }
        return L"";
    }
    return windowTitle.get();
}
