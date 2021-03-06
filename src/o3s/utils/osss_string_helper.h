/* -----------------------------------------------------------------------
 * Copyright (c) 2005-2008  OFFIS Institute for Information Technology,
 *                          Carl von Ossietzky University,
 *                          Oldenburg, Germany
 * All rights reserved.
 *
 * This file is directly or indirectly part of the OSSS simulation
 * library, a library for synthesisable system level models in
 * SystemC.
 *
 * Created for the projects:
 *   - ICODES  (1)
 *   - PolyDyn (2)
 *   - ANDRES  (3)
 *
 * 1) http://icodes.offis.de/
 * 2) http://ehs.informatik.uni-oldenburg.de/en/research/projects/polydyn/
 * 3) http://andres.offis.de/
 *
 * A list of authors and contributors can be found in the accompanying
 * AUTHORS file.  For detailed copyright information, please refer
 * to the COPYING file.
 *
 * -----------------------------------------------------------------------
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301,
 * USA.
 *
 * Contact information:
 *  OFFIS
 *  Institute for Information Technology
 *    Escherweg 2
 *    D-26121 Oldenburg
 *    Germany
 *  www  : http://offis.de/
 *  phone: +49 (441) 9722-0
 *  fax  : +49 (441) 9722-102
 * -----------------------------------------------------------------------
 */

#ifndef OSSS_STRING_HELPER_HH
#define OSSS_STRING_HELPER_HH

#include <string>
#include <vector>

/**
 * \file osss_string_helper.h
 *
 * FIXME: cleanup naming and exposition to public osss namespace
 */

namespace osss {
namespace osssi {

/*
 * transforms the string str into tokens separated by user
 * defined delimiters. The resulting tokens are returned by the tokens vector
 */
void tokenize(const std::string& str,
              std::vector<std::string>& tokens,
              const std::string& delimiters = " ");

//------------------------------------------------------------------------------

/*
 * counts the number of characters ch contained in string str
 */
unsigned int countChar(const std::string& str, const char& ch);

//------------------------------------------------------------------------------

void StrSub(std::string& cp, 
            const std::string& sub_this, 
            const std::string& for_this, 
            const int num_times);

std::string removeClassName(const std::string& fullSignature, 
                            const std::string& fullClassName);

//------------------------------------------------------------------------------

std::string transformToGuard(const std::string& fullSignature, 
                             const std::string& fullClassName,
                             const std::string& methodName);

//------------------------------------------------------------------------------

/*
 * excpects a trimmed method signature (see method above) and extracts
 * the type of the parameters.
 *
 * Trimmed parameter lists are of the form:
 *   (TypeName0*, TypeName1*, ..., TypeNameN*)
 * whereas parameters are always from pointer type.
 * Up to now no templated types are allowed for parameters.
 *
 * The above parameter list results in a vector of strings containing:
 *  0: TypeName0
 *  1: TypeName1
 *     ...
 *  N: TypeNameN
 *
 */
std::vector<std::string> extractTypesFromParameterList(const std::string& str);

} // namespace osssi
} //namespace osss

#endif
// $Id: osss_string_helper.h 2586 2008-08-26 13:01:24Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
