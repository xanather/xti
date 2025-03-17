// Copyright © Jordan Singh

#ifndef WINDOWS_SUBSYSTEM_H
#define WINDOWS_SUBSYSTEM_H

// 1. Qt framework headers
// 2. System/OS headers
#include <Windows.h>
// 3. C++ standard library headers
#include <string>
#include <cstdint>
// 4. Project classes
#include "app_dimensions.h"
// 5. Forward decl

class windows_subsystem // static members only
{
public:
    // public apply_keyboard_window_style(): Tells windows to apply for keyboard native window styling.
    // see cpp file for more info.
    static void apply_keyboard_window_style(HWND window);

    // public apply_system_super_admin_privilege(): Tells windows to apply for super admin privileges.
    // see cpp file for more info.
public:
    static void apply_system_super_admin_privilege();

    // public move_active_window(): Moves the current active foreground window above or below the xti keyboard.
    // see cpp file for more info.
public:
    static void move_active_window(bool above, const app_dimensions& appDimensions);

    // public start_process(): Starts a new process and positioning either above or below the xti keyboard.
    // see cpp file for more info.
public:
    static void start_process(const std::wstring& path, const std::wstring& workingDirectory, bool above, const app_dimensions& appDimensions);

    // public is_process_running(): Determines if a process is running within the system.
    // see cpp file for more info.
public:
    static bool is_process_running(const std::wstring& processName);

    // public get_window(): Get a window based on specific underlying exe name and title.
    // see cpp file for more info.
public:
    static HWND get_window(const std::wstring& runningExe, const std::wstring& requiredTitleContains /* empty means no requirement*/);
private:
    static thread_local std::wstring enumWindowProcExeName;
    static thread_local std::wstring enumWindowProcTitleContains;
    static thread_local HWND enumWindowProcHwndOut;
    static int32_t __stdcall enum_window_proc(HWND window, int64_t param);

    // public move_window(): moves a window either above, or below the xti keyboard.
    // see cpp file for more info.
public:
    static void move_window(HWND window, bool above, const app_dimensions& dimensions);

    // private get_exe_name_from_process_id(): get the executable file name (without directory) for a given process ID.
    // see cpp file for more info.
private:
    static std::wstring get_exe_name_from_process_id(uint32_t processId);
};

#endif // WINDOWS_SUBSYSTEM_H
