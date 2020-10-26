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

#ifndef O3S_UTILS_TIME_CONSUMER_H_INCLUDED
#define O3S_UTILS_TIME_CONSUMER_H_INCLUDED

#include "o3s/utils/osss_common.h"
#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/osss_types.h"

namespace osss {

/// currently unused
enum osss_sync_policy
{
  OSSS_SYNC_KEEP = 0,     //< no change to current policy
  OSSS_SYNC_STRICT,       //< adhere to SystemC default synchronisation
  OSSS_SYNC_CONSERVATIVE, //< only omit synchronisation, if correctness is ensured
  OSSS_SYNC_LAZY          //< omit synchronisation, if not explicitly requested
};

/// currently unused
osss_sync_policy
osss_default_sync_policy();

namespace osssi {

class time_consumer_base
{
  friend osss::osss_sync_policy osss::osss_default_sync_policy();
public:
  time_consumer_base()
    : policy_( osss_default_sync_policy() )
    , step_width_( sc_core::SC_ZERO_TIME )
    , step_cnt_( 0 )
    , sync_pending_( false )
  {}
  // empty destructor
  virtual ~time_consumer_base(){}

  void wait( unsigned int          steps = 1 );
  void wait( osss_time_type const& delay );

  void policy( osss_sync_policy p )
    { policy_ = p; }
  osss_sync_policy policy() const
    { return policy_; }

  void sync( osss_sync_policy new_policy = osss_default_sync_policy() );

  osss_time_type local_time() const;

protected:
  virtual void do_wait( unsigned int ) = 0;
  virtual void do_wait( osss_time_type const& ) = 0;
  virtual void do_sync() = 0;

  osss_sync_policy policy_;
  osss_time_type   step_width_;
  unsigned long    step_cnt_;
  bool             sync_pending_;

private:
  static osss_sync_policy default_sync_policy_;
};

class no_time_consumer
  : public time_consumer_base
{
protected:
  virtual void do_wait( unsigned int );
  virtual void do_wait( osss_time_type const& );
  virtual void do_sync();
private:
  void report_error();
};

#if 0
class free_time_consumer
  : public time_consumer_base
{
protected:
  virtual void do_wait( unsigned int );
  virtual void do_wait( osss_time_type const& );
  virtual void do_sync();
};
#endif

class clocked_time_consumer
  : public  time_consumer_base
{
  typedef sc_core::sc_signal_in_if<bool>         clock_signal_type;
  typedef sc_core::sc_port<clock_signal_type, 1> clock_port_type;
public:

  clocked_time_consumer()
    : ref_( 0 )
  {}

  void reference_clock( clock_signal_type const & );
  void reference_clock( clock_port_type const & );

protected:
  virtual void do_wait( unsigned int );
  virtual void do_wait( osss_time_type const& );
  virtual void do_sync();

private:
  sc_core::sc_event const* ref_;
};

} // namespace osssi
} // namespace osss

#include "time_consumer.tpp"

#endif // O3S_UTILS_TIME_CONSUMER_H_INCLUDED

// $Id: time_consumer.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour: (osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
