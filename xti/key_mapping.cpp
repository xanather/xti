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

#include "key_mapping.h"

// 1. Qt framework headers
// 2. System/OS headers
// 3. C++ standard library headers
// 4. Project classes

std::unordered_map<std::wstring, int32_t> key_mapping::translateSet;

void key_mapping::initialize()
{
    /*HANDLED*/ translateSet[L"pushButton_escape"] = VK_ESCAPE;
    /*HANDLED*/ translateSet[L"pushButton_f1"] = VK_F1;
    /*HANDLED*/ translateSet[L"pushButton_f2"] = VK_F2;
    /*HANDLED*/ translateSet[L"pushButton_f3"] = VK_F3;
    /*HANDLED*/ translateSet[L"pushButton_f4"] = VK_F4;
    /*HANDLED*/ translateSet[L"pushButton_f5"] = VK_F5;
    /*HANDLED*/ translateSet[L"pushButton_f6"] = VK_F6;
    /*HANDLED*/ translateSet[L"pushButton_f7"] = VK_F7;
    /*HANDLED*/ translateSet[L"pushButton_f8"] = VK_F8;
    /*HANDLED*/ translateSet[L"pushButton_f9"] = VK_F9;
    /*HANDLED*/ translateSet[L"pushButton_f10"] = VK_F10;
    /*HANDLED*/ translateSet[L"pushButton_f11"] = VK_F11;
    /*HANDLED*/ translateSet[L"pushButton_f12"] = VK_F12;
    /*HANDLED*/ translateSet[L"pushButton_backspace"] = VK_BACK;
    /*HANDLED*/ translateSet[L"pushButton_tilde"] = VK_OEM_3;
    /*HANDLED*/ translateSet[L"pushButton_exclamationMark"] = VK_XTI_CUSTOM_EXCLAMATION_MARK;
    /*HANDLED*/ translateSet[L"pushButton_at"] = VK_XTI_CUSTOM_AT;
    /*HANDLED*/ translateSet[L"pushButton_hash"] = VK_XTI_CUSTOM_HASH;
    /*HANDLED*/ translateSet[L"pushButton_dollar"] = VK_XTI_CUSTOM_DOLLAR;
    /*HANDLED*/ translateSet[L"pushButton_percent"] = VK_XTI_CUSTOM_PERCENT;
    /*HANDLED*/ translateSet[L"pushButton_next"] = VK_MEDIA_NEXT_TRACK;
    /*HANDLED*/ translateSet[L"pushButton_previous"] = VK_MEDIA_PREV_TRACK;
    /*HANDLED*/ translateSet[L"pushButton_circumflex"] = VK_XTI_CUSTOM_CIRCUMFLEX;
    /*HANDLED*/ translateSet[L"pushButton_ampersand"] = VK_XTI_CUSTOM_AMPERSAND;
    /*HANDLED*/ translateSet[L"pushButton_asterisk"] = VK_XTI_CUSTOM_ASTERISK;
    /*HANDLED*/ translateSet[L"pushButton_leftRoundBracket"] = VK_XTI_CUSTOM_LEFT_ROUND_BRACKET;
    /*HANDLED*/ translateSet[L"pushButton_rightRoundBracket"] = VK_XTI_CUSTOM_RIGHT_ROUND_BRACKET;
    /*HANDLED*/ translateSet[L"pushButton_minus"] = VK_OEM_MINUS;
    /*HANDLED*/ translateSet[L"pushButton_graveAccent"] = VK_OEM_3;
    /*HANDLED*/ translateSet[L"pushButton_num1"] = 0x31;
    /*HANDLED*/ translateSet[L"pushButton_num2"] = 0x32;
    /*HANDLED*/ translateSet[L"pushButton_num3"] = 0x33;
    /*HANDLED*/ translateSet[L"pushButton_num4"] = 0x34;
    /*HANDLED*/ translateSet[L"pushButton_num5"] = 0x35;
    /*HANDLED*/ translateSet[L"pushButton_mute"] = VK_VOLUME_MUTE;
    /*HANDLED*/ translateSet[L"pushButton_pause"] = VK_MEDIA_PLAY_PAUSE;
    /*HANDLED*/ translateSet[L"pushButton_num6"] = 0x36;
    /*HANDLED*/ translateSet[L"pushButton_num7"] = 0x37;
    /*HANDLED*/ translateSet[L"pushButton_num8"] = 0x38;
    /*HANDLED*/ translateSet[L"pushButton_num9"] = 0x39;
    /*HANDLED*/ translateSet[L"pushButton_num0"] = 0x30;
    /*HANDLED*/ translateSet[L"pushButton_underscore"] = VK_OEM_MINUS;
    /*HANDLED*/ translateSet[L"pushButton_singleQuote"] = VK_OEM_7;
    translateSet[L"pushButton_Q"] = 0x51;
    translateSet[L"pushButton_W"] = 0x57;
    translateSet[L"pushButton_E"] = 0x45;
    translateSet[L"pushButton_R"] = 0x52;
    translateSet[L"pushButton_T"] = 0x54;
    translateSet[L"pushButton_pageUp"] = VK_PRIOR;
    translateSet[L"pushButton_pageDown"] = VK_NEXT;
    translateSet[L"pushButton_Y"] = 0x59;
    translateSet[L"pushButton_U"] = 0x55;
    translateSet[L"pushButton_I"] = 0x49;
    translateSet[L"pushButton_O"] = 0x4F;
    translateSet[L"pushButton_P"] = 0x50;
    translateSet[L"pushButton_equals"] = VK_NEED_MORE_INFO;
    /*HANDLED*/ translateSet[L"pushButton_doubleQuote"] = VK_OEM_7;
    translateSet[L"pushButton_A"] = 0x41;
    translateSet[L"pushButton_S"] = 0x53;
    translateSet[L"pushButton_D"] = 0x44;
    translateSet[L"pushButton_F"] = 0x46;
    translateSet[L"pushButton_G"] = 0x47;
    translateSet[L"pushButton_home"] = VK_HOME;
    translateSet[L"pushButton_end"] = VK_END;
    translateSet[L"pushButton_H"] = 0x48;
    translateSet[L"pushButton_J"] = 0x4A;
    translateSet[L"pushButton_K"] = 0x4B;
    translateSet[L"pushButton_L"] = 0x4C;
    /*HANDLED*/ translateSet[L"pushButton_leftSquareBracket"] = VK_OEM_4;
    translateSet[L"pushButton_plus"] = VK_ADD;
    translateSet[L"pushButton_semicolon"] = VK_OEM_1;
    translateSet[L"pushButton_Z"] = 0x5A;
    translateSet[L"pushButton_X"] = 0x58;
    translateSet[L"pushButton_C"] = 0x43;
    translateSet[L"pushButton_V"] = 0x56;
    translateSet[L"pushButton_B"] = 0x42;
    translateSet[L"pushButton_tab"] = VK_TAB;
    translateSet[L"pushButton_break"] = VK_NEED_MORE_INFO;
    translateSet[L"pushButton_N"] = 0x4E;
    translateSet[L"pushButton_M"] = 0x4D;
    translateSet[L"pushButton_lessThan"] = VK_OEM_102;
    translateSet[L"pushButton_greaterThan"] = VK_OEM_102;
    /*HANDLED*/ translateSet[L"pushButton_rightSquareBracket"] = VK_OEM_6;
    translateSet[L"pushButton_backslash"] = VK_OEM_5;
    translateSet[L"pushButton_colon"] = VK_OEM_1;
    translateSet[L"pushButton_q"] = 0x51;
    translateSet[L"pushButton_w"] = 0x57;
    translateSet[L"pushButton_e"] = 0x45;
    translateSet[L"pushButton_r"] = 0x52;
    translateSet[L"pushButton_t"] = 0x54;
    /*HANDLED*/ translateSet[L"pushButton_volumeUp"] = VK_VOLUME_UP;
    /*HANDLED*/ translateSet[L"pushButton_volumeDown"] = VK_VOLUME_DOWN;
    translateSet[L"pushButton_y"] = 0x59;
    translateSet[L"pushButton_u"] = 0x55;
    translateSet[L"pushButton_i"] = 0x49;
    translateSet[L"pushButton_o"] = 0x4F;
    translateSet[L"pushButton_p"] = 0x50;
    /*HANDLED*/ translateSet[L"pushButton_leftBraces"] = VK_OEM_4;
    translateSet[L"pushButton_comma"] = VK_OEM_COMMA;
    translateSet[L"pushButton_a"] = 0x41;
    translateSet[L"pushButton_s"] = 0x53;
    translateSet[L"pushButton_d"] = 0x44;
    translateSet[L"pushButton_f"] = 0x46;
    translateSet[L"pushButton_g"] = 0x47;
    translateSet[L"pushButton_printScreen"] = VK_SNAPSHOT;
    translateSet[L"pushButton_numLock"] = VK_NUMLOCK;
    translateSet[L"pushButton_h"] = 0x48;
    translateSet[L"pushButton_j"] = 0x4A;
    translateSet[L"pushButton_k"] = 0x4B;
    translateSet[L"pushButton_l"] = 0x4C;
    translateSet[L"pushButton_verticalSlash"] = VK_OEM_5;
    /*HANDLED*/ translateSet[L"pushButton_rightBraces"] = VK_OEM_6;
    /*HANDLED*/ translateSet[L"pushButton_fullstop"] = VK_OEM_PERIOD;
    translateSet[L"pushButton_z"] = 0x5A;
    translateSet[L"pushButton_x"] = 0x58;
    translateSet[L"pushButton_c"] = 0x43;
    translateSet[L"pushButton_v"] = 0x56;
    translateSet[L"pushButton_b"] = 0x42;
    translateSet[L"pushButton_shift"] = VK_SHIFT;
    translateSet[L"pushButton_scrollLock"] = VK_SCROLL;
    translateSet[L"pushButton_n"] = 0x4E;
    translateSet[L"pushButton_m"] = 0x4D;
    /*HANDLED*/ translateSet[L"pushButton_slash"] = VK_OEM_2;
    /*HANDLED*/ translateSet[L"pushButton_questionMark"] = VK_OEM_2;
    translateSet[L"pushButton_menu"] = VK_NEED_MORE_INFO;
    translateSet[L"pushButton_capsLock"] = VK_CAPITAL;
    /*HANDLED*/ translateSet[L"pushButton_space"] = VK_SPACE;
    translateSet[L"pushButton_undo"] = VK_NEED_MORE_INFO;
    translateSet[L"pushButton_redo"] = VK_NEED_MORE_INFO;
    /*HANDLED*/ translateSet[L"pushButton_up"] = VK_UP;
    /*HANDLED*/ translateSet[L"pushButton_down"] = VK_DOWN;
    /*HANDLED*/ translateSet[L"pushButton_left"] = VK_LEFT;
    /*HANDLED*/ translateSet[L"pushButton_right"] = VK_RIGHT;
    translateSet[L"pushButton_control"] = VK_LCONTROL;
    translateSet[L"pushButton_windows"] = VK_LWIN;
    translateSet[L"pushButton_alt"] = VK_LMENU;
    /*HANDLED*/ translateSet[L"pushButton_copy"] = VK_XTI_CUSTOM_COPY;
    translateSet[L"pushButton_cut"] = VK_NEED_MORE_INFO;
    /*HANDLED*/ translateSet[L"pushButton_paste"] = VK_XTI_CUSTOM_PASTE;
    translateSet[L"pushButton_fileFind"] = VK_NEED_MORE_INFO;
    translateSet[L"pushButton_find"] = VK_NEED_MORE_INFO;
    translateSet[L"pushButton_findAll"] = VK_NEED_MORE_INFO;
    translateSet[L"pushButton_insert"] = VK_INSERT;
    /*HANDLED*/ translateSet[L"pushButton_delete"] = VK_DELETE;
    /*HANDLED*/ translateSet[L"pushButton_enter"] = VK_RETURN;
}
