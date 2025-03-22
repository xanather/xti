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

#include "error_reporter.h"

// 1. Qt framework headers
#include <QString>
// 2. System/OS headers
#include <string>
#include <stdexcept>
// 3. C++ standard library headers
// 4. Project classes
#include "windows_subsystem.h"

void error_reporter::halt(const char* file, int32_t line, const char* message)
{
    std::string fullMessage = file;
    fullMessage.push_back('@');
    fullMessage.append(std::to_string(line));
    fullMessage.append(": ");
    fullMessage.append(message);
    std::wstring userMsg = QString::fromStdString(fullMessage).toStdWString();
    windows_subsystem::show_exception_to_user(userMsg);
    throw std::runtime_error(fullMessage);
}
