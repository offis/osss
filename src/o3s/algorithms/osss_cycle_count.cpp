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
#include "osss_cycle_count.h"
#include "o3s/structural/osss_recon_macros.h"
#include <iostream>

// selectively import std namespace elements
using std::cerr;
using std::endl;

// selectively import SystemC namespace elements
using sc_core::sc_time;

namespace osss {

osss_cycle_count::osss_cycle_count()
{
  OSSS_BASE_CLASS(osss_object);
}

sc_dt::uint64
osss_cycle_count::convert(sc_time & duration, sc_time & on_clock)
{
  sc_dt::uint64 d = duration.value();
  sc_dt::uint64 c = on_clock.value();
  return (d + c - 1) / c; // round up
}

bool osss_cycle_count::operator<(const osss_cycle_count & rhs) const
{
  return getCycleCount() < rhs.getCycleCount();
}

bool osss_cycle_count::operator>(const osss_cycle_count & rhs) const
{
  return getCycleCount() > rhs.getCycleCount();
}

bool osss_cycle_count::operator==(const osss_cycle_count & rhs) const
{
  return getCycleCount() == rhs.getCycleCount();
}

bool osss_cycle_count::operator!=(const osss_cycle_count & rhs) const
{
  return getCycleCount() != rhs.getCycleCount();
}

sc_time osss_cycle_count::getTime(sc_time & reference_clock) const
{
  return reference_clock * getCycleCount();
}

void osss_cycle_count::wait() const
{
  sc_core::wait(getCycleCount());
}

sc_dt::uint64
osss_cycle_count::getCycleCount() const
{
  OSSS_PURE_VIRTUAL_RETURN(0);
}

osss_cycle_count &
osss_cycle_count::operator+(const osss_polymorphic<osss_cycle_count> &)
{
  OSSS_PURE_VIRTUAL_RETURN(*this);
}

} // namespace osss
// $Id: osss_cycle_count.cpp 3022 2009-03-04 22:55:24Z philipph $
// :flavour: (osss,s) (osss_recon,s) (osss_full,s)
