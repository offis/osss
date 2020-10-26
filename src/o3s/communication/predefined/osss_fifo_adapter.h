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
#ifndef OSSS_FIFO_ADAPTER_H_INCLUDED_
#define OSSS_FIFO_ADAPTER_H_INCLUDED_

#include "osss_fifo.h"
#include "o3s/structural/arch/osss_clock_reset_if.h"
#include "osss_fifo_traits.h"

namespace osss {

// --------------------------------------------------------------------

template< typename T, template<typename T> class FifoType >
struct osss_fifo_traits;

// --------------------------------------------------------------------

template< typename DataType
        , template<typename> class FifoType = sc_core::sc_fifo >
class osss_fifo_source
  : public  osss_module
{
public:
  typedef DataType item_type;
  typedef osss_fifo_traits< DataType, FifoType >     traits_type;

  typedef typename traits_type::input_if_type        input_if_type;
  typedef typename traits_type::input_port_type      input_port_type;

  typedef typename osss::osss_shared_if<>            output_if_type;
  typedef typename osss_fifo_out< item_type >::type  output_port_type;

  SC_HAS_PROCESS( osss_fifo_source );
  explicit
  osss_fifo_source( sc_core::sc_module_name = "osss_fifo_source" );

  const char* kind()
    { return "osss_fifo_source"; }

  void bind( input_if_type& i )
    { in_( i ); }
  void bind( input_port_type& p )
    { in_( p ); }

  void bind( output_if_type& i )
    { out_( i ); }
  void bind( output_port_type& p )
    { out_( p ); }

  template<typename BindType>
  void operator()( BindType& b )
  {
    this->bind( b );
  }

private:
  virtual void before_end_of_elaboration();
  void forward();

  input_port_type  in_;
  output_port_type out_;

}; // osss_fifo_source

// --------------------------------------------------------------------

template< typename DataType
        , template<typename> class FifoType = sc_core::sc_fifo >
struct osss_fifo_sink
  : public  osss_module
{
  typedef DataType item_type;
  typedef osss_fifo_traits< DataType, FifoType >     traits_type;

  typedef typename osss::osss_shared_if<>            input_if_type;
  typedef typename osss_fifo_in< item_type >::type   input_port_type;

  typedef typename traits_type::output_if_type       output_if_type;
  typedef typename traits_type::output_port_type     output_port_type;

  SC_HAS_PROCESS( osss_fifo_sink );
  explicit
  osss_fifo_sink( sc_core::sc_module_name = "osss_fifo_sink" );

  const char* kind()
    { return "osss_fifo_sink"; }

  void bind( input_if_type& i )
    { in_( i ); }
  void bind( input_port_type& p )
    { in_( p ); }

  void bind( output_if_type& i )
    { out_( i ); }
  void bind( output_port_type& p )
    { out_( p ); }

  template<typename BindType>
  void operator()( BindType& b )
  {
    this->bind( b );
  }

private:
  virtual void before_end_of_elaboration();
  void forward();

  input_port_type  in_;
  output_port_type out_;

}; // osss_fifo_sink

// --------------------------------------------------------------------

} // namespace osss

#include "osss_fifo_adapter.tpp"

#endif // OSSS_FIFO_ADAPTER_H_INCLUDED_
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
// // Taf!
