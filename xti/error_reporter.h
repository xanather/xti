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

#ifndef ERROR_REPORTER_H
#define ERROR_REPORTER_H

// 1. Qt framework headers
// 2. System/OS headers
// 3. C++ standard library headers
#include <cstdint>
// 4. Project classes
// 5. Forward decl

class error_reporter // static members only
{
public:
    static void halt(const char* file, int32_t line, const char* message);
};

#endif // ERROR_REPORTER_H
