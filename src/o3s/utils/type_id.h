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

#ifndef OSSS_O3S_UTILS_TYPE_ID_H_INCLUDED_
#define OSSS_O3S_UTILS_TYPE_ID_H_INCLUDED_

#include <typeinfo>
#include <string>

#include "o3s/utils/osss_report.h"
#include "o3s/utils/osss_assert.h"

namespace osss {
namespace osssi {

class type_id
{
public:
  typedef const std::type_info* type_ptr;
  typedef type_id               this_type;

private:
  type_id()
    : ptr_( &typeid(void) )
  {}

  type_id( const std::type_info* );

public:
  template< typename T >
  explicit type_id( T const& t )
    : ptr_( &typeid( t ) )
  {
    OSSS_ASSERT( ptr_ && "No RTTI information found!" );
  }

  std::type_info const & info() const
    { return *ptr_; }

  std::string name() const;

  bool operator==( this_type const & rhs) const
    { return *ptr_==*(rhs.ptr_); }

  bool operator!=( this_type const & rhs) const
    { return *ptr_!=*(rhs.ptr_); }

  bool operator<( this_type const & rhs ) const
    { return ptr_->before( *(rhs.ptr_) ); }

  bool operator>( this_type const & rhs ) const
    { return rhs.ptr_->before( *ptr_ ); }

  bool operator>=( this_type const & rhs ) const
    { return !( *this < rhs ); }

  bool operator<=( this_type const & rhs ) const
    { return !( *this > rhs ); }

  template<typename T>
  static
  type_id from()
  {
    return type_id( typeid(T) );
  }

  static const type_id none;

private:
  type_ptr ptr_;

  explicit
  type_id( std::type_info const & t )
    : ptr_( &t )
  {}
};

std::ostream&
operator<<( std::ostream&, type_id );

} // namespace osssi
} // namespace osss

#endif /* OSSS_O3S_UTILS_TYPE_ID_H_INCLUDED_ */
/* Taf!
 * :flavour: (all,h)
 */
