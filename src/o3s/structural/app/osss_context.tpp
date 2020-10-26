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
#ifndef OSSS_CONTEXT_TPP_INCLUDED
#define OSSS_CONTEXT_TPP_INCLUDED

#include "osss_recon_base.h"
#include "../arch/system/osss_device_type.h"
#include "o3s/datatypes/osss_object.h"
#include "o3s/utils/osss_class_proxy_registry.h"
#include "o3s/utils/osss_recon_types.h"

#define DEBUG_NAMED_CONTEXT 0

namespace osss {

template< class InterfaceT >
osss_context< InterfaceT >::osss_context()
  : osss_context_base()
  , m_type_descriptor(0)
{
  m_content_ptr = new InterfaceT();
  osss_context_base::registry_subscribe();
}

template< class InterfaceT >
osss_context< InterfaceT >::osss_context(const char * name)
  : osss_context_base( name )
  , m_type_descriptor(0)
{
  m_content_ptr = new InterfaceT();
  osss_context_base::registry_subscribe();
}

template< class InterfaceT >
std::string const&
osss_context< InterfaceT >::if_typename() const
{
  static std::string if_type_name_
    = osssi::type_id::from<InterfaceT>().name();
  return if_type_name_;
}

template< class InterfaceT >
template< class ReconObjectInterfaceT  >
void
osss_context< InterfaceT >::bind(osss_recon< ReconObjectInterfaceT > & recon)
{
  {
    // static compatibility check of classes
    // - pointer upcast assignment
    ReconObjectInterfaceT * recon_if_p = m_content_ptr;
    recon_if_p = 0; // just to use this dummy variable
  }
  base_type::do_bind( recon );
 }

template< class InterfaceT >
template< class ReconObjectInterfaceT >
void
osss_context< InterfaceT >::operator()(osss_recon< ReconObjectInterfaceT > & recon)
{
  // We are a binding operator, too. Use the more verbose one.
  bind(recon);
}

template< class InterfaceT >
osss_context< InterfaceT >::~osss_context()
{
  if (NULL != m_content_ptr)
  {
    osss_context_base::registry_unsubscribe();
    delete m_content_ptr;
  }
}

#if 0 /* no references yet */
template< class InterfaceT >
template< class DerivedInterfaceT >
osss_context< InterfaceT >& osss_context< InterfaceT >::operator=( const osss_reference< DerivedInterfaceT > & source)
{
  if (m_recon_object_ptr == NULL)
  {
    std::cerr << "ERROR: named context class not been bound to a recon object" << std::endl;
    OSSS_ABORT();
  }
  // The device may have changed, that might
  // require a descriptor for same object type but
  // different device type
  const osssi::osss_type_descriptor * new_descriptor = m_recon_object_ptr->getDevicePtr()->getDeviceTypePtr()->findTypePtr(
                                    m_type_descriptor->getTypeName());
  if (dynamic_cast< InterfaceT * >( source.getOSSSObjectPtr() ) == NULL)
  {
    std::cerr << "ERROR: osss_reference interface type "
              << typeid(DerivedInterfaceT).name();
    std::cerr << " is not derived from or identical to osss_context interface type "
              << typeid(InterfaceT).name()
              << std::endl;
    OSSS_ABORT();
  }
  m_recon_object_ptr->requestPermission(this, source->getOSSSObjectPtr()->_getClone_(), new_descriptor);
  m_recon_object_ptr->returnPermission(this);
  return *this;
}
#endif

template< class InterfaceT >
template< class DerivedInterfaceT >
osss_context< InterfaceT >&
osss_context< InterfaceT >::operator=( osss_context< DerivedInterfaceT > & source)
{
  // Sanity check: Are we bound?
  if (m_recon_object_ptr == NULL)
  {
    std::cerr << "ERROR: named context class not been bound to a recon object" << std::endl;
    OSSS_ABORT();
  }

  if (dynamic_cast< InterfaceT * >( source.getOSSSObjectPtr() ) == NULL)
  {
    std::cerr << "ERROR: RHS osss_context interface type "
              << osssi::type_id::from<DerivedInterfaceT>();
    std::cerr << " is not derived from or identical to LHS osss_context interface type "
              << this->if_typename()
              << std::endl;
    OSSS_ABORT();
  }

  osssi::osss_user_process_descriptor * src_upd = source.requestPermission();

  OSSS_MESSAGE(DEBUG_NAMED_CONTEXT, "assignment permission on source obtained [ context := context ]");
  osss_object * src_copy_ptr =
    osssi::osss_class_proxy_registry::getClone( *source.getOSSSObjectPtr() );

  wait(2); // Method call overhead

  source.returnPermission(src_upd);

  osssi::osss_user_process_descriptor * dest_upd = m_recon_object_ptr->requestPermission(this, src_copy_ptr);

  OSSS_MESSAGE(DEBUG_NAMED_CONTEXT, "assignment permission on destination obtained [ context := context ]");

  wait(2); // Method call overhead

  m_recon_object_ptr->returnPermission(dest_upd);

  return *this;
}

template< class InterfaceT >
template< class DerivedInterfaceT >
osss_context< InterfaceT >& osss_context< InterfaceT >::operator=( osss_recon< DerivedInterfaceT > & source)
{
  // Sanity check: Are we bound?
  if (m_recon_object_ptr == NULL)
  {
    std::cerr << "ERROR: named context class not been bound to a recon object" << std::endl;
    OSSS_ABORT();
  }
  if (dynamic_cast< InterfaceT * >( source.getOSSSObjectPtr() ) == NULL)
  {
    std::cerr << "ERROR: RHS osss_recon interface type "
              << osssi::type_id::from<DerivedInterfaceT>();
    std::cerr << " is not derived from or identical to LHS osss_context interface type "
              << this->if_typename()
              << std::endl;
    OSSS_ABORT();
  }


  osssi::osss_user_process_descriptor * src_upd = source.requestPermission();

  osss_object * src_copy_ptr =
    osssi::osss_class_proxy_registry::getClone( *source.getOSSSObjectPtr() );

  OSSS_MESSAGE(DEBUG_NAMED_CONTEXT, "assignment permission on source obtained [ context := recon ]");

  wait(2); // Method call overhead

  source.returnPermission(src_upd);

  osssi::osss_user_process_descriptor * dest_upd = m_recon_object_ptr->requestPermission(this, src_copy_ptr);

  OSSS_MESSAGE(DEBUG_NAMED_CONTEXT, "assignment permission on destination obtained [ context := recon ]");

  wait(2); // Method call overhead

  m_recon_object_ptr->returnPermission(dest_upd);

  return *this;
}

template< class InterfaceT >
osss_context< InterfaceT >& osss_context< InterfaceT >::operator=( const osss_polymorphic< InterfaceT > & source)
{
  // Sanity check: Are we bound?
  if (m_recon_object_ptr == NULL)
  {
    std::cerr << "ERROR: named context class not been bound to a recon object" << std::endl;
    OSSS_ABORT();
  }

  osssi::osss_user_process_descriptor * upd = m_recon_object_ptr->requestPermission(this,
    osssi::osss_class_proxy_registry::getClone( source.cast_to_const() ));

  OSSS_MESSAGE(DEBUG_NAMED_CONTEXT, "assignment permission on destination obtained [ context := poly ]");

  wait(2); // Method call overhead

  m_recon_object_ptr->returnPermission(upd);

  return *this;
}

template< class InterfaceT >
osss_context< InterfaceT >&
osss_context< InterfaceT >::operator=( const InterfaceT & source)
{
  // Sanity check: Are we bound?
  if (m_recon_object_ptr == NULL)
  {
    std::cerr << "ERROR: named context class not been bound to a recon object" << std::endl;
    OSSS_ABORT();
  }

  osssi::osss_user_process_descriptor * upd = m_recon_object_ptr->requestPermission(this,
    osssi::osss_class_proxy_registry::getClone( source ));


  OSSS_MESSAGE(DEBUG_NAMED_CONTEXT, "assignment permission on destination obtained [ context := plain ]");

  wait(2); // Method call overhead

  m_recon_object_ptr->returnPermission(upd);
  return *this;
}

template< class InterfaceT >
typename osss_context< InterfaceT >::CallWrapperClass
osss_context< InterfaceT >::operator->()
{
  return CallWrapperClass( *this );
}


template< class InterfaceT >
template< class RHSType>
void
osss_context< InterfaceT >::construct()
{
  InterfaceT* invalid_rhs_type_given = (RHSType*) 0;
  osssi::osss_user_process_descriptor * upd = m_recon_object_ptr->requestPermission(this, NULL);
  updateContent( new RHSType() );
  m_recon_object_ptr->returnPermission(upd);
}

template< class InterfaceT >
void
osss_context< InterfaceT >::updateType(const osssi::osss_type_descriptor * new_type)
{
  // type argument
  if (NULL == new_type)
  {
    std::cerr << "ERROR! Cannot set type to NULL" << std::endl;
    OSSS_ABORT();
  }
  m_type_descriptor = new_type;
}

template< class InterfaceT >
void
osss_context< InterfaceT >::updateContent(osss_object * new_content)
{
  // context base argument
  if (NULL == new_content)
  {
    std::cerr << "ERROR! Cannot set content to NULL" << std::endl;
    OSSS_ABORT();
  }
  InterfaceT * rc_ptr = dynamic_cast< InterfaceT * >(new_content);
  if (NULL == rc_ptr)
  {
    std::cerr << "ERROR! Cannot update named context with new content, type mismatch" << std::endl;
    OSSS_ABORT();
  }
  if (NULL != m_content_ptr)
  {
    osss_context_base::registry_unsubscribe();
    delete m_content_ptr;
  }

  // We could simply use "m_content_ptr = rc_ptr" but this would
  // make using sockets impossible. We need the old objects socket
  // binding(s) to be moved into the shelter just before the new
  // object is created (which fetches this binding from shelter).
  m_content_ptr = osssi::osss_class_proxy_registry::getClone( *rc_ptr );
  osss_context_base::registry_subscribe();

  // No memory leak, please
  delete rc_ptr;

}

template< class InterfaceT >
osssi::osss_user_process_descriptor *
osss_context< InterfaceT >::requestPermission()
{
  // Request access permission on ourselves from reconfigurable object
  return m_recon_object_ptr->requestPermission(this);
}

template< class InterfaceT >
void
osss_context< InterfaceT >::returnPermission(osssi::osss_user_process_descriptor * upd)
{
  if (  (NULL != osssi::whichCurrentPermission(upd))
     && (this != osssi::whichCurrentPermission(upd)))
  {
    std::cerr << "ERROR! You may not return permission on a different context!" << std::endl;
    OSSS_ABORT();
  }
  // Return access permission for ourselves to recon object
  m_recon_object_ptr->returnPermission(upd);
}


template< class InterfaceT >
void
osss_context< InterfaceT >::enable()
{
  // This will enable the context
  osssi::osss_user_process_descriptor * upd = m_recon_object_ptr->requestPermission(this);
  m_recon_object_ptr->returnPermission(upd);
  // But is not locked somehow!
}

template< class InterfaceT >
void
osss_context< InterfaceT >::printOn(std::ostream & os) const
{
  os << "[osss_context:osss_recon=" << m_recon_object_ptr;
  //::operator<<(os,  m_context_base_ptr);
  os << " class_id=" << m_type_descriptor;
  os << "]";
}

template< class InterfaceT >
void
osss_context< InterfaceT >::writeXMLStringOn(std::ostream & os) const
{
  os << "<named_context id=\"" << name() << "\""
     << " recon_object=\"" << m_recon_object_ptr << "\" "
     << " class_id=\"" << m_type_descriptor << "\" "
      << "/>" << std::endl;
}


/*
template< class InterfaceT >
void
osss_context< InterfaceT >::prefetch()
{
  sanityCheck();
  recon_object_ptr->prefetchNamedContext(this);
}
*/

#if 0 /* no references yet */
template< class InterfaceT >
osss_reference< InterfaceT >
osss_context< InterfaceT >::getReference()
{
  osss_reference< InterfaceT > ref;
  ref.setNamedContextPtr( this );
  return ( ref );
}
#endif

// cast operator
template< class InterfaceT >
osss_context< InterfaceT >::operator const osss_polymorphic < InterfaceT > ()
{
  osssi::osss_user_process_descriptor * upd = m_recon_object_ptr->requestPermission(this);
  m_recon_object_ptr->returnPermission(upd);
  return ( osss_polymorphic< InterfaceT>( *m_content_ptr ));
}

template< class InterfaceT >
void
osss_context< InterfaceT >::reset()
{
  if (NULL != m_content_ptr)
  {
    osss_context_base::registry_unsubscribe();
    delete m_content_ptr;
    // Immediately re-create the content, otherwise a socket binding
    // would get lost.
    m_content_ptr = new InterfaceT();
    osss_context_base::registry_subscribe();
  }
  m_type_descriptor = NULL;
}

template< class InterfaceT >
InterfaceT *
osss_context< InterfaceT >::getContentPtr(bool check_for_null)
{
#if DEBUG_NAMED_CONTEXT == 1
  if ( NULL == m_recon_object_ptr)
  {
    std::cerr << "ERROR: osss_context not bound to reconfigurable object"
              << std::endl;
    OSSS_ABORT();
  }
#endif
  if ( NULL == m_content_ptr)
  {
    if (check_for_null)
    {
      std::cerr << "ERROR: osss_context with NULL content"
                << std::endl;
      OSSS_ABORT();
    }
    else
    {
      return NULL;
    }
  }
  else
  {
    osss_object * cb_ptr = dynamic_cast< osss_object * >(m_content_ptr);
    if (check_for_null && (NULL == cb_ptr))
    {
      std::cerr << "ERROR: osss_context has content of type \""
           << osssi::type_id(*m_content_ptr)
           << "\""
           << std::endl
           << "       which is not derived from its restriction class \""
           << this->if_typename()
           << "\""
           << std::endl;
      std::cerr << "Possible reasons:"
           << "1) You forgot to assign something to an osss_context object"
           << "2) The class " << this->if_typename() << " has no OSSS_BASE_CLASS()"
           << "   and/or no virtual destructor (this may cause the your compilers RTTI to fail)"
           << std::endl;
      OSSS_ABORT();
    }
    return m_content_ptr;
  }
  return NULL;
}

template< class InterfaceT >
osss_object *
osss_context< InterfaceT >::getOSSSObjectPtr(bool check_for_null) const
{
#if DEBUG_NAMED_CONTEXT == 1
  if ( NULL == m_recon_object_ptr)
  {
    std::cerr << "ERROR: osss_context not bound to reconfigurable object"
              << std::endl;
    OSSS_ABORT();
  }
#endif
  if ( NULL == m_content_ptr)
  {
    if (check_for_null)
    {
      std::cerr << "ERROR: osss_context with NULL content"
                << std::endl;
      OSSS_ABORT();
    }
    else
    {
      return NULL;
    }
  }
  else
  {
    osss_object * cb_ptr = dynamic_cast< osss_object * >(m_content_ptr);
    if (check_for_null && (NULL == cb_ptr))
    {
      std::cerr << "ERROR: osss_context has content of type \""
           << osssi::type_id(*m_content_ptr)
           << "\""
           << std::endl
           << "       which is not derived from its restriction class \""
           << this->if_typename()
           << "\""
           << std::endl
           << "       or not derived from osss_object"
           << std::endl;
      std::cerr << "Possible reasons:"
           << "1) You forgot to assign something to an osss_context object"
           << "2) The class " << this->if_typename() << " has no OSSS_BASE_CLASS()"
           << "   and/or no virtual destructor (this may cause the your compilers RTTI to fail)"
           << std::endl;
      OSSS_ABORT();
    }
    return cb_ptr;
  }
  return 0;
}

template< class InterfaceT >
osssi::osss_recon_base *
osss_context< InterfaceT >::getReconPtr() const
{
  return m_recon_object_ptr;
}

template< class InterfaceT >
const osssi::osss_type_descriptor *
osss_context< InterfaceT >::getTypePtr() const
{
  return m_type_descriptor;
}

/** always returns true (support for OSSS_KEEP_ENABLED() macro) */
template< class InterfaceT >
void
osss_context< InterfaceT >::lock()
{
  m_recon_object_ptr->lock(this);
}

/**
 * This method can be used by a client, to determine
 * the state of his external lock, associated with
 * this context.
 * \brief returns client-side locking state
 */
template< class InterfaceT >
bool
osss_context< InterfaceT >::isLocked()
{
  return m_recon_object_ptr->isExternallyLocked(this);
}

template< class InterfaceT >
void
osss_context< InterfaceT >::unlock()
{
  m_recon_object_ptr->unlock(this);
}

template< class InterfaceT >
bool
osss_context< InterfaceT >::isLazy()
{
  return m_recon_object_ptr->isLazy();
}

template< class InterfaceT >
void
osss_context< InterfaceT >::startLazy()
{
  m_recon_object_ptr->startLazy();
}

template< class InterfaceT >
void
osss_context< InterfaceT >::endLazy()
{
  m_recon_object_ptr->endLazy();
}



template< class InterfaceT >
bool
osss_context< InterfaceT >::evaluateInternalLock() const
{
  // If the guard is true, reconfiguration is permitted.
  // Then the lock is false... ("not locked")
  return (false == m_content_ptr->_internal_lock_release_guard_method_());
}

template< class InterfaceT >
inline
std::ostream & operator<<(std::ostream & os, const osss_context< InterfaceT > & object)
{
  object.printOn(os);
  return os;
}

} // end namespace osss

#endif // OSSS_CONTEXT_TPP_INCLUDED
// $Id: osss_context.tpp 3021 2009-03-04 22:55:21Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
