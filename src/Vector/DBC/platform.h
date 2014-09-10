/*
 * Copyright (C) 2013 Tobias Lorenz.
 * Contact: tobias.lorenz@gmx.net
 *
 * This file is part of Tobias Lorenz's Toolkit.
 *
 * Commercial License Usage
 * Licensees holding valid commercial licenses may use this file in
 * accordance with the commercial license agreement provided with the
 * Software or, alternatively, in accordance with the terms contained in
 * a written agreement between you and Tobias Lorenz.
 *
 * GNU General Public License 3.0 Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl.html.
 */

#pragma once

#include "config.h"

#ifdef __linux__

#if __cplusplus < 201103L
#define constexpr static const
#endif

#endif

#ifdef _WIN32

/* '..' : class '..' needs to have dll-interface to be used by clients of class '..' */
#pragma warning (disable: 4251)

/* nonstandard extension used: enum '...' used in qualified name */
#pragma warning (disable: 4482)

#endif

#ifdef OPTION_USE_CPP11_REGEX
#include <regex>
#define smatch       std::smatch
#define regex        std::regex
#define regex_search std::regex_search
#else
#include <boost/regex.hpp>
#define smatch       boost::smatch
#define regex        boost::regex
#define regex_search boost::regex_search
#endif
