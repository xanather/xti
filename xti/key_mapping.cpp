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
    translateSet[L"pushButton_num1"] = VK_NUMPAD0;
    translateSet[L"pushButton_num2"] = VK_NUMPAD1;
    translateSet[L"pushButton_num3"] = VK_NUMPAD2;
    translateSet[L"pushButton_num4"] = VK_NUMPAD3;
    translateSet[L"pushButton_num5"] = VK_NUMPAD4;
    translateSet[L"pushButton_num6"] = VK_NUMPAD5;
    translateSet[L"pushButton_num7"] = VK_NUMPAD6;
    translateSet[L"pushButton_num8"] = VK_NUMPAD7;
    translateSet[L"pushButton_num9"] = VK_NUMPAD8;
}
