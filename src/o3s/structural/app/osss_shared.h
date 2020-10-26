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
#ifndef OSSS_SHARED_H_INCLUDED_
#define OSSS_SHARED_H_INCLUDED_

#include "o3s/structural/app/osss_shared_if.h"
#include "o3s/structural/arch/osss_clock_reset_if.h"
#include "o3s/datatypes/osss_polymorphic.h"
#include "o3s/algorithms/osss_modified_round_robin.h"
#include "o3s/utils/osss_registry_shared.h"
#include <map>

namespace osss {

////////////////////////////////////////////////////////////////////////////
///
/// \brief This class implements the shared object.
///
/// This class implements a shared object, that means a container for a
/// user-defined class and a scheduler, that allows concurrent accesses.
///
/// \note Class template.
///       Requires to pass a scheduler class and any kind of
///       other class, except for osss_polymorphic<>, as template argument on
///       instantiation.
///
////////////////////////////////////////////////////////////////////////////

template<class ObjectType,
         class SchedulerType = osss_modified_round_robin >
class osss_shared
  : public osss_shared_if<>
  , public sc_core::sc_module
  , public osssi::osss_clock_reset_if
{
public:
  using osssi::osss_clock_reset_if::clock_port;
  using osssi::osss_clock_reset_if::reset_port;

  //! Needed for the reset_process
  SC_HAS_PROCESS(osss_shared);

  //! \brief Default constructor.
  //!
  //! Calls the default constructor of the embedded
  //! arbiter class instance, theuser defined class instance and registers
  //! the reset_process (as SC_METHOD) at the SystemC scheduler.
  explicit
  osss_shared( sc_core::sc_module_name =
                 sc_core::sc_gen_unique_name("osss_shared") );

  ~osss_shared();

  //! \brief Specify the reset sensitivity of the shared object
  //!
  //! The reset sensitivity can be:
  //!   - OSSS_POS  : sensitive on the positive edge of the reset signal
  //!   - OSSS_NEG  : sensitive on the negative edge of the reset signal
  //!   - OSSS_LEVEL: sensitive on the level (high or low) of the
  //!                 reset signal
  //!
  //! The default value is: OSSS_POS
  void reset_sensitivity(const osss_sensitivity sens);

  //! Calls the subscribe method on the embedded arbiter.
  //! \see ::osss::osssi::osss_arbiter_shared
  void
  subscribe( const IDType clientID, osss_priority priority = 0 );

#if 0
  //! Returns a pointer to a copy of the object to which m_objectPointer is
  //! pointing.
  ObjectType *
  _getClone_() const;
#endif

  //! Assigns the passed argument to *m_objectPointer (if possible). The
  //! arbiter stays untouched.
  template< class SourceType >
  inline
  osss_shared &
  operator=( const SourceType & source );

  //! This assignment operator is used, iff target and source of the
  //! assignment are exactly of the same type. The arbiter stays untouched.
  inline
  osss_shared&
  operator=( const osss_shared< ObjectType, SchedulerType > & source );

  //! A type conversion that enables assignments from shared objects
  //! to normal objects:
  inline
  operator const ObjectType& () const;

  //! This operator must be used for calling methods on a shared object. The
  //! calls are redirected to m_objectPointer. This allows to specifiy
  //! m_objectPointer protected.
  inline
  ObjectType *
  operator->() const;

  //! Returns the interface of the object inside the shared object.
  //! If the object has no interface of type sc_interface it returns NULL.
  inline
  sc_core::sc_interface *
  get_interface() const;

  //! Returns a pointer to the arbiter of the shared object.
  osss_arbiter_shared_base * getArbiter();

  //! Registers the address of an osss_port_to_shared (paddr) together with the
  //! process ID (clientID) of the client process using the osss_port_to_shared.
  void registerPortUsage(const IDType paddr,
			 const osssi::OSSS_Process_ID_t proc_id);

  //! Returns the process ID registered for the address of an
  //! osss_port_to_shared (paddr).
  IDType lookupPortID(const osssi::OSSS_Process_ID_t proc_id);

  //! Prints the osss_port_to_shared address to process ID mapping.
  //! \note This is used for debugging only
  void printPortMap();

  //! Returns the number of clients that are subscribed to this Shared Object
  //! \note This value changes during the elaboration phase and should only
  //!       be used at end_of_elaboration or during simulation
  osss_index_type numClients();

  //! Overwritten from SystemC.
  //! Identifies this as "osss_shared".
  virtual const char* kind() const;

  virtual const char * name() const
    { return sc_core::sc_module::name(); }

protected:
  template<class> friend class osss_socket;

  //! SystemC hook which is executed by the SystemC kernel just before the
  //! end of elaboration phase.
  //! This hook is used to set the user definable reset sensitivity.
  virtual void before_end_of_elaboration();

  //! A type conversion that provides non-const access to the underlying
  //! object. This is normally done using "->" but the specialized constructor
  //! of osss_socket requires the same method to be available by both
  //! osss_shared and osss_polymorphic
  inline
  ObjectType &
  plugged_object();

  //! \brief Reset Process.
  //!
  //! Is called once when the signal bound to the reset_port
  //! changes. This process calls the reset() method of the shared object.
  void reset_process();
  void reset_method();

  //! A type conversion that provides non-const access to the underlying
  //! object
  inline
  operator ObjectType& ();

  //! Points to an instance of osss_arbiter_shared<>, which will perform
  //! arbitration on concurrent accesses.
  osssi::osss_arbiter_shared< SchedulerType > *m_arbiter;

  //! This pointer points to an instance of the user-defined class that was
  //! passed as template argument to the shared object at declaration.
  ObjectType *m_objectPointer;

private:

  //! This pointer is used to store the process handle of the sc_method
  //! which serves as the reset process of the shared object.
  osssi::osss_cthread_handle m_reset_process_handle;

  //! Used to store the user definable reset sensitivity which can be
  //! specified by the reset_sensitivity method
  osss_sensitivity m_reset_sensitivity;

  //! Calls reset() on the embedded arbiter.
  //! \see ::osss::osssi::osss_arbiter_shared
  inline
  void
  reset();

  //! This map stores the osss_port_to_shared address to process ID
  //! mapping.
  //! OSSS_Process_ID_t:  process id
  //! IDType:          client id (address of the osss_port_to_shared)
  std::map<const osssi::OSSS_Process_ID_t, IDType > m_port_map;

  //! the number of clients that are subscribed to this Shared Object
  osss_index_type m_numClients;
};

//============================================================================
// Partial specialisation for polymorphic objects
//============================================================================

////////////////////////////////////////////////////////////////////////////
///
/// \brief This class implements the shared object for polymorphic objects
///
/// This class implements a shared object, that means a container for a
/// polymorphic object containing a user-defined class and a scheduler,
/// that allows concurrent accesses.
///
/// \note Partial template specialisation for polymorphic objects.
///       Requires to pass a scheduler class and an osss_polymorphic
///       carrying any kind of other user defined class,
///       as template argument on instantiation.
///
////////////////////////////////////////////////////////////////////////////

template<class ObjectType,
         class SchedulerType>
class osss_shared< osss_polymorphic< ObjectType >, SchedulerType >
  : public osss_shared_if<>
  , public sc_core::sc_module
  , public osssi::osss_clock_reset_if
{
public:
  using sc_core::sc_module::name;
  using osssi::osss_clock_reset_if::clock_port;
  using osssi::osss_clock_reset_if::reset_port;

  //! Needed for the reset_process
  SC_HAS_PROCESS(osss_shared);

  //! \brief Default constructor.
  //!
  //! Calls the default constructor of the embedded
  //! arbiter class instance, theuser defined class instance and registers
  //! the reset_process (as SC_METHOD) at the SystemC scheduler.
  explicit
  osss_shared( const sc_core::sc_module_name& =
                 sc_core::sc_gen_unique_name("osss_shared") );

  ~osss_shared();

  //! \brief Specify the reset sensitivity of the shared object
  //!
  //! The reset sensitivity can be:
  //!   - OSSS_POS  : sensitive on the positive edge of the reset signal
  //!   - OSSS_NEG  : sensitive on the negative edge of the reset signal
  //!   - OSSS_LEVEL: sensitive on the level (high or low) of the
  //!                 reset signal
  //!
  //! The default value is: OSSS_POS
  void reset_sensitivity(const osss_sensitivity sens);

  //! Calls the subscribe method on the embedded arbiter.
  //! \see ::osss::osssi::osss_arbiter_shared
  void
  subscribe( const IDType clientID, osss_priority priority = 0 );

  //! Returns a pointer to a copy of the object to which m_objectPointer is
  //! pointing.
  osss_polymorphic<ObjectType> *
  _getClone_() const;

  //! Assigns the passed argument to *m_objectPointer (if possible). The
  //! arbiter stays untouched.
  template< class SourceType >
  inline
  osss_shared &
  operator=( const SourceType & source );

  //! This assignment operator is used, iff target and source of the
  //! assignment are exactly of the same type. The arbiter stays untouched.
  inline
  osss_shared&
  operator=( const osss_shared< osss_polymorphic<ObjectType>,
	                        SchedulerType > & source );

  //! A type conversion that enables assignments from shared objects
  //! to normal objects:
  inline
  operator const osss_polymorphic< ObjectType >& () const;

  //! This operator must be used for calling methods on a shared object. The
  //! calls are redirected to m_objectPointer. This allows to specifiy
  //! m_objectPointer protected.
  inline
  osss_polymorphic< ObjectType > *
  operator->() const;

  //! Returns the interface of the object inside the shared object.
  //! If the object has no interface of type sc_interface it returns NULL.
  inline
  sc_core::sc_interface*
  get_interface() const;

  //! Returns a pointer to the arbiter of the shared object.
  osss_arbiter_shared_base * getArbiter();

  //! Registers the address of an osss_port_to_shared (paddr) together with the
  //! process ID (clientID) of the client process using the osss_port_to_shared.
  void registerPortUsage(const IDType paddr,
			 const osssi::OSSS_Process_ID_t proc_id);

  //! Returns the process ID registered for the address of an
  //! osss_port_to_shared (paddr).
  IDType lookupPortID(const osssi::OSSS_Process_ID_t proc_id);

  //! Prints the osss_port_to_shared address to process ID mapping.
  //! \note This is used for debugging only
  void printPortMap();

  //! Returns the number of clients that are subscribed to this Shared Object
  //! \note This value changes during the elaboration phase and should only
  //!       be used at end_of_elaboration or during simulation
  unsigned int numClients();

  //! Overwritten from SystemC.
  //! Identifies this as "osss_shared".
  virtual const char* kind() const;

  virtual const char * name() const
    { return sc_core::sc_module::name(); }

protected:
  template<class> friend class osss_socket;

  //! SystemC hook which is executed by the SystemC kernel just before the
  //! end of elaboration phase.
  //! This hook is used to set the user definable reset sensitivity.
  virtual void before_end_of_elaboration();

  //! A type conversion that provides non-const access to the underlying
  //! object. This is normally done using "->" but the specialized constructor
  //! of osss_socket requires the same method to be available by both
  //! osss_shared and osss_polymorphic
  inline
  osss_polymorphic<ObjectType> &
  plugged_object();

  //! \brief Reset Process.
  //!
  //! Is called once when the signal bound to the reset_port
  //! changes. This process calls the reset() method of the shared object.
  void reset_process();
  void reset_method();

  //! Points to an instance of osss_arbiter_shared<>, which will perform
  //! arbitration on concurrent accesses.
  osssi::osss_arbiter_shared< SchedulerType > *m_arbiter;

  //! This pointer points to an instance of the user-defined class that was
  //! passed as template argument to the shared object at declaration.
  osss_polymorphic< ObjectType > *m_objectPointer;

private:

  //! This pointer is used to store the process handle of the sc_method
  //! which serves as the reset process of the shared object.
  osssi::osss_method_handle m_reset_process_handle;

  //! Used to store the user definable reset sensitivity which can be
  //! specified by the reset_sensitivity method
  osss_sensitivity m_reset_sensitivity;

  //! Calls reset() on the embedded arbiter.
  //! \see ::osss::osssi::osss_arbiter_shared
  inline
  void
  reset();

  //! This map stores the osss_port_to_shared address to process ID
  //! mapping.
  //! OSSS_Process_ID_t:  process id
  //! IDType:          client id (address of the osss_port_to_shared)
  std::map<const osssi::OSSS_Process_ID_t, IDType > m_port_map;

  //! the number of clients that are subscribed to this Shared Object
  osss_index_type m_numClients;

};

#if defined( OSSS_BLUE )

/**
 * \brief register implicit usage of a Shared Object
 *
 * Use this method after a process declaration, to register
 * a process as an implicit client of a Shared Object.
 *
 * \warning An explicit registration via a port binding
 *          is preferred.  For synthesis, it is strictly
 *          required (at least for a Shared Object).
 */

template< class ObjectType, class SchedulerType >
void osss_uses( osss_shared<ObjectType, SchedulerType > & so
              , osss_priority p = 0 );

template< class ObjectType, class SchedulerType >
void uses( osss_shared<ObjectType, SchedulerType > & so
         , osss_priority p = 0 );

#endif // OSSS_BLUE

} //namespace osss

#include "o3s/structural/app/osss_shared.tpp"

#endif // OSSS_SHARED_H_INCLUDED_
// $Id: osss_shared.h 3079 2009-03-27 13:10:46Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
