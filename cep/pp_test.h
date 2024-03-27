/**
 * @file    pp_test.h
 * @author  Samuel Martel
 * @date    2024-03-26
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


#ifndef CEP_PP_TEST_H
#define CEP_PP_TEST_H

#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/list/adt.hpp>
#include <boost/preprocessor/list/at.hpp>
#include <boost/preprocessor/list/enum.hpp>
#include <boost/preprocessor/list/first_n.hpp>
#include <boost/preprocessor/list/rest_n.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#define PRINT_TEST_ITER_COUNT(n)              BOOST_PP_ADD(n, 1)
#define PRINT_TEST_ITER_ARG_LIST(flag, value) (flag, (value, BOOST_PP_NIL))

#define PRINT_TEST_ITER_MAKE_FLAG(z, n, data) data
#define PRINT_TEST_ITER_DUPP_FLAG(flag, n)    BOOST_PP_REPEAT(n, PRINT_TEST_ITER_MAKE_FLAG, flag)

#define PRINT_TEST_ITER_VALUE_RPAREN(z, n, data) )
#define PRINT_TEST_ITER_CLOSE_VALUE_LIST(n) BOOST_PP_NIL BOOST_PP_REPEAT(n, PRINT_TEST_ITER_VALUE_RPAREN, )
#define PRINT_TEST_ITER_MAKE_VALUE(z, n, value) (value,
#define PRINT_TEST_ITER_DUPP_VALUE_HELPER(value, n)                                                                    \
    BOOST_PP_LIST_ENUM(BOOST_PP_REPEAT(n, PRINT_TEST_ITER_MAKE_VALUE, value) PRINT_TEST_ITER_CLOSE_VALUE_LIST(n))
#define PRINT_TEST_ITER_DUPP_VALUE(value, n) PRINT_TEST_ITER_DUPP_VALUE_HELPER(value, n)

#define PRINT_TEST_ITER_HELPER_4(n, flag, value)                                                                       \
    print(PRINT_TEST_ITER_DUPP_FLAG(flag, n) "\n", PRINT_TEST_ITER_DUPP_VALUE(value, n));
#define PRINT_TEST_ITER_HELPER_3(n, flag, value) PRINT_TEST_ITER_HELPER_4(n, flag, value)
#define PRINT_TEST_ITER_HELPER_2(n, flag, data)  PRINT_TEST_ITER_HELPER_3(n, flag, BOOST_PP_LIST_AT(data, 1))
#define PRINT_TEST_ITER_HELPER(n, data)          PRINT_TEST_ITER_HELPER_2(n, BOOST_PP_LIST_AT(data, 0), data)

#define PRINT_TEST_ITER(z, n, data) PRINT_TEST_ITER_HELPER(PRINT_TEST_ITER_COUNT(n), data)


#if defined(FACTORIAL_CALLS)
#    define PRINT_TEST(iter, flag, value) BOOST_PP_REPEAT(iter, PRINT_TEST_ITER, PRINT_TEST_ITER_ARG_LIST(flag, value))
#else
#    define PRINT_TEST(iter, flag, value) PRINT_TEST_ITER_HELPER(iter, PRINT_TEST_ITER_ARG_LIST(flag, value))
#endif

#endif    // CEP_PP_TEST_H
