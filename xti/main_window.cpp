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

    connect(ui->pushButton_control, &QPushButton::clicked, this, &main_window::ui_on_control);
    connect(ui->pushButton_windows, &QPushButton::clicked, this, &main_window::ui_on_windows);
}

main_window::~main_window()
{
    delete ui;
}


void main_window::ui_on_control() {
    QPalette palette = ui->pushButton_control->palette();
    palette.setColor(QPalette::Button, Qt::green);
    ui->pushButton_control->setAutoFillBackground(true);
    ui->pushButton_control->setPalette(palette);
    ui->pushButton_windows->setPalette(QApplication::palette());
}

void main_window::ui_on_windows() {
    QPalette palette = ui->pushButton_windows->palette();
    palette.setColor(QPalette::Button, Qt::green);
    ui->pushButton_windows->setAutoFillBackground(true);
    ui->pushButton_windows->setPalette(palette);
    ui->pushButton_control->setPalette(QApplication::palette());
}
