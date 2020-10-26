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

template< unsigned short int bits >
osss_precise_cycle_count<bits>::osss_precise_cycle_count()
{
  OSSS_BASE_CLASS(osss_cycle_count);
  m_value = 0;
}

template< unsigned short int bits >
osss_precise_cycle_count<bits>::osss_precise_cycle_count(sc_dt::uint64 cycles)
{
  OSSS_BASE_CLASS(osss_cycle_count);
  m_value = cycles;
}

template< unsigned short int bits >
osss_precise_cycle_count<bits>::osss_precise_cycle_count
    (sc_core::sc_time duration, sc_core::sc_time on_clock)
{
  OSSS_BASE_CLASS(osss_cycle_count);
  m_value = convert(duration, on_clock);
}

template< unsigned short int bits >
sc_dt::uint64
osss_precise_cycle_count<bits>::getCycleCount()  const
{
  return m_value;
}

template< unsigned short int bits >
void
osss_precise_cycle_count<bits>::decrease(sc_dt::uint64 by_cycles)
{
  m_value -= by_cycles;
}

template< unsigned short int bits >
void
osss_precise_cycle_count<bits>::increase(sc_dt::uint64 by_cycles)
{
  m_value += by_cycles;
}

template< unsigned short int bits >
osss_precise_cycle_count<bits> &
osss_precise_cycle_count<bits>::operator+(const osss_polymorphic<osss_cycle_count> & rhs)
{
  m_value += rhs->getCycleCount();
  return *this;
}

} // namespace osss
// $Id: osss_precise_cycle_count.tpp 2394 2008-07-01 20:45:28Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
