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

#ifndef O3S_UTILS_REPORT_MESSAGE_H_INCLUDED
#define O3S_UTILS_REPORT_MESSAGE_H_INCLUDED

#include "o3s/utils/noncopyable.h"
#include <iosfwd>
#include <sstream>
#include <string>

namespace osss {
namespace report {

/**
 * \class message
 * \brief helper class for osss_report classes
 *
 * This class is used as an internal wrapper/storage for an OSSS report.
 * The class provides a simplistic printf()-like formatting mechanism.
 *
 * \see osss_report
 *
 */
class message
  : private osssi::noncopyable
{
  /// real message implementation
  class impl;
  /// pointer to real implementation
  impl * const impl_;
public:
  typedef std::stringstream stream_type;

  /// create a message from a template
  explicit     message( char const * );
  /// set the next replacer
  stream_type& inject();
  /// append an additional string to the message
  stream_type& append();
  /// combine all parts to a single string
  std::string  combine() const;
  /// destructor
  ~message();
};

/// fill the current replacer
template< typename T >
message& operator % ( message& msg, T const& data )
{
  msg.inject() << data;
  return msg;
}

/// append an arbitrary element
template< typename T >
message& operator << ( message& msg, T const& data )
{
  msg.append() << data;
  return msg;
}
/// support for at least some manipulators (at the end of the message)
inline
message& operator<<( message& msg,
                     std::ostream& (*pfn)(std::ostream&) )
{
  // Call manipulator function on apnnd's stream
  pfn( msg.append() );
  return msg;
}

} // namespace report
} // namespace osss

#endif /* O3S_UTILS_REPORT_MESSAGE_H_INCLUDED */
/* Taf!
 * $Id: message.h 2392 2008-07-01 20:45:11Z philipph $
 * :flavour: (osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
 */
