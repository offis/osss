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

#include "o3s/utils/osss_report.h"

namespace osss {
namespace osssi {

template<typename T>
osss_ram_base<T>::osss_ram_base( size_type size )
  : osss_architecture_object()
  , array_type( size )
  , m_init_addr(0)
  , s_read_prepared( "read_prepared" )
  , s_read_address( "read_address" )
  , s_data_out( "data_out" )
{
  s_read_prepared.write( false );
  s_read_address.write( 0 );

  SC_METHOD( clocked_update );
  sensitive << clock.pos();
}

template<typename T>
void
osss_ram_base<T>::init( const_reference value )
{
  if( sc_core::sc_start_of_simulation_invoked() )
  {
    if( m_init_addr < this->size() ) {
      m_init_addr = this->size();
      OSSS_REPORT_WARNING( osss::report::called_after_elaboration )
        % "osss_ram<T>::init()"
        << "\nRAM initialisation of " << this->name()
        << " ignored.";
    }
    return; // to be sure
  }

  if( m_init_addr >= this->size() )
  {
    OSSS_REPORT_ERROR( osss::report::ram_init_overflow )
      % this->name();
  }

  (*this)[m_init_addr] = value;
  ++m_init_addr;
}

template<typename T>
void
osss_ram_base<T>::prepare_read(size_type address)
{
  if( osss_unlikely(address >= this->size() ) )
  {
    OSSS_REPORT_ERROR( osss::report::index_out_of_bounds )
      % this->name() % address % this->size();
  }
  s_read_prepared.write( true );
  s_read_address.write(address);
}

template<typename T>
typename osss_ram_base<T>::const_reference
osss_ram_base<T>::read() const
{
  if( ! s_read_prepared.read() )
  {
    OSSS_REPORT_WARNING(osss::report::ram_read_too_early);
  }
  return s_data_out.read();
}

template<typename T>
void
osss_ram_base<T>::write( size_type       address
                       , const_reference value )
{
  if( osss_unlikely(address >= this->size() ) )
  {
    OSSS_REPORT_ERROR( osss::report::index_out_of_bounds )
      % this->name() % address % this->size();
  }
  /// TODO: check colliding write()/read()
  (*this)[address] = value;
  // m_clock_count.write(-1);
}

template<typename T>
void
osss_ram_base<T>::clocked_update()
{
  // update output data
  s_data_out.write( (*this)[ s_read_address.read() ] );
#if 0
  if(m_clock_count.read() == 0)
    m_clock_count.write(1);
#endif
}

} // namespace osssi
} // namespace osss

// $Id: osss_memory.tpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
