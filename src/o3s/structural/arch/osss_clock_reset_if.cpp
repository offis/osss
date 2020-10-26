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

#include "osss_clock_reset_if.h"
#include "o3s/utils/osss_global_port_registry.h"
#include "o3s/utils/osss_report.h"
#include "o3s/utils/osss_assert.h"

using sc_core::sc_object;
using sc_core::sc_interface;
using sc_core::sc_port;
using sc_core::sc_signal_in_if;
using sc_core::sc_signal_inout_if;

namespace osss {
namespace osssi {

struct osss_clock_reset_if::delta_delay
  : sc_core::sc_module
{
  sc_core::sc_in<bool>     clock_port;
  sc_core::sc_signal<bool> clock_sig;

  sc_core::sc_in<bool>     reset_port;
  sc_core::sc_signal<bool> reset_sig;

  SC_CTOR( delta_delay )
    : clock_port( "clock_port" )
    , clock_sig( "clock_delayed" )
    , reset_port( "reset_port" )
    , reset_sig( "reset_delayed" )
  {
    SC_METHOD( forward );
    sensitive << clock_port << reset_port;
  }

  void forward()
  {
    clock_sig.write( clock_port.read() );
    reset_sig.write( reset_port.read() );
  }
};

osss_clock_reset_if::~osss_clock_reset_if()
{
  delete delta_;
}


void
osss_clock_reset_if::clock_port
  ( const sc_signal_in_if<bool>& sif )
{
  clock_port_port(sif);
  clock_port_bound = true;
}

void
osss_clock_reset_if::clock_port
  ( sc_port<sc_signal_in_if<bool>, 1>& port )
{
  clock_port_port(port);
  clock_port_bound = true;
}

void
osss_clock_reset_if::clock_port
  ( sc_port<sc_signal_inout_if<bool>, 1>& port )
{
  clock_port_port(port);
  clock_port_bound = true;
}

void
osss_clock_reset_if::reset_port
  ( const sc_signal_in_if<bool>& sif )
{
  reset_port_port(sif);
  reset_port_bound = true;
}

void
osss_clock_reset_if::reset_port
  (sc_port<sc_signal_in_if<bool>, 1>& port)
{
  reset_port_port(port);
  reset_port_bound = true;
}

void
osss_clock_reset_if::reset_port
  (sc_port<sc_signal_inout_if<bool>, 1>& port)
{
  reset_port_port(port);
  reset_port_bound = true;
}

const char*
osss_clock_reset_if::name() const
{
  sc_object const * this_ = dynamic_cast<sc_object const *>( this );
  OSSS_ASSERT( this_ );
  return this_->name();
}

const char*
osss_clock_reset_if::kind() const
{
  sc_object const * this_ = dynamic_cast<sc_object const *>( this );
  OSSS_ASSERT( this_ );
  return this_->kind();
}

const char*
osss_clock_reset_if::getClockPortName() const
{
  sc_interface const * iface = clock_port_port.get_interface();
  sc_object const * signal = dynamic_cast<sc_object const *>(iface);
  return signal->basename();
}

const char*
osss_clock_reset_if::getResetPortName() const
{
  sc_interface const * iface = reset_port_port.get_interface();
  sc_object const * signal = dynamic_cast<sc_object const *>(iface);
  return signal->basename();
}

void
osss_clock_reset_if::forward_ports( osss_clock_reset_if& child )
{
  if( osss_unlikely( sc_core::sc_start_of_simulation_invoked() ) )
  {
    OSSS_REPORT_ERROR( report::called_after_elaboration )
      % __PRETTY_FUNCTION__
      << "\n On " << this->kind()
      << " module '" << this->name() << "'.";
  }

  child.clock_port( clock_port_port );
  child.reset_port( reset_port_port );
}

void
osss_clock_reset_if::
  forward_ports_delayed( osss_clock_reset_if& child )
{
  if( osss_unlikely( sc_core::sc_start_of_simulation_invoked() ) )
  {
    OSSS_REPORT_ERROR( report::called_after_elaboration )
      % __PRETTY_FUNCTION__
      << "\n On " << this->kind()
      << " module '" << this->name() << "'.";
  }

  if( !delta_ ) {
    delta_ = new delta_delay( "delta" );
    delta_->clock_port( clock_port_port );
    delta_->reset_port( reset_port_port );
  }

  child.clock_port( delta_->clock_sig );
  child.reset_port( delta_->reset_sig );
}

void
osss_clock_reset_if::check_clock_reset_port()
{
  sc_object* this_ = dynamic_cast<sc_object*>( this );
  OSSS_ASSERT( this_ );

  if (! clock_port_bound)
  {
    if (osss_global_port_registry::is_clock_port_registered())
      this->clock_port(osss_global_port_registry::get_clock_port());
    else
    {
      std::cerr << "Error: " << this_->kind() << " "
                << this_->name() << " is not bound to clock port." 
                << std::endl;
      OSSS_ABORT();
    }
  }

  if(! reset_port_bound)
  {
    if (osss_global_port_registry::is_reset_port_registered())
      this->reset_port(osss_global_port_registry::get_reset_port());
    else
    {
      std::cerr << "Error: " << this_->kind() << " "
                << name() << " is not bound to reset port." 
                << std::endl;
      OSSS_ABORT();
    }
  }
}

} // namespace osssi
} // namespace osss

// $Id: osss_clock_reset_if.cpp 2923 2008-12-16 10:32:25Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
