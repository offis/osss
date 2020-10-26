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

#ifndef OSSS_PRECISE_CYCLE_COUNT_H
#define OSSS_PRECISE_CYCLE_COUNT_H

#include "osss_cycle_count.h"

namespace osss{
OSSS_SYNTHESISABLE_BEGIN_

/** This template class provides a naive implementation
    of a cycle counter. It basically provides an
    unsigned integer of arbitrary size (default 32 bits)
    which can be used to count clock cycles.
    By providing a template argument the user may
    use his own counter bitwidth. */
template< unsigned short int bits = 32 >
class osss_precise_cycle_count
  : public osss_cycle_count
{
private:
  /// The current counter value
  sc_dt::sc_uint<bits> m_value;

public:
  /// Construct the counter, set the value to zero
  osss_precise_cycle_count();

  /** Constructs the counter, sets the value to the
      given number of cycles */
  explicit
  osss_precise_cycle_count(sc_dt::uint64 cycles);

#ifndef OSSS_SYNTHESIS_
  /** Constructs a counter. The inital value is the
      number of clock cycles (period given by "on_clock")
      a given duration lasts. The value is rounded up,
      if necessary. */
  osss_precise_cycle_count(sc_core::sc_time duration, sc_core::sc_time on_clock);
#endif // OSSS_SYNTHESIS_

  virtual
  /** Returns the number of cycles as an unsigned SystemC integer */
  sc_dt::uint64 getCycleCount()  const;

  /** Increases the number of cycles by the given value. */
  virtual
  void increase(sc_dt::uint64 by_cycles);

  /** Decreases the number of cycles by the given value.
      There is no check against wrap-arounds. */
  virtual
  void decrease(sc_dt::uint64 by_cycles);

  // Add operator
#ifndef OSSS_SYNTHESIS_
  virtual
  osss_precise_cycle_count<bits> &
  operator+( const osss_polymorphic<osss_cycle_count> & );
#endif // OSSS_SYNTHESIS_
};

OSSS_SYNTHESISABLE_END_
OSSS_SYNTHESISABLE_EXPORT_( osss_precise_cycle_count );
} // namespace osss

#include "osss_precise_cycle_count.tpp"

#endif // OSSS_PRECISE_CYCLE_COUNT_H
// $Id: osss_precise_cycle_count.h 2910 2008-12-15 18:25:07Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
