// Copyright © Jordan Singh

// 1. Qt framework headers
#include <QApplication>
// 2. System/OS headers
#include <combaseapi.h>
// 3. C++ standard library headers
// 4. Project classes
#include "main_window.h"

int main(int argc, char *argv[])
{
    // Used by ShellExecuteW in windows_subsystem.cpp
    int32_t r = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (r != S_OK)
    {
        throw std::runtime_error("Failure on CoInitializeEx()");
    }
    QApplication a(argc, argv);
    a.setStyle("fusion");
    main_window w(nullptr);
    w.show();
    return a.exec();
}
