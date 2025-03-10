// Copyright © Jordan Singh

#ifndef WINDOWS_SUBSYSTEM_H
#define WINDOWS_SUBSYSTEM_H

// 1. Qt framework headers
// 2. System/OS headers
// 3. C++ standard library headers
#include <string>
// 4. Project classes
// 5. Forward decl

class windows_subsystem
{
public:
    static void apply_system_super_admin_privilege();
    static bool is_process_running(const std::wstring& processName);
    static void start_process(const std::wstring& path, const std::wstring& workingDirectory);
};

#endif // WINDOWS_SUBSYSTEM_H
