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
#include "key_modifiers.h"
// 5. Forward decl

class windows_subsystem // static members only
{
public:
    // USED AT APP STARTUP
    // public initialize_apply_keyboard_window_style(): Tells windows to apply for keyboard native window styling.
    // see cpp file for more info.
    static void initialize_apply_keyboard_window_style(::HWND window);

public:
    // USED AT APP STARTUP
    // public initialize_force_cursor_visible(): Forces the cursor to be visible even in tablet mode contexts.
    // see cpp file for more info.
    static void initialize_force_cursor_visible();

    // USED AT APP STARTUP
    // public initialize_orientate_main_window(): Moves the main QT window into position.
    // see cpp file for more info.
public:
    static app_dimensions initialize_orientate_main_window(::HWND window);

    // USED AT APP STARTUP
    // public initialize_apply_system_super_admin_privilege(): Tells windows to apply for super admin privileges.
    // see cpp file for more info.
public:
    static void initialize_apply_system_super_admin_privilege();

    // public move_active_window(): Moves the current active foreground window above or below the xti keyboard.
    // see cpp file for more info.
public:
    static void move_active_window(bool above, const app_dimensions& appDimensions);

    // public start_process(): Starts a new process and positioning either above or below the xti keyboard.
    // see cpp file for more info.
public:
    static void start_process(const std::wstring& path, const std::wstring& params, const std::wstring& workingDirectory,
    const std::wstring& expectedExeName, const std::wstring& expectedTitleName, bool above, const app_dimensions& appDimensions);

    // public is_process_running(): Determines if a process is running within the system.
    // see cpp file for more info.
public:
    static bool is_process_running(const std::wstring& processName);

    // public get_window(): Get a window based on specific underlying exe name and title.
    // see cpp file for more info.
public:
    static ::HWND get_window(const std::wstring& runningExe, const std::wstring& requiredTitleContains /* empty means no requirement*/);
private:
    static thread_local std::wstring enumWindowProcExeNameUpper;
    static thread_local std::wstring enumWindowProcTitleContains;
    static thread_local ::HWND enumWindowProcHwndOut;
    static int32_t __stdcall enum_window_proc(::HWND window, int64_t param);

    // public move_window(): moves a window either above, or below the xti keyboard.
    // see cpp file for more info.
public:
    static void move_window(::HWND window, bool above, const app_dimensions& dimensions);

    // private get_exe_name_from_process_id(): get the executable file name (without directory) for a given process ID.
    // see cpp file for more info.
private:
    static std::wstring get_exe_name_from_process_id(uint32_t processId);

public:
    // public show_exception_to_user(): Shows a message box with given error message.
    // see cpp file for more info.
    static void show_exception_to_user(const std::wstring& error);

public:
    // public get_key_modifiers(): Gets the current active key modifiers on the system.
    // see cpp file for more info.
    static key_modifiers get_key_modifiers();

public:
    // public get_focus_window_name(): Gets the current window title that has focus.
    // see cpp file for more info.
    static std::wstring get_focus_window_name();
};

#endif // WINDOWS_SUBSYSTEM_H
