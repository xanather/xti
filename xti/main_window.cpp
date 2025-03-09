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

    ui->comboBox_shortcutsAbove->addItem("Firefox");
    ui->comboBox_shortcutsAbove->addItem("OneNote");
    ui->comboBox_shortcutsAbove->addItem("Bitwarden");
    ui->comboBox_shortcutsAbove->addItem("Yubico");
    ui->comboBox_shortcutsAbove->addItem("Spotify");

    ui->comboBox_shortcutsBelow->addItem("recrypt_gateway");
    ui->comboBox_shortcutsBelow->addItem("recrypt_core");
    ui->comboBox_shortcutsBelow->addItem("recrypt_account");
    ui->comboBox_shortcutsBelow->addItem("recrypt_auth");
    ui->comboBox_shortcutsBelow->addItem("recrypt_node");
    ui->comboBox_shortcutsBelow->addItem("recrypt_db");
    ui->comboBox_shortcutsBelow->addItem("recrypt_model");
    ui->comboBox_shortcutsBelow->addItem("recrypt_model_admin");
    ui->comboBox_shortcutsBelow->addItem("recrypt_model_internal");
    ui->comboBox_shortcutsBelow->addItem("recrypt_admin");
    ui->comboBox_shortcutsBelow->addItem("recrypt_client");

    ui->comboBox_shortcutsBelow->addItem("system_monitor");
    ui->comboBox_shortcutsBelow->addItem("ami_gen");
    ui->comboBox_shortcutsBelow->addItem("v_common");
    ui->comboBox_shortcutsBelow->addItem("aws_helpers");
    ui->comboBox_shortcutsBelow->addItem("vpc");
    ui->comboBox_shortcutsBelow->addItem("repository");
    ui->comboBox_shortcutsBelow->addItem("secrets");
    ui->comboBox_shortcutsBelow->addItem("dns_records");
    ui->comboBox_shortcutsBelow->addItem("ephemeral");
    ui->comboBox_shortcutsBelow->addItem("recrypt_template");
    ui->comboBox_shortcutsBelow->addItem("enrichment_center");
    ui->comboBox_shortcutsBelow->addItem("environment_manager");
    ui->comboBox_shortcutsBelow->addItem("viron_site");
    ui->comboBox_shortcutsBelow->addItem("vbalance");
    ui->comboBox_shortcutsBelow->addItem("ci_script_generator");
    ui->comboBox_shortcutsBelow->addItem("documentation");

    ui->comboBox_shortcutsBelow->addItem("notepad++");

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
