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
#ifndef OSSS_RAM_H_INCLUDED_
#define OSSS_RAM_H_INCLUDED_

#include "o3s/utils/osss_systemc.h"
#include "o3s/datatypes/osss_array.h"
#include "o3s/structural/arch/osss_architecture_object.h"

namespace osss {
namespace osssi {

template< typename T >
class osss_ram_base
  : public    osss_architecture_object
  , protected osssi::osss_array_base<T>
{
  typedef osss_ram_base<T> this_type;
  SC_HAS_PROCESS( this_type );

protected:

  typedef osss_array_base<T>                   array_type;
  typedef typename array_type::item_type       item_type;
  typedef typename array_type::size_type       size_type;
  typedef typename array_type::const_reference const_reference;

  /// protected constructor to ensure proper naming
  explicit
  osss_ram_base( size_type size );

public:

  sc_core::sc_in<bool> clock; /// TODO: use osss_clock_reset_if?

  void init( const_reference value );

  void prepare_read( size_type address );
  const_reference read() const;

  void write( size_type address, const_reference value );

private:

  virtual const char* kind() const
    { return "osss_ram"; }

  void clocked_update();

  size_type            m_init_addr;
  sc_core::sc_signal<bool>      s_read_prepared;
  sc_core::sc_signal<size_type> s_read_address;
  sc_core::sc_signal<item_type> s_data_out;

};

} // end namespace osssi

template<typename T, unsigned int Size>
class osss_ram
  : public osssi::osss_ram_base<T>
{
  typedef osssi::osss_ram_base<T> base_type;

public:

  explicit
  osss_ram( sc_core::sc_module_name /* unused */
              = sc_core::sc_gen_unique_name( "osss_ram" ) )
    : base_type( Size )
  {}
};

} // namespace osss

#include "o3s/structural/arch/memory/osss_ram.tpp"

#endif // OSSS_RAM_INCLUDED

// $Id: osss_memory.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
