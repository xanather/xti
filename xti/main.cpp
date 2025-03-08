// Copyright © Jordan Singh

// 1. Qt framework headers
#include <QApplication>
// 2. System/OS headers
// 3. C++ standard library headers
// 4. Project classes
#include "main_window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");
    main_window w(nullptr);
    w.show();
    return a.exec();
}
