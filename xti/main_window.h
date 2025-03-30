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
#include <QPoint>
// 2. System/OS headers
// 3. C++ standard library headers
#include <vector>
#include <cstdint>
// 4. Project classes
#include "app_dimensions.h"
#include "touchpad_cursor.h"
#include "key_modifiers.h"
// 5. Forward decl
class QWidget;
class QPushButton;
class QVariant;
class QEvent;
class QTimer;
namespace Ui {
class main_window;
}

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window(QWidget* parent);
    virtual ~main_window();

private:
    std::vector<QPushButton*> m_keyButtonList;
    std::vector<QPushButton*> m_keyButtonLeftList;
    std::vector<QPushButton*> m_keyButtonRightTopList;
    std::vector<QPushButton*> m_keyButtonRightBottomList;
    std::vector<QPushButton*> m_allButtonsList;
    QPushButton* m_downButton = nullptr;

    QJsonDocument m_appConfig;

    app_dimensions m_appDimensions;
    key_modifiers m_keyModifiers;

    QTimer* m_activeKeyColorTimer = nullptr;

    Ui::main_window* ui;
    void open_or_show_app(const QVariant& shortcutConfig);

    // SECTION: Virtual keyboard functions.
private slots:
    void ui_on_post_ctor();
    void ui_on_state_refresher_loop();
    void ui_on_key_press();
private:
    void post_key_press(QPushButton* srcButton, bool modChanged, bool modOn);
private slots:
    void ui_on_key_press_fade();
private:
    void update_modifier_colors();

    // SECTION: Virtual touchpad functions.
protected:
    bool m_cursorIsMoving = false;
    bool m_cursorIsHooked = false;
    QPoint m_cursorStartPosition;
    int32_t m_cursorSpeed = 1;
    QTimer* m_cursorMoveTimerDelay = nullptr;
    touchpad_cursor* m_cursor = nullptr;
    virtual bool event(QEvent* ev) override;
private slots:
    void ui_on_cursor_move_ready();

    // SECTION: Opening apps, and other utility functions.
private slots:
    void ui_on_shortcuts_above_changed(int32_t index);
    void ui_on_shortcuts_above_reopen();
    void ui_on_shortcuts_below_changed(int32_t index);
    void ui_on_shortcuts_below_reopen();
    void ui_on_move_active_above();
    void ui_on_move_active_below();
    void ui_on_panic();
    void ui_on_restart();
};
#endif // MAIN_WINDOW_H
