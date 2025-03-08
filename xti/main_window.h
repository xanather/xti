// Copyright © Jordan Singh

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

// 1. Qt framework headers
#include <QMainWindow>
// 2. System/OS headers
// 3. C++ standard library headers
// 4. Project classes
// 5. Forward decl

QT_BEGIN_NAMESPACE
namespace Ui {
class main_window;
}
QT_END_NAMESPACE

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window(QWidget *parent);
    ~main_window();

private:
    Ui::main_window *ui;

private slots:
    void ui_on_control();
    void ui_on_windows();
};
#endif // MAIN_WINDOW_H
