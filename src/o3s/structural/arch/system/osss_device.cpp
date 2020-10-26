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

#include "o3s/utils/osss_device_job_descriptor.h"
#include "osss_device.h"
#include "o3s/utils/osss_log.h"
#include "o3s/algorithms/osss_device_modified_round_robin.h"
#include "o3s/algorithms/osss_precise_cycle_count.h"
#include "o3s/structural/app/osss_context_base.h"
#include "o3s/structural/app/osss_recon_base.h"

#include <cmath>

#define DEBUG_OSSS_DEVICE false

using namespace osss::osssi;

namespace osss {

osss_device::osss_device( osss_device_type      & new_type
                        , sc_core::sc_module_name module_name )
  : sc_core::sc_module(module_name)
  , m_recon_object_table(0)
  , clock_port("clock_port")
  , reset_port("reset_port")
{
  m_scheduler_factory    = NULL;
  m_cycle_count_factory  = NULL;
  m_scheduler            = NULL;
  m_used_device_type_ptr = &new_type;

  //SC_CTHREAD(prefetchThread, clock_port);
  //prefetch_thread_systemc_handle = sc_core::sc_get_curr_process_handle();
  //reset_signal_is( reset_port, true );

  SC_CTHREAD(configurationThread, clock_port);
  //m_configuration_thread_systemc_handle =
  //  ::osss::osssi::osss_process_handle::current().get_proc_id();

  reset_signal_is( reset_port, true );

  SC_METHOD(pendingCalcThread);
  sensitive << clock_port;

  // We're done. Now let's tell the log facility about us.
  osss_event_log.addDevice(this);
}

osss_device::~osss_device()
{
  delete m_scheduler;
  delete m_scheduler_factory;
  delete m_cycle_count_factory;
}

const osss_device_job_descriptor &
osss_device::getJob(osss_device_job_idx index)
{
  return m_job_table[index];
}

void
osss_device::declareBusy(bool new_state)
{
  // This method updates the busy indicator signals towards all clients
  // (client = recon obj's access ctrl)
  for (osss_recon_idx idx = 0;
       idx < m_recon_object_table.size();
       ++idx)
  {
    m_recon_object_table[idx]->m_rci_busy.write( new_state );
  }
}

osss_device_type *
osss_device::getDeviceTypePtr()
{
  return m_used_device_type_ptr;
};


/* to be re-enabled later...
/ ** this is the runtime code for the prefetch thread * /
template< class ReconfigurationSchedulerClass >
void
osss_device< ReconfigurationSchedulerClass >::prefetchThread()
{
  int access_controller_table_size = access_controller_table.size();
  if (access_controller_table_size == 0)
  {
    cerr << "WARNING: No reconfigurable objects!" << endl;
    cerr << "       Device: " << *this;
    cerr << endl;
    // This thread starts its holidays...
    while (true) { wait(); }
  }
  while (true)
  {
    osssi::osss_context_base * prefetch_named_context_ptr = NULL;
    for (int recon_object_index = 0;
         recon_object_index < access_controller_table_size;
         ++recon_object_index)
    {
      osssi::osss_recon_base * recon_object_ptr = access_controller_table[recon_object_index].recon_object;
      prefetch_named_context_ptr = recon_object_ptr->fetchPrefetchNamedContextPtr();
      if (prefetch_named_context_ptr != NULL)
      {
        // make it active but do nothing
        prefetch_named_context_ptr.enable();
      }
    }
    if (prefetch_named_context_ptr == NULL)
    {
      // we were lazy, no time elapsed
      wait();
    }
  }
}
*/

void
osss_device::printOn(std::ostream & os) const
{
  os << "[HwDevice:type=" << *m_used_device_type_ptr
     << "|name=" << name()
     << "|recon_objects:";
  for (unsigned int i = 0; i < m_recon_object_table.size(); ++i)
  {
    os << std::endl << *(m_recon_object_table[i]);
  }
  os << std::endl;
}

void
osss_device::writeXMLStringOn(std::ostream & os) const
{
  os << "<device name=\"" << name()
     << "\" id=\"" << this
     << "\" systemc_identifier=\"unknown_" << name()
     << "\" type_ref=\"" << m_used_device_type_ptr
     << "\">";
  for (std::vector< osss_recon_base * >::size_type recon_object_index = 0;
       recon_object_index < m_recon_object_table.size();
       ++recon_object_index)
  {
    m_recon_object_table[recon_object_index]->writeXMLStringOn(os);
  }
  os << "</device>";
}

void
osss_device::signInReconObject(osss_recon_base * new_recon_object)
{
   // Make a new recon object knwon to us
   m_recon_object_table.push_back(new_recon_object);
}

void
osss_device::configure(      osss_recon_handle                    _recon_object,
                             osss_slot_idx                        _slot_idx,
                             osss_type_const_handle               _old_type,
                             osss_type_const_handle               _new_type,
                             osss_polymorphic< osss_cycle_count > _deadline,
                       const OSSS_Process_ID_t                    _originator,
                             osss_context_const_handle            _context)
{
  // First: find our index in access_controller_table. If this
  // is performance critical in future: Replace it with a
  // tree or hash datastructure for the mapping.
  unsigned int index = 0;
  while (m_recon_object_table[index] != _recon_object)
  {
    ++index;
  }
  // Then update the entries with the caller's data.
  _recon_object->m_rci_slot     = _slot_idx;
  _recon_object->m_rci_old_class= _old_type;
  _recon_object->m_rci_new_class= _new_type;
  _recon_object->m_rci_deadline = _deadline; // unsupported for now
  _recon_object->m_rci_originator = _originator; // just for logging
  _recon_object->m_rci_context    = _context;  // just for logging

  osss_slot_descriptor * sd_ptr
            = _recon_object->resolveSlotIdx(_slot_idx);

  if (NULL == _context)
  {
    std::cerr << "ERROR! configure(): _context==NULL" << std::endl;
    OSSS_ABORT();
  }

  osss_event_log.write(osssi::osss_log::configurationEvent,
                  osssi::osss_log::requestEvent,
                  osssi::osss_log::startEvent,
                  this,
                  _recon_object,
                  sd_ptr,
                  _context->name(),
                  _originator);

  // Request an action
  OSSS_MESSAGE(DEBUG_OSSS_DEVICE, "AC: Requesting reconfiguration");
  _recon_object->m_rci_do.write( true );
  // Wait at least one cycle
  wait();

  osss_event_log.write(osssi::osss_log::configurationEvent,
                  osssi::osss_log::requestEvent,
                  osssi::osss_log::stopEvent,
                  this,
                  _recon_object,
                  sd_ptr,
                  _context->name(),
                  _originator);

  // Wait until action performed
  OSSS_MESSAGE(DEBUG_OSSS_DEVICE, "AC: Wait for acknowledgement");
  while ( ! _recon_object->m_rci_done.read() )
  {
    wait();
  }
  OSSS_MESSAGE(DEBUG_OSSS_DEVICE, "AC: Acknowledgement seen");
  _recon_object->m_rci_do.write( false );

  // Apparently the reconfiguration controller has served our request
  osss_event_log.write(osssi::osss_log::configurationEvent,
                    osssi::osss_log::returnEvent,
                    osssi::osss_log::startEvent,
                    this,
                    _recon_object,
                    sd_ptr,
                    _context->name(),
                    _originator);
  osss_event_log.write(osssi::osss_log::configurationEvent,
                    osssi::osss_log::returnEvent,
                    osssi::osss_log::stopEvent,
                    this,
                    _recon_object,
                    sd_ptr,
                    _context->name(),
                    _originator);
}

// SC_METHOD
void
osss_device::pendingCalcThread()
{
  std::vector< osss_recon_base * >::size_type access_controller_table_size = m_recon_object_table.size();
  bool pending = false;
  for (std::vector< osss_recon_base * >::size_type index = 0;
       (false == pending) && (index < access_controller_table_size);
       ++index)
  {
    // go != ok ===> request is pending
    pending = pending || m_recon_object_table[index]->m_rci_do.read();
  }
  for (std::vector< osss_recon_base * >::size_type index = 0;
       index < access_controller_table_size;
       ++index)
  {
    // go != ok ===> request is pending
    m_recon_object_table[index]->m_rci_pending.write(pending);
  }
}
/*
void
osss_device::operator()(osss_recon_base & recon)
{
  // call the binding the other way round
  recon(*this);
}
*/

osss_recon_idx
osss_device::getNumberOfReconObjects() const
{
  return m_recon_object_table.size();
}

void
osss_device::before_end_of_elaboration()
{
  m_job_table = osss_vector< osss_device_job_descriptor >( m_recon_object_table.size() );

  if (NULL == m_scheduler_factory)
  {
    // use default one
    setScheduler< osss::osss_device_modified_round_robin >();
  }
  if (NULL == m_cycle_count_factory)
  {
    // use default one
    setCycleCounter< osss_precise_cycle_count<> >();
  }
}

// Note that this thread does NOT rely on the pending-signal produced
// by the pendingCalcThread()! That would be visible one cycle delayed!
void
osss_device::configurationThread()
{
  // TODO: Allow user to override this manually since configuration
  // clock is likely to differ from this thread's clock!
  sc_core::sc_time configuration_clock_period =
    dynamic_cast< sc_core::sc_clock * >(clock_port.get_interface())->period();

  // Get period ouf the clock driving this process.
  // TODO: Use configuration clock in future, make this obsolete
  sc_core::sc_time process_clock_period =
    dynamic_cast< sc_core::sc_clock * >(clock_port.get_interface())->period();

  // Sanity check
  std::vector< osss_recon_base * >::size_type recon_object_table_size = m_recon_object_table.size();
  if (recon_object_table_size == 0)
  {
    std::cerr << "WARNING: No reconfigurable objects!" << std::endl;
    std::cerr << "       Device: " << *this  << std::endl;
    // This thread starts its holidays...
    while (true) { sc_core::wait(100000); }
  }

  // Reset things:
  // Communication with clients
  declareBusy(true);
  for (std::vector< osss_recon_base * >::size_type index = 0;
       index < recon_object_table_size;
       ++index)
  {
    m_recon_object_table[index]->m_rci_done.write(false);
  }
  // Fresh scheduler
  if (NULL != m_scheduler)
  {
    delete m_scheduler;
  }
  m_scheduler = m_scheduler_factory->getInstance(this);
  // Signal to outside world: We are not busy right now
  declareBusy(false);
  // Wait for all the above to settle.
  wait();

  while (true)
  {
    //OSSS_MESSAGE(DEBUG_OSSS_DEVICE, "collecting");
    bool request = false;

    // This loop may take several cycles to iterate.
    for (std::vector< osss_recon_base * >::size_type index = 0;
         index < recon_object_table_size;
         ++index)
    {
      osss_recon_base * recon_ptr = m_recon_object_table[index];
      osss_device_job_descriptor & job = m_job_table[index];
      // request is pending
      job.m_valid = recon_ptr->m_rci_do.read();
      if (true == job.m_valid)
      {
        request = true;
        job.m_new_logic              = recon_ptr->m_rci_new_class;
        job.m_old_logic              = recon_ptr->m_rci_old_class;
        job.m_location               = recon_ptr->m_rci_slot;
        sc_core::sc_time conf_time   = job.m_new_logic->getConfigurationTime(recon_ptr, job.m_old_logic);
        job.m_configuration_cycles   = m_cycle_count_factory->getInstance(conf_time, configuration_clock_period);
      }
    }
    // end of collection cycle
    wait();

    // Is there something to do at all?
    if (true == request)
    {
      // We'll do some work from next cycle on, we're busy...
      declareBusy(true);

      OSSS_MESSAGE(DEBUG_OSSS_DEVICE, "PIRC: Scheduling");

      // let the scheduler work (may take some cycles)
      std::vector< osss_recon_base * >::size_type index = m_scheduler->schedule();

      // Scheduler cycle
      wait();

      OSSS_MESSAGE(DEBUG_OSSS_DEVICE, "PIRC: Scheduling completed");

      // Safety check:
      if (index >= recon_object_table_size)
      {
        std::cerr << "ERROR! Reconfiguration scheduler returned illegal grant." << std::endl;
        OSSS_ABORT();
      }
      // "Serve..."
      osss_device_job_descriptor job = m_job_table[index];

      osss_slot_descriptor * sd_ptr = m_recon_object_table[index]->resolveSlotIdx( job.m_location );

      osss_event_log.write(osss_log::configurationEvent,  osss_log::executeEvent, osss_log::startEvent,
                           this,
                           m_recon_object_table[index],
                           sd_ptr,
                           m_recon_object_table[index]->m_rci_context->name(),
                           m_recon_object_table[index]->m_rci_originator);
      sc_core::sc_time duration =
        job.m_new_logic->getConfigurationTime(m_recon_object_table[index], job.m_old_logic);
      if (duration > sc_core::SC_ZERO_TIME)
      {
        OSSS_MESSAGE( DEBUG_OSSS_DEVICE, "PIRC: Requesting configuration start from PDRC");
        wait(); // assume one clock cycle overhead to tell
                // dev.dep.reconf.ctrl. part to start

        wait(); // assume one clock cycle overhead for scheduling inside PDRC

        // duration to clock cycles, rounded up
        unsigned int cycles =
          static_cast< unsigned int >( std::ceil(duration / process_clock_period) );

        OSSS_MESSAGE(DEBUG_OSSS_DEVICE,
          "PDRC: Re-configuring to class "
            << job.m_new_logic->getTypeName()
            << ", lasts " << cycles << " clock cycles" );
        // wait
        wait(cycles);

        OSSS_MESSAGE(DEBUG_OSSS_DEVICE,  "PDRC: Re-configuration done, asserting \"done\"");

        wait(); // assume one clock cycle overhead to recognise
                // dev.dep.reconf.ctrl. part telling us: "job done"
        OSSS_MESSAGE(DEBUG_OSSS_DEVICE,  "PDRC: De-asserting \"done\", PIRC: Recognising configuration done");
      }

      osss_event_log.write(osss_log::configurationEvent,  osss_log::executeEvent, osss_log::stopEvent,
                           this,
                           m_recon_object_table[index],
                           sd_ptr,
                           m_recon_object_table[index]->m_rci_context->name(),
                           m_recon_object_table[index]->m_rci_originator);

      OSSS_MESSAGE(DEBUG_OSSS_DEVICE, "PIRC: Assert \"done\"");

      // Acknowledge to reconfigurable object: we're done
      m_recon_object_table[index]->m_rci_done.write( true );

      // We're no longer busy in future
      declareBusy(false);

      wait();
      m_recon_object_table[index]->m_rci_done.write( false );

      OSSS_MESSAGE(DEBUG_OSSS_DEVICE, "PIRC: De-assert \"done\"");

      // Give access controller one cycle to take back m_rci_do
      wait();
    } // if request
  } // while true
}

osss_polymorphic< osss_cycle_count >
osss_device::getCycleValue( sc_dt::uint64 value ) const
{
  return m_cycle_count_factory->getInstance( value );
}

} // end namespace osss

// $Id: osss_device.cpp 2888 2008-12-10 12:49:47Z andreas $
// :flavour: (osss,s) (osss_recon,s) (osss_full,s)
