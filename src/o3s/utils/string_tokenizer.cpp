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

#include "string_tokenizer.h"
#include "o3s/utils/osss_common.h"
#include "o3s/utils/osss_report.h"
#include "o3s/utils/osss_assert.h"
#include <algorithm> // std::swap

namespace osss {
namespace osssi {

string_tokenizer::string_tokenizer( const std::string& str
                                  , const std::string& delimiter /* =" " */ )
  : str_(str)
  , delim_(delimiter)
  , pos_(0)
{
  OSSS_ASSERT( delim_.length() );

  if( ! str_.length() ) // skip empty string
    pos_ = std::string::npos;
  else // skip empty tokens
    skip();
}

/// skip delimiter and following empty tokens
void
string_tokenizer::skip()
{
  while( pos_ == str_.find( delim_, pos_ ) ) {
    pos_ += delim_.length();
  }
}

void
string_tokenizer::swap( string_tokenizer & other )
{
  std::swap( str_,   other.str_   );
  std::swap( delim_, other.delim_ );
  std::swap( pos_,   other.pos_   );
}

// return remaining string (including all delimiters)
std::string
string_tokenizer::tail()
{
  if( pos_ == std::string::npos ) {
    OSSS_REPORT_ERROR( report::internal_error )
      % "string_tokenizer: No remaining string!";
  }
  // return remaining string
  size_type tail_pos_ = pos_;
  pos_                = std::string::npos;
  return str_.substr( tail_pos_ );
}

std::string
string_tokenizer::next()
{
  if( !has_next() ){
    OSSS_REPORT_ERROR( report::internal_error )
      % "string_tokenizer: No next token!";
  }

  const size_type start = pos_;
  const size_type end   = str_.find( delim_, start );

  if( end == std::string::npos ) {
      pos_ = end;
      return str_.substr( start );
  } else {
      pos_ = end + delim_.length();
      skip();
      if( pos_ >= str_.length() ) pos_ = std::string::npos;
      return str_.substr( start, end - start );
    }
  }

} // namespace osssi
} // namespace osss

// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
