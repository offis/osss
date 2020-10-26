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

#ifndef OSSS_RECON_OBJECT_INTERFACE_H
#define OSSS_RECON_OBJECT_INTERFACE_H

#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/osss_types.h"
#include "o3s/utils/osss_process_handle.h"
#include "o3s/utils/osss_recon_types.h"
#include "o3s/utils/osss_pool.h"
#include "o3s/utils/object_container.h"
#include "o3s/algorithms/osss_placer.h"
#include "o3s/algorithms/osss_recon_scheduler.h"

#define DEBUG_RECON_OBJECT false

namespace osss {

/// Forward declaration to avoid circular inclusion
class osss_object;
class osss_device;

namespace osssi {

/// Forward declarations
class osss_type_descriptor;
class osss_context_base;

/** This is the base class for all osss_recon<> containers.
    It is used within osss_context<> etc. in order to be
    able to access a common interface for all recon objects.
*/
class osss_recon_base
  : public sc_core::sc_module
  , public object_container
{
  friend class osss_context_base;
  friend class osss::osss_device;
  friend class osss::osss_recon_scheduler;

  /**
   * \todo  This is a quick hack, to get the pure-virtual
   *        object_container::content() method working
   *        without having to change this beast of a class
   *        too much.
   */
  object_container::object_handle content() const
    { return this->getOSSSObjectPtr(); }

public:

  /// These ports have to be bound by the surrounding SC_MODULE.
  sc_core::sc_in< bool > clock_port;
  sc_core::sc_in< bool > reset_port;

protected:
  /// Required since we don't use SC_CTOR
  SC_HAS_PROCESS( osss_recon_base );

  osss_recon_base( sc_core::sc_module_name );

  virtual ~osss_recon_base();

  /// For synchronization with reconfiguration controller
  ///(residing in osss_device)
  sc_core::sc_signal< bool > m_rci_do;      /// written by recon object (which includes access controller)
  sc_core::sc_signal< bool > m_rci_done;    /// writen by reconfiguration controller (osss_device class)
  sc_core::sc_signal< bool > m_rci_busy;    /// writen by reconfiguration controller (osss_device class)
  sc_core::sc_signal< bool > m_rci_pending; /// writen by reconfiguration controller (osss_device class)

  /// These are arguments set by the recon object
  osss_slot_idx                        m_rci_slot;
  osss_type_const_handle               m_rci_new_class;
  osss_type_const_handle               m_rci_old_class;
  osss_polymorphic< osss_cycle_count > m_rci_deadline; /// unsupported up today

  OSSS_Process_ID_t                    m_rci_originator; /// For logging facility only!
  osss_context_const_handle            m_rci_context; /// For logging facility only!

  virtual time_consumer_base* time_consumer() const;

  using object_container::wait;

public:

  /** This method is accessed by osss_recon_scheduler class. It gives
      an status information for more intelligent access scheduler
      decisions ("select a job which doesn't require reconfiguration
      if that controller is busy anyway" etc.)
    */
  inline
  const bool
  isConfigurationControllerBusy() const
  {
    return m_rci_busy.read();
  };

  /*
     These two methods dealing with configuration prefetching are
     leftovers from library version 1 and to be modified to the
     version 2 in future.
     virtual osss_context_base * fetchPrefetchNamedContextPtr() = 0;
     virtual void prefetchNamedContext(osss_context_base * desired_named_context) = 0;
  */

  virtual void printOn(std::ostream & os) const;
  virtual void writeXMLStringOn(std::ostream & os);

  /** Returns a pointer to the device (where the reconfiguration
      controller can be found).
    */
  osss_device * getDevicePtr() const;

  /// Set the desired number of slots. Use within elaboration phase only.
  void setNumSlots(osss_slot_idx num_slots);

  /// Set the upper bound of parallel accesses. Use within elaboration phase only.
  void setNumParallelAccessesAllowed(unsigned int num_parallel_accesses);

  /** Allows specifying which cycle counter is to be used in the
      scheduler. May be called once and during elaboration only. */
  template< class CycleCountClass >
  void setCycleCounter();

  /** This allows specifying a scheduler class. May be called once
      and during elaboration only. */
  template< class SchedulerClass >
  void setScheduler();

  /** This allows specifying a scheduler class. May be called once
      and during elaboration only. */
  template< class PlacerClass >
  void setPlacer();


  /** This is the method to be called if the user process requests
      something from the access controller. The request indicated
      this way always includes access permission but may (!) also
      include re-creation of a context (initialize it with a fresh object).
      For the arguments of this method, see the comments in osss_recon class.
    */
  osss_user_process_descriptor*
  requestPermission(osss_context_base * calling_named_context_ptr = NULL,
                    osss_object       * new_content_ptr = NULL);

  /** This is the counterpart to requestPermission(). It is used to return
      the access permission.
    */
  void
  returnPermission(osss_user_process_descriptor*);

  /** This method establishes an external context lock. Note that it is
      not legal to pass NULL here! Locks cannot be nested and are individual
      to each user process.
    */
  void
  lock(osss_context_base * nc_ptr);

  /** Test for locking condition. For use within OSSS_KEEP_ENABLED macro
      only. There is no synthesis semantics for this method as
      standalone functionality.
    */
  bool
  isExternallyLocked(osss_context_base * nc_ptr);

  /// Removal of a previously established lock.
  void
  unlock(osss_context_base * nc_ptr);

  /** Start lazy-return-of-named-context-permission mode */
  void startLazy();

  /** End lazy mode */
  void endLazy();

  /** Test of laziness */
  bool isLazy();

  /** This method is used by named contexts during elaboration phase.
      It makes the context known to this reconfigurable object (and bound to it).
    */
  void
  signInNamedContext(osss_context_base * named_context);

  /** Make a user process known to the reconfigurable object.
      The priority is fed through to the scheduler.
    */
  void
  signInUserProcess( osss_process_handle const & user_process,
                     osss_priority               priority );

  /// This method returns valid values after elaboration.
  osss_user_process_idx
  getNumberOfUserProcesses() const;

  /// This method returns valid values after elaboration.
  osss_context_idx
  getNumberOfContexts() const;

  /** Returns a ptr to the content. For use in user macros,
      always keep the argument at its default value.
      You may give "false" for OSSS lib internal use
      if you know what you are doing.
    */
  osss_object *
  getOSSSObjectPtr(bool check_for_null = true) const;

  /// Resolves a slot index handle into the pointer
  /// of the descriptor structure
  osss_slot_descriptor *
  resolveSlotIdx( osss_slot_idx idx );

  /** This method returns the number of classes
      that we've timing descriptions for. These
      are those possibly instantiated by user
      processes in recon objects. */
  osss_type_idx
  getNumberOfClasses() const;

  osss_slot_idx
  getNumberOfSlots() const;

  /// Binding of this reconfigurable object to a device.
  void
  operator()(osss_device & device);

  /// Same as above, more verbose method name.
  void
  bind(osss_device & device);

protected:
  /// This member stores our "uplink" to the reconfiguration controller
  osss_device * m_hardware_device_ptr;

  /** Save the number of contexts being bound
      to this object. This identical to
      m_context_table.size(). This
      includes the temporary context
      m_temporary_context in case of one slot only. */
  unsigned int m_num_contexts;

   /** Here we store the number of slots.
      This value may be modified
      by end_of_elaboration()
    */
  osss_slot_idx m_num_slots;

  /** This is the upper bound for parallel accesses.
      This value may be modified
      by end_of_elaboration()
    */
  unsigned int m_num_parallel_accesses_allowed;

  /** This signal indicates the number of currently
      available grants. It is written by the single-cycle-thread
      and read by both single-cycle-thread and multi-cycle-thread.
      It is set to "m_num_parallel_accesses_allowed" initially
      and upon reset.
    */
  sc_core::sc_signal<unsigned int> m_num_grants_available;

  std::vector< osss_slot_descriptor > m_slot_table;

  /// Remeber: object_ptr and type descriptor are stored in a named context object itself
  typedef osss_pool< osss_context_base *,  osss_context_descriptor >
          context_pool_type;
  /// Here we keep reference to all named contexts and the temporary one.
  context_pool_type m_context_table;

  typedef osss_pool< OSSS_Process_ID_t, osss_user_process_descriptor >
          user_process_pool_type;
  user_process_pool_type m_user_process_table;

  user_process_pool_type::index_type myUserProcessIndex() const;

  //------------------------------------------------------------------------
protected:
  /// Stores a factory object to be used to create cycle count objects.
  osss_cycle_count::factory_base     * m_cycle_count_factory;
  /// Stores a factory object for creating schedulers
  osss_recon_scheduler::factory_base * m_scheduler_factory;
  /// Stores the current scheduler (discarded on each reset)
  osss_recon_scheduler               * m_scheduler;
  /** Stores the jobs to be visible to the scheduler. The table
      contains one entry for each user process. */
  osss_vector< osss_recon_job_descriptor > m_job_table;
  /// Stores a factory object for creating placers
  osss_placer::factory_base          * m_placer_factory;
  /// Stores the current placer (discarded on each reset)
  osss_placer                        * m_placer;

  /** This resolves NULL to the address of the temporary
      context, if passed "false" as a second op. This is
      the resolution required by anonymous assignments.
      Passing NULL and "true" yields the address of the currently
      enabled context. This is the proper resolution for
      all other anonymous accesses.

      All other values are returned unchanged. */
  osss_context_base *
  resolveContextPtr( osss_context_base *, bool /* resolve to current */ );


  /// Allows the scheduler to have a look at the jobs
  virtual
  const osss_recon_job_descriptor &
  getJob(osss_user_process_idx index) const;

  /** Returns a value as an object of the correct cycle count class,
      representing a given number of cycles (specified by 64 bit
      unsigned integer argument)
    */
  virtual
  osss_polymorphic< osss_cycle_count >
  getCycleValue( sc_dt::uint64 value ) const;

  /// Internal use only. See requestThread() for the only caller.
  void
  precalc();

  /** In a multi-slot world this is always NULL.
      Otherwise (single-slot) this is NULL if a
      named context is active. Non-null values indicate
      that a temporary, unnamed context is active.
    */
  osss_context_base*         m_temporary_context;
  osss_context_idx           m_temporary_context_index;

  /** Tell the caller iff the given context is the temporary
      one. The purpose of this method is more readable source only.
    */
  inline
  bool
  isTemporary(osss_context_idx idx) const
  {
    return (m_temporary_context) == m_context_table.getKey( idx );
  }

  /// This method sets up the object to be ready for use
  void end_of_elaboration();

  /** This controller handles everything done using
      the "go_request/ok_request" signals
    */
  void
  multiCycleThread();

  /** This controller handles everything done using
      the "go_return/ok_return" signals
    */
  void
  singleCycleThread();

  /** Signal for communication between the two controller threads.
      This is written by the request thread. It is set to true
      for one single cycle, if a new grant was given. */
  sc_core::sc_signal<bool>  m_new_grant;

  /** Variable for communication between the two controller threads.
      This is written by the request thread. It is updated
      whenever m_new_grant is set to true and carries the
      slot number for the grant. */
  osss_slot_idx m_grant_slot;

  virtual
  bool check_runtime_type( osss_object* ) const = 0;

};

inline
::std::ostream & operator<<( ::std::ostream & os,
                             const osss_recon_base & object)
{
  object.printOn(os);
  return os;
}

/** This method is to be used like SystemC's reset_signal_is() call.
    Place one such statement after each process declaration
    (SC_CTHREAD macro) that uses the reconfigurable object.
    It may be substituted with a uses() call on a context that
    is bound (or will be bound) to the reconfigurable object.
  */
void osss_uses( osss_recon_base &, osss_priority = 0 );
void uses( osss_recon_base &, osss_priority = 0 );

} // end namespace osssi

using osssi::osss_uses;

} // end namespace osss

// include template implementation
#include "osss_recon_base.tpp"

#endif // OSSS_RECON_OBJECT_INTERFACE_H
// $Id: osss_recon_base.h 2881 2008-12-09 14:48:30Z andreas $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
