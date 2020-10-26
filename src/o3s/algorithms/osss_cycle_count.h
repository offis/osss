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

#ifndef OSSS_CYCLE_COUNT_H
#define OSSS_CYCLE_COUNT_H

#ifndef OSSS_MAX_TIME_BITS
#  define OSSS_MAX_TIME_BITS 32
#endif

#include "o3s/utils/osss_systemc.h"
#include "o3s/structural/osss_hw_macros.h"
#include "o3s/datatypes/osss_object.h"
#include "o3s/datatypes/osss_polymorphic.h"

namespace osss {
OSSS_SYNTHESISABLE_BEGIN_

/** This class implements handling of time
    measured in terms of clock cycles. */
class osss_cycle_count
  : public osss_object
{
protected:
  /** Utility method. It calculates the number of clock cycles
      which the given time interval (-> duration) will last
      when using a specific clock (on_clock). If necessary,
      the result is rounded up. */
  static sc_dt::uint64 convert(sc_core::sc_time & duration,
                               sc_core::sc_time & on_clock );

public:

  /** default constructor */
  osss_cycle_count();

  virtual ~osss_cycle_count(){}

  /** Return the number of clock cycles this object is representing.
      Overload this method, this implementation may never be called! */
  virtual
  sc_dt::uint64 getCycleCount() const;

  /** Returns the time when the current number of clock
      cycles (as returned by getCycleCount()) is counted
      using the given reference clock */
  sc_core::sc_time getTime(sc_core::sc_time & reference_clock) const;

  /** Waits as many clock cycles as this object represents */
  void wait() const;

  /** Comparison operators */
  bool operator<(const osss_cycle_count &) const;
  bool operator>(const osss_cycle_count &) const;
  bool operator==(const osss_cycle_count &) const;
  bool operator!=(const osss_cycle_count &) const;

  // Add operator
  virtual
  osss_cycle_count &
  operator+(const osss_polymorphic<osss_cycle_count> &);

  /** This subclass may be used to obtain instances of osss_cycle_count.
      Use this abstrace class type for passing around factories. */
  class factory_base
  {
    public:
    virtual ~factory_base(){};
    /** Obtain an instance which is to be constructed using an
        duration (a) and a clock period (b) */
    virtual osss_polymorphic< osss_cycle_count > getInstance(sc_core::sc_time duration, sc_core::sc_time clock_period) = 0;
    /** Obtain an instance which is to be initialised with a given number of cycles */
    virtual osss_polymorphic< osss_cycle_count > getInstance(sc_dt::uint64 cycles) = 0;
  };

  /** This template is to be used when constructing a non-abstract factory.  */
  template< class T >
  class factory : public factory_base
  {
    public:
    factory(){}
    virtual ~factory(){};

    virtual
    osss_polymorphic< osss_cycle_count > getInstance(sc_dt::uint64 a)
    {
      return T(a);
    }

    virtual
    osss_polymorphic< osss_cycle_count > getInstance(sc_core::sc_time a, sc_core::sc_time b)
    {
      return T(a,b);
    }
  };
}; // end class

inline
::std::ostream & operator<<( ::std::ostream & os,
                             const osss_cycle_count & object )
{
  os << object.getCycleCount();
  return os;
}

OSSS_SYNTHESISABLE_END_
OSSS_SYNTHESISABLE_EXPORT_( osss_cycle_count );

} // namespace osss

#endif // OSSS_CYCLE_COUNT_H
// $Id: osss_cycle_count.h 3022 2009-03-04 22:55:24Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
