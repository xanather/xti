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

#include "touchpad_cursor.h"
#include "ui_touchpad_cursor.h"

// 1. Qt framework headers
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
// 2. System/OS headers
#include <Windows.h>
// 3. C++ standard library headers
// 4. Project classes

touchpad_cursor::touchpad_cursor(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::touchpad_cursor)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(ui->label);
    effect->setOffset(0, 0);
    effect->setBlurRadius(5);
    ui->label->setGraphicsEffect(effect);
    QPropertyAnimation* animation = new QPropertyAnimation(effect, "color");
    animation->setDuration(2000);
    animation->setStartValue(QColor(Qt::red));
    animation->setEndValue(QColor(Qt::blue));
    animation->setLoopCount(-1);
    animation->start();
    HWND hwnd = reinterpret_cast<HWND>(this->winId());
    LONG exStyle = GetWindowLongW(hwnd, GWL_EXSTYLE);
    SetWindowLongW(hwnd, GWL_EXSTYLE, exStyle | WS_EX_TRANSPARENT);
}

touchpad_cursor::~touchpad_cursor()
{
    delete ui;
}
