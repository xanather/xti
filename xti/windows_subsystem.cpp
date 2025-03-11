// Copyright © Jordan Singh

#include "windows_subsystem.h"

// 1. Qt framework headers
// 2. System/OS headers
#include <windows.h>
#include <psapi.h>
#include <shellapi.h>
// 3. C++ standard library headers
#include <stdexcept>
// 4. Project classes

// Even though we run the app as Admin modern windows kernel will not
// grant certain privileges unless we ask for it directly.
void windows_subsystem::apply_system_super_admin_privilege() {
    HANDLE processTokenHandle;
    int32_t r = ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &processTokenHandle);
    if (r == 0)
    {
        throw std::runtime_error("Failure on OpenProcessToken()");
    }

    ::LUID privilegeId;
    r = ::LookupPrivilegeValueW(nullptr, SE_DEBUG_NAME, &privilegeId);
    if (r == 0)
    {
        throw std::runtime_error("Failure on LookupPrivilegeValueW()");
    }

    ::TOKEN_PRIVILEGES tokenPrivilegesRequest;
    tokenPrivilegesRequest.PrivilegeCount = 1;
    tokenPrivilegesRequest.Privileges[0].Luid = privilegeId;
    tokenPrivilegesRequest.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    r = AdjustTokenPrivileges(processTokenHandle, false, &tokenPrivilegesRequest, sizeof(TOKEN_PRIVILEGES), nullptr, nullptr);
    if (r == 0)
    {
        throw std::runtime_error("Failure on LookupPrivilegeValueW()");
    }
    // edge case: need to check GetLastError to ensure not ERROR_NOT_ALL_ASSIGNED
    uint32_t errorCode = GetLastError();
    if (errorCode != ERROR_SUCCESS) {
        throw std::runtime_error("Failure on LookupPrivilegeValueW()");
    }
}

bool windows_subsystem::is_process_running(const std::wstring& processName) {
    uint32_t processesArray[1024];
    uint32_t needed;
    int32_t r;
    r = ::EnumProcesses(reinterpret_cast<::DWORD*>(processesArray), sizeof(processesArray), reinterpret_cast<::DWORD*>(&needed));
    if (r == 0)
    {
        throw std::runtime_error("Failure on EnumProcesses()");
    }
    uint32_t processCount = needed / sizeof(uint32_t);
    for (size_t i = 0; i < processCount; i++)
    {
        if (processesArray[i] == 0) {
            continue; // skip kernel
        }
        std::wstring processNameRunning = get_exe_name_from_process_id(processesArray[i]);
        if (processNameRunning == processName)
        {
            return true;
        }
    }
    return false;
}

void windows_subsystem::start_process(const std::wstring& exePath, const std::wstring& workingDirectory) {
    HINSTANCE instance = ::ShellExecuteW(nullptr, L"open", exePath.c_str(), nullptr, workingDirectory.c_str(), SW_NORMAL);
    if (instance == nullptr) {
        throw std::runtime_error("Failure on ShellExecuteW()");
    }
}

void* windows_subsystem::show_window(const std::wstring& runningExe, const std::wstring& requiredTitleContains) {
    enumWindowProcExeName = runningExe;
    enumWindowProcTitleContains = requiredTitleContains;
    enumWindowProcHwndOut = nullptr;
    int32_t r = ::EnumDesktopWindows(nullptr, reinterpret_cast<WNDENUMPROC>(enum_window_proc), 0);
    if (r == 0)
    {
        throw std::runtime_error("Failure on EnumDesktopWindows()");
    }
    // TODO show window to top here
    return enumWindowProcHwndOut;
}

void windows_subsystem::move_window_above([[maybe_unused]] const void* window) {

}

void windows_subsystem::move_window_below([[maybe_unused]] const void* window) {

}

thread_local std::wstring windows_subsystem::enumWindowProcExeName;
thread_local std::wstring windows_subsystem::enumWindowProcTitleContains;
thread_local void* windows_subsystem::enumWindowProcHwndOut;
int32_t windows_subsystem::enum_window_proc(void* window, [[maybe_unused]] int64_t param)
{
    uint32_t processId;
    int32_t r = ::GetWindowThreadProcessId(reinterpret_cast<HWND>(window), reinterpret_cast<::DWORD*>(&processId));
    if (r == 0)
    {
        throw std::runtime_error("Failure on GetWindowThreadProcessId()");
    }
    std::wstring exeName = get_exe_name_from_process_id(processId);
    if (enumWindowProcExeName == exeName)
    {
        enumWindowProcHwndOut = window;
        return false;
    }
    return true;
}

std::wstring windows_subsystem::get_exe_name_from_process_id(uint32_t processId) {
    ::HANDLE processHandle = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, processId);
    if (processHandle == nullptr) {
        if (GetLastError() == ERROR_ACCESS_DENIED)
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
        uint32_t errorCode = GetLastError();
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
    return processName;
}
