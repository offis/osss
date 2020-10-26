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

#include "o3s/utils/osss_common.h"
#include "o3s/utils/osss_systemc.h"
#include "o3s/communication/arch/osss_channel/osss_via.h"

namespace osss {

std::ostream&
operator<<( std::ostream& o, osss_via_base const & vb )
{
  o << "type: ";
  switch(vb.type())
  {
    case OSSS_NO_VIA :
      o << "OSSS_NO_VIA";
      break;

    case OSSS_MUX_VIA :
      o << "OSSS_MUX_VIA";
      break;

    case OSSS_OR_VIA :
      o << "OSSS_OR_VIA";
      break;

    case OSSS_AND_VIA :
      o << "OSSS_AND_VIA";
      break;

    case OSSS_AND_OR_VIA :
      o << "OSSS_AND_OR_VIA";
      break;
  }

  if (vb.type() != OSSS_NO_VIA)
    o << " | guard: " << vb.guard();

  return o;
}

//-----------------------------------------------------------------------------

namespace osssi {

std::string
osss_no_via::guard() const
{
  return std::string();
}

osss_via_type
osss_no_via::type() const
{
  return OSSS_NO_VIA;
}

//-----------------------------------------------------------------------------

osss_mux::osss_mux( osss_mux_controller muxController )
  : m_muxController( muxController )
{}

std::string
osss_mux::guard() const
{
  switch( m_muxController)
  {
    case OSSS_ARBITER :
      return std::string("OSSS_ARBITER");

    case OSSS_ADDRESS_DECODER :
      return std::string("OSSS_ADDRESS_DECODER");
  }

  return std::string();
}

osss_via_type
osss_mux::type() const
{
  return OSSS_MUX_VIA;
}

} // end namespace osssi

//-----------------------------------------------------------------------------

osss_via_logic::osss_via_logic()
  : m_port( NULL )
  , m_signal(0)
{ }

osss_via_logic::osss_via_logic( osss_port_list_element_base * outPort )
  : m_port( outPort )
  , m_signal(NULL)
{ }

osss_via_logic::osss_via_logic( osss_signal_base * signal )
  : m_port(0)
  , m_signal(signal)
{ }

std::string
osss_via_logic::guard() const
{
  if (m_port != NULL) {
    return m_port->kind();
  } else if (m_signal != NULL) {
    return m_signal->name();
  }

  return std::string();
}

osss_port_list_element_base *
osss_via_logic::port()
{
  return m_port;
}

osss_signal_base *
osss_via_logic::osss_signal()
{
  return m_signal;
}

osss_via_logic::~osss_via_logic()
{
  delete m_port;
  delete m_signal;
}

//-----------------------------------------------------------------------------

namespace osssi {

osss_or::osss_or()
  : osss_via_logic()
{}

osss_via_type
osss_or::type() const
{
  return OSSS_OR_VIA;
}

//-----------------------------------------------------------------------------

osss_and::osss_and( osss_port_list_element_base * outPort )
  : osss_via_logic(outPort)
{}

osss_and::osss_and(osss_signal_base * signal)
  : osss_via_logic(signal)
{}

osss_via_type
osss_and::type() const
{
  return OSSS_AND_VIA;
}

//-----------------------------------------------------------------------------

osss_and_or::osss_and_or(osss_port_list_element_base * outPort)
  : osss_via_logic(outPort)
{
}

osss_and_or::osss_and_or(osss_signal_base * signal)
  : osss_via_logic(signal)
{
}

osss_via_type osss_and_or::type() const
{
  return OSSS_AND_OR_VIA;
}

} // namespace osssi
} // namespace osss

// $Id: osss_via.cpp 2839 2008-12-04 09:29:59Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
