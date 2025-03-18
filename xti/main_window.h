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
    void post_ctor();
    void ui_on_key_press();
    void ui_on_shortcuts_above_changed(int32_t index);
    void ui_on_shortcuts_above_reopen();
    void ui_on_shortcuts_below_changed(int32_t index);
    void ui_on_shortcuts_below_reopen();
    void ui_on_move_active_above();
    void ui_on_move_active_below();
};
#endif // MAIN_WINDOW_H
