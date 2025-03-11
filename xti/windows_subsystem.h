// Copyright © Jordan Singh

#ifndef WINDOWS_SUBSYSTEM_H
#define WINDOWS_SUBSYSTEM_H

// 1. Qt framework headers
// 2. System/OS headers
// 3. C++ standard library headers
#include <string>
#include <cstdint>
// 4. Project classes
// 5. Forward decl

class windows_subsystem
{
public:
    static void apply_system_super_admin_privilege();
    static bool is_process_running(const std::wstring& processName);
    static void start_process(const std::wstring& path, const std::wstring& workingDirectory);
    static void* show_window(const std::wstring& runningExe, const std::wstring& requiredTitleContains /* empty means no requirement*/);
    static void move_window_above(const void* windowHandle);
    static void move_window_below(const void* windowHandle);
private:
    static thread_local std::wstring enumWindowProcExeName;
    static thread_local std::wstring enumWindowProcTitleContains;
    static thread_local void* enumWindowProcHwndOut;
    static int32_t enum_window_proc(void* window, int64_t param);
    static std::wstring get_exe_name_from_process_id(uint32_t processId);
};

#endif // WINDOWS_SUBSYSTEM_H
