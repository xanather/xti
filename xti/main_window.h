// Copyright © Jordan Singh

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

// 1. Qt framework headers
#include <QMainWindow>
#include <QJsonDocument>
#include <QVariant>
// 2. System/OS headers
// 3. C++ standard library headers
#include <vector>
#include <string>
#include <cstdint>
// 4. Project classes
#include "app_dimensions.h"
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
    app_dimensions m_appDimensions;
    QJsonDocument m_appConfig;
    Ui::main_window *ui;
    void open_or_show_app(const QVariant& iObj);

private slots:
    void ui_on_control();
    void ui_on_windows();
    void ui_on_shortcuts_above_changed(int32_t index);
    void ui_on_shortcuts_above_reopen();
    void ui_on_shortcuts_below_changed(int32_t index);
    void ui_on_shortcuts_below_reopen();
    void ui_on_move_active_above();
    void ui_on_move_active_below();
};
#endif // MAIN_WINDOW_H
