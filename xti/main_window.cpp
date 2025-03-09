// Copyright © Jordan Singh

#include "main_window.h"
#include "./ui_main_window.h"

// 1. Qt framework headers
#include <QScreen>
#include <QPalette>
#include <QBrush>
#include <QColor>
#include <QPushButton>
#include <QDebug>
// 2. System/OS headers
#include <windows.h>
#include <psapi.h>
// 3. C++ standard library headers
#include "stdint.h"
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

    // Collecting all buttons
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

    // Inserting shortcuits
    // FORKERS: adjust comboBox_shortcutsAbove and comboBox_shortcutsBelow contents accordingly
    ui->comboBox_shortcutsAbove->addItem("Firefox");
    ui->comboBox_shortcutsAbove->addItem("OneNote");
    ui->comboBox_shortcutsAbove->addItem("Bitwarden");
    ui->comboBox_shortcutsAbove->addItem("Yubico");
    ui->comboBox_shortcutsAbove->addItem("Spotify");
    ui->comboBox_shortcutsAbove->addItem("Terminal");
    ui->comboBox_shortcutsAbove->addItem("iCloud Mail");

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

    std::wstring nam = L"Firefox";
    main_window::open_or_show_app(nam);
}

main_window::~main_window()
{
    delete ui;
}

void main_window::open_or_show_app(std::wstring& name) {
    // FORKERS: adjust code below accordingly.
    if (name == L"Firefox") {
        std::vector<std::wstring> processes = get_process_list();
        for (size_t i = 0; i < processes.size(); i++) {
            qDebug() << processes[i];
        }
    }
}

std::vector<std::wstring> main_window::get_process_list() {
    uint32_t processesArray[1024];
    uint32_t needed;
    int32_t errorCode;
    std::vector<std::wstring> output;
    errorCode = ::EnumProcesses(reinterpret_cast<::DWORD*>(processesArray), sizeof(processesArray), reinterpret_cast<::DWORD*>(&needed));
    if (errorCode == 0)
    {
        throw std::runtime_error("Failure on EnumProcesses()");
    }
    uint32_t processCount = needed / sizeof(uint32_t);
    for (size_t i = 0; i < processCount; i++)
    {
        if (processesArray[i] == 0) {
            continue; // kernel
        }
        ::HANDLE processHandle = ::OpenProcess(PROCESS_QUERY_INFORMATION, false, processesArray[i]);
        if (processHandle == nullptr) {
            throw std::runtime_error("Failure on OpenProcess()");
        }
        ::HMODULE moduleHandle;
        errorCode = ::EnumProcessModulesEx(processHandle, &moduleHandle, sizeof(moduleHandle), reinterpret_cast<::DWORD*>(&needed), LIST_MODULES_DEFAULT);
        if (errorCode == 0)
        {
            throw std::runtime_error("Failure on EnumProcessModulesEx()");
        }
        wchar_t processName[MAX_PATH];
        errorCode = ::GetModuleBaseNameW(processHandle, moduleHandle, processName, sizeof(processName) / sizeof(wchar_t));
        if (errorCode == 0)
        {
            throw std::runtime_error("Failure on GetModuleBaseNameW()");
        }
        output.push_back(processName);
        errorCode = ::CloseHandle(processHandle);
        if (errorCode == 0)
        {
            throw std::runtime_error("Failure on CloseHandle()");
        }
    }
    return output;
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
