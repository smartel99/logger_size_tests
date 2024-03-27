/**
 * @file    logger_test.cpp
 * @author  Samuel Martel
 * @date    2024-03-27
 * @brief
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */
#include "pp_test.h"

#include <cstdarg>
#include <cstdio>

#define STRINGIFY_(x) #x
#define STRINGIFY(x)  STRINGIFY_(x)

#define FLAG_VALUE STRINGIFY(FLAG)

#ifdef USE_CVAR
void print(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::vprintf(fmt, args);
    va_end(args);
}
#else
template<typename... Args>
void print(const char* fmt, Args... args)
{
    std::printf(fmt, args...);
}
#endif

int main()
{
    PRINT_TEST(ITER_COUNT, FLAG_VALUE, VALUE)

    return 0;
}
