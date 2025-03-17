// Copyright © Jordan Singh

#include "main_window.h"
#include "./ui_main_window.h"

// 1. Qt framework headers
#include <QApplication>
#include <QPushButton>
#include <QScreen>
#include <QPalette>
#include <QBrush>
#include <QColor>
#include <QFile>
#include <QIODevice>
#include <QStandardPaths>
#include <QByteArray>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
// 2. System/OS headers
// 3. C++ standard library headers
// 4. Project classes
#include "windows_subsystem.h"

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::main_window)
{
    ui->setupUi(this);

    // Make window top-most with no border + make background black.
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus);
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(0, 0, 0));
    setPalette(palette);

    // Configure xti dimensions.
    QScreen* screen = QApplication::primaryScreen();
    if (screen == nullptr) {
        throw std::runtime_error("Missing desktop screen");
    }
    int32_t height = this->height();
    setGeometry(0, screen->availableGeometry().height() / 2 - (height / 2), screen->availableGeometry().width(), height);
    m_appDimensions.dimensionsAvailableScreenWidth = screen->availableGeometry().width();
    m_appDimensions.dimensionsAboveYEnd = screen->availableGeometry().height() / 2 - (height / 2);
    m_appDimensions.dimensionsBelowYStart = m_appDimensions.dimensionsAboveYEnd + height;
    m_appDimensions.dimensionsBelowYEnd = screen->availableGeometry().height();

    // Load app config. Assumes UTF-8 encoding.
    QFile configFile(QDir::homePath() + "/xti.json");
    if (!configFile.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("No config file found");
    }
    QByteArray configData = configFile.readAll();
    configFile.close();
    m_appConfig = QJsonDocument::fromJson(configData);

    // Validate config.
    if (!m_appConfig.isArray()) {
        throw std::runtime_error("Bad config");
    }
    QJsonArray configEntries = m_appConfig.array();
    for (QJsonArray::iterator entry = configEntries.begin(); entry != configEntries.end(); ++entry)
    {
        if (!entry->isObject())
        {
            throw std::runtime_error("Bad config");
        }
        QJsonObject obj = entry->toObject();
        QJsonObject::iterator display = obj.find("exe");
        QJsonObject::iterator exe = obj.find("exe");
        QJsonObject::iterator dir = obj.find("dir");
        QJsonObject::iterator title = obj.find("title");
        QJsonObject::iterator above = obj.find("above");
        if (display == obj.end() || exe == obj.end() || dir == obj.end() || title == obj.end() || above == obj.end())
        {
            throw std::runtime_error("Bad config");
        }
        if (!display->isString() || !exe->isString() || !dir->isString() || !title->isString() || !above->isBool())
        {
            throw std::runtime_error("Bad config");
        }
        std::wstring exePath = exe->toString().toStdWString();
        size_t find = exePath.find_last_of('\\');
        if (find == std::wstring::npos) {
            throw std::runtime_error("Bad config");
        }
    }

    // Collecting all buttons.
    m_buttonList.push_back(ui->pushButton_escape);
    m_buttonList.push_back(ui->pushButton_f1);
    m_buttonList.push_back(ui->pushButton_f2);
    m_buttonList.push_back(ui->pushButton_f3);
    m_buttonList.push_back(ui->pushButton_f4);
    m_buttonList.push_back(ui->pushButton_f5);
    m_buttonList.push_back(ui->pushButton_f6);
    m_buttonList.push_back(ui->pushButton_f7);
    m_buttonList.push_back(ui->pushButton_f8);
    m_buttonList.push_back(ui->pushButton_f9);
    m_buttonList.push_back(ui->pushButton_f10);
    m_buttonList.push_back(ui->pushButton_f11);
    m_buttonList.push_back(ui->pushButton_f12);
    m_buttonList.push_back(ui->pushButton_backspace);
    m_buttonList.push_back(ui->pushButton_tilde);
    m_buttonList.push_back(ui->pushButton_exclamationMark);
    m_buttonList.push_back(ui->pushButton_at);
    m_buttonList.push_back(ui->pushButton_hash);
    m_buttonList.push_back(ui->pushButton_dollar);
    m_buttonList.push_back(ui->pushButton_percent);
    m_buttonList.push_back(ui->pushButton_next);
    m_buttonList.push_back(ui->pushButton_previous);
    m_buttonList.push_back(ui->pushButton_circumflex);
    m_buttonList.push_back(ui->pushButton_ampersand);
    m_buttonList.push_back(ui->pushButton_asterisk);
    m_buttonList.push_back(ui->pushButton_leftRoundBracket);
    m_buttonList.push_back(ui->pushButton_rightRoundBracket);
    m_buttonList.push_back(ui->pushButton_minus);
    m_buttonList.push_back(ui->pushButton_graveAccent);
    m_buttonList.push_back(ui->pushButton_num1);
    m_buttonList.push_back(ui->pushButton_num2);
    m_buttonList.push_back(ui->pushButton_num3);
    m_buttonList.push_back(ui->pushButton_num4);
    m_buttonList.push_back(ui->pushButton_num5);
    m_buttonList.push_back(ui->pushButton_mute);
    m_buttonList.push_back(ui->pushButton_pause);
    m_buttonList.push_back(ui->pushButton_num6);
    m_buttonList.push_back(ui->pushButton_num7);
    m_buttonList.push_back(ui->pushButton_num8);
    m_buttonList.push_back(ui->pushButton_num9);
    m_buttonList.push_back(ui->pushButton_num0);
    m_buttonList.push_back(ui->pushButton_underscore);
    m_buttonList.push_back(ui->pushButton_singleQuote);
    m_buttonList.push_back(ui->pushButton_Q);
    m_buttonList.push_back(ui->pushButton_W);
    m_buttonList.push_back(ui->pushButton_E);
    m_buttonList.push_back(ui->pushButton_R);
    m_buttonList.push_back(ui->pushButton_T);
    m_buttonList.push_back(ui->pushButton_pageUp);
    m_buttonList.push_back(ui->pushButton_pageDown);
    m_buttonList.push_back(ui->pushButton_Y);
    m_buttonList.push_back(ui->pushButton_U);
    m_buttonList.push_back(ui->pushButton_I);
    m_buttonList.push_back(ui->pushButton_O);
    m_buttonList.push_back(ui->pushButton_P);
    m_buttonList.push_back(ui->pushButton_equals);
    m_buttonList.push_back(ui->pushButton_doubleQuote);
    m_buttonList.push_back(ui->pushButton_A);
    m_buttonList.push_back(ui->pushButton_S);
    m_buttonList.push_back(ui->pushButton_D);
    m_buttonList.push_back(ui->pushButton_F);
    m_buttonList.push_back(ui->pushButton_G);
    m_buttonList.push_back(ui->pushButton_home);
    m_buttonList.push_back(ui->pushButton_end);
    m_buttonList.push_back(ui->pushButton_H);
    m_buttonList.push_back(ui->pushButton_J);
    m_buttonList.push_back(ui->pushButton_K);
    m_buttonList.push_back(ui->pushButton_L);
    m_buttonList.push_back(ui->pushButton_leftSquareBracket);
    m_buttonList.push_back(ui->pushButton_plus);
    m_buttonList.push_back(ui->pushButton_semicolon);
    m_buttonList.push_back(ui->pushButton_Z);
    m_buttonList.push_back(ui->pushButton_X);
    m_buttonList.push_back(ui->pushButton_C);
    m_buttonList.push_back(ui->pushButton_V);
    m_buttonList.push_back(ui->pushButton_B);
    m_buttonList.push_back(ui->pushButton_tab);
    m_buttonList.push_back(ui->pushButton_break);
    m_buttonList.push_back(ui->pushButton_N);
    m_buttonList.push_back(ui->pushButton_M);
    m_buttonList.push_back(ui->pushButton_lessThan);
    m_buttonList.push_back(ui->pushButton_greaterThan);
    m_buttonList.push_back(ui->pushButton_rightSquareBracket);
    m_buttonList.push_back(ui->pushButton_backslash);
    m_buttonList.push_back(ui->pushButton_colon);
    m_buttonList.push_back(ui->pushButton_q);
    m_buttonList.push_back(ui->pushButton_w);
    m_buttonList.push_back(ui->pushButton_e);
    m_buttonList.push_back(ui->pushButton_r);
    m_buttonList.push_back(ui->pushButton_t);
    m_buttonList.push_back(ui->pushButton_volumeUp);
    m_buttonList.push_back(ui->pushButton_volumeDown);
    m_buttonList.push_back(ui->pushButton_y);
    m_buttonList.push_back(ui->pushButton_u);
    m_buttonList.push_back(ui->pushButton_i);
    m_buttonList.push_back(ui->pushButton_o);
    m_buttonList.push_back(ui->pushButton_p);
    m_buttonList.push_back(ui->pushButton_leftBraces);
    m_buttonList.push_back(ui->pushButton_comma);
    m_buttonList.push_back(ui->pushButton_a);
    m_buttonList.push_back(ui->pushButton_s);
    m_buttonList.push_back(ui->pushButton_d);
    m_buttonList.push_back(ui->pushButton_f);
    m_buttonList.push_back(ui->pushButton_g);
    m_buttonList.push_back(ui->pushButton_printScreen);
    m_buttonList.push_back(ui->pushButton_numLock);
    m_buttonList.push_back(ui->pushButton_h);
    m_buttonList.push_back(ui->pushButton_j);
    m_buttonList.push_back(ui->pushButton_k);
    m_buttonList.push_back(ui->pushButton_l);
    m_buttonList.push_back(ui->pushButton_verticalSlash);
    m_buttonList.push_back(ui->pushButton_rightBraces);
    m_buttonList.push_back(ui->pushButton_fullstop);
    m_buttonList.push_back(ui->pushButton_z);
    m_buttonList.push_back(ui->pushButton_x);
    m_buttonList.push_back(ui->pushButton_c);
    m_buttonList.push_back(ui->pushButton_v);
    m_buttonList.push_back(ui->pushButton_b);
    m_buttonList.push_back(ui->pushButton_shift);
    m_buttonList.push_back(ui->pushButton_scrollLock);
    m_buttonList.push_back(ui->pushButton_n);
    m_buttonList.push_back(ui->pushButton_m);
    m_buttonList.push_back(ui->pushButton_slash);
    m_buttonList.push_back(ui->pushButton_questionMark);
    m_buttonList.push_back(ui->pushButton_menu);
    m_buttonList.push_back(ui->pushButton_capsLock);
    m_buttonList.push_back(ui->pushButton_space);
    m_buttonList.push_back(ui->pushButton_undo);
    m_buttonList.push_back(ui->pushButton_redo);
    m_buttonList.push_back(ui->pushButton_up);
    m_buttonList.push_back(ui->pushButton_down);
    m_buttonList.push_back(ui->pushButton_left);
    m_buttonList.push_back(ui->pushButton_right);
    m_buttonList.push_back(ui->pushButton_control);
    m_buttonList.push_back(ui->pushButton_windows);
    m_buttonList.push_back(ui->pushButton_alt);
    m_buttonList.push_back(ui->pushButton_copy);
    m_buttonList.push_back(ui->pushButton_cut);
    m_buttonList.push_back(ui->pushButton_paste);
    m_buttonList.push_back(ui->pushButton_fileFind);
    m_buttonList.push_back(ui->pushButton_find);
    m_buttonList.push_back(ui->pushButton_findAll);
    m_buttonList.push_back(ui->pushButton_insert);
    m_buttonList.push_back(ui->pushButton_delete);
    m_buttonList.push_back(ui->pushButton_enter);

    // Inserting shortcuts.
    for (QJsonArray::iterator entry = configEntries.begin(); entry != configEntries.end(); ++entry)
    {
        QJsonObject obj = entry->toObject();
        if (obj.find("above")->toBool()) {
            ui->comboBox_shortcutsAbove->addItem(obj.find("display")->toString(), obj);
        } else {
            ui->comboBox_shortcutsBelow->addItem(obj.find("display")->toString(), obj);
        }
    }

    // Hook buttons and controls.
    connect(ui->pushButton_control, &QPushButton::clicked, this, &main_window::ui_on_control);
    connect(ui->pushButton_windows, &QPushButton::clicked, this, &main_window::ui_on_windows);

    connect(ui->pushButton_reopenAbove, &QPushButton::clicked, this, &main_window::ui_on_shortcuts_above_reopen);
    connect(ui->comboBox_shortcutsAbove, &QComboBox::currentIndexChanged, this, &main_window::ui_on_shortcuts_above_changed);
    connect(ui->pushButton_reopenBelow, &QPushButton::clicked, this, &main_window::ui_on_shortcuts_below_reopen);
    connect(ui->comboBox_shortcutsBelow, &QComboBox::currentIndexChanged, this, &main_window::ui_on_shortcuts_below_changed);

    connect(ui->pushButton_moveAbove, &QPushButton::clicked, this, &main_window::ui_on_move_active_above);
    connect(ui->pushButton_moveBelow, &QPushButton::clicked, this, &main_window::ui_on_move_active_below);

    // windows_subsystem::apply_system_super_admin_privilege(); --- not needed at this time. see cpp definition in file for more info
}

main_window::~main_window()
{
    delete ui;
}

void main_window::open_or_show_app(const QVariant& iObj) {
    QJsonObject obj = iObj.toJsonObject();
    std::wstring exePath = obj.find("exe")->toString().toStdWString();
    size_t find = exePath.find_last_of('\\');
    std::wstring exeOnly = exePath.substr(find + 1);
    std::wstring windowTitle = obj.find("title")->toString().toStdWString();
    bool isAbove = obj.find("above")->toBool();
    if (windows_subsystem::is_process_running(exeOnly)) {
        HWND window = windows_subsystem::get_window(exeOnly, windowTitle);
        if (window != nullptr) {
            windows_subsystem::move_window(window, isAbove, m_appDimensions);
        }
    } else {
        std::wstring workingDirectory = obj.find("dir")->toString().toStdWString();
        windows_subsystem::start_process(exePath, workingDirectory, isAbove, m_appDimensions);
    }
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

void main_window::ui_on_shortcuts_above_changed(int32_t index) {
    open_or_show_app(ui->comboBox_shortcutsAbove->itemData(index));
}

void main_window::ui_on_shortcuts_above_reopen() {
    open_or_show_app(ui->comboBox_shortcutsAbove->currentData());
}

void main_window::ui_on_shortcuts_below_changed(int32_t index) {
    open_or_show_app(ui->comboBox_shortcutsBelow->itemData(index));
}

void main_window::ui_on_shortcuts_below_reopen() {
    open_or_show_app(ui->comboBox_shortcutsBelow->currentData());
}

void main_window::ui_on_move_active_above() {
    windows_subsystem::move_active_window(true, m_appDimensions);
}

void main_window::ui_on_move_active_below() {
    windows_subsystem::move_active_window(false, m_appDimensions);
}
