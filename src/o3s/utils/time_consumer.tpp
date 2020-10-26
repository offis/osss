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

#include "time_consumer.h"
#include "o3s/utils/osss_report.h"
#include "o3s/utils/osss_assert.h"
#include "o3s/utils/osss_process_handle.h"

namespace osss {

inline
osss_sync_policy
osss_default_sync_policy()
{
  return OSSS_SYNC_STRICT; // osssi::time_consumer_base::default_sync_policy_;
}

#if 0
void
osss_default_sync_policy( osss_sync_policy new_policy )
{
  osssi::time_consumer_base::default_sync_policy_ = new_policy;
}
#endif

namespace osssi {

#if 0
osss_sync_policy
time_consumer_base::default_sync_policy_ = OSSS_SYNC_STRICT;
#endif

inline
void
time_consumer_base::wait( unsigned int steps )
{
  OSSS_ASSERT( steps );
  sync_pending_ = true;
  do_wait( steps );

  sync(); // currently, always sync()
}

inline
void
time_consumer_base::wait( osss_time_type const & delay )
{
  sync_pending_ = true;
  do_wait( delay );

  sync(); // currently, always sync()
}

inline
void
time_consumer_base::sync( osss_sync_policy /* UNUSED new_policy */ )
{
  if( sync_pending_ ) {
    do_sync();
  }
  sync_pending_ = false;
}

/* -------------------------------------------------------------------- */

inline
void
no_time_consumer::do_wait( unsigned int )
  { report_error(); }

inline
void
no_time_consumer::do_wait( osss_time_type const& )
  { report_error(); }

inline
void
no_time_consumer::do_sync()
  { report_error(); }

inline
void
no_time_consumer::report_error()
{
  OSSS_REPORT_FATAL( osss::report::internal_error )
    % "Invalid time consumer.";
}

/* -------------------------------------------------------------------- */

inline
void
clocked_time_consumer::reference_clock( clock_signal_type const & clk )
{
  // ensure, we have a clock signal
  sc_core::sc_clock const * clock_signal =
    dynamic_cast< sc_core::sc_clock const * >( &clk );
  OSSS_ASSERT( clock_signal );

  // try to get the clock period
  step_width_ = clock_signal->period();

  // extract the clock event
  // TODO: support negative edges?
  ref_ = & clock_signal->posedge_event();
}

inline
void 
clocked_time_consumer::reference_clock( clock_port_type const & clk )
{
  sc_core::sc_interface const * clock_interface = clk.get_interface();
  OSSS_ASSERT( clock_interface );
  clock_signal_type const * clock_signal =
    dynamic_cast< clock_signal_type const * >( clock_interface );
  OSSS_ASSERT( clock_signal );
  this->reference_clock( *clock_signal );
}

inline
void
clocked_time_consumer::do_wait( unsigned int steps )
{
  step_cnt_ += steps;

  if( osss_unlikely( step_width_.value() == 0LL ) ) {
    // TODO: Sample clock period, if not present

    // sc_core::wait( ref_ );
    // osss_time_type start_time = sc_core::sc_time_stamp();
    // sc_core::wait( ref_ );
    // step_width_ = sc_core::sc_time_stamp() - start_time;

    OSSS_REPORT_FATAL( osss::report::internal_error )
      % "No clock period known!";
  }
}

inline
void
clocked_time_consumer::do_wait( osss_time_type const& time )
{
  using sc_dt::uint64;

  if( osss_unlikely( step_width_.value() == 0LL ) ) {
    OSSS_REPORT_FATAL( osss::report::internal_error )
      % "No clock period known!";
  }

  uint64 ticks = time.value();
  step_cnt_ += ticks / step_width_.value();
  // wait an additional cycle, if delay equals zero
  // or there is a remainder of the division
  step_cnt_ += ( ticks % step_width_.value() || ticks == 0 );
}

inline
void
clocked_time_consumer::do_sync()
{
  OSSS_ASSERT( step_cnt_ && step_width_.value() && ref_ );

  // TODO: sanity check clock consistency

  // Do we need to wait for more than one edge?
  if( step_cnt_ -- > 1 ) {

    // wait for all but the last clock in one step
    // Note: We need to ensure, that we step over the
    //       last edge before the final clock, therefore
    //       we add an artificial '0.5' to the delay factor.
    sc_core::wait( step_width_ * ( step_cnt_ + 0.5 ) );

  }
  // wait for the final clock
  sc_core::wait( *ref_ );
  // reset step counter
  step_cnt_ = 0;
}

} // namespace osssi
} // namespace osss


// $Id: time_consumer.tpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour: (osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
