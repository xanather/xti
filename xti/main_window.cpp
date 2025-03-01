// Copyright © Jordan Singh

#include "main_window.h"
#include "./ui_main_window.h"

// 1. Qt framework headers
#include <QScreen>
#include <QPalette>
#include <QBrush>
#include <QColor>
// 2. System/OS headers
// 3. C++ standard library headers
// 4. Project classes

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::main_window)
{
    ui->setupUi(this);

    QScreen *screen = QApplication::primaryScreen();
    if (screen == nullptr) {
        throw std::runtime_error("main_window::main_window (1)");
    }

    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);

    QPalette palette;
    palette.setColor(QPalette::Window, QColor(0, 0, 0));
    setPalette(palette);

    setGeometry(0, screen->geometry().height() / 2 - 100, screen->geometry().width(), 200);
}

main_window::~main_window()
{
    delete ui;
}
