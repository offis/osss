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
#ifndef OSSS_DEVICE_H_INCLUDED
#define OSSS_DEVICE_H_INCLUDED

#include <vector>

#include "o3s/utils/osss_systemc.h"
#include "osss_device_type.h"
#include "o3s/structural/app/osss_type_descriptor.h"
#include "o3s/algorithms/osss_device_scheduler.h"


namespace osss {

/** This class represents a hardware device. It implements
    the reconfiguration controller and makes use of recon
    scheduler instances. */
class osss_device
  : public sc_core::sc_module
{
private:
  /// Current scheduler. Destroyed and replaced on each reset.
  osss_device_scheduler * m_scheduler;

  /// Table to remember all recon objects bound to this device
  std::vector< osss_recon_handle > m_recon_object_table;

  /** This attribute stores a pointer to the device type object
      responsible for us */
  osss_device_type * m_used_device_type_ptr;

  /** This method updates the busy indicator signals towards all clients
      (client = recon obj's access ctrl)
    */
  void
  declareBusy(bool new_state);

public:
  /** This method may be used once during elaboration to specify
      a scheduler class. */
  template< class SchedulerClass >
  void
  setScheduler();

  /** This method may be used once during elaboration to specify
      a cycle counter class to be used by the scheduler. */
  template< class CycleCountClass >
  void
  setCycleCounter();

  /// Simple destructor.
  virtual ~osss_device();

  /// Returns a ptr to the device object we belong to.
  virtual
  osss::osss_device_type *
  getDeviceTypePtr();

  /** Returns a value as an object of the correct cycle count class,
      representing a given number of cycles (specified by 64 bit
      unsigned integer argument)
    */
  virtual
  osss_polymorphic< osss_cycle_count >
  getCycleValue( sc_dt::uint64 value ) const;

  /** After alaboration this can be used to find out
      how many reconfigurable objects are bound to us.
    */
  virtual
  osss_recon_idx
  getNumberOfReconObjects() const;

  /** This method is to be called by the reconfigurable
      object. It performs all necessary protocol simulation.
      The "recon_object", "slot" and "new_type" arguments
      must be given valid values. "old_type" may be
      NULL, this indicates that no differential
      configuration is requested.
      "deadline" is passed on to the appropriate controller
      but has no effect yet.
    */
  virtual
  void
  configure(      osss_recon_handle                    _recon_object,
                  osss_slot_idx                        _slot_nr,
                  osss_type_const_handle               _old_type,
                  osss_type_const_handle               _new_type,
                  osss_polymorphic< osss_cycle_count > _deadline,
            const osssi::OSSS_Process_ID_t             _originator,
                  osss_context_const_handle            _context);

  virtual
  void
  printOn(std::ostream & os) const;

  virtual
  void
  writeXMLStringOn(std::ostream & os) const;

  /// This clock port has to be connected by the user
  sc_core::sc_in< bool > clock_port;
  /// This reset port has to be connected by the user
  sc_core::sc_in<bool> reset_port;

  // temp. disable /** this is the runtime code for the prefetch thread */
  //void prefetchThread();

  /** This is the runtime code for the configuration thread */
  void configurationThread();

  /** This is a SC_METHOD for the "pending" signal only */
  void pendingCalcThread();

  /** Make another reconfigurable object known to the device. */
  virtual
  void
  signInReconObject(osssi::osss_recon_base * fresh_recon_object);

  //osssi::osss_process_handle prefetch_thread_systemc_handle;
  //osssi::osss_process_handle m_configuration_thread_systemc_handle;

  /// Here we store our the scheduler factory.
  osss_device_scheduler::factory_base * m_scheduler_factory;

  /// This member carries our cycle count factory.
  osss_cycle_count::factory_base      * m_cycle_count_factory;

  /** Here we line up all jobs. The order of the jobs
      corresponds to the order of sign-ins for recon objects */
  osss_vector< osss_device_job_descriptor > m_job_table;

  /** This is to be used by the scheduler to get job info */
  const osss_device_job_descriptor &
  getJob(osss_device_job_idx index);

  /** This SystemC hook performs handling of default
      values for scheduler factory and cycle count
      factory entries. */
  virtual
  void
  before_end_of_elaboration();

public:
  /** Constructor. Necessarily pass the device type. */
  osss_device( osss_device_type & new_type
             , sc_core::sc_module_name module_name
                 = sc_core::sc_gen_unique_name("osss_device") );

  /// Required since we don't use SC_CTOR
  SC_HAS_PROCESS( osss_device );
};

inline
::std::ostream &
operator<<(std::ostream & os, const osss_device & object)
{
  object.printOn(os);
  return os;
}

} // end namespace osss

#include "osss_device.tpp"

#endif // OSSS_DEVICE_H
// $Id: osss_device.h 2833 2008-12-04 09:29:20Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
