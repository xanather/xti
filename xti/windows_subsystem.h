// Copyright © Jordan Singh

#ifndef WINDOWS_SUBSYSTEM_H
#define WINDOWS_SUBSYSTEM_H

// 1. Qt framework headers
// 2. System/OS headers
// 3. C++ standard library headers
#include <vector>
#include <string>
#include <cstdint>
// 4. Project classes
// 5. Forward decl

class windows_subsystem
{
public:
    static void apply_system_super_admin_privilege();
    static std::vector<std::wstring> get_all_process_names();
};

#endif // WINDOWS_SUBSYSTEM_H
