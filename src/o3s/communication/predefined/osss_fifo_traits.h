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
#ifndef OSSS_FIFO_TRAITS_H_INCLUDED_
#define OSSS_FIFO_TRAITS_H_INCLUDED_

namespace osss {

template< typename T, template<typename T> class FifoType >
struct osss_fifo_traits;

// --------------------------------------------------------------------

template< typename T >
struct osss_fifo_traits< T, sc_core::sc_fifo >
{
  typedef T value_type;

  typedef typename sc_core::sc_fifo_blocking_in_if<T>  input_if_type;
  typedef typename sc_core::sc_fifo_blocking_out_if<T> output_if_type;

  typedef typename sc_core::sc_port< input_if_type >   input_port_type;
  typedef typename sc_core::sc_port< output_if_type >  output_port_type;

  static T read_from( input_if_type& fifo )
    { return fifo.read();  }
  static T read_from( input_port_type& fifo )
    { return fifo->read(); }

  static void write_to( output_if_type& fifo, T const & val )
    { fifo.write( val );  }
  static void write_to( output_port_type& fifo, T const & val )
    { fifo->write( val ); }
};

// --------------------------------------------------------------------

#if OSSS_HAVE_TLM_

template< typename T >
struct osss_fifo_traits< T, tlm::tlm_fifo >
{
  typedef T value_type;

  typedef typename tlm::tlm_blocking_put_if<T>        input_if_type;
  typedef typename tlm::tlm_blocking_get_if<T>        output_if_type;

  typedef typename sc_core::sc_port< input_if_type >  input_port_type;
  typedef typename sc_core::sc_port< output_if_type > output_port_type;

  static T read_from( input_if_type& fifo )
    { return fifo.get( (tlm::tlm_tag<T>*) 0 );  }
  static T read_from( input_port_type& fifo )
    { return fifo->get( (tlm::tlm_tag<T>*) 0 ); }

  static void write_to( output_if_type& fifo, T const & val )
    { fifo.put( val );  }
  static void write_to( output_port_type& fifo, T const & val )
    { fifo->put( val ); }
};

#endif // OSSS_HAVE_TLM_

// --------------------------------------------------------------------

#if OSSS_HAVE_HETSC_

template< typename T >
struct osss_fifo_traits< T, hetsc::uc_fifo >
  : osss_fifo_traits< T, sc_core::sc_fifo >
{};

template< typename T >
struct osss_fifo_traits< T, hetsc::uc_inf_fifo >
  : osss_fifo_traits< T, sc_core::sc_fifo >
{};

#endif // OSSS_HAVE_HETSC_

// --------------------------------------------------------------------

} // namespace osss

#endif // OSSS_FIFO_TRAITS_H_INCLUDED_
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
// // Taf!
