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

/**
 * \file  string_tokenizer.h
 * \brief simple string tokenizer
 */
#ifndef O3S_UTILS_STRING_TOKENIZER_H_INCLUDED
#define O3S_UTILS_STRING_TOKENIZER_H_INCLUDED

#include "osss_common.h"

#include <string>
#include <sstream>

namespace osss {
namespace osssi {

class string_tokenizer
{
  typedef std::string::size_type size_type;
public:
  string_tokenizer( const std::string& str
                  , const std::string& delimiter = " " );

  /// returns next token - empty tokens are skipped
  std::string next();

  /// return next token - and cast to requested type
  template< typename TargetType > TargetType next();

  bool has_next() const
    { return pos_ != std::string::npos; }

  /// returns remaining string
  std::string tail();

  void swap( string_tokenizer& );

  /// reset tokenizer to beginning
  void reset()
    { reset( delim_ ); }

  /// reset tokenizer to beginning with new delimiter
  void reset( const std::string& delimiter )
    { string_tokenizer( str_, delimiter ).swap( *this ); }

protected:
  /// skip empty tokens
  void skip();

  std::string  str_;   ///< string to split
  std::string  delim_; ///< delimiter
  size_type    pos_;   ///< position of next token, or 'string::npos' on end
};

template< typename TargetType >
TargetType string_tokenizer::next()
{
  TargetType   result;
  std::istringstream token_str( this->next() );
  token_str >> result;
  return result;
}

} // namespace osssi
} // namespace osss

#endif /* O3S_UTILS_STRING_TOKENIZER_H_INCLUDED */
//
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
