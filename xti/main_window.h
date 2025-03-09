// Copyright © Jordan Singh

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

// 1. Qt framework headers
#include <QMainWindow>
// 2. System/OS headers
// 3. C++ standard library headers
#include <vector>
#include <string>
// 4. Project classes
// 5. Forward decl
class QWidget;
class QPushButton;
namespace Ui {
class main_window;
}

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window(QWidget *parent);
    ~main_window();

private:
    std::vector<QPushButton*> m_buttonList;
    Ui::main_window *ui;
    void open_or_show_app(std::wstring& name);

private slots:
    void ui_on_control();
    void ui_on_windows();
};
#endif // MAIN_WINDOW_H
