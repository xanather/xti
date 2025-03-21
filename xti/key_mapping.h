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

#ifndef KEY_MAPPING_H
#define KEY_MAPPING_H

// 1. Qt framework headers
// 2. System/OS headers
#include <Windows.h>
// 3. C++ standard library headers
#include <cstdint>
#include <string>
#include <unordered_map>
// 4. Project classes
// 5. Forward decl

#define VK_NEED_MORE_INFO -1

#define VK_XTI_CUSTOM_EXCLAMATION_MARK -1002
#define VK_XTI_CUSTOM_AT -1003
#define VK_XTI_CUSTOM_HASH -1004
#define VK_XTI_CUSTOM_DOLLAR -1005
#define VK_XTI_CUSTOM_PERCENT -1006
#define VK_XTI_CUSTOM_CIRCUMFLEX -1007
#define VK_XTI_CUSTOM_AMPERSAND -1008
#define VK_XTI_CUSTOM_ASTERISK -1009
#define VK_XTI_CUSTOM_LEFT_ROUND_BRACKET -1010
#define VK_XTI_CUSTOM_RIGHT_ROUND_BRACKET -1011

#define VK_XTI_CUSTOM_COPY -2002
#define VK_XTI_CUSTOM_PASTE -2003
#define VK_XTI_CUSTOM_FIND_FILE -2004
#define VK_XTI_CUSTOM_FIND -2005
#define VK_XTI_CUSTOM_FIND_ALL -2006
#define VK_XTI_CUSTOM_CUT -2007

// Maps push buttons on the main_window.ui to virtual key codes: https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
class key_mapping  // static members only
{
public:
    static std::unordered_map<std::wstring, int32_t> translateSet;
    static void initialize();
};

#endif // KEY_MAPPING_H
