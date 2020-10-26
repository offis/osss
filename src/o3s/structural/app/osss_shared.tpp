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
//-----------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
osss_shared<ObjectType, SchedulerType>::
 osss_shared( sc_core::sc_module_name name )
  : sc_core::sc_module( name )
{
  m_arbiter = new osssi::osss_arbiter_shared< SchedulerType >();
  m_objectPointer = new ObjectType();
  osssi::osss_registry_shared::add_plugged_object(this, m_objectPointer);

  m_reset_sensitivity = OSSS_POS;
  m_numClients = 0;

  //debug
  //osssi::osss_registry_shared::print();
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
void
osss_shared<ObjectType, SchedulerType>::
 reset_sensitivity( const osss_sensitivity sens )
{
  m_reset_sensitivity = sens;
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
void
osss_shared<ObjectType, SchedulerType>::before_end_of_elaboration()
{
  osssi::osss_clock_reset_if::check_clock_reset_port();

  switch(m_reset_sensitivity)
  {
    case OSSS_POS :
    {
      OSSS_UNIQUE_CTHREAD(reset_process, clock_port().pos());
      reset_signal_is( reset_port(), true );
      break;
    }
    case OSSS_NEG :
    {
      OSSS_UNIQUE_CTHREAD(reset_process, clock_port().pos());
      reset_signal_is( reset_port(), false );
      break;
    }
    case OSSS_LEVEL :
    {
      OSSS_UNIQUE_METHOD(reset_method);
      dont_initialize();
      sensitive << reset_port();
      break;
    }
    default:
    {
      OSSS_UNIQUE_CTHREAD(reset_process, clock_port().pos());
      reset_signal_is( reset_port(), true );
      break;
    }
  }
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
void
osss_shared<ObjectType, SchedulerType>::reset_process()
{
  reset();
  wait();

  while(true) { wait(); }
}

template< class ObjectType, class SchedulerType >
void
osss_shared<ObjectType, SchedulerType>::reset_method()
{
  reset();
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
osss_shared<ObjectType, SchedulerType>::~osss_shared()
{
  delete( m_arbiter );
  delete( m_objectPointer );
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
void
osss_shared<ObjectType, SchedulerType>::
 subscribe(const IDType clientID, osss_priority priority)
{
  m_arbiter->subscribe( clientID, priority );
  m_numClients++;
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
void
osss_shared<ObjectType, SchedulerType>::reset()
{
  m_arbiter->reset();
  if ( m_objectPointer )
  {
    osssi::osss_registry_shared::delete_plugged_object(m_objectPointer);
    delete( m_objectPointer );
  }
  m_objectPointer = new ObjectType();
  osssi::osss_registry_shared::add_plugged_object(this, m_objectPointer);
}

//----------------------------------------------------------------------------//

#if 0 /* disabled */
template< class ObjectType, class SchedulerType >
ObjectType *
osss_shared<ObjectType, SchedulerType>::_getClone_() const
{
  ObjectType * clone = new ObjectType();
  *clone = *m_objectPointer;
  return( clone );
}
#endif /* disabled _getClone_() */

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
template< class SourceType >
inline
osss_shared<ObjectType, SchedulerType> &
osss_shared<ObjectType, SchedulerType>::operator=( const SourceType & source )
{
  osssi::osss_registry_shared::delete_plugged_object(m_objectPointer);
  *m_objectPointer = source;
  osssi::osss_registry_shared::add_plugged_object(this, m_objectPointer);
  return( *this );
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
osss_shared<ObjectType, SchedulerType> &
osss_shared<ObjectType, SchedulerType>::operator=(
  const osss_shared<ObjectType, SchedulerType> & source )
{
  osssi::osss_registry_shared::delete_plugged_object(m_objectPointer);
  *m_objectPointer = *source.m_objectPointer;
  osssi::osss_registry_shared::add_plugged_object(this, m_objectPointer);
  return( *this );
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
osss_shared<ObjectType, SchedulerType>::operator const ObjectType& () const
{
  return ( *( this->m_objectPointer ) );
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
osss_shared<ObjectType, SchedulerType>::operator ObjectType& ()
{
  return ( *( this->m_objectPointer ) );
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
ObjectType *
osss_shared<ObjectType, SchedulerType>::operator->() const
{
  return( m_objectPointer );
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
sc_core::sc_interface *
osss_shared<ObjectType, SchedulerType>::get_interface() const
{
  return dynamic_cast<sc_core::sc_interface*>( m_objectPointer );
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
ObjectType &
osss_shared<ObjectType, SchedulerType>::plugged_object()
{
  return( * m_objectPointer );
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
IDType
osss_shared<ObjectType, SchedulerType>::
lookupPortID(const osssi::OSSS_Process_ID_t proc_id)
{
  std::map<const osssi::OSSS_Process_ID_t, IDType>::iterator map_iter;
  map_iter = m_port_map.find(proc_id);
  if (map_iter != m_port_map.end())
    return (*map_iter).second;
  else
  {
    std::cout << "Error: port ID has not been registered "
                 "within this shared object: " << name()
              << std::endl;
    OSSS_ABORT();
    return NULL;
  }
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
void
osss_shared<ObjectType,
	    SchedulerType>::registerPortUsage(const IDType clientID,
					      const osssi::OSSS_Process_ID_t proc_id)
{
  m_port_map[proc_id] = clientID;
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
void
osss_shared<ObjectType, SchedulerType>::printPortMap()
{
  using std::cout; using std::endl;

  std::cout << std::endl;
  std::cout << "osss shared port registry: " << name() << std::endl;
  std::cout << "process ID -> client ID" << std::endl;
  std::cout << "-----------------------" << std::endl;
  for (std::map<const osssi::OSSS_Process_ID_t, IDType>::iterator it =
            m_port_map.begin();
       it != m_port_map.end();
       ++it)
  {
    std::cout << (*it).first << " -> " << (*it).second << std::endl;
  }
  std::cout << std::endl;
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
const char*
osss_shared<ObjectType, SchedulerType>::kind() const
{
  return "osss_shared";
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
osss_index_type
osss_shared<ObjectType, SchedulerType>::numClients()
{
  return m_numClients;
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
osss_arbiter_shared_base *
osss_shared<ObjectType, SchedulerType>::getArbiter()
{
  return m_arbiter;
}

//----------------------------------------------------------------------------//
// Partial specialisation for polymorphic objects
//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
osss_shared<osss_polymorphic<ObjectType>,SchedulerType>::
 osss_shared( const sc_core::sc_module_name& name )
  : sc_core::sc_module( name )
{
  m_arbiter = new osssi::osss_arbiter_shared< SchedulerType >();
  m_objectPointer = new osss_polymorphic< ObjectType >();
  osssi::osss_registry_shared::add_plugged_object(this,
                                                  m_objectPointer->
                                                    operator->());
/*
  m_reset_process_handle = osssi::osss_create_method_process (
      sc_core::sc_gen_unique_name("reset_process"),
      SC_MAKE_FUNC_PTR( SC_CURRENT_USER_MODULE, reset_process ), this );
*/
  m_reset_sensitivity = OSSS_POS;
  m_numClients = 0;
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
void
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::
 reset_sensitivity(const osss_sensitivity sens)
{
  m_reset_sensitivity = sens;
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
void
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::
 before_end_of_elaboration()
{
  osssi::osss_clock_reset_if::check_clock_reset_port();

  switch(m_reset_sensitivity)
  {
    case OSSS_POS :
    {
      OSSS_UNIQUE_CTHREAD(reset_process, clock_port().pos());
      reset_signal_is(reset_port(), true);
      break;
    }
    case OSSS_NEG :
    {
      OSSS_UNIQUE_CTHREAD(reset_process, clock_port().pos());
      reset_signal_is(reset_port(), false);
      break;
    }
    case OSSS_LEVEL :
    {
      OSSS_UNIQUE_METHOD(reset_method);
      dont_initialize();
      sensitive << reset_port();
      break;
    }
    default:
    {
      OSSS_UNIQUE_CTHREAD(reset_process, clock_port().pos());
      reset_signal_is(reset_port(), true);
      break;
    }
  }
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
void
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::reset_process()
{
  reset();
  wait();

  while(true) { wait(); }
}

template< class ObjectType, class SchedulerType >
void
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::reset_method()
{
  reset();
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::~osss_shared()
{
  delete( m_arbiter );
  delete( m_objectPointer );
}

//----------------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
void
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::
 subscribe( const IDType clientID, osss_priority priority )
{
  m_arbiter->subscribe( clientID, priority );
  m_numClients++;
}

//-----------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
void
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::reset()
{
  m_arbiter->reset();
  osssi::osss_registry_shared::delete_plugged_object(m_objectPointer->
						       operator->());
  if ( m_objectPointer )
  {
    delete( m_objectPointer );
  }
  m_objectPointer = new osss_polymorphic< ObjectType >();
  osssi::osss_registry_shared::add_plugged_object(this,
						  m_objectPointer->
						    operator->());
}

//-----------------------------------------------------------------------//

#if 0 /* disabled */
template< class ObjectType, class SchedulerType >
osss_polymorphic<ObjectType> *
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::_getClone_() const
{
  ObjectType *clone = new osss_polymorphic<ObjectType>();
  *clone = *m_objectPointer;
  return( clone );
}
#endif /* disabled _getClone_() */

//-----------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
template< class SourceType >
inline
osss_shared<osss_polymorphic<ObjectType>, SchedulerType> &
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::operator=(
  const SourceType & source )
{
  osssi::osss_registry_shared::delete_plugged_object(m_objectPointer->
						       operator->());
  *m_objectPointer = source;
  osssi::osss_registry_shared::add_plugged_object(this,
						  m_objectPointer->
						    operator->());
  return( *this );
}

//-----------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
osss_shared<osss_polymorphic<ObjectType>, SchedulerType> &
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::operator=(
  const osss_shared<osss_polymorphic<ObjectType>, SchedulerType> & source )
{
  osssi::osss_registry_shared::delete_plugged_object(m_objectPointer->
						       operator->());
  *m_objectPointer = *source.m_objectPointer;
  osssi::osss_registry_shared::add_plugged_object(this,
						  m_objectPointer->
						    operator->());
  return( *this );
}

//-----------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::operator const
osss_polymorphic< ObjectType >& () const
{
  return ( *( this->m_objectPointer ) );
}

//-----------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
osss_polymorphic< ObjectType > *
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::operator->() const
{
  return( m_objectPointer );
}

//-----------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
sc_core::sc_interface *
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::get_interface() const
{
  return dynamic_cast<sc_core::sc_interface*>( m_objectPointer );
}

//-----------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
osss_polymorphic<ObjectType> &
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::plugged_object()
{
  return( *m_objectPointer );
}

//-----------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
IDType
osss_shared<osss_polymorphic<ObjectType>,
            SchedulerType>::lookupPortID(const osssi::OSSS_Process_ID_t proc_id)
{
  std::map<const osssi::OSSS_Process_ID_t, IDType>::iterator map_iter;
  map_iter = m_port_map.find(proc_id);
  if (map_iter != m_port_map.end())
    return (*map_iter).second;
  else
  {
    std::cout << "Error: port ID has not been registered "
                 "within this shared object: "
              << name() << std::endl;
    OSSS_ABORT();
    return NULL;
  }
}

//-----------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
void
osss_shared<osss_polymorphic<ObjectType>,
	    SchedulerType>::registerPortUsage(const IDType  clientID,
					      const osssi::OSSS_Process_ID_t proc_id)
{
  m_port_map[proc_id] = clientID;
}

//-----------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
void
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::printPortMap()
{
  using std::cout; using std::endl;
  std::cout << std::endl;
  std::cout << "osss shared port registry: " << name() << std::endl;
  std::cout << "process ID -> client ID" << std::endl;
  std::cout << "-----------------------" << std::endl;
  for (std::map<const osssi::OSSS_Process_ID_t, IDType>::iterator it =
         m_port_map.begin();
       it != m_port_map.end();
       ++it)
  {
    std::cout << (*it).first << " -> " << (*it).second << std::endl;
  }
  std::cout << std::endl;
}

//-----------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
const char*
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::
kind() const
{
  return "osss_shared";
}

//-----------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
unsigned int
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::numClients()
{
  return m_numClients;
}

//-----------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
inline
osss_arbiter_shared_base *
osss_shared<osss_polymorphic<ObjectType>, SchedulerType>::getArbiter()
{
  return m_arbiter;
}

//-----------------------------------------------------------------------//

template< class ObjectType, class SchedulerType >
void osss_uses( osss_shared<ObjectType, SchedulerType> & so
              , osss_priority p )
{
  // It is not supported, to register a client after the
  // simulation has started.
  if ( sc_core::sc_start_of_simulation_invoked() )
  {
    OSSS_REPORT_WARNING( osss::report::called_after_elaboration )
      % __PRETTY_FUNCTION__;
    // abort
    return;
  }

  osssi::osss_process_handle cur_proc
    = osssi::osss_process_handle::last_created();
  osssi::OSSS_Process_ID_t proc_id
    = cur_proc.get_proc_id();

  // this is a workaround for direct access,
  // since we don't have an explicit port in this case
  IDType client_id =
    static_cast< osssi::osss_process_handle::handle_type >( cur_proc );

  // register process as new client (with an implicit port)
  so.subscribe( client_id, p );
  so.registerPortUsage( client_id, proc_id );
}

template< class ObjectType, class SchedulerType >
void uses( osss_shared<ObjectType, SchedulerType> & so
         , osss_priority p )
{
  osss_uses( so, p );
}


//-----------------------------------------------------------------------//
} // namespace osss

// $Id: osss_shared.tpp 3079 2009-03-27 13:10:46Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
