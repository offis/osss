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
#ifndef OSSS_CLOCK_RESET_IF_H_INCLUDED
#define OSSS_CLOCK_RESET_IF_H_INCLUDED

#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/noncopyable.h"

namespace osss {
namespace osssi {

class osss_clock_reset_if
  : private noncopyable
{
  typedef sc_core::sc_in<bool> port_type;
public:

  osss_clock_reset_if();
  virtual ~osss_clock_reset_if() = 0;

  void clock_port( const sc_core::sc_signal_in_if<bool>& );
  void clock_port(sc_core::sc_port<sc_core::sc_signal_in_if<bool>, 1>& );
  void clock_port(sc_core::sc_port<sc_core::sc_signal_inout_if<bool>, 1>& );

  void reset_port(const sc_core::sc_signal_in_if<bool>& );
  void reset_port(sc_core::sc_port<sc_core::sc_signal_in_if<bool>, 1>& );
  void reset_port(sc_core::sc_port<sc_core::sc_signal_inout_if<bool>, 1>& );

  /// forward port bindings to another clock/reset interface
  void forward_ports( osss_clock_reset_if& );

  virtual const char* name() const;
  virtual const char* kind() const;
  const char* getClockPortName() const;
  const char* getResetPortName() const;

  port_type &       clock_port()       { return clock_port_port; }
  port_type const & clock_port() const { return clock_port_port; }

  port_type &       reset_port()       { return reset_port_port; }
  port_type const & reset_port() const { return reset_port_port; }

protected:

  void check_clock_reset_port();

  /// FIXME: this is a hack and should not be required!
  void forward_ports_delayed( osss_clock_reset_if& );

//private:
protected: // should be private, soon
  port_type  clock_port_port;
  port_type  reset_port_port;

  bool clock_port_bound;
  bool reset_port_bound;

private:
  /// FIXME: this is a hack and should not be required!
  class delta_delay;
  delta_delay* delta_;

};

inline
osss_clock_reset_if::osss_clock_reset_if()
  : clock_port_port("clock_port")
  , reset_port_port("reset_port")
  , clock_port_bound(false)
  , reset_port_bound(false)
  , delta_(0)
{}

} // namespace osssi
} // namespace osss

#endif // OSSS_CLOCK_RESET_IF_H_INCLUDED
// $Id: osss_clock_reset_if.h 2842 2008-12-04 09:30:13Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
