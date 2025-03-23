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
#include <QProcess>
#include <QPushButton>
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
#include <QVariant>
#include <QEvent>
#include <QList>
#include <QTouchEvent>
#include <QEventPoint>
// 2. System/OS headers
// 3. C++ standard library headers
#include <string>
#include <cctype>
#include <algorithm>
// 4. Project classes
#include "windows_subsystem.h"
#include "key_mapping.h"
#include "error_reporter.h"

const char configError[] = "Invalid XTI config at ~/xti.json. Read the README.md.";

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::main_window)
{
    ui->setupUi(this);

    // STEP 1: Make window top-most with no border + make background translucent.
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_AcceptTouchEvents);

    // STEP 2: Load app config. Assumes UTF-8 encoding.
    QFile configFile(QDir::homePath() + "/xti.json");
    if (!configFile.open(QIODevice::ReadOnly))
    {
        error_reporter::stop(__FILE__, __LINE__, configError);
    }
    QByteArray configData = configFile.readAll();
    configFile.close();
    m_appConfig = QJsonDocument::fromJson(configData);

    // STEP 3: Validate config so we can trust it later.
    if (!m_appConfig.isArray())
    {
        error_reporter::stop(__FILE__, __LINE__, configError);
    }
    QJsonArray configEntries = m_appConfig.array();
    for (qsizetype i = 0; i < configEntries.size(); i++)
    {
        QJsonValueRef entry = configEntries[i];
        if (!entry.isObject())
        {
            error_reporter::stop(__FILE__, __LINE__, configError);
        }
        QJsonObject obj = entry.toObject();
        QJsonObject::iterator displayName = obj.find("displayName");
        QJsonObject::iterator startExePath = obj.find("startExePath");
        QJsonObject::iterator startParams = obj.find("startParams");
        QJsonObject::iterator startWorkingDir = obj.find("startWorkingDir");
        QJsonObject::iterator checkExeName = obj.find("checkExeName");
        QJsonObject::iterator checkTitleName = obj.find("checkTitleName");
        QJsonObject::iterator above = obj.find("above");
        if (displayName == obj.end() ||
            startExePath == obj.end() ||
            startParams == obj.end() ||
            startWorkingDir == obj.end() ||
            checkExeName == obj.end() ||
            checkTitleName == obj.end() ||
            above == obj.end())
        {
            error_reporter::stop(__FILE__, __LINE__, configError);
        }
        if (!displayName->isString() ||
            !startExePath->isString() ||
            !startParams->isString() ||
            !startWorkingDir->isString() ||
            !checkExeName->isString() ||
            !checkTitleName->isString() ||
            !above->isBool())
        {
            error_reporter::stop(__FILE__, __LINE__, configError);
        }

        // Replace / with native \ in windows paths.
        std::wstring startExePathVal = startExePath->toString().toStdWString();
        for (size_t j = 0; j < startExePathVal.size(); j++)
        {
            if (startExePathVal[j] == '/')
            {
                startExePathVal[j] = '\\';
            }
        }
        *startExePath = QJsonValue(QString(startExePathVal));
        std::wstring startWorkingDirVal = startWorkingDir->toString().toStdWString();
        for (size_t j = 0; j < startWorkingDirVal.size(); j++)
        {
            if (startWorkingDirVal[j] == '/')
            {
                startWorkingDirVal[j] = '\\';
            }
        }
        *startWorkingDir = QJsonValue(QString(startWorkingDirVal));

        // Make display names uppercase.
        std::wstring displayNameVal = displayName->toString().toStdWString();
        std::transform(displayNameVal.begin(), displayNameVal.end(), displayNameVal.begin(), ::toupper);
        *displayName = QJsonValue(QString(displayNameVal));
        configEntries[i] = obj;
    }

    // STEP 4: Collecting all keyboard push buttons.
    m_keyButtonList.push_back(ui->pushButton_escape);
    m_keyButtonList.push_back(ui->pushButton_f1);
    m_keyButtonList.push_back(ui->pushButton_f2);
    m_keyButtonList.push_back(ui->pushButton_f3);
    m_keyButtonList.push_back(ui->pushButton_f4);
    m_keyButtonList.push_back(ui->pushButton_f5);
    m_keyButtonList.push_back(ui->pushButton_f6);
    m_keyButtonList.push_back(ui->pushButton_f7);
    m_keyButtonList.push_back(ui->pushButton_f8);
    m_keyButtonList.push_back(ui->pushButton_f9);
    m_keyButtonList.push_back(ui->pushButton_f10);
    m_keyButtonList.push_back(ui->pushButton_f11);
    m_keyButtonList.push_back(ui->pushButton_f12);
    m_keyButtonList.push_back(ui->pushButton_backspace);
    m_keyButtonList.push_back(ui->pushButton_tilde);
    m_keyButtonList.push_back(ui->pushButton_exclamationMark);
    m_keyButtonList.push_back(ui->pushButton_at);
    m_keyButtonList.push_back(ui->pushButton_hash);
    m_keyButtonList.push_back(ui->pushButton_dollar);
    m_keyButtonList.push_back(ui->pushButton_percent);
    m_keyButtonList.push_back(ui->pushButton_next);
    m_keyButtonList.push_back(ui->pushButton_previous);
    m_keyButtonList.push_back(ui->pushButton_circumflex);
    m_keyButtonList.push_back(ui->pushButton_ampersand);
    m_keyButtonList.push_back(ui->pushButton_asterisk);
    m_keyButtonList.push_back(ui->pushButton_leftRoundBracket);
    m_keyButtonList.push_back(ui->pushButton_rightRoundBracket);
    m_keyButtonList.push_back(ui->pushButton_minus);
    m_keyButtonList.push_back(ui->pushButton_graveAccent);
    m_keyButtonList.push_back(ui->pushButton_num1);
    m_keyButtonList.push_back(ui->pushButton_num2);
    m_keyButtonList.push_back(ui->pushButton_num3);
    m_keyButtonList.push_back(ui->pushButton_num4);
    m_keyButtonList.push_back(ui->pushButton_num5);
    m_keyButtonList.push_back(ui->pushButton_mute);
    m_keyButtonList.push_back(ui->pushButton_pause);
    m_keyButtonList.push_back(ui->pushButton_num6);
    m_keyButtonList.push_back(ui->pushButton_num7);
    m_keyButtonList.push_back(ui->pushButton_num8);
    m_keyButtonList.push_back(ui->pushButton_num9);
    m_keyButtonList.push_back(ui->pushButton_num0);
    m_keyButtonList.push_back(ui->pushButton_underscore);
    m_keyButtonList.push_back(ui->pushButton_singleQuote);
    m_keyButtonList.push_back(ui->pushButton_Q);
    m_keyButtonList.push_back(ui->pushButton_W);
    m_keyButtonList.push_back(ui->pushButton_E);
    m_keyButtonList.push_back(ui->pushButton_R);
    m_keyButtonList.push_back(ui->pushButton_T);
    m_keyButtonList.push_back(ui->pushButton_pageUp);
    m_keyButtonList.push_back(ui->pushButton_pageDown);
    m_keyButtonList.push_back(ui->pushButton_Y);
    m_keyButtonList.push_back(ui->pushButton_U);
    m_keyButtonList.push_back(ui->pushButton_I);
    m_keyButtonList.push_back(ui->pushButton_O);
    m_keyButtonList.push_back(ui->pushButton_P);
    m_keyButtonList.push_back(ui->pushButton_equals);
    m_keyButtonList.push_back(ui->pushButton_doubleQuote);
    m_keyButtonList.push_back(ui->pushButton_A);
    m_keyButtonList.push_back(ui->pushButton_S);
    m_keyButtonList.push_back(ui->pushButton_D);
    m_keyButtonList.push_back(ui->pushButton_F);
    m_keyButtonList.push_back(ui->pushButton_G);
    m_keyButtonList.push_back(ui->pushButton_home);
    m_keyButtonList.push_back(ui->pushButton_end);
    m_keyButtonList.push_back(ui->pushButton_H);
    m_keyButtonList.push_back(ui->pushButton_J);
    m_keyButtonList.push_back(ui->pushButton_K);
    m_keyButtonList.push_back(ui->pushButton_L);
    m_keyButtonList.push_back(ui->pushButton_leftSquareBracket);
    m_keyButtonList.push_back(ui->pushButton_plus);
    m_keyButtonList.push_back(ui->pushButton_semicolon);
    m_keyButtonList.push_back(ui->pushButton_Z);
    m_keyButtonList.push_back(ui->pushButton_X);
    m_keyButtonList.push_back(ui->pushButton_C);
    m_keyButtonList.push_back(ui->pushButton_V);
    m_keyButtonList.push_back(ui->pushButton_B);
    m_keyButtonList.push_back(ui->pushButton_tab);
    m_keyButtonList.push_back(ui->pushButton_break);
    m_keyButtonList.push_back(ui->pushButton_N);
    m_keyButtonList.push_back(ui->pushButton_M);
    m_keyButtonList.push_back(ui->pushButton_lessThan);
    m_keyButtonList.push_back(ui->pushButton_greaterThan);
    m_keyButtonList.push_back(ui->pushButton_rightSquareBracket);
    m_keyButtonList.push_back(ui->pushButton_backslash);
    m_keyButtonList.push_back(ui->pushButton_colon);
    m_keyButtonList.push_back(ui->pushButton_q);
    m_keyButtonList.push_back(ui->pushButton_w);
    m_keyButtonList.push_back(ui->pushButton_e);
    m_keyButtonList.push_back(ui->pushButton_r);
    m_keyButtonList.push_back(ui->pushButton_t);
    m_keyButtonList.push_back(ui->pushButton_volumeUp);
    m_keyButtonList.push_back(ui->pushButton_volumeDown);
    m_keyButtonList.push_back(ui->pushButton_y);
    m_keyButtonList.push_back(ui->pushButton_u);
    m_keyButtonList.push_back(ui->pushButton_i);
    m_keyButtonList.push_back(ui->pushButton_o);
    m_keyButtonList.push_back(ui->pushButton_p);
    m_keyButtonList.push_back(ui->pushButton_leftBraces);
    m_keyButtonList.push_back(ui->pushButton_comma);
    m_keyButtonList.push_back(ui->pushButton_a);
    m_keyButtonList.push_back(ui->pushButton_s);
    m_keyButtonList.push_back(ui->pushButton_d);
    m_keyButtonList.push_back(ui->pushButton_f);
    m_keyButtonList.push_back(ui->pushButton_g);
    m_keyButtonList.push_back(ui->pushButton_printScreen);
    m_keyButtonList.push_back(ui->pushButton_numLock);
    m_keyButtonList.push_back(ui->pushButton_h);
    m_keyButtonList.push_back(ui->pushButton_j);
    m_keyButtonList.push_back(ui->pushButton_k);
    m_keyButtonList.push_back(ui->pushButton_l);
    m_keyButtonList.push_back(ui->pushButton_verticalSlash);
    m_keyButtonList.push_back(ui->pushButton_rightBraces);
    m_keyButtonList.push_back(ui->pushButton_fullstop);
    m_keyButtonList.push_back(ui->pushButton_z);
    m_keyButtonList.push_back(ui->pushButton_x);
    m_keyButtonList.push_back(ui->pushButton_c);
    m_keyButtonList.push_back(ui->pushButton_v);
    m_keyButtonList.push_back(ui->pushButton_b);
    m_keyButtonList.push_back(ui->pushButton_shift);
    m_keyButtonList.push_back(ui->pushButton_scrollLock);
    m_keyButtonList.push_back(ui->pushButton_n);
    m_keyButtonList.push_back(ui->pushButton_m);
    m_keyButtonList.push_back(ui->pushButton_slash);
    m_keyButtonList.push_back(ui->pushButton_questionMark);
    m_keyButtonList.push_back(ui->pushButton_menu);
    m_keyButtonList.push_back(ui->pushButton_capsLock);
    m_keyButtonList.push_back(ui->pushButton_space);
    m_keyButtonList.push_back(ui->pushButton_undo);
    m_keyButtonList.push_back(ui->pushButton_redo);
    m_keyButtonList.push_back(ui->pushButton_up);
    m_keyButtonList.push_back(ui->pushButton_down);
    m_keyButtonList.push_back(ui->pushButton_left);
    m_keyButtonList.push_back(ui->pushButton_right);
    m_keyButtonList.push_back(ui->pushButton_control);
    m_keyButtonList.push_back(ui->pushButton_windows);
    m_keyButtonList.push_back(ui->pushButton_alt);
    m_keyButtonList.push_back(ui->pushButton_copy);
    m_keyButtonList.push_back(ui->pushButton_cut);
    m_keyButtonList.push_back(ui->pushButton_paste);
    m_keyButtonList.push_back(ui->pushButton_fileFind);
    m_keyButtonList.push_back(ui->pushButton_find);
    m_keyButtonList.push_back(ui->pushButton_findAll);
    m_keyButtonList.push_back(ui->pushButton_insert);
    m_keyButtonList.push_back(ui->pushButton_delete);
    m_keyButtonList.push_back(ui->pushButton_enter);
    for (size_t i = 0; i < m_keyButtonList.size(); i++)
    {
        // Prevent button corners being able to be pushed-through to desktop.
        m_keyButtonList[i]->setAutoFillBackground(true);
    }

    // STEP 5: Collecting all keyboard push buttons (left side).
    m_keyButtonLeftList.push_back(ui->pushButton_escape);
    m_keyButtonLeftList.push_back(ui->pushButton_f1);
    m_keyButtonLeftList.push_back(ui->pushButton_f2);
    m_keyButtonLeftList.push_back(ui->pushButton_f3);
    m_keyButtonLeftList.push_back(ui->pushButton_f4);
    m_keyButtonLeftList.push_back(ui->pushButton_f5);
    m_keyButtonLeftList.push_back(ui->pushButton_f6);
    m_keyButtonLeftList.push_back(ui->pushButton_tilde);
    m_keyButtonLeftList.push_back(ui->pushButton_exclamationMark);
    m_keyButtonLeftList.push_back(ui->pushButton_at);
    m_keyButtonLeftList.push_back(ui->pushButton_hash);
    m_keyButtonLeftList.push_back(ui->pushButton_dollar);
    m_keyButtonLeftList.push_back(ui->pushButton_percent);
    m_keyButtonLeftList.push_back(ui->pushButton_next);
    m_keyButtonLeftList.push_back(ui->pushButton_graveAccent);
    m_keyButtonLeftList.push_back(ui->pushButton_num1);
    m_keyButtonLeftList.push_back(ui->pushButton_num2);
    m_keyButtonLeftList.push_back(ui->pushButton_num3);
    m_keyButtonLeftList.push_back(ui->pushButton_num4);
    m_keyButtonLeftList.push_back(ui->pushButton_num5);
    m_keyButtonLeftList.push_back(ui->pushButton_mute);
    m_keyButtonLeftList.push_back(ui->pushButton_singleQuote);
    m_keyButtonLeftList.push_back(ui->pushButton_Q);
    m_keyButtonLeftList.push_back(ui->pushButton_W);
    m_keyButtonLeftList.push_back(ui->pushButton_E);
    m_keyButtonLeftList.push_back(ui->pushButton_R);
    m_keyButtonLeftList.push_back(ui->pushButton_T);
    m_keyButtonLeftList.push_back(ui->pushButton_pageUp);
    m_keyButtonLeftList.push_back(ui->pushButton_doubleQuote);
    m_keyButtonLeftList.push_back(ui->pushButton_A);
    m_keyButtonLeftList.push_back(ui->pushButton_S);
    m_keyButtonLeftList.push_back(ui->pushButton_D);
    m_keyButtonLeftList.push_back(ui->pushButton_F);
    m_keyButtonLeftList.push_back(ui->pushButton_G);
    m_keyButtonLeftList.push_back(ui->pushButton_home);
    m_keyButtonLeftList.push_back(ui->pushButton_semicolon);
    m_keyButtonLeftList.push_back(ui->pushButton_Z);
    m_keyButtonLeftList.push_back(ui->pushButton_X);
    m_keyButtonLeftList.push_back(ui->pushButton_C);
    m_keyButtonLeftList.push_back(ui->pushButton_V);
    m_keyButtonLeftList.push_back(ui->pushButton_B);
    m_keyButtonLeftList.push_back(ui->pushButton_tab);
    m_keyButtonLeftList.push_back(ui->pushButton_colon);
    m_keyButtonLeftList.push_back(ui->pushButton_q);
    m_keyButtonLeftList.push_back(ui->pushButton_w);
    m_keyButtonLeftList.push_back(ui->pushButton_e);
    m_keyButtonLeftList.push_back(ui->pushButton_r);
    m_keyButtonLeftList.push_back(ui->pushButton_t);
    m_keyButtonLeftList.push_back(ui->pushButton_volumeUp);
    m_keyButtonLeftList.push_back(ui->pushButton_comma);
    m_keyButtonLeftList.push_back(ui->pushButton_a);
    m_keyButtonLeftList.push_back(ui->pushButton_s);
    m_keyButtonLeftList.push_back(ui->pushButton_d);
    m_keyButtonLeftList.push_back(ui->pushButton_f);
    m_keyButtonLeftList.push_back(ui->pushButton_g);
    m_keyButtonLeftList.push_back(ui->pushButton_printScreen);
    m_keyButtonLeftList.push_back(ui->pushButton_fullstop);
    m_keyButtonLeftList.push_back(ui->pushButton_z);
    m_keyButtonLeftList.push_back(ui->pushButton_x);
    m_keyButtonLeftList.push_back(ui->pushButton_c);
    m_keyButtonLeftList.push_back(ui->pushButton_v);
    m_keyButtonLeftList.push_back(ui->pushButton_b);
    m_keyButtonLeftList.push_back(ui->pushButton_shift);
    m_keyButtonLeftList.push_back(ui->pushButton_space);
    m_keyButtonLeftList.push_back(ui->pushButton_undo);
    m_keyButtonLeftList.push_back(ui->pushButton_control);
    m_keyButtonLeftList.push_back(ui->pushButton_windows);
    m_keyButtonLeftList.push_back(ui->pushButton_alt);
    m_keyButtonLeftList.push_back(ui->pushButton_copy);
    m_keyButtonLeftList.push_back(ui->pushButton_cut);
    m_keyButtonLeftList.push_back(ui->pushButton_paste);

    // STEP 6: Collecting all keyboard push buttons (right side).
    m_keyButtonRightTopList.push_back(ui->pushButton_f7);
    m_keyButtonRightTopList.push_back(ui->pushButton_f8);
    m_keyButtonRightTopList.push_back(ui->pushButton_f9);
    m_keyButtonRightTopList.push_back(ui->pushButton_f10);
    m_keyButtonRightTopList.push_back(ui->pushButton_f11);
    m_keyButtonRightTopList.push_back(ui->pushButton_f12);
    m_keyButtonRightTopList.push_back(ui->pushButton_backspace);
    m_keyButtonRightTopList.push_back(ui->pushButton_previous);
    m_keyButtonRightTopList.push_back(ui->pushButton_circumflex);
    m_keyButtonRightTopList.push_back(ui->pushButton_ampersand);
    m_keyButtonRightTopList.push_back(ui->pushButton_asterisk);
    m_keyButtonRightTopList.push_back(ui->pushButton_leftRoundBracket);
    m_keyButtonRightTopList.push_back(ui->pushButton_rightRoundBracket);
    m_keyButtonRightTopList.push_back(ui->pushButton_minus);
    m_keyButtonRightTopList.push_back(ui->pushButton_pause);
    m_keyButtonRightTopList.push_back(ui->pushButton_num6);
    m_keyButtonRightTopList.push_back(ui->pushButton_num7);
    m_keyButtonRightTopList.push_back(ui->pushButton_num8);
    m_keyButtonRightTopList.push_back(ui->pushButton_num9);
    m_keyButtonRightTopList.push_back(ui->pushButton_num0);
    m_keyButtonRightTopList.push_back(ui->pushButton_underscore);
    m_keyButtonRightTopList.push_back(ui->pushButton_pageDown);
    m_keyButtonRightTopList.push_back(ui->pushButton_Y);
    m_keyButtonRightTopList.push_back(ui->pushButton_U);
    m_keyButtonRightTopList.push_back(ui->pushButton_I);
    m_keyButtonRightTopList.push_back(ui->pushButton_O);
    m_keyButtonRightTopList.push_back(ui->pushButton_P);
    m_keyButtonRightTopList.push_back(ui->pushButton_equals);
    m_keyButtonRightTopList.push_back(ui->pushButton_end);
    m_keyButtonRightTopList.push_back(ui->pushButton_H);
    m_keyButtonRightTopList.push_back(ui->pushButton_J);
    m_keyButtonRightTopList.push_back(ui->pushButton_K);
    m_keyButtonRightTopList.push_back(ui->pushButton_L);
    m_keyButtonRightTopList.push_back(ui->pushButton_leftSquareBracket);
    m_keyButtonRightTopList.push_back(ui->pushButton_plus);
    m_keyButtonRightTopList.push_back(ui->pushButton_break);
    m_keyButtonRightTopList.push_back(ui->pushButton_N);
    m_keyButtonRightTopList.push_back(ui->pushButton_M);
    m_keyButtonRightTopList.push_back(ui->pushButton_lessThan);
    m_keyButtonRightTopList.push_back(ui->pushButton_greaterThan);
    m_keyButtonRightTopList.push_back(ui->pushButton_rightSquareBracket);
    m_keyButtonRightTopList.push_back(ui->pushButton_backslash);
    m_keyButtonRightBottomList.push_back(ui->pushButton_volumeDown);
    m_keyButtonRightBottomList.push_back(ui->pushButton_y);
    m_keyButtonRightBottomList.push_back(ui->pushButton_u);
    m_keyButtonRightBottomList.push_back(ui->pushButton_i);
    m_keyButtonRightBottomList.push_back(ui->pushButton_o);
    m_keyButtonRightBottomList.push_back(ui->pushButton_p);
    m_keyButtonRightBottomList.push_back(ui->pushButton_leftBraces);
    m_keyButtonRightBottomList.push_back(ui->pushButton_numLock);
    m_keyButtonRightBottomList.push_back(ui->pushButton_h);
    m_keyButtonRightBottomList.push_back(ui->pushButton_j);
    m_keyButtonRightBottomList.push_back(ui->pushButton_k);
    m_keyButtonRightBottomList.push_back(ui->pushButton_l);
    m_keyButtonRightBottomList.push_back(ui->pushButton_verticalSlash);
    m_keyButtonRightBottomList.push_back(ui->pushButton_rightBraces);
    m_keyButtonRightBottomList.push_back(ui->pushButton_scrollLock);
    m_keyButtonRightBottomList.push_back(ui->pushButton_n);
    m_keyButtonRightBottomList.push_back(ui->pushButton_m);
    m_keyButtonRightBottomList.push_back(ui->pushButton_slash);
    m_keyButtonRightBottomList.push_back(ui->pushButton_questionMark);
    m_keyButtonRightBottomList.push_back(ui->pushButton_menu);
    m_keyButtonRightBottomList.push_back(ui->pushButton_capsLock);
    m_keyButtonRightBottomList.push_back(ui->pushButton_redo);
    m_keyButtonRightBottomList.push_back(ui->pushButton_up);
    m_keyButtonRightBottomList.push_back(ui->pushButton_down);
    m_keyButtonRightBottomList.push_back(ui->pushButton_left);
    m_keyButtonRightBottomList.push_back(ui->pushButton_right);
    m_keyButtonRightBottomList.push_back(ui->pushButton_fileFind);
    m_keyButtonRightBottomList.push_back(ui->pushButton_find);
    m_keyButtonRightBottomList.push_back(ui->pushButton_findAll);
    m_keyButtonRightBottomList.push_back(ui->pushButton_insert);
    m_keyButtonRightBottomList.push_back(ui->pushButton_delete);
    m_keyButtonRightBottomList.push_back(ui->pushButton_enter);

    // STEP 7: Inserting configured shortcuts.
    for (QJsonArray::iterator entry = configEntries.begin(); entry != configEntries.end(); ++entry)
    {
        QJsonObject obj = entry->toObject();
        if (obj.find("above")->toBool())
        {
            ui->comboBox_shortcutsAbove->addItem(obj.find("displayName")->toString(), obj);
        }
        else
        {
            ui->comboBox_shortcutsBelow->addItem(obj.find("displayName")->toString(), obj);
        }
    }

    // STEP 8: Hook QT buttons and controls.
    for (size_t i = 0; i < m_keyButtonList.size(); i++)
    {
        connect(m_keyButtonList[i], &QPushButton::clicked, this, &main_window::ui_on_key_press);
    }
    connect(ui->pushButton_reopenAbove, &QPushButton::clicked, this, &main_window::ui_on_shortcuts_above_reopen);
    connect(ui->comboBox_shortcutsAbove, &QComboBox::currentIndexChanged, this, &main_window::ui_on_shortcuts_above_changed);
    connect(ui->pushButton_reopenBelow, &QPushButton::clicked, this, &main_window::ui_on_shortcuts_below_reopen);
    connect(ui->comboBox_shortcutsBelow, &QComboBox::currentIndexChanged, this, &main_window::ui_on_shortcuts_below_changed);
    connect(ui->pushButton_moveAbove, &QPushButton::clicked, this, &main_window::ui_on_move_active_above);
    connect(ui->pushButton_moveBelow, &QPushButton::clicked, this, &main_window::ui_on_move_active_below);
    connect(ui->pushButton_panic, &QPushButton::clicked, this, &main_window::ui_on_panic);
    connect(ui->pushButton_restart, &QPushButton::clicked, this, &main_window::ui_on_restart);

    // STEP 9: Final system setup.
    // windows_subsystem::initialize_apply_system_super_admin_privilege(); --- not needed at this time. see cpp definition in file for more info.
    // windows_subsystem::initialize_force_cursor_visible(); --- not needed at this time.  see cpp definition in file for more info.
    windows_subsystem::initialize_apply_keyboard_window_style(reinterpret_cast<HWND>(winId()));
    key_mapping::initialize();
    // continue at post_ctor after win32 message pump has had the opportunity to process above changes.
    QTimer::singleShot(0, this, &main_window::ui_on_post_ctor);
}

main_window::~main_window()
{
    delete ui;
}

void main_window::ui_on_post_ctor() {
    // Needs to be after the window has been constructed, otherwise certain resize values get ignored.
    m_appDimensions = windows_subsystem::initialize_orientate_main_window(reinterpret_cast<HWND>(winId()));
    m_keyModifiers = windows_subsystem::get_key_modifiers();
    update_modifier_colors();

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &main_window::ui_on_state_refresher_loop);
    timer->start(3000);
}

void main_window::ui_on_state_refresher_loop()
{
    ui->label_activeWindow->setText(QString::fromStdWString(windows_subsystem::get_focus_window_name()));
    m_keyModifiers = windows_subsystem::get_key_modifiers();
    update_modifier_colors();
}

void main_window::open_or_show_app(const QVariant& shortcutConfig)
{
    QJsonObject jsonObj = shortcutConfig.toJsonObject();
    std::wstring checkExeName = jsonObj.find("checkExeName")->toString().toStdWString();
    std::wstring checkTitleName = jsonObj.find("checkTitleName")->toString().toStdWString();
    bool isAbove = jsonObj.find("above")->toBool();
    if (windows_subsystem::is_process_running(checkExeName))
    {
        HWND window = windows_subsystem::get_window(checkExeName, checkTitleName);
        if (window != nullptr)
        {
            windows_subsystem::move_window(window, isAbove, m_appDimensions);
            return;
        }
    }

    // Not found, start it.
    std::wstring startExePath = jsonObj.find("startExePath")->toString().toStdWString();
    std::wstring startParams = jsonObj.find("startParams")->toString().toStdWString();
    std::wstring startWorkingDir = jsonObj.find("startWorkingDir")->toString().toStdWString();
    windows_subsystem::start_process(startExePath, startParams, startWorkingDir, checkExeName, checkTitleName, isAbove, m_appDimensions);
}

void main_window::ui_on_key_press()
{
    QPushButton* srcButton = qobject_cast<QPushButton*>(sender());
    bool modChanged = false;
    bool modOn = false;
    // Key press back color changes -> Key modifiers and locks.
    if (srcButton == ui->pushButton_shift ||
        srcButton == ui->pushButton_control ||
        srcButton == ui->pushButton_windows ||
        srcButton == ui->pushButton_alt ||
        srcButton == ui->pushButton_scrollLock ||
        srcButton == ui->pushButton_numLock ||
        srcButton == ui->pushButton_capsLock)
    {
        modChanged = true;
    }
    // Everything else
    else
    {
        QPalette palette = srcButton->palette();
        palette.setColor(QPalette::Button, Qt::blue);
        srcButton->setPalette(palette);

        QPalette defaultPalette = QApplication::palette();
        for (size_t i = 0; i < m_keyButtonList.size(); i++)
        {
            QPushButton* dstButton = m_keyButtonList[i];
            if (dstButton == ui->pushButton_shift ||
                dstButton == ui->pushButton_control ||
                dstButton == ui->pushButton_windows ||
                dstButton == ui->pushButton_alt ||
                dstButton == ui->pushButton_scrollLock ||
                dstButton == ui->pushButton_numLock ||
                dstButton == ui->pushButton_capsLock)
            {
                continue;
            }
            if (srcButton != dstButton) {
                dstButton->setPalette(defaultPalette);
            }
        }
    }

    // Send off the key press.
    std::wstring buttonName = srcButton->objectName().toStdWString();
    auto keyCode = key_mapping::translateSet.find(buttonName);
    if (keyCode == key_mapping::translateSet.end())
    {
        error_reporter::stop(__FILE__, __LINE__, "Missing key_mapping entry for pushButton.");
    }
    int32_t virtualKeyCode = keyCode->second;

    // This is only place we call win32 API directly in this file, see windows_subsystem otherwise.
    ::INPUT input = {};
    input.type = INPUT_KEYBOARD;
    int32_t r;

    // Long if else chain for handling all QPushButton keyboard keys on the UI.
    bool toggleControl = false;
    bool toggleShift = false;
    // CHAIN 1: PASS THROUGH PIPE
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
    // CHAIN 2: ALPHABET
    else if (virtualKeyCode >= 0x41 && virtualKeyCode <= 0x5A)
    {
        input.ki.wVk = virtualKeyCode;
        if (buttonName[11] == ::toupper(buttonName[11]))
        {
            toggleShift = true;
        }
    }
    // CHAIN 3: VK_OEM_1 EXTENSIONS
    else if (virtualKeyCode == VK_OEM_1)
    {
        input.ki.wVk = VK_OEM_1;
        if (buttonName == L"pushButton_colon")
        {
            toggleShift = true;
        }
    }
    // CHAIN 4: VK_OEM_2 EXTENSIONS
    else if (virtualKeyCode == VK_OEM_2)
    {
        input.ki.wVk = VK_OEM_2;
        if (buttonName == L"pushButton_questionMark")
        {
            toggleShift = true;
        }
    }
    // CHAIN 5: VK_OEM_3 EXTENSIONS
    else if (virtualKeyCode == VK_OEM_3)
    {
        input.ki.wVk = VK_OEM_3;
        if (buttonName == L"pushButton_tilde")
        {
            toggleShift = true;
        }
    }
    // CHAIN 6: VK_OEM_4 EXTENSIONS
    else if (virtualKeyCode == VK_OEM_4)
    {
        input.ki.wVk = VK_OEM_4;
        if (buttonName == L"pushButton_leftBraces")
        {
            toggleShift = true;
        }
    }
    // CHAIN 7: VK_OEM_5 EXTENSIONS
    else if (virtualKeyCode == VK_OEM_5)
    {
        input.ki.wVk = VK_OEM_5;
        if (buttonName == L"pushButton_verticalSlash")
        {
            toggleShift = true;
        }
    }
    // CHAIN 8: VK_OEM_6 EXTENSIONS
    else if (virtualKeyCode == VK_OEM_6)
    {
        input.ki.wVk = VK_OEM_6;
        if (buttonName == L"pushButton_rightBraces")
        {
            toggleShift = true;
        }
    }
    // CHAIN 9: VK_OEM_7 EXTENSIONS
    else if (virtualKeyCode == VK_OEM_7)
    {
        input.ki.wVk = VK_OEM_7;
        if (buttonName == L"pushButton_doubleQuote")
        {
            toggleShift = true;
        }
    }
    // CHAIN 10: VK_OEM_MINUS EXTENSIONS
    else if (virtualKeyCode == VK_OEM_MINUS)
    {
        input.ki.wVk = VK_OEM_MINUS;
        if (buttonName == L"pushButton_underscore")
        {
            toggleShift = true;
        }
    }
    // CHAIN 11: VK_OEM_PLUS EXTENSIONS
    else if (virtualKeyCode == VK_OEM_PLUS)
    {
        input.ki.wVk = VK_OEM_PLUS;
        if (buttonName == L"pushButton_plus")
        {
            toggleShift = true;
        }
    }
    // CHAIN 12: VK_OEM_PERIOD EXTENSIONS
    else if (virtualKeyCode == VK_OEM_PERIOD)
    {
        input.ki.wVk = VK_OEM_PERIOD;
        if (buttonName == L"pushButton_greaterThan")
        {
            toggleShift = true;
        }
    }
    // CHAIN 13: VK_OEM_COMMA EXTENSIONS
    else if (virtualKeyCode == VK_OEM_COMMA)
    {
        input.ki.wVk = VK_OEM_COMMA;
        if (buttonName == L"pushButton_lessThan")
        {
            toggleShift = true;
        }
    }
    // CHAIN 14: NUMBERS SHIFT
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
    // CHAIN 15: CUSTOM KEY COMBINATORS
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
    // CHAIN 16: KEY LOCKS
    else if (virtualKeyCode == VK_CAPITAL ||
             virtualKeyCode == VK_SCROLL ||
             virtualKeyCode == VK_NUMLOCK)
    {
        switch (virtualKeyCode) {
        case VK_CAPITAL:
            modOn = !m_keyModifiers.capsLock;
            break;
        case VK_SCROLL:
            modOn = !m_keyModifiers.scrollLock;
            break;
        case VK_NUMLOCK:
            modOn = !m_keyModifiers.numLock;
            break;
        }
        input.ki.wVk = virtualKeyCode;
    }
    // CHAIN 17: KEY MODIFIERS
    else if (virtualKeyCode == VK_RCONTROL ||
               virtualKeyCode == VK_RSHIFT ||
               virtualKeyCode == VK_RMENU ||
               virtualKeyCode == VK_RWIN)
    {
        // Modifiers stay down or up using the right-side virtual key codes.
        // Prevents the rest of the virtual keyboard from messing with it.
        bool currentlyDown = false;
        switch (virtualKeyCode)
        {
        case VK_RCONTROL:
            currentlyDown = m_keyModifiers.control;
            break;
        case VK_RSHIFT:
            currentlyDown = m_keyModifiers.shift;
            break;
        case VK_RMENU:
            currentlyDown = m_keyModifiers.alt;
            break;
        case VK_RWIN:
            currentlyDown = m_keyModifiers.windows;
            break;
        }
        if (currentlyDown)
        {
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            if (virtualKeyCode == VK_RCONTROL || virtualKeyCode == VK_RMENU)
            {
                // https://learn.microsoft.com/en-us/windows/win32/inputdev/about-keyboard-input go to Extended-key flag
                // Without this the keys get stuck in down mode by the OS.
                input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
            }
        }
        input.ki.wVk = virtualKeyCode;
        r = ::SendInput(1, &input, sizeof(input));
        if (r == 0)
        {
            error_reporter::stop(__FILE__, __LINE__, "Win32::SendInput() failure.");
        }
        post_key_press(srcButton, modChanged, !currentlyDown);
        // since we are emulating control and other non-lock modifier keys we
        // return early so we can leave the keys pressed down.
        return;
    }
    else
    {
        error_reporter::stop(__FILE__, __LINE__, "Missing key_mapping to virtual key translation for pushButton.");
    }
    uint16_t toSendVKC = input.ki.wVk;
    if (toggleControl)
    {
        input.ki.wVk = VK_LCONTROL;
        r = ::SendInput(1, &input, sizeof(input));
        if (r == 0)
        {
            error_reporter::stop(__FILE__, __LINE__, "Win32::SendInput() failure.");
        }
    }
    if (toggleShift)
    {
        input.ki.wVk = VK_LSHIFT;
        r = ::SendInput(1, &input, sizeof(input));
        if (r == 0)
        {
            error_reporter::stop(__FILE__, __LINE__, "Win32::SendInput() failure.");
        }
    }
    input.ki.wVk = toSendVKC;
    r = ::SendInput(1, &input, sizeof(input));
    if (r == 0)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::SendInput() failure.");
    }
    // https://learn.microsoft.com/en-us/windows/win32/inputdev/about-keyboard-input go to Extended-key flag
    // Without this the keys get stuck in down mode by the OS.
    if (toSendVKC == VK_RMENU ||
        toSendVKC == VK_RCONTROL ||
        toSendVKC == VK_INSERT ||
        toSendVKC == VK_DELETE ||
        toSendVKC == VK_HOME ||
        toSendVKC == VK_END ||
        toSendVKC == VK_PRIOR ||
        toSendVKC == VK_NEXT ||
        toSendVKC == VK_UP ||
        toSendVKC == VK_DOWN ||
        toSendVKC == VK_LEFT ||
        toSendVKC == VK_RIGHT)
    {
        input.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_EXTENDEDKEY;
    }
    else
    {
        input.ki.dwFlags = KEYEVENTF_KEYUP;
    }
    r = ::SendInput(1, &input, sizeof(input));
    if (r == 0)
    {
        error_reporter::stop(__FILE__, __LINE__, "Win32::SendInput() failure.");
    }
    input.ki.wVk = KEYEVENTF_KEYUP;
    if (toggleShift) {
        input.ki.wVk = VK_LSHIFT;
        r = ::SendInput(1, &input, sizeof(input));
        if (r == 0)
        {
            error_reporter::stop(__FILE__, __LINE__, "Win32::SendInput() failure.");
        }
    }
    if (toggleControl)
    {
        input.ki.wVk = VK_LCONTROL;
        r = ::SendInput(1, &input, sizeof(input));
        if (r == 0)
        {
            error_reporter::stop(__FILE__, __LINE__, "Win32::SendInput() failure.");
        }
    }
    post_key_press(srcButton, modChanged, modOn);
}

void main_window::post_key_press(QPushButton* srcButton, bool modChanged, bool modOn)
{
    if (modChanged)
    {
        m_keyModifiers = windows_subsystem::get_key_modifiers();
        update_modifier_colors();
    }
    if (srcButton == ui->pushButton_shift ||
        srcButton == ui->pushButton_control ||
        srcButton == ui->pushButton_windows ||
        srcButton == ui->pushButton_alt ||
        srcButton == ui->pushButton_scrollLock ||
        srcButton == ui->pushButton_numLock ||
        srcButton == ui->pushButton_capsLock)
    {
        ui->label_activeKey->setText(srcButton->text() + QString(modOn ? L" ON" : L" OFF"));
    }
    else
    {
        ui->label_activeKey->setText(srcButton->text());
    }

    // Flash the touched key cyan.
    QPalette palette = ui->label_activeKey->palette();
    palette.setColor(QPalette::WindowText, Qt::cyan);
    ui->label_activeKey->setPalette(palette);
    if (m_activeKeyColorTimer == nullptr)
    {
        m_activeKeyColorTimer = new QTimer(this);
        m_activeKeyColorTimer->setSingleShot(true);
        connect(m_activeKeyColorTimer, &QTimer::timeout, this, &main_window::ui_on_key_press_fade);
    }
    else
    {
        m_activeKeyColorTimer->stop();
    }
    m_activeKeyColorTimer->start(200);
}

void main_window::ui_on_key_press_fade()
{
    QPalette defaultPalette = QApplication::palette();
    ui->label_activeKey->setPalette(defaultPalette);
}

void main_window::update_modifier_colors()
{
    QPalette defaultPalette = QApplication::palette();
    if (m_keyModifiers.shift)
    {
        QPalette palette = ui->pushButton_shift->palette();
        palette.setColor(QPalette::Button, Qt::darkCyan);
        ui->pushButton_shift->setPalette(palette);
    }
    else
    {
        ui->pushButton_shift->setPalette(defaultPalette);
    }
    if (m_keyModifiers.control)
    {
        QPalette palette = ui->pushButton_control->palette();
        palette.setColor(QPalette::Button, Qt::darkCyan);
        ui->pushButton_control->setPalette(palette);
    }
    else
    {
        ui->pushButton_control->setPalette(defaultPalette);
    }
    if (m_keyModifiers.alt)
    {
        QPalette palette = ui->pushButton_alt->palette();
        palette.setColor(QPalette::Button, Qt::darkCyan);
        ui->pushButton_alt->setPalette(palette);
    }
    else
    {
        ui->pushButton_alt->setPalette(defaultPalette);
    }
    if (m_keyModifiers.windows)
    {
        QPalette palette = ui->pushButton_windows->palette();
        palette.setColor(QPalette::Button, Qt::darkCyan);
        ui->pushButton_windows->setPalette(palette);
    }
    else
    {
        ui->pushButton_windows->setPalette(defaultPalette);
    }
    if (m_keyModifiers.capsLock)
    {
        QPalette palette = ui->pushButton_capsLock->palette();
        palette.setColor(QPalette::Button, Qt::darkCyan);
        ui->pushButton_capsLock->setPalette(palette);
    }
    else
    {
        ui->pushButton_capsLock->setPalette(defaultPalette);
    }
    if (m_keyModifiers.numLock)
    {
        QPalette palette = ui->pushButton_numLock->palette();
        palette.setColor(QPalette::Button, Qt::darkCyan);
        ui->pushButton_numLock->setPalette(palette);
    }
    else
    {
        ui->pushButton_numLock->setPalette(defaultPalette);
    }
    if (m_keyModifiers.scrollLock)
    {
        QPalette palette = ui->pushButton_scrollLock->palette();
        palette.setColor(QPalette::Button, Qt::darkCyan);
        ui->pushButton_scrollLock->setPalette(palette);
    }
    else
    {
        ui->pushButton_scrollLock->setPalette(defaultPalette);
    }
}

bool main_window::event(QEvent* event) {
    if (event->type() == QEvent::TouchBegin ||
        event->type() == QEvent::TouchUpdate ||
        event->type() == QEvent::TouchEnd) {
        QTouchEvent* touchEvent = dynamic_cast<QTouchEvent*>(event);
        for (QList<QEventPoint>::const_iterator touch = touchEvent->points().begin(); touch != touchEvent->points().end(); ++touch)
        {
            if (!m_cursorIsMoving && event->type() == QEvent::TouchBegin && touch->id() == 0)
            {
                QPushButton* topLeftKey = m_keyButtonLeftList[0];
                QPushButton* bottomRightKey = m_keyButtonLeftList[m_keyButtonLeftList.size() - 1];
                qDebug() << "topLeftKey" << topLeftKey->pos() << "position" << touch->position() << "bottomRightKey" << bottomRightKey->pos();
                if (topLeftKey->pos().x() <= touch->position().x() &&
                    topLeftKey->pos().y() <= touch->position().y() &&
                    bottomRightKey->pos().x() + bottomRightKey->width() > touch->position().x() &&
                    bottomRightKey->pos().y() + bottomRightKey->height() > touch->position().y())
                {
                    POINT startPos;
                    int32_t r = ::GetCursorPos(&startPos);
                    if (r == 0)
                    {
                        error_reporter::stop(__FILE__, __LINE__, "Win32::GetCursorPos() failure.");
                    }
                    m_cursorIsMoving = true;
                    qDebug() << "m_cursorIsMoving = true";
                    m_cursorStartPosition.setX(startPos.x);
                    m_cursorStartPosition.setY(startPos.y);
                }
            }
            if (m_cursorIsMoving)
            {
                QPoint diff = touch->globalPosition().toPoint() - touch->globalPressPosition().toPoint();
                qDebug() << diff;
            }
        }
        if (event->type() == QEvent::TouchEnd)
        {
            m_cursorIsMoving = false;
            qDebug() << "m_cursorIsMoving = false";
        }
    }
    if (event->type() == QEvent::TouchBegin)
    {
        return true;
    }
    return QMainWindow::event(event);
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
    qApp->quit();
}

void main_window::ui_on_restart()
{
    // restarting at same location should be OK.
    // nothing conflicts with system running two at once for short period.
    // this won't work when debugging.
    qApp->quit();
    QString program = qApp->arguments()[0];
    QStringList arguments = qApp->arguments().mid(1);
    QProcess::startDetached(program, arguments);
}
