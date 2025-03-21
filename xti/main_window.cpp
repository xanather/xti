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

#include "main_window.h"
#include "ui_main_window.h"

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
#include <QTimer>
// 2. System/OS headers
// 3. C++ standard library headers
#include <string>
#include <cctype>
#include <algorithm>
// 4. Project classes
#include "windows_subsystem.h"
#include "key_mapping.h"

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::main_window)
{
    ui->setupUi(this);

    // Make window top-most with no border + make background translucent.
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    windows_subsystem::initialize_apply_keyboard_window_style(reinterpret_cast<HWND>(winId()));

    // Load app config. Assumes UTF-8 encoding.
    QFile configFile(QDir::homePath() + "/xti.json");
    if (!configFile.open(QIODevice::ReadOnly))
    {
        throw std::runtime_error("No config file found");
    }
    QByteArray configData = configFile.readAll();
    configFile.close();
    m_appConfig = QJsonDocument::fromJson(configData);

    // Validate config.
    if (!m_appConfig.isArray())
    {
        throw std::runtime_error("Bad config");
    }
    QJsonArray configEntries = m_appConfig.array();
    for (qsizetype i = 0; i < configEntries.size(); i++)
    {
        QJsonValueRef entry = configEntries[i];
        if (!entry.isObject())
        {
            throw std::runtime_error("Bad config");
        }
        QJsonObject obj = entry.toObject();
        QJsonObject::iterator display = obj.find("display");
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
        if (find == std::wstring::npos)
        {
            throw std::runtime_error("Bad config");
        }

        // Replace / with native \ in windows paths.
        std::wstring dirVal = dir->toString().toStdWString();
        for (size_t j = 0; j < dirVal.size(); j++)
        {
            if (dirVal[j] == '/')
            {
                dirVal[j] = '\\';
            }
        }
        *dir = QJsonValue(QString(dirVal));
        std::wstring exeVal = exe->toString().toStdWString();
        for (size_t j = 0; j < exeVal.size(); j++)
        {
            if (exeVal[j] == '/')
            {
                exeVal[j] = '\\';
            }
        }
        *exe = QJsonValue(QString(exeVal));

        // Make display uppercase.
        std::wstring displayStr = display->toString().toStdWString();
        std::transform(displayStr.begin(), displayStr.end(), displayStr.begin(), ::toupper);
        *display = QJsonValue(QString(displayStr));
        configEntries[i] = obj;
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
        if (obj.find("above")->toBool())
        {
            ui->comboBox_shortcutsAbove->addItem(obj.find("display")->toString(), obj);
        }
        else
        {
            ui->comboBox_shortcutsBelow->addItem(obj.find("display")->toString(), obj);
        }
    }

    // Hook buttons and controls.
    for (size_t i = 0; i < m_buttonList.size(); i++) {
        connect(m_buttonList[i], &QPushButton::clicked, this, &main_window::ui_on_key_press);
    }
    connect(ui->pushButton_reopenAbove, &QPushButton::clicked, this, &main_window::ui_on_shortcuts_above_reopen);
    connect(ui->comboBox_shortcutsAbove, &QComboBox::currentIndexChanged, this, &main_window::ui_on_shortcuts_above_changed);
    connect(ui->pushButton_reopenBelow, &QPushButton::clicked, this, &main_window::ui_on_shortcuts_below_reopen);
    connect(ui->comboBox_shortcutsBelow, &QComboBox::currentIndexChanged, this, &main_window::ui_on_shortcuts_below_changed);
    connect(ui->pushButton_moveAbove, &QPushButton::clicked, this, &main_window::ui_on_move_active_above);
    connect(ui->pushButton_moveBelow, &QPushButton::clicked, this, &main_window::ui_on_move_active_below);
    connect(ui->pushButton_panic, &QPushButton::clicked, this, &main_window::ui_on_panic);

    // windows_subsystem::apply_system_super_admin_privilege(); --- not needed at this time. see cpp definition in file for more info
    windows_subsystem::initialize_force_cursor_visible();
    key_mapping::initialize();
    QTimer::singleShot(0, this, &main_window::post_ctor);
}

void main_window::post_ctor() {
    // Needs to be after the window has been constructed, otherwise certain resize values get ignored.
    m_appDimensions = windows_subsystem::initialize_orientate_main_window(reinterpret_cast<HWND>(winId()));
}

main_window::~main_window()
{
    delete ui;
}

void main_window::open_or_show_app(const QVariant& iObj)
{
    QJsonObject obj = iObj.toJsonObject();
    std::wstring exePath = obj.find("exe")->toString().toStdWString();
    size_t find = exePath.find_last_of('\\');
    std::wstring exeOnly = exePath.substr(find + 1);
    std::wstring windowTitle = obj.find("title")->toString().toStdWString();
    bool isAbove = obj.find("above")->toBool();
    if (windows_subsystem::is_process_running(exeOnly))
    {
        HWND window = windows_subsystem::get_window(exeOnly, windowTitle);
        if (window != nullptr)
        {
            windows_subsystem::move_window(window, isAbove, m_appDimensions);
        }
    }
    else
    {
        std::wstring workingDirectory = obj.find("dir")->toString().toStdWString();
        windows_subsystem::start_process(exePath, workingDirectory, isAbove, m_appDimensions);
    }
}

void main_window::ui_on_key_press()
{
    QPushButton* srcButton = qobject_cast<QPushButton*>(sender());
    QPalette palette = srcButton->palette();
    palette.setColor(QPalette::Button, Qt::blue);
    srcButton->setAutoFillBackground(true);
    srcButton->setPalette(palette);

    QPalette defaultPalette = QApplication::palette();
    for (size_t i = 0; i < m_buttonList.size(); i++)
    {
        QPushButton* dstButton = m_buttonList[i];
        if (srcButton != dstButton) {
            dstButton->setPalette(defaultPalette);
        }
    }

    // Send off the key press
    std::wstring buttonName = srcButton->objectName().toStdWString();
    auto keyCode = key_mapping::translateSet.find(buttonName);
    if (keyCode == key_mapping::translateSet.end())
    {
        throw std::runtime_error("Missing keyCode");
    }
    int32_t virtualKeyCode = keyCode->second;

    // This is only place we call win32 API directly in this file, see windows_subsystem otherwise.
    INPUT input = {};
    input.type = INPUT_KEYBOARD;

    // Long if else chain for handling all QPushButtons keys on the UI.
    bool toggleControl = false;
    bool toggleShift = false;
    // PASS THROUGH PIPE
    if ((virtualKeyCode == VK_ESCAPE) ||
        (virtualKeyCode >= VK_F1 && virtualKeyCode <= VK_F12) ||
        (virtualKeyCode == VK_BACK) ||
        (virtualKeyCode >= 0x30 && virtualKeyCode <= 0x39) || // 0-9 numbers.
        (virtualKeyCode == VK_MEDIA_PLAY_PAUSE) ||
        (virtualKeyCode == VK_VOLUME_MUTE) ||
        (virtualKeyCode == VK_MEDIA_NEXT_TRACK) ||
        (virtualKeyCode == VK_MEDIA_PREV_TRACK) ||
        (virtualKeyCode == VK_VOLUME_UP) ||
        (virtualKeyCode == VK_VOLUME_DOWN) ||
        (virtualKeyCode == VK_SPACE) ||
        (virtualKeyCode == VK_UP) ||
        (virtualKeyCode == VK_DOWN) ||
        (virtualKeyCode == VK_LEFT) ||
        (virtualKeyCode == VK_RIGHT) ||
        (virtualKeyCode == VK_DELETE) ||
        (virtualKeyCode == VK_RETURN) ||
        (virtualKeyCode == VK_SNAPSHOT) ||
        (virtualKeyCode == VK_PRIOR) ||
        (virtualKeyCode == VK_NEXT) ||
        (virtualKeyCode == VK_HOME) ||
        (virtualKeyCode == VK_END) ||
        (virtualKeyCode == VK_TAB) ||
        (virtualKeyCode == VK_APPS) ||
        (virtualKeyCode == VK_INSERT) ||
        (virtualKeyCode == VK_PAUSE))
    {
        input.ki.wVk = virtualKeyCode;
    }
    // ALPHABET
    else if (virtualKeyCode >= 0x41 && virtualKeyCode <= 0x5A)
    {
        input.ki.wVk = virtualKeyCode;
        if (buttonName[11] == ::toupper(buttonName[11]))
        {
            toggleShift = true;
        }
    }
    // VK_OEM_1 EXTENSIONS
    else if (virtualKeyCode == VK_OEM_1)
    {
        input.ki.wVk = VK_OEM_1;
        if (buttonName == L"pushButton_colon")
        {
            toggleShift = true;
        }
    }
    // VK_OEM_2 EXTENSIONS
    else if (virtualKeyCode == VK_OEM_2)
    {
        input.ki.wVk = VK_OEM_2;
        if (buttonName == L"pushButton_questionMark")
        {
            toggleShift = true;
        }
    }
    // VK_OEM_3 EXTENSIONS
    else if (virtualKeyCode == VK_OEM_3)
    {
        input.ki.wVk = VK_OEM_3;
        if (buttonName == L"pushButton_tilde")
        {
            toggleShift = true;
        }
    }
    // VK_OEM_4 EXTENSIONS
    else if (virtualKeyCode == VK_OEM_4)
    {
        input.ki.wVk = VK_OEM_4;
        if (buttonName == L"pushButton_leftBraces")
        {
            toggleShift = true;
        }
    }
    // VK_OEM_5 EXTENSIONS
    else if (virtualKeyCode == VK_OEM_5)
    {
        input.ki.wVk = VK_OEM_5;
        if (buttonName == L"pushButton_verticalSlash")
        {
            toggleShift = true;
        }
    }
    // VK_OEM_6 EXTENSIONS
    else if (virtualKeyCode == VK_OEM_6)
    {
        input.ki.wVk = VK_OEM_6;
        if (buttonName == L"pushButton_rightBraces")
        {
            toggleShift = true;
        }
    }
    // VK_OEM_7 EXTENSIONS
    else if (virtualKeyCode == VK_OEM_7)
    {
        input.ki.wVk = VK_OEM_7;
        if (buttonName == L"pushButton_doubleQuote")
        {
            toggleShift = true;
        }
    }
    // VK_OEM_MINUS EXTENSIONS
    else if (virtualKeyCode == VK_OEM_MINUS)
    {
        input.ki.wVk = VK_OEM_MINUS;
        if (buttonName == L"pushButton_underscore")
        {
            toggleShift = true;
        }
    }
    // VK_OEM_PLUS EXTENSIONS
    else if (virtualKeyCode == VK_OEM_PLUS)
    {
        input.ki.wVk = VK_OEM_PLUS;
        if (buttonName == L"pushButton_plus")
        {
            toggleShift = true;
        }
    }
    // VK_OEM_PERIOD EXTENSIONS
    else if (virtualKeyCode == VK_OEM_PERIOD)
    {
        input.ki.wVk = VK_OEM_PERIOD;
        if (buttonName == L"pushButton_greaterThan")
        {
            toggleShift = true;
        }
    }
    // VK_OEM_COMMA EXTENSIONS
    else if (virtualKeyCode == VK_OEM_COMMA)
    {
        input.ki.wVk = VK_OEM_COMMA;
        if (buttonName == L"pushButton_lessThan")
        {
            toggleShift = true;
        }
    }
    // KEY COMBINATORS
    else if (virtualKeyCode == VK_XTI_CUSTOM_COPY)
    {
        input.ki.wVk = 0x43; // C
        toggleControl = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_PASTE)
    {
        input.ki.wVk = 0x56; // V
        toggleControl = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_EXCLAMATION_MARK)
    {
        input.ki.wVk = 0x31; // 1
        toggleShift = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_AT)
    {
        input.ki.wVk = 0x32; // 2
        toggleShift = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_HASH)
    {
        input.ki.wVk = 0x33; // 3
        toggleShift = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_DOLLAR)
    {
        input.ki.wVk = 0x34; // 4
        toggleShift = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_PERCENT)
    {
        input.ki.wVk = 0x35; // 5
        toggleShift = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_CIRCUMFLEX)
    {
        input.ki.wVk = 0x36; // 6
        toggleShift = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_AMPERSAND)
    {
        input.ki.wVk = 0x37; // 7
        toggleShift = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_ASTERISK)
    {
        input.ki.wVk = 0x38; // 8
        toggleShift = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_LEFT_ROUND_BRACKET)
    {
        input.ki.wVk = 0x39; // 9
        toggleShift = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_RIGHT_ROUND_BRACKET)
    {
        input.ki.wVk = 0x30; // 0
        toggleShift = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_FIND_FILE)
    {
        input.ki.wVk = 0x4E; // N
        toggleShift = true;
        toggleControl = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_FIND)
    {
        input.ki.wVk = 0x46; // F
        toggleControl = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_FIND_ALL)
    {
        input.ki.wVk = 0x46; // F
        toggleShift = true;
        toggleControl = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_CUT)
    {
        input.ki.wVk = 0x58; // X
        toggleControl = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_UNDO)
    {
        input.ki.wVk = 0x5A; // Z
        toggleControl = true;
    }
    else if (virtualKeyCode == VK_XTI_CUSTOM_REDO)
    {
        input.ki.wVk = 0x59; // Y
        toggleControl = true;
    }
    else
    {
        throw std::runtime_error("Unhandled native SendInput translation");
    }
    uint32_t r;
    uint16_t toSendVKC = input.ki.wVk;
    if (toggleControl)
    {
        input.ki.wVk = VK_LCONTROL;
        r = ::SendInput(1, &input, sizeof(input));
        if (r == 0)
        {
            throw std::runtime_error("SendInput() failure");
        }
    }
    if (toggleShift)
    {
        input.ki.wVk = VK_LSHIFT;
        r = ::SendInput(1, &input, sizeof(input));
        if (r == 0)
        {
            throw std::runtime_error("SendInput() failure");
        }
    }
    input.ki.wVk = toSendVKC;
    r = ::SendInput(1, &input, sizeof(input));
    if (r == 0)
    {
        throw std::runtime_error("SendInput() failure");
    }
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    r = ::SendInput(1, &input, sizeof(input));
    if (r == 0)
    {
        throw std::runtime_error("SendInput() failure");
    }
    if (toggleShift) {
        input.ki.wVk = VK_LSHIFT;
        r = ::SendInput(1, &input, sizeof(input));
        if (r == 0)
        {
            throw std::runtime_error("SendInput() failure");
        }
    }
    if (toggleControl)
    {
        input.ki.wVk = VK_LCONTROL;
        r = ::SendInput(1, &input, sizeof(input));
        if (r == 0)
        {
            throw std::runtime_error("SendInput() failure");
        }
    }
}

void main_window::ui_on_shortcuts_above_changed(int32_t index)
{
    open_or_show_app(ui->comboBox_shortcutsAbove->itemData(index));
}

void main_window::ui_on_shortcuts_above_reopen()
{
    open_or_show_app(ui->comboBox_shortcutsAbove->currentData());
}

void main_window::ui_on_shortcuts_below_changed(int32_t index)
{
    open_or_show_app(ui->comboBox_shortcutsBelow->itemData(index));
}

void main_window::ui_on_shortcuts_below_reopen()
{
    open_or_show_app(ui->comboBox_shortcutsBelow->currentData());
}

void main_window::ui_on_move_active_above()
{
    windows_subsystem::move_active_window(true, m_appDimensions);
}

void main_window::ui_on_move_active_below()
{
    windows_subsystem::move_active_window(false, m_appDimensions);
}

void main_window::ui_on_panic()
{
    QApplication::quit();
}

void main_window::ui_on_restart()
{
    // TODO
}
