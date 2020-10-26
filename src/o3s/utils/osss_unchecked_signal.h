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

#ifndef OSSS_UNCHECKED_SIGNAL_H
#define OSSS_UNCHECKED_SIGNAL_H

namespace osss {

namespace osssi {

/**
 *  \class osss_unchecked_signal
 *
 *  Behaves like an sc_signal without multiple writer check.
 *  
 *  \note This is for library internal use only and should be
 *        marked as deprecated as soon as a new implementation of
 *        class ::osss::osss_arbiter_shared is available.
 * 
 *  \see ::sc_core::sc_signal
 */
template <class T>
class osss_unchecked_signal : public sc_core::sc_signal<T>
{
 public:

  osss_unchecked_signal() : sc_core::sc_signal<T>()
    {}

  explicit osss_unchecked_signal( const char* name_ ) : 
    sc_core::sc_signal<T>(name_)
    {}

  virtual ~osss_unchecked_signal()
    {}

  // interface methods
  virtual void register_port( sc_core::sc_port_base&, const char* );

  // write the new value
  virtual void write( const T& );

  virtual const char* kind() const
    { return "osss_unchecked_signal"; }

 private:

  // disabled
  osss_unchecked_signal( const osss_unchecked_signal<T>& );
};

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

template <class T>
inline
void
osss_unchecked_signal<T>::
register_port( sc_core::sc_port_base& port_, const char* if_typename_ )
{
  if ( sc_core::sc_get_curr_simcontext()->write_check() )
  {
    std::string nm( if_typename_ );
    if( nm == typeid( sc_core::sc_signal_inout_if<T> ).name() ) {
      sc_core::sc_signal<T>::m_output = &port_;
    }
  }
}

// write the new value
template <class T>
inline
void
osss_unchecked_signal<T>::write( const T& value_ )
{
  sc_core::sc_signal<T>::m_new_val = value_;
  if( !( sc_core::sc_signal<T>::m_new_val == 
         sc_core::sc_signal<T>::m_cur_val ) ) {
    sc_core::sc_signal<T>::request_update();
  }
}

// ----------------------------------------------------------------------------

template <class T>
inline
::std::ostream&
operator << ( ::std::ostream& os, 
              const osss_unchecked_signal<T>& a )
{
  return ( os << a.read() );
}

} // namespace osssi

} // namespace osss

#endif
// $Id: osss_unchecked_signal.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
