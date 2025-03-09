// Copyright © Jordan Singh

#include "windows_subsystem.h"

// 1. Qt framework headers
// 2. System/OS headers
#include <windows.h>
#include <psapi.h>
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

std::vector<std::wstring> windows_subsystem::get_all_process_names() {
    uint32_t processesArray[1024];
    uint32_t needed;
    int32_t r;
    std::vector<std::wstring> output;
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
        ::HANDLE processHandle = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, processesArray[i]);
        if (processHandle == nullptr) {
            if (GetLastError() == ERROR_ACCESS_DENIED)
            {
                // system processes are off bounds, just skip them
                continue;
            }
            throw std::runtime_error("Failure on OpenProcess()");
        }
        ::HMODULE moduleHandle;
        r = ::EnumProcessModulesEx(processHandle, &moduleHandle, sizeof(moduleHandle), reinterpret_cast<::DWORD*>(&needed), LIST_MODULES_DEFAULT);
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
                continue;
            }
            throw std::runtime_error("Failure on EnumProcessModulesEx()");
        }
        wchar_t processName[MAX_PATH];
        r = ::GetModuleBaseNameW(processHandle, moduleHandle, processName, sizeof(processName) / sizeof(wchar_t));
        if (r == 0)
        {
            throw std::runtime_error("Failure on GetModuleBaseNameW()");
        }
        output.push_back(processName);
        r = ::CloseHandle(processHandle);
        if (r == 0)
        {
            throw std::runtime_error("Failure on CloseHandle()");
        }
    }
    return output;
}
