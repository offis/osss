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
#include "o3s/communication/arch/osss_channel/osss_signal.h"

namespace osss {

osss_signal_base::osss_signal_base(const std::string& name)
  : m_name(name)
{}

const std::string&
osss_signal_base::name() const
{
  return m_name;
}

osss_signal_base::~osss_signal_base()
{ }

std::ostream& operator<< (std::ostream& o,
                          const osss_signal_base& sb)
{
  o << "name: " << sb.name() << " | ";
  o << "type: ";
  if (sb.isShared())
    o << "shared";
  else
    o << "multi";

  return o;
}

namespace osssi {

//-----------------------------------------------------------------------------

osss_shared_signal::osss_shared_signal(const std::string& name)
  : osss_signal_base(name)
{}

bool osss_shared_signal::isShared() const
{
  return true;
}

//-----------------------------------------------------------------------------

osss_multi_signal::osss_multi_signal(const std::string& name)
  : osss_signal_base(name)
{
}

bool osss_multi_signal::isShared() const
{
  return false;
}

} // namespace osssi
} // namespace osss

// $Id: osss_signal.cpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
