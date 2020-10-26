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

#include "osss_recon_base.h"
#include "osss_context_base.h"
#include "o3s/utils/osss_process_handle.h"
#include "o3s/utils/osss_recon_job_descriptor.h"
#include "o3s/utils/osss_slot_descriptor.h"
#include "o3s/utils/time_consumer.h"
#include "o3s/utils/osss_log.h"
#include "o3s/structural/arch/system/osss_device.h"
#include "o3s/algorithms/osss_precise_cycle_count.h"
#include "o3s/algorithms/osss_first_fit_placer.h"
#include "o3s/algorithms/osss_recon_modified_round_robin.h"
#include "o3s/structural/arch/system/osss_device_type.h"
#include "o3s/structural/arch/system/osss_device.h"
#include "o3s/structural/app/osss_clear_helper_registry.h"
#include "o3s/structural/app/osss_context_base.h"
#include "o3s/utils/osss_user_process_descriptor.h"

namespace osss {
namespace osssi {

osss_recon_base::osss_recon_base( sc_core::sc_module_name )
// TODO: initialise all members properly!
  : sc_core::sc_module()
  , clock_port("clock_port")
  , reset_port("reset_port")
  , m_hardware_device_ptr(NULL)
  , m_num_contexts(0)
  , m_num_slots(1)
  , m_num_parallel_accesses_allowed(1)

  , m_job_table(0) // no elements
  , m_temporary_context(0)
{
  m_cycle_count_factory              = NULL;
  m_scheduler_factory                = NULL;
  m_scheduler                        = NULL;
  m_num_contexts                     = 0;
  m_placer_factory                   = NULL;
  m_placer                           = NULL;

  SC_CTHREAD(multiCycleThread, clock_port.pos() );
  sc_core::sc_module::reset_signal_is(reset_port, true);

  SC_CTHREAD(singleCycleThread, clock_port.pos() );
  sc_module::reset_signal_is(reset_port, true);
}

osss_recon_base::~osss_recon_base()
{
  delete m_cycle_count_factory;
  delete m_scheduler_factory;
  delete m_placer_factory;
  delete m_scheduler;
  delete m_placer;
  delete m_temporary_context;
}

void
osss_recon_base::end_of_elaboration()
{
  sc_module::end_of_elaboration();

  if (NULL == m_hardware_device_ptr)
  {
    std::cerr << "ERROR! The recon object named \""
              << name()
              << "\" is not bound to a device instance at end of elaboration"
              << std::endl;
    OSSS_ABORT();
  }

  // All user processes must have registered by now.

  // First: Sanity checks!
  if ((m_num_slots > 1) && (m_num_slots > m_num_contexts))
  {
    std::cerr << "WARNING! Number of slots is " << m_num_slots
              << " while number of named contexts" << std::endl
              << "         bound to the reconfigurable object is "
              << m_num_contexts << "." << std::endl
              << "         Reducing amount of slots for reconfigurable object \""
              << this->name() << "\"" << std::endl;
    m_num_slots = m_num_contexts;
  }

  if (m_num_parallel_accesses_allowed > m_num_slots)
  {
    std::cerr << "WARNING! Number of parallel accesses to reconfigurable object is "
              << m_num_parallel_accesses_allowed << std::endl
              << "         while number of slots is " << m_num_slots
              << "." << std::endl
              << "         Adjusting amount of max parallel accesses for reconfigurable object \""
              << this->name() << "\"" << std::endl;
    m_num_parallel_accesses_allowed = m_num_slots;
  }

  if (m_num_parallel_accesses_allowed > m_user_process_table.size())
  {
    std::cerr << "WARNING! Number of parallel accesses to reconfigurable object is "
        << m_num_parallel_accesses_allowed << std::endl
        << "         while number of user processes is "
        << m_user_process_table.size() << "." << std::endl
        << "         Adjusting amount of max parallel accesses for reconfigurable object \""
        << this->name() << "\"" << std::endl;
    m_num_parallel_accesses_allowed = m_user_process_table.size();
  }

  // This tiny statement sets up the appropriate slot table.
  m_slot_table.resize(m_num_slots);

  if (NULL == m_cycle_count_factory)
  {
    setCycleCounter< osss_precise_cycle_count<> >();
  }

  if (NULL == m_scheduler_factory)
  {
    // use default one
    setScheduler< osss_recon_modified_round_robin >();
  }

  if (NULL == m_placer_factory)
  {
    // use default one
    setPlacer< osss_first_fit_placer >();
  }

  // Now, that we know all user processes and all contexts,
  // we can set up the external-lock entries for all contexts.
  // This was impossible before all user processes were known.
  for (context_pool_type::index_type context_index = 0;
       context_index < m_num_contexts;
       ++context_index)
  {
    osss_context_base * nc_ptr = m_context_table.getKey( context_index );
    nc_ptr->m_external_locks.resize(m_user_process_table.size());
    for (osss_user_process_idx index = 0;
         index < m_user_process_table.size();
         ++index)
    {
      nc_ptr->m_external_locks[index] = false;
    }
  }

  // set up job table since all clients are signed in by now
  m_job_table =
    osss_vector< osss_recon_job_descriptor >( getNumberOfUserProcesses() );

  // prepare time consumers

  typedef user_process_pool_type::index_type index_type;

  index_type num_clients = m_user_process_table.size();
  for( index_type idx = 0; idx < num_clients; ++idx )
  {
    time_consumer_base * tc =
         m_user_process_table.getInfoPtrByIndex( idx )->m_time_consumer;

    if( clocked_time_consumer* ctc =
          dynamic_cast<clocked_time_consumer*>(tc) )
    {
      ctc->reference_clock( clock_port );
    } else {
      // ignore this - time consumer might work on its own
    }
  }
}

time_consumer_base*
osss_recon_base::time_consumer() const
{
  osss_user_process_descriptor * upd =
    m_user_process_table.getInfoPtrByIndex( myUserProcessIndex() );

  if( upd->m_time_consumer )
    return upd->m_time_consumer;

  // no custom time consumer found, but one is needed
  //
  // This can be caused due to wait( sc_time ) calls from
  // within a user object.  In this case, update the current
  // process descriptor to use a newly created time consumer, even
  // if no consumer would be strictly needed.
  //
  // Warning: This can hide errors in case of multiple clocks
  //          (and therefore inconsistent time computations)!
  clocked_time_consumer* ctc = new clocked_time_consumer();
  ctc->reference_clock( clock_port );
  // we need to remember the newly created time consumer to avoid
  // a memory leak here!
  upd->m_time_consumer = ctc;
  return ctc;
}

void
osss_recon_base::setNumSlots(osss_slot_idx number_of_slots)
{
  // It doesn't make sense to modify the number of slots for
  // a reconfigurable object after the elaboration phase is done.
  if ( sc_core::sc_start_of_simulation_invoked() )
  {
    OSSS_REPORT_WARNING( osss::report::called_after_elaboration )
      % __PRETTY_FUNCTION__
      << "Ignored, number of slots for ReconObject "
      << name() << " stays " << m_num_slots << ".";
    // abort
    return;
  }
  // A recon object with no slots would be able to do nothing!
  if (0 == number_of_slots)
  {
    std::cerr << "ERROR: cannot set number of slots to zero" << std::endl;
    OSSS_ABORT();
  }
  // Accept
  m_num_slots = number_of_slots;
}

void
osss_recon_base::setNumParallelAccessesAllowed(unsigned int number_of_parallel_accesses)
{
  // The number of accesses has to be fixed at end of elaboration.
  if (sc_core::sc_start_of_simulation_invoked())
  {
    OSSS_REPORT_WARNING( osss::report::called_after_elaboration )
      % __PRETTY_FUNCTION__
      << "Ignored, number of parallel accesses for ReconObject '"
      << name() << "' stays " << m_num_parallel_accesses_allowed << ".";
    // abort
    return;
  }
  if (0 == number_of_parallel_accesses)
  {
    std::cerr << "ERROR: cannot set maximum number of parallel accesses to zero" << std::endl;
    OSSS_ABORT();
  }
  m_num_parallel_accesses_allowed = number_of_parallel_accesses;
}


void
osss_recon_base::signInNamedContext( osss_context_base * named_context_object )
{
  // Check, if we are still within elaboration phase. It would make
  // no sense to add a context afterwards.
  if (sc_core::sc_start_of_simulation_invoked())
  {
     OSSS_REPORT_ERROR( osss::report::called_after_elaboration )
       %  "osss_recon::signInNamedContext()";
     return;
  }

  // add a fresh descriptor
  m_context_table.add(named_context_object, osss_context_descriptor());
  ++m_num_contexts;
}

void
osss_recon_base::signInUserProcess( osss_process_handle  const & user_process,
                                    osss_priority                priority )
{
  // We cannot accept new user processes after simulation start. New user processes
  // would be equivalent to new port connections, larger internal tables
  // etc. That doesn't make sense.
  if (sc_core::sc_start_of_simulation_invoked())
  {
     OSSS_REPORT_ERROR( osss::report::called_after_elaboration )
       %  "osss_recon::signInUserProcess()";
     return;
  }

  OSSS_Process_ID_t proc_id = user_process.get_proc_id();

  // Add new user process to the table
  m_user_process_table.add(proc_id);
  osss_user_process_descriptor * upd = m_user_process_table.getInfoPtrByKey(proc_id);
  upd->m_user_process_priority = priority;
  upd->m_id = proc_id;

  switch( user_process.proc_kind() ) {
    case sc_core::SC_THREAD_PROC_:
      // create a corresponding time consumer
      upd->m_time_consumer = new clocked_time_consumer;
      break;
    case sc_core::SC_CTHREAD_PROC_:
      // we have a clock, do nothing
      break;
    case sc_core::SC_METHOD_PROC_:
    case sc_core::SC_NO_PROC_:
      // TODO: report error here
      break;
  }

  ::osss_event_log.addUserProcess(proc_id, priority);
}


osss_user_process_idx
osss_recon_base::getNumberOfUserProcesses() const
{
  return m_user_process_table.size();
}

osss_context_idx
osss_recon_base::getNumberOfContexts() const
{
  return m_num_contexts;
}

osss_type_idx
osss_recon_base::getNumberOfClasses() const
{
  return m_hardware_device_ptr->getDeviceTypePtr()->getNumberOfTypes();
}

osss_slot_idx
osss_recon_base::getNumberOfSlots() const
{
  return m_num_slots;
}

osss_device*
osss_recon_base::getDevicePtr() const
{
  return m_hardware_device_ptr;
}

const osss_recon_job_descriptor &
osss_recon_base::getJob(osss_user_process_idx index) const
{
  return m_job_table[index];
}

void
osss_recon_base::bind(osss_device & hw_device)
{
  // Check, if we were bound before
  if (m_hardware_device_ptr == NULL)
  {
    // No. Well, then hw_device denotes the hardware device
    // this reconfigurable object is located. We can access the
    // reconfiguration controller via this information.
    m_hardware_device_ptr = &hw_device;
    // Let the device know that we do exist.
    m_hardware_device_ptr->signInReconObject(this);
  }
  else
  {
    // We are already placed on a device. It doesn't make sense to do
    // this twice. Complain. Most likely the user has written something
    // he didn't intend to do.
    std::cerr << "ERROR: object already bound to a hardware device"
              << std::endl;
    OSSS_ABORT();
  }
}

void
osss_recon_base::operator()(osss_device & hw_device)
{
  // call the more verbose form of ourselves
  bind(hw_device);
}

osss_recon_base::user_process_pool_type::index_type
osss_recon_base::myUserProcessIndex() const
{
  user_process_pool_type::index_type user_process_index;
  OSSS_Process_ID_t key = osss_process_handle::current().get_proc_id();
  user_process_index = m_user_process_table.findIndex( key );
  if (false == m_user_process_table.isIndexLegal(user_process_index))
  {
    std::cerr << "ERROR! User process was not registered to use this osss_recon object!"
              << std::endl;
    std::cerr << "       osss_recon named \"" << name() << "\"" << std::endl;
    std::cerr << "       Process is named \"" << osss_process_handle::current().name()
              << std::endl;
    OSSS_ABORT();
  }
  return user_process_index;
}

osss_object *
osss_recon_base::getOSSSObjectPtr(bool check_for_null) const
{
  // Check if we live in a multi-slot world. If that is the case, it is
  // ambiguous which content ptr should be returned. We do not accept this.
  if (1 != m_num_slots)
  {
    std::cerr << "ERROR! Cannot access anonymous context of a multi-slot reconfigurable object" << std::endl;
    OSSS_ABORT();
  }
  // Get the pointer to the object. Here we can assume that we live
  // in a single-slot world, therefore 0 is the one and only valid
  // slot index. We grab in there and use the named context pointer
  // which itself can tell us the current content of that named context.
  // Note that the anonymous context is, technically speaking, a
  // named context where the user cannot access the name (and we
  // know how to treat this special one).
  osss_object * cb_ptr = m_context_table.getKey(m_slot_table[0].m_context)->getOSSSObjectPtr();

  // If requested, perform NULL check
  if (check_for_null && (NULL == cb_ptr))
  {
    std::cerr << "ERROR! osss_recon has content NULL" << std::endl;
    OSSS_ABORT();
  }

  // The pointer is valid. Now double-check, if the type is ok.
  if( !check_runtime_type( cb_ptr ) )
  {
    std::cerr << "ERROR! osss_recon has context_base object which is not derived from interface root class" << std::endl;
    std::cerr << "osss_context object is of type \""
           << osssi::type_id(*cb_ptr)
           << "\""
           << std::endl
           << "which is not derived from osss_recon's class \""
           << if_typename()
           << "\""
           << std::endl;
     std::cerr << "Possible reasons:"
           << "1) You forgot to assign something to an osss_context object"
           << "2) The class " << if_typename() << " (or one of its derived classes) has no OSSS_BASE_CLASS()"
           << "   and/or non-virtal destructor (this may cause the your compilers RTTI to fail)."
           << std::endl;
      OSSS_ABORT();
  }

  // All checks passed. Proceed.
  return cb_ptr;
}

void
osss_recon_base::multiCycleThread()
{
  // This CTHREAD does all the maintenance of user process requests. It does not
  // handle returning of permissions.

  user_process_pool_type::index_type number_of_user_processes = m_user_process_table.size();
  // Find out the clock period which drives us.
  sc_core::sc_time clock_period = dynamic_cast< sc_core::sc_clock * >(clock_port.get_interface())->period();

  // Reset things. First, clear the "ok" towards the user processes.
  for (user_process_pool_type::index_type user_process_index = 0;
       user_process_index < number_of_user_processes;
       ++user_process_index)
  {
    m_user_process_table.getInfoPtrByIndex(user_process_index)->m_ack.write(false);
  }
  // Clear all contexts.
  for (context_pool_type::index_type context_index = 0;
        context_index < m_num_contexts;
        ++context_index)
  {
    m_context_table.getInfoPtrByIndex( context_index )->m_enabled = false;
    m_context_table.getKey( context_index )->reset();
  }
  // Clear all slots.
  for (osss_slot_idx slot_index = 0;
       slot_index < m_num_slots;
       ++slot_index)
  {
    // Everybody back to default values...
    m_slot_table[slot_index] = osss_slot_descriptor();
  }
  // Don't request any work from the return thread. It does
  // something for us each clock cycle when we set this signal to "true".
  m_new_grant.write(false);
  // Don't request anything from reconfiguration controller.
  m_rci_do.write(false);

  delete m_scheduler;
  delete m_placer;
  m_scheduler = m_scheduler_factory->getInstance();
  m_placer    = m_placer_factory->getInstance();

  sc_core::wait(); // This cycle is for the reset phase.
                   // The synthesised version resets their
                   // slots now, too.

  while (true)
  {
    // In contrast to return thread, iterating this loop
    // may take several cycles.

    // Remember, if we updated the descriptor tables
    // with convenience values (see precalc() method).
    bool tables_are_updated = false;

    // Remember, if we found out that it is necessary
    // to ask the scheduler for a decision.
    bool call_scheduler     = false;

    // Now process all user processes and see, if they have requests.
    // Collect these requests.
    for (user_process_pool_type::index_type user_process_index = 0;
         user_process_index < number_of_user_processes;
         ++user_process_index)
    {
      // This is the job descriptor we fill for each request.
      // It is consumed by the access scheduler.
      osss_recon_job_descriptor & ajd
           = m_job_table[user_process_index];
      osss_user_process_descriptor * upi
           = m_user_process_table.getInfoPtrByIndex(user_process_index);

      // Default case. Descriptor invalid, nothing to do.
      ajd.m_valid = false;

      // Request from user process?
      if (upi->m_switch_and_permission_obtain.read() || upi->m_create_logic.read())
      {

        // The user process may (simultaneously) request these things:
        // - context activation (by requesting access permission)
        // - context assignment (by requesting creation)
        if (  (true == upi->m_switch_and_permission_obtain.read()) // permission requested?
           && (0 == m_num_grants_available.read()) // no permission left?
           )
        {
          // We don't have any grants to give now... try again later...
          continue; // next user process, please
        }

        // pick slots etc.
        if (! tables_are_updated)
        {
          // do this when we come here the first time
          // in a clock cycle
          precalc();
          tables_are_updated = true;
        }

        // See, which context is requested.
        if (NULL == upi->m_context)
        {
          // Anonymous context. This is ok
          // in a one-slot world...
          if (1 == m_num_slots)
          {
            // Does the user process wish to re-create a context?
            if (true == upi->m_create_logic.read())
            {
              // "Creation of anonymous context" is a special case: We create a
              // temporary context!
              ajd.m_context = m_temporary_context_index;
            }
            else
            {
              // An ordinary access, no re-creation.
              // Translate request to anonymous context: "Anything there currently is
              // in the first slot". 0 is the first (and only) slot.
              if (NULL == m_slot_table[0].m_class)
              {
                std::cerr << "ERROR! Encountered a request upon a non-initialised anonymous context" << std::endl
                          << "       of reconfigurable object \"" << name() << "\"." << std::endl
                          << "       Solution: Assign content first." << std::endl;
                OSSS_ABORT();
              }
              else
              {
                ajd.m_context = m_slot_table[0].m_context;
              }
            }
          }
          else
          {
            // Argh. Complain!
            std::cerr << "ERROR! Request for anonymous context from multi-slot reconfigurable object" << std::endl
                      << "       \"" << name() << "\"." << std::endl;
            OSSS_ABORT();
          }
        }
        else
        {
          // We don't need to translate anything, the user process
          // gave us a valid context id.
          ajd.m_context  = m_context_table.findIndex(upi->m_context);
        }
        // now ajd.m_context points to the context to activate, access or lock
        osss_context_base * new_nc_ptr
            = m_context_table.getKey( ajd.m_context );

        // Now determine involved classes:
        // a) The class which currently sits in the designated target slot
        ajd.m_old_slot_class    = m_slot_table[new_nc_ptr->m_suitable_slot_index_precalc].m_class;
        // b) The context's class before a possible "create" step
        ajd.m_old_context_class = new_nc_ptr->getTypePtr();
        // c) The context's class after a possible "create" step
        if (true == upi->m_create_logic.read())
        {
          // There IS a create step :)
          ajd.m_new_class = upi->m_new_class;
        }
        else
        {
          // We don't re-create, type stays the same
          ajd.m_new_class = ajd.m_old_context_class;
        }

        // Now the new class must be known, otherwise we can't make
        // the context accessible and/or cannot create it
        if (NULL == ajd.m_new_class)
        {
          std::cerr << "ERROR! Attempt to get permission on an uninitialized context of" << std::endl
                    << "       reconfigurable object \"" << name() << "\""
                    << std::endl;
          OSSS_ABORT();
        }

        // Find the context descriptor
        osss_context_descriptor * cd = m_context_table.getInfoPtrByIndex( ajd.m_context );

        // See, if the context is currently enabled
        if (cd->m_enabled)
        {
          // Yes, currently enabled, sir...

          // We permit the request, if
          //     a) a grant is requested on a context on which nobody got a grant before
          // or  b) the user process desires to create logic only (no grant request),
          //        which means that the user process already holds a grant
          if (  (false == m_slot_table[cd->m_current_slot_index].m_granted)
             || (false == upi->m_switch_and_permission_obtain.read()))
          {
            // everything ok, we accept the job
            ajd.m_valid = true;
            ajd.m_user_process_priority = upi->m_user_process_priority;

            // The configuration time is zero if classes are the same
            sc_core::sc_time duration = ajd.m_new_class->getConfigurationTime(this, ajd.m_old_slot_class);
            ajd.m_configuration_cycles = m_cycle_count_factory->getInstance(duration, clock_period);

            // We don't need to save & restore an enabled context.
            ajd.m_save_required = false;
            ajd.m_save_cycles = m_cycle_count_factory->getInstance(0);
            ajd.m_restore_required = false;
            ajd.m_restore_cycles = m_cycle_count_factory->getInstance(0);

            // We call the scheduler, even if this stays the only job.
            // This enables the scheduler to track all grants without
            // gaps (important for Round Robin etc.).
            call_scheduler = true;
          }
        }
        else
        {
          // No, desired context is disabled.
          osss_slot_idx placers_choice = new_nc_ptr->m_suitable_slot_index_precalc;

          // Was the placer successful in finding a slot for the context?
          if (placers_choice != m_num_slots)
          {
            ajd.m_valid = true;
            ajd.m_user_process_priority = upi->m_user_process_priority;

            // Determine which context sits in this slot
            // prior to enabling the new one.
            osss_slot_descriptor * target_slot = &(m_slot_table[placers_choice]);

            // Context saving
            ajd.m_save_required =
                // save required if:
                // a) old one is not NULL (uninitialised) and
                (  (target_slot->m_allocated)
                // b) old one != temporary one
                && (false  == isTemporary(target_slot->m_context)));

            // Calculate cycles required for context saving
            if (true == ajd.m_save_required)
            {
              // 2 additional cycles for "accept" phase overhead
              // 2 additional cycles for access ctrl <-> storage "go/ok" protocol
              ajd.m_save_cycles = m_cycle_count_factory->getInstance(ajd.m_old_slot_class->getAttributeCopyTime()
                                                                     + (2+2)*clock_period,
                                                                     clock_period);
            }
            else
            {
              ajd.m_save_cycles = m_cycle_count_factory->getInstance(0);
            }

            // Reconfiguration
            sc_core::sc_time duration  = ajd.m_new_class->getConfigurationTime(this, ajd.m_old_slot_class);
            // Duration is SC_ZERO_TIME, iff classes match
            ajd.m_configuration_cycles = m_cycle_count_factory->getInstance(duration, clock_period);

            // Context restauration
            ajd.m_restore_required =
                        // a) the new one is not the temporary one and
                        (  (false == isTemporary( ajd.m_context ))
                        // b) the new context is not re-created
                        && (false == upi->m_create_logic.read())
                        // c) the new one is already initialised
                        && (ajd.m_new_class != NULL) );
            if (ajd.m_restore_required)
            {
              // 2 additional cycle for "accept" overhead
              // 2 additional cycles for access ctrl <-> storage "go/ok" protocol
              ajd.m_restore_cycles = m_cycle_count_factory->getInstance(ajd.m_new_class->getAttributeCopyTime()
                                                                    + (2+2)*clock_period,
                                                 clock_period);
            }
            else
            {
              ajd.m_restore_cycles = m_cycle_count_factory->getInstance(0);
            }

            // There is at least one valid job (this one...)
            call_scheduler = true;
          }
        } // else inactive
      }  // if request
    } // for user process

    // end of collection cycle
    sc_core::wait();

    // Did the collection cycle give us any homework?
    if (call_scheduler)
    {
      // there is at least one request a scheduler may accept

      osss_recon_job_idx job_index = 0;
      if (getNumberOfUserProcesses() > 1)
      {
        // Schedule! This may take several cycles. The job index is
        // also usable as user process index.
        OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Execution cycle: calling scheduler");
        job_index  = m_scheduler->schedule();
        // But it takes at least one cycle
        sc_core::wait();
      }
      else
      {
        OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Execution cycle: no need to call scheduler");
      }

      // For convenience only
      osss_recon_job_descriptor    & ajd = m_job_table[job_index];
      osss_context_descriptor      * cd  = m_context_table.getInfoPtrByIndex(ajd.m_context);
      osss_context_base     * new_nc_ptr = m_context_table.getKey( ajd.m_context );
      osss_slot_descriptor         * sd  = &(m_slot_table[new_nc_ptr->m_suitable_slot_index_precalc]);
      osss_user_process_descriptor * upi = m_user_process_table.getInfoPtrByIndex(job_index);

      // update upi->context_id, requestPermission() needs it
      upi->m_context = m_context_table.getKey(ajd.m_context);

      // We now execute the selected job

      // save
      if ( true == ajd.m_save_required )
      {
        OSSS_MESSAGE(DEBUG_RECON_OBJECT,
          "Clearing transient attributes of " << sd->m_context );
        // Clear transient attributes. We consume the time for that step later on
        // during slot reset cycle.
        osss_context_base * old_nc_ptr = m_context_table.getKey( sd->m_context );
        osss_clear_helper_registry::instance.clear(old_nc_ptr->getOSSSObjectPtr() );

        OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Saving context " << sd->m_context
                                          << " [" << ajd.m_save_cycles->getCycleCount() << " cycles]");
        ::osss_event_log.write(osss_log::attrSaveEvent,
                               osss_log::executeEvent,
                               osss_log::startEvent,
                               m_hardware_device_ptr,
                               this,
                               sd,
                               old_nc_ptr->name(), // old context!
                               upi->m_id);
        sc_core::wait( ajd.m_save_cycles->getCycleCount() );
        ::osss_event_log.write(osss_log::attrSaveEvent,
                               osss_log::executeEvent,
                               osss_log::stopEvent,
                               m_hardware_device_ptr,
                               this,
                               sd,
                               old_nc_ptr->name(),
                               upi->m_id);
      }
      // update old context, if any
      if (sd->m_allocated)
      {
        m_context_table.getInfoPtrByIndex(sd->m_context)->m_enabled = false;
      }
      // request and perform re-configuration
      if (ajd.m_old_slot_class != ajd.m_new_class)
      {
        OSSS_MESSAGE(DEBUG_RECON_OBJECT,
          "Requesting configuration for context "
          << ajd.m_context );
        m_hardware_device_ptr->configure(this, // recon obj
                                         new_nc_ptr->m_suitable_slot_index_precalc, // slot
                                         ajd.m_old_slot_class, // old class
                                         ajd.m_new_class, // new class
                                         m_cycle_count_factory->getInstance(0),
                                         upi->m_id,
                                         m_context_table.getKey(ajd.m_context));
        OSSS_MESSAGE(DEBUG_RECON_OBJECT,
          "Recognising configuration done for context "
          << ajd.m_context );
        // update slot
        sd->m_class   = ajd.m_new_class;
      }

      // AC performs a slot reset if
      // a) A reconfiguration was performed OR
      // b) A context restore is to be performed
      if (  (ajd.m_old_slot_class != ajd.m_new_class)
         || ( true == ajd.m_restore_required ) )
      {
        // One cycle delay, we simulate slot reset here
        // In the synthesis model this is where transient
        // attributes are cleared.

        if (m_num_contexts > 1)
        {
          sc_core::wait();
          OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Slot reset done");
        }
        else
        {
          // We have no contexts, therefore our state machine can
          // skip the restore state and we go directly to new_grant
          // indication to singleCycleThread. We merge the states
          // in synthesis then. Faster.
          OSSS_MESSAGE(DEBUG_RECON_OBJECT,
            "Slot reset will be performed parallel to new_grant cycle");
        }
      }

      sd->m_context   = ajd.m_context;
      sd->m_allocated = true;

      // Restore
      if ( true == ajd.m_restore_required )
      {
        OSSS_MESSAGE(DEBUG_RECON_OBJECT,
          "restoring context " << ajd.m_context
          << " [" << ajd.m_restore_cycles->getCycleCount() << " cycles]");
        ::osss_event_log.write(osss_log::attrRestoreEvent,
                               osss_log::executeEvent,
                               osss_log::startEvent,
                               m_hardware_device_ptr,
                               this,
                               sd,
                               new_nc_ptr->name(), // new context!
                               upi->m_id);
        sc_core::wait( ajd.m_restore_cycles->getCycleCount() );
        ::osss_event_log.write(osss_log::attrRestoreEvent,
                               osss_log::executeEvent,
                               osss_log::stopEvent,
                               m_hardware_device_ptr,
                               this,
                               sd,
                               new_nc_ptr->name(),
                               upi->m_id);
      }

      // update new context
      cd->m_current_slot_index = new_nc_ptr->m_suitable_slot_index_precalc;
      cd->m_enabled            = true;

      if (true == upi->m_create_logic.read())
      {
        new_nc_ptr->updateType(ajd.m_new_class);
        if (NULL != upi->m_new_content)
        {
          // We got a new content
          new_nc_ptr->updateContent(upi->m_new_content);
        }
        /*
        else
        {
          This case would be a "create" request without
          a value assignment. Currently expressed in OSSS+R
          using construct<A>().

          We got nothing to to in this case, this else
          statement is just for documentation.
        }
        */
      }

      // The user process wants access permission.
      // We need to remember the permission, iff there
      // is at least 2 user processes. Otherwise we just
      // perform switches.
      if (  upi->m_switch_and_permission_obtain.read()
         && (1 < m_user_process_table.size())) // permission handling done at all?
      {
        // Wave to singleCycleThread (which does book keeping for us),
        // that there is a new grant given to a user process.
        m_new_grant.write(true);
        // And this is the slot we granted access to.
        m_grant_slot = new_nc_ptr->m_suitable_slot_index_precalc;
        OSSS_MESSAGE(DEBUG_RECON_OBJECT,
                     "Showing new grant to singleCycleThread");
      }

      // Acknowledge completion to user process
      OSSS_MESSAGE(DEBUG_RECON_OBJECT,
                   "Granting access to context " << ajd.m_context);
      upi->m_ack.write( true );

      sc_core::wait();

      // These signals may be used for one cycle only.
      // Having m_new_grant set to true for two cycles
      // means indicating two grants!
      m_new_grant.write(false);

      // Take back ack after one cycle
      upi->m_ack.write( false );

      sc_core::wait();
    } // if call scheduler
  } // forever while
}

void
osss_recon_base::singleCycleThread()
{
  // This CTHREAD handles returning of access permission, external locks
  // and updates the internal registers "m_num_grants_available" and
  // m_external_locks[user_process]. These registers are read by the
  // multiCycleThread but not altered there.

  user_process_pool_type::index_type number_of_user_processes
             = m_user_process_table.size();

  // reset things
  // Clear all external locks
  for (context_pool_type::index_type context_index = 0;
        context_index < m_num_contexts;
        ++context_index)
  {
    osss_context_base * nc_ptr = m_context_table.getKey( context_index );
    for (osss_user_process_idx user_process_index = 0;
         user_process_index < number_of_user_processes;
         ++user_process_index)
    {
      nc_ptr->m_external_locks[user_process_index] = false;
    }
  }
  // No grants are given
  for (osss_slot_idx slot_index = 0;
       slot_index < m_num_slots;
       ++slot_index)
  {
    m_slot_table[slot_index].m_granted = false;
  }
  m_num_grants_available.write( m_num_parallel_accesses_allowed );
  sc_core::wait();

  while (true)
  {
    // We iterate over this loop in each cycle.

    // This is the counter of all grants released within the current clock cycle.
    unsigned int released_grants = 0;

    // Handle all user processes
    for (osss_user_process_idx user_process = 0;
         user_process < number_of_user_processes;
         ++user_process)
    {
      osss_user_process_descriptor * upi = m_user_process_table.getInfoPtrByIndex(user_process);

      // Establish a lock?
      if (true == upi->m_lock_obtain.read())
      {
        // Ok, the user process wishes to establish an external lock
        if (NULL == upi->m_context)
        {
          std::cerr << "ERROR! Attempt to lock the anonymous context!" << std::endl;
          OSSS_ABORT();
        }
        if (true == upi->m_context->m_external_locks[user_process])
        {
          std::cerr << "ERROR! Attempt to lock a context twice!" << std::endl;
          OSSS_ABORT();
        }
        // Set the external lock flag
        upi->m_context->m_external_locks[user_process] = true;
        OSSS_MESSAGE(DEBUG_RECON_OBJECT, "locking context " << upi->m_context);
      }
      // And/or a lock release?
      if (true == upi->m_lock_release.read())
      {
        // Ok, the user process wishes to release a lock
        if (NULL == upi->m_context)
        {
          std::cerr << "ERROR! Attempt to unlock the anonymous context!" << std::endl;
          OSSS_ABORT();
        }
        if (false == upi->m_context->m_external_locks[user_process])
        {
          std::cerr << "ERROR! Attempt to unlock a context which is not locked!" << std::endl;
          OSSS_ABORT();
        }
        // Clear the external lock flag
        upi->m_context->m_external_locks[user_process] = false;
        OSSS_MESSAGE(DEBUG_RECON_OBJECT, "unlocking context " << upi->m_context);
      }
      // And/or access permission?
      if (true == upi->m_permission_release.read())
      {
        // The user process returns access permission
        // Do we live in a one-slot world? Translate?
        // (context_id == NULL means anonymous context)
        if (( 1 == m_num_slots ) && (NULL == upi->m_context))
        {
          // Translate:
          m_slot_table[0].m_granted = false; // 0 = one and only slot
        }
        else
        {
          if (NULL == upi->m_context)
          {
            std::cerr << "ERROR! Usage of anonymous context on a multi-slot recon object. Cannot return permission." << std::endl;
            OSSS_ABORT();
          }
          // We got more than one slot OR the given context is != NULL.
          // (upi->context_id carries ID of a real context)
          osss_context_descriptor * cd = m_context_table.getInfoPtrByKey(upi->m_context);
          // Clear the grant flag
          m_slot_table[cd->m_current_slot_index].m_granted  = false;
        }
        // Remember that we got back an additional grant
        ++released_grants;
        //OSSS_MESSAGE(DEBUG_RECON_OBJECT, "returning permission for "<< upi->m_context);
      } // return permission
    } // for user_process

    // Now honor new_grant (set by process below)
    if (true == m_new_grant.read())
    {
      // Yes, the request thread decided to grant a user process.
      // Let's do the book-keeping for him.
      m_num_grants_available.write( m_num_grants_available.read() + released_grants - 1);
      m_slot_table[m_grant_slot].m_granted = true;
    }
    else
    {
      // There is no fresh grant to remember. However, we may have
      // released some.
      // We re-write the signal on change only, saves simulation performance.
      if (released_grants > 0)
      {
        m_num_grants_available.write( m_num_grants_available.read() + released_grants );
      }
    }
    sc_core::wait();
  } // while true
}

//----------------------------------------------------------------------------//

osss_slot_descriptor *
osss_recon_base::resolveSlotIdx( osss_slot_idx idx )
{
  return &(m_slot_table[idx]);
}

//----------------------------------------------------------------------------//

osss_context_base *
osss_recon_base::resolveContextPtr( osss_context_base * raw,
                                    bool resolve_to_current)
{
  if (NULL == raw)
  {
    if (1 == m_num_slots)
    {
      // Resolve anonymous
      if (resolve_to_current)
      { // Translate NULL to whatever context the slot currently holds
        return m_context_table.getKey(m_slot_table[0].m_context);
      }
      else
      { // Translate NULL to the temporary context
        return m_temporary_context;
      }
    }
    else
    {
      std::cerr << "ERROR! Cannot resolve anonymous context in case of multi-slot" << std::endl;
      OSSS_ABORT();
    }
  }
  return raw; // Already a named context
}


//----------------------------------------------------------------------------//

/*
template< class InterfaceT >
osss_context_descriptor *
osss_recon_base::resolveContextIdx( osss_context_idx idx )
{
  return &(m_context_table.getInfoPtrByIndex(idx));
}
*/
//--------------------------------------------------------------------------------------------------------

void
osss_recon_base::precalc()
{
  // Here we pre-calculate some values within context and slot tables.
  // It could be done lazy (on demand) but that would make the code less
  // readable. Additionally, we'd have to check for valid entries each
  // time which could eat up the runtime advantage.

  // Figure out the number of user processes and number of named contexts.
  // Remember, that the anonymous context is listed in the table of
  // named contexts, too.
  user_process_pool_type::index_type number_of_user_processes
    = m_user_process_table.size();
  context_pool_type::index_type number_of_contexts = m_num_contexts;

  // Figure out the period of the clock driving the controller
  // TODO: Store this in an class member, calculate it once
  sc_core::sc_time clock_period =
    dynamic_cast< sc_core::sc_clock * >(clock_port.get_interface())->period();

  // Re-calculate "is_locked" entries in context table
  for (context_pool_type::index_type context_index = 0;
       context_index < number_of_contexts;
       ++context_index)
  {
    // First, get the osss_recon's internal descriptor entry for that context
    osss_context_descriptor * cd = m_context_table.getInfoPtrByIndex(context_index);
    osss_context_base * nc_ptr = m_context_table.getKey( context_index );

    // For simplicity, assume that the context is not locked
    cd->m_is_locked = false;
    // Test, if the context is enabled
    if (false == cd->m_enabled)
    {
      // disabled context ==> cannot be locked, assumption holds, proceed to next
      continue;
    }
    else
    {
      // Current context is enabled.
      // See, if there is at least one user process having established an
      // external lock on it.
      for (user_process_pool_type::index_type user_process_index = 0;
           (false == cd->m_is_locked) && (user_process_index < number_of_user_processes);
           ++user_process_index)
      {
        cd->m_is_locked = cd->m_is_locked | nc_ptr->m_external_locks[user_process_index];
      }
      // If there is no external lock...
      if (false == cd->m_is_locked)
      {
        // Then take the time to evaluate the internal lock
        cd->m_is_locked = m_context_table.getKey(context_index)->evaluateInternalLock();
      }
    }
  }

  // Re-calculate "free" in slot-table for each entry.
  for (osss_slot_idx slot_index = 0;
       slot_index < m_num_slots;
       ++slot_index)
  {
    // This is the case iff...
    m_slot_table[slot_index].m_free = ( (false == m_slot_table[slot_index].m_allocated)
                                          // case one: currently no named context in this slot
                                        ? true // we're free
                                          // case two: there IS a named context in this slot (at the moment)
                                        : // then we require that there is no grant given to someone for this slot
                                              (false == m_slot_table[slot_index].m_granted)
                                           // and we require that the slot is not locked
                                           && (false == m_context_table.getInfoPtrByIndex(m_slot_table[slot_index].m_context)->m_is_locked));
  }

  // Re-calculate suitable-slot entries in context table. That is the
  // answer to the question:
  // "If the context happens is to be enabled, in which slot should it go?"
  for (context_pool_type::index_type context_index = 0;
       context_index < number_of_contexts;
       ++context_index)
  {
    // Again, grab our internal descriptor object
    osss_context_descriptor * cd = m_context_table.getInfoPtrByIndex(context_index);

    if (false == cd->m_enabled)
    {
      // And the context pointer
      osss_context_base * nc_ptr = m_context_table.getKey(context_index);
      nc_ptr->m_suitable_slot_index_precalc = m_placer->place( *cd );
    }
  }
}

void
osss_recon_base::lock(osss_context_base * nc_ptr)
{
  // This method is to be executed by the user process CTHREAD
  // if a lock is to be established.

  // Sanity check. We can't lock the anonymous context.
  if (NULL == nc_ptr)
  {
    std::cerr << "ERROR! Cannot lock() context NULL to enabled state" << std::endl;
    OSSS_ABORT();
  }
  // Sanity check. One cannot use the temporary context for lock().
  if (m_temporary_context == nc_ptr)
  {
    std::cerr << "ERROR! Cannot lock() temporary context to enabled state" << std::endl;
    OSSS_ABORT();
  }

  // Sanity checks passed ok. Now, first find our signals to communicate
  // with the access controller.
  osss_user_process_descriptor * upi = m_user_process_table.getInfoPtrByKey( osss_process_handle::current().get_proc_id() );

  // Then fill in our request to the single-cycle thread
  upi->m_create_logic.write( false );
  upi->m_lock_obtain.write( true );
  upi->m_lock_release.write( false );
  upi->m_switch_and_permission_obtain.write( false );
  upi->m_permission_release.write( false );
  upi->m_new_class = NULL;
  upi->m_context   = nc_ptr;

  wait();
  upi->m_lock_obtain.write( false );
  // no need to await completion
}

bool
osss_recon_base::isExternallyLocked(osss_context_base * nc_ptr)
{
  // This method is for use in OSSS_KEEP_ENABLED() macro only.

  // First, do sanity checks.
  if (NULL == nc_ptr)
  {
    std::cerr << "ERROR! Cannot access context NULL" << std::endl;
    OSSS_ABORT();
  }
  if (m_temporary_context == nc_ptr)
  {
    std::cerr << "WARNING: Attempt to read lock of temporary context!" << std::endl;
    return false;
  }
  // Now see if this very user process has established an external lock.
  // We do not care for other user processes and for internal locks.
  return nc_ptr->m_external_locks[ myUserProcessIndex() ];
}


void
osss_recon_base::unlock(osss_context_base * nc_ptr)
{
  // Reverse a lock() call.

  if (NULL == nc_ptr)
  {
    std::cerr << "ERROR! Cannot unlock() context NULL" << std::endl;
    OSSS_ABORT();
  }
  if (m_temporary_context == nc_ptr)
  {
    std::cerr << "ERROR! Cannot unlock() temporary context" << std::endl;
    OSSS_ABORT();
  }

  // First find our index
  osss_user_process_descriptor * upi = m_user_process_table.getInfoPtrByKey( osss_process_handle::current().get_proc_id() );

  // Then fill in our request
  upi->m_create_logic.write( false );
  upi->m_lock_obtain.write( false );
  upi->m_lock_release.write( true );
  upi->m_switch_and_permission_obtain.write( false );
  upi->m_permission_release.write( false );
  upi->m_new_class    = NULL;
  upi->m_context      = nc_ptr;

  wait();
  upi->m_lock_release.write( false );
  // no need to await completion
}

// Enter "lazy permission return" mode
void
osss_recon_base::startLazy()
{
  osss_user_process_descriptor * upi = m_user_process_table.getInfoPtrByIndex( myUserProcessIndex() );
  if (upi->m_lazy_mode)
  {
    std::cerr << "ERROR! User process already entered lazy return mode for this osss_recon object!" << std::endl;
    std::cerr << "       osss_recon named \"" << name() << "\"" << std::endl;
    std::cerr << "       Process is named \"" << osss_process_handle::current().name() << std::endl;
    OSSS_ABORT();
  }
  upi->m_lazy_mode = true;
}

void
osss_recon_base::endLazy()
{
  osss_user_process_descriptor * upi =
    m_user_process_table.getInfoPtrByIndex( myUserProcessIndex() );
  if (! upi->m_lazy_mode)
  {
    std::cerr << "ERROR! User process not within lazy return mode for this osss_recon object!" << std::endl;
    std::cerr << "       osss_recon named \"" << name() << "\"" << std::endl;
    std::cerr << "       Process is named \"" << osss_process_handle::current().name() << std::endl;
    OSSS_ABORT();
  }
  upi->m_lazy_mode = false;
  if (NULL != upi->m_current_permission)
  {
    returnPermission(upi);
  }
}

bool
osss_recon_base::isLazy()
{
  return m_user_process_table.getInfoPtrByIndex( myUserProcessIndex() )->m_lazy_mode;
}


// If new_content_ptr!=NULL, the context is assigned that value
osss_user_process_descriptor *
osss_recon_base::requestPermission( osss_context_base * calling_named_context_ptr
                                  , osss_object       * new_content_ptr)
{
  // sanity checks
  if ((NULL != new_content_ptr) && !check_runtime_type( new_content_ptr) )
  {
    std::cerr << "ERROR! New content is not derived from interface root class "
              << if_typename() << std::endl;
    OSSS_ABORT();
  }

  osss_user_process_descriptor * upi =
    m_user_process_table.getInfoPtrByIndex( myUserProcessIndex() );

  // Are we doing permission handling at all?
  // Yes, if there are multiple contexts and
  // yes, if we have multiple user-defined processes.
  bool permission_handling = (1 < m_num_contexts)
                          || (1 < m_user_process_table.size());
  bool is_assignment    = (NULL != new_content_ptr);

  const osss_type_descriptor * content_type_descriptor = NULL;
  if (is_assignment)
  {
    // Type descriptor may be different from
    // the one used by the caller since
    // hardware device for source and destination
    // device types may differ. We do not share
    // descriptors over device types. Therefore
    // we may need a different descriptor for the
    // very same C++ type.
    content_type_descriptor = m_hardware_device_ptr->getDeviceTypePtr()->findType( osssi::type_id(*new_content_ptr));
    if (NULL == content_type_descriptor)
    {
      std::cerr << "ERROR! No type information provided for type \""
                << osssi::type_id(*new_content_ptr)
                << "\"" << std::endl;
      std::cerr << "       that can be used with osss_recon named \"" << name() << "\"" << std::endl;
      std::cerr << "       Maybe you forgot an OSSS_DECLARE_TIME statement?" << std::endl;
      OSSS_ABORT();
    }
  }

  // Resolve context (Note: At the time of request! This is not
  // necessarily the context we're being granted, if doing
  // anonymous access!)
  osss_context_base * resolved_calling_named_context_ptr =
     resolveContextPtr( calling_named_context_ptr, NULL == new_content_ptr );

  // return old permission?
  if (  (upi->m_lazy_mode == true)
     && (upi->m_current_permission != NULL)
     && (  (upi->m_current_permission != resolved_calling_named_context_ptr)
        || (upi->m_permission_obtained_anonymous && (NULL != calling_named_context_ptr))
        )
     )
  {
    // We're lazy and hold a permission
    // a) we hold a a different permission than requested
    // b) we don't know which permission we hold (anonymous one)
    // Make returnPermission really return it
    upi->m_lazy_mode = false;
    returnPermission(upi);
    // restore lazy mode
    upi->m_lazy_mode = true;
  }
  // Now we can be sure that: If we are in lazy mode,
  // we do not hold permission of a different context
  // than the requested one.

  // We need to request, iff...
  if (  (  // We do not hold the right permission
           // (Note: Non-matching ones were returned some code lines above)
           (NULL == upi->m_current_permission)
           // ... and permission handling is done at all...
        && permission_handling
        )
     || is_assignment // create: always request it
     )
  {
    std::string named_context_name;
    if (NULL == calling_named_context_ptr) {
      named_context_name  = this->name();
      named_context_name += ".anonymous";
    } else {
      named_context_name = calling_named_context_ptr->name();
    }

    OSSS_MESSAGE(DEBUG_RECON_OBJECT,
      "Requesting permission for " << named_context_name );

    // Note: We do have to use m_switch_and_permission_obtain,
    // even if we are the only user process. In that case,
    // only the "switch" is performed and we don't need to
    // return any permission. This is implemented in
    // returnPermission().
    upi->m_create_logic.write( is_assignment );
    upi->m_lock_obtain.write( false );
    upi->m_lock_release.write( false );
    upi->m_switch_and_permission_obtain.write( permission_handling );
    upi->m_permission_release.write( false );
    upi->m_new_class    = content_type_descriptor;
    upi->m_context      = calling_named_context_ptr;
    upi->m_new_content  = new_content_ptr;

    // Here, we can do logging with "named_context_name" only,
    // since we didn't resolve anonymous accesses yet.
    // The multiCycleThread will tell us by updating upi->m_context,
    // which context we really got permission for.
    ::osss_event_log.write(osss_log::accessEvent,
                    osss_log::requestEvent,
                    osss_log::startEvent,
                    m_hardware_device_ptr,
                    this,
                    NULL, // we don't know the slot descriptor, not decided yet
                    named_context_name,
                    upi->m_id);
    try {
      // We try to get access permission
      // and await completion
      do { wait(); }
        while ( ! upi->m_ack.read() );
    } catch ( ... ) {

      // TODO: improve exception safety of communication protocols
      //       The following is a hotfix for a reset during a
      //       reconfiguration.

      OSSS_MESSAGE(DEBUG_RECON_OBJECT,
        "Reset caught within requestPermission for "
        << named_context_name );

      upi->m_create_logic.write( false );
      upi->m_lock_obtain.write( false );
      upi->m_lock_release.write( false );
      upi->m_switch_and_permission_obtain.write( false );
      upi->m_permission_release.write( false );

      throw;
    }

    upi->m_switch_and_permission_obtain.write( false );
    upi->m_create_logic.write( false );

    OSSS_MESSAGE(DEBUG_RECON_OBJECT,
      "Permission received for " << named_context_name );

    if (permission_handling)
    {
      // We just received a permission.
      // upi->m_context was updated by multiCycleThread,
      // now carrying the context pointer we received
      // permission for.
      upi->m_current_permission            = upi->m_context;
      // Even if we know the context, remember "if the user-defined
      // process knows it". For synthesis, this makes a difference.
      // This, in turn, influences when we know that we can re-use
      // a permission.
      upi->m_permission_obtained_anonymous = (NULL == calling_named_context_ptr);
    }

    // We need the context descriptor to find out where the
    // context was placed. Logging needs this info.
    osss_context_descriptor * cd;
    if (calling_named_context_ptr == NULL)
    {
      cd = m_context_table.getInfoPtrByKey( m_temporary_context );
    }
    else
    {
      cd = m_context_table.getInfoPtrByKey( calling_named_context_ptr );
    }

    // Here, we can log using upi->m_context, it is
    // resolved (by multiCycleThread).
    ::osss_event_log.write(osss_log::accessEvent,
                          osss_log::requestEvent,
                          osss_log::stopEvent,
                          m_hardware_device_ptr,
                          this,
                          &(m_slot_table[cd->m_current_slot_index]),
                          upi->m_context->name(), // now we know our context for sure
                          upi->m_id);
  }

  return upi;
}

// This does nothing in lazy mode
void
osss_recon_base::returnPermission(osss_user_process_descriptor * upi)
{
  if (NULL == upi)
  {
    // We gave the user process a non-NULL ptr upon requestPermission().
    // We expect that one to be given here.
    std::cerr << "ERROR! user process descriptor not given" << std::endl;
    OSSS_ABORT();
  }

  // If we really have a permission and are not lazy: Do return it
  if (  (NULL  != upi->m_current_permission)
     && (false == upi-> m_lazy_mode))
  {
    osss_context_descriptor * cd =
      m_context_table.getInfoPtrByKey( upi->m_context );

    // Then fill in our request
    upi->m_create_logic.write( false );
    upi->m_lock_obtain.write( false );
    upi->m_lock_release.write( false );
    upi->m_switch_and_permission_obtain.write( false );
    upi->m_permission_release.write( true );
    upi->m_new_class    = NULL;
    upi->m_context      = upi->m_current_permission;
    upi->m_new_content  = NULL;
    OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Returning permission for "<< upi->m_context->name() );

    // We try to return access permission
    ::osss_event_log.write(osss_log::accessEvent,
                    osss_log::returnEvent,
                    osss_log::startEvent,
                    m_hardware_device_ptr,
                    this,
                    &(m_slot_table[cd->m_current_slot_index]),
                    upi->m_current_permission->name(),
                    upi->m_id);
    wait();
    ::osss_event_log.write(osss_log::accessEvent,
                        osss_log::returnEvent,
                        osss_log::stopEvent,
                        m_hardware_device_ptr,
                        this,
                        &(m_slot_table[cd->m_current_slot_index]),
                        upi->m_current_permission->name(),
                        upi->m_id);
    upi->m_permission_release.write( false );
    upi->m_current_permission = NULL;
    OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Permission returned for "<< upi->m_context->name());
  }
}

osss_polymorphic< osss_cycle_count >
osss_recon_base::getCycleValue( sc_dt::uint64 value ) const
{
  return m_cycle_count_factory->getInstance( value );
}

void
osss_recon_base::printOn(std::ostream & os) const
{
  os << "[osss_recon:hdevice=" << m_hardware_device_ptr
     << "|arbiter=" << m_scheduler_factory->getName()
     << "|interface=" << if_typename()
     << "|named_contexts=" << std::endl;
  for (context_pool_type::index_type named_context_count = 0;
       named_context_count < m_num_contexts;
       ++named_context_count)
  {
    /*
    os << "#" << named_context_count
       << "@" << m_context_table.getKey(named_context_count)
       << ": "
       << *(named_context_table.getKey(named_context_count))
       << std::endl;
    */
  }
  os << "|slots=" << std::endl;
  // TODO re-enable it with a fresh implementation
  /*
  for (std::vector< slot_descriptor >::size_type index = 0; index < m_slot_table.size(); ++index)
  {
    os << "#" << index
       << "(age " << aged_slot_table[index].age
       << "): "
       << *(aged_slot_table[index].slot_ptr)
       << std::endl;
  }
  */
  os << "]";
}

void
osss_recon_base::writeXMLStringOn(std::ostream & os)
{
  os << "<recon_object id=\"" << this << "\" device_ref=\"" << m_hardware_device_ptr << "\">";
  for (unsigned int index = 0; index < m_slot_table.size(); ++index)
  {
    os << "<slot id=\"" << &(m_slot_table[index]) << "\" nr=\"" << index << "\"/>";
  }
  for (unsigned int named_context_count = 0; named_context_count < m_num_contexts; ++named_context_count)
  {
    m_context_table.getKey(named_context_count)->writeXMLStringOn(os);
  }
  os << "</recon_object>";
}

/*
void
osss_recon_base::prefetchNamedContext(osss_context_base * desired_named_context_ptr)
{
  // Sanity check
  if (desired_named_context_ptr == NULL)
  {
    std::cerr << "ERROR: non-blocking prefetch for named context NULL"
              << std::endl
              << *this << std::endl;
    OSSS_ABORT();
  }
  osss_event_log.write(osss_log::prefetchEvent,
                    osss_log::requestEvent,
                    osss_log::startEvent,
                    m_hardware_device_ptr,
                    this,
                    NULL,
                    desired_named_context_ptr->getOSSSObjectPtr(),
                    desired_named_context_ptr);

  prefetch_named_context_ptr = desired_named_context_ptr;
  // Signalling this will take at least one cycle
  wait();

  osss_event_log.write(osss_log::prefetchEvent,
                    osss_log::requestEvent,
                    osss_log::stopEvent,
                    m_hardware_device_ptr,
                    this,
                    NULL,
                    desired_named_context_ptr->getOSSSObjectPtr(),
                    desired_named_context_ptr);

  sanityCheck();
}

template< class InterfaceT >

osss_context_base *
osss_recon_base::fetchPrefetchNamedContextPtr()
{
  osss_context_base * tmp = prefetch_named_context_ptr;
  prefetch_named_context_ptr = NULL;
  return tmp;
}
*/

void
osss_uses( osss_recon_base & ri, osss_priority priority  )
{
  ri.signInUserProcess( osss_process_handle::last_created(), priority );
}

void
uses( osss_recon_base & ri, osss_priority priority )
{
  osss_uses( ri, priority );
}

} // end namespace osssi
} // end namespace osss

// $Id: osss_recon_base.cpp 3021 2009-03-04 22:55:21Z philipph $
// :flavour: (osss,s) (osss_recon,s) (osss_full,s)
