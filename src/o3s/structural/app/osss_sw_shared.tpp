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

//----------------------------------------------------------------------------//

template< class ObjectType >
osss_sw_shared<ObjectType>::
osss_sw_shared( sc_core::sc_module_name name )
  : sc_core::sc_module( name )
  , m_reset_sensitivity( OSSS_POS )
  , m_swTaskRegistered(false)
  , m_period(sc_core::sc_time(0.0, sc_core::SC_NS))
  , m_hwModuleRegistered(false)
  , m_priority(0)
  , m_request(false)
  , m_access_granted(false)
{
  m_objectPointer = new ObjectType();
  osss_sw_shared_registry::addPluggedObject(this, m_objectPointer);

/*
  m_reset_process_handle = osssi::osss_create_method_process(
      sc_core::sc_gen_unique_name("reset_process"),
      SC_MAKE_FUNC_PTR( SC_CURRENT_USER_MODULE, reset_process ), this );
*/
}

//----------------------------------------------------------------------------//

template< class ObjectType >
void
osss_sw_shared<ObjectType>::reset_sensitivity(const osss_sensitivity sens)
{
  m_reset_sensitivity = sens;
}

//----------------------------------------------------------------------------//

template< class ObjectType >
void 
osss_sw_shared<ObjectType>::before_end_of_elaboration()
{
  switch(m_reset_sensitivity)
  {
    case OSSS_POS :
    {
      OSSS_UNIQUE_CTHREAD(reset_process, clock_port.pos());
      reset_signal_is(reset_port, true);
      /*
      sensitive_pos << m_reset_process_handle;
      sensitive_pos << reset_port;
      */
      break;
    }
    case OSSS_NEG :
    {
      OSSS_UNIQUE_CTHREAD(reset_process, clock_port.pos());
      reset_signal_is(reset_port, false);
      /*
      sensitive_neg << m_reset_process_handle;
      sensitive_neg << reset_port;
      */
      break;
    }
    case OSSS_LEVEL :
    {
      OSSS_UNIQUE_METHOD(reset_method);
      dont_initialize();
      /*
      sensitive << m_reset_process_handle;
      */
      sensitive << reset_port;
      break;
    }
    default:
    {
      OSSS_UNIQUE_CTHREAD(reset_process, clock_port.pos());
      reset_signal_is(reset_port, true);
      /*
      sensitive_pos << m_reset_process_handle;
      sensitive_pos << reset_port;
      */
      break;
    }
  }
}

//----------------------------------------------------------------------------//

template< class ObjectType >
void
osss_sw_shared<ObjectType>::reset_process()
{
  reset();
  wait();

  while(true) { wait(); }
}

template< class ObjectType >
void
osss_sw_shared<ObjectType>::reset_method()
{
  reset();
}

//----------------------------------------------------------------------------//

template< class ObjectType >
osss_sw_shared<ObjectType>::~osss_sw_shared()
{
  if ( m_objectPointer )
  {
    delete( m_objectPointer );
  }
  m_objectPointer = NULL;
}

//----------------------------------------------------------------------------//

template< class ObjectType >
inline
void
osss_sw_shared<ObjectType>::reset()
{
  if ( m_objectPointer )
  {
    osss_sw_shared_registry::deletePluggedObject(m_objectPointer);
    delete( m_objectPointer );
  }
  m_objectPointer = new ObjectType();
  osss_sw_shared_registry::addPluggedObject(this, m_objectPointer);

  m_request = false;
  m_access_granted = false;
}

//----------------------------------------------------------------------------//

template< class ObjectType >
inline
ObjectType *
osss_sw_shared<ObjectType>::operator->() const
{
  return( m_objectPointer );
}

template< class ObjectType >
inline
sc_core::sc_interface *
osss_sw_shared<ObjectType>::get_interface() const
{
  return dynamic_cast<sc_core::sc_interface*>( m_objectPointer );
}

template< class ObjectType >
inline
ObjectType &
osss_sw_shared<ObjectType>::plugged_object()
{
  return( * m_objectPointer );
}

//----------------------------------------------------------------------------//

template< class ObjectType >
inline
void 
osss_sw_shared<ObjectType>::registerHWModule()
{
  if (! m_hwModuleRegistered)
    m_hwModuleRegistered = true;
  else
  {
    std::cerr << "Error: " << name()
              << " is already bound to a module" << std::endl;
    OSSS_ABORT();
  }
}

//----------------------------------------------------------------------------//

template< class ObjectType >
inline
void 
osss_sw_shared<ObjectType>::setRequest()
{
  m_request = true;
}

template< class ObjectType >
inline
void 
osss_sw_shared<ObjectType>::cancelRequest()
{
  m_request = false;
}

template< class ObjectType >
inline
bool 
osss_sw_shared<ObjectType>::accessGranted()
{
  return m_access_granted;
}

//----------------------------------------------------------------------------//

template< class ObjectType >
inline
void
osss_sw_shared<ObjectType>::registerSWTask( osss_priority priority )
{
  if (! m_swTaskRegistered )
  {
    m_swTaskRegistered = true;
    m_priority = priority;
  }
  else
  {
    std::cerr << "Error: " << name()
              << " is already bound to a software module" << std::endl;
    OSSS_ABORT();
  }
}

//----------------------------------------------------------------------------//

template< class ObjectType >
inline
bool 
osss_sw_shared<ObjectType>::pendingRequest()
{
  return m_request;
}

template< class ObjectType >
inline
void
osss_sw_shared<ObjectType>::grantAccess(bool access)
{
  m_access_granted = access;
}

//----------------------------------------------------------------------------//

template< class ObjectType >
inline
osss_priority
osss_sw_shared<ObjectType>::getPriority()
{
  return m_priority;
}

//----------------------------------------------------------------------------//

template< class ObjectType >
inline
void
osss_sw_shared<ObjectType>::setPeriod(const sc_core::sc_time period)
{
  m_period = period;
}

template< class ObjectType >
sc_core::sc_time
osss_sw_shared<ObjectType>::getPeriod()
{
  return m_period;
}

} /* namespace osss */

// $Id: osss_sw_shared.tpp 2915 2008-12-15 18:25:28Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
