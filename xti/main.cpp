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

// 1. Qt framework headers
#include <QApplication>
// 2. System/OS headers
#include <combaseapi.h>
// 3. C++ standard library headers
// 4. Project classes
#include "error_reporter.h"
#include "main_window.h"

int main(int argc, char *argv[])
{
    // Used by ShellExecuteW in windows_subsystem.cpp
    int32_t r = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (r != S_OK)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::CoInitializeEx() failure.");
    }
    ::SetPriorityClass(::GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    QApplication a(argc, argv);
    a.setStyle("fusion");
    main_window w(nullptr);
    w.show();
    return a.exec();
}
