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

namespace osss {

template< typename DataType
        , template<typename> class FifoType >
osss_fifo_source<DataType,FifoType>::osss_fifo_source( sc_core::sc_module_name )
  : in_("input")
  , out_("output")
{
  SC_THREAD(forward);
  this->sensitive << this->clock_port().pos();
}

template< typename DataType
        , template<typename> class FifoType >
void
osss_fifo_source<DataType,FifoType>::before_end_of_elaboration()
{
  if( !this->clock_port().get_interface() ) {
    sc_core::sc_interface* i      = out_.get_interface();
    osssi::osss_clock_reset_if* p =
      dynamic_cast<osssi::osss_clock_reset_if*>(i);
    if( p ) {
      p->forward_ports( *this );
    }
  }
}

template< typename DataType
        , template<typename> class FifoType >
void
osss_fifo_source<DataType,FifoType>::forward()
{
  while( true )
  {
    // read from source FIFO
    item_type item = traits_type::read_from( in_ );
    // write to osss_shared_fifo
    out_->put( item );
  }
}

// --------------------------------------------------------------------

template< typename DataType
        , template<typename> class FifoType >
osss_fifo_sink<DataType,FifoType>::osss_fifo_sink( sc_core::sc_module_name )
  : in_("input")
  , out_("output")
{
  SC_THREAD(forward);
  this->sensitive << this->clock_port().pos();
}

template< typename DataType
        , template<typename> class FifoType >
void
osss_fifo_sink<DataType,FifoType>::before_end_of_elaboration()
{
  if( !this->clock_port().get_interface() ) {
    sc_core::sc_interface* i      = in_.get_interface();
    osssi::osss_clock_reset_if* p =
      dynamic_cast<osssi::osss_clock_reset_if*>(i);
    if( p ) {
      p->forward_ports( *this );
    }
  }
}

template< typename DataType
        , template<typename> class FifoType >
void
osss_fifo_sink<DataType,FifoType>::forward()
{
  while( true )
  {
    // read from osss_shared_fifo source
    item_type item = in_->get();
    // write to sink FIFO
    traits_type::write_to( out_, item );
  }
}

} // namespace osss

// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
// // Taf!
