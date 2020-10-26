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

#include "o3s/datatypes/osss_object.h"
#include "o3s/utils/osss_class_proxy_registry.h"

namespace osss {

template< class InterfaceT >
osss_recon< InterfaceT >::osss_recon( sc_core::sc_module_name module_name )
  : osss_recon_base( module_name )
{}

template< class InterfaceT >
std::string const &
osss_recon< InterfaceT >::if_typename() const
{
   static std::string if_typename_
     = osssi::type_id::from<InterfaceT>().name();
   return if_typename_;
}

template< class InterfaceT >
void
osss_recon<InterfaceT>::end_of_elaboration()
{
  if (1 == m_num_slots) {
    // One slot: The user may utilise temporary context
    // create temporary context and bind it
    osss_context<InterfaceT>* tmp_ctx
      = new osss_context< InterfaceT >("temporary");
    m_temporary_context = tmp_ctx;
    tmp_ctx->bind( *this );
    m_temporary_context_index = m_context_table.findIndex( m_temporary_context );
  } else {
    // We don't have a temporary context
  }

  osss_recon_base::end_of_elaboration();
}


template< class InterfaceT >
bool
osss_recon<InterfaceT>::check_runtime_type( osss_object* ptr ) const
{
  // NULL is always a "valid" type
  if( !ptr )
    return true;
  return ( NULL != dynamic_cast< InterfaceT * >(ptr) );
}

#if 0 /* no references yet */
template< class InterfaceT >
template < class DerivedT >
void
osss_recon< InterfaceT >::operator=( const osss_reference< DerivedT > & source )
{
  if (NULL == dynamic_cast< InterfaceT * >(source.getOSSSObjectPtr()))
  {
    std::cerr << "ERROR: osss_reference interface type "
              << typeid(DerivedT).name();
    std::cerr << " is not derived from or identical to osss_recon interface type "
              << typeid(InterfaceT).name()
              << std::endl;
    OSSS_ABORT();
  }
  // Type descriptor may be different since
  // hardware device type for source and destination
  // may differ and they do not share descriptors.
  const osssi::osss_type_descriptor * td = m_hardware_device_ptr->getDeviceTypePtr()->findTypePtr(source.getOSSSObjectPtr()->getTypePtr()->getTypeName());

  // This will complain if we are a multi-slot object since
  // access to anonymous context is requested.
  // NULL == anonymous context
  osssi::osss_context_base * nc_ptr = requestPermission(NULL, source->getOSSSObjectPtr()->_getClone_(), td);
  // Create && anonymous context: ==> creation of temporary context
  returnPermission(nc_ptr);
}
#endif

template< class InterfaceT >
template < class DerivedT >
void
osss_recon< InterfaceT >::operator=( osss_context< DerivedT > & source )
{
  // Test, if the right hand side is assignment compatible. We use
  // the C++ RTTI for this task.
  if (NULL == dynamic_cast< InterfaceT * >(source.getOSSSObjectPtr()))
  {
    std::cerr << "ERROR: osss_context interface type "
              << osssi::type_id::from<DerivedT>();
    std::cerr << " is not derived from or identical to osss_recon interface type "
              << this->if_typename()
              << std::endl;
    OSSS_ABORT();
  }

  // request permission on source
  osssi::osss_user_process_descriptor * src_upd = source.requestPermission();

  OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Requesting rhs read method execution [ recon := context ]");

  // read source
  osss_object * src_copy_ptr = // source.getOSSSObjectPtr()->_getClone_();
    osssi::osss_class_proxy_registry::getClone( *source.getOSSSObjectPtr() );

  wait(2); // Method call overhead

  OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Recognising rhs read method completed [ recon := context ]");

  // return permission
  source.returnPermission(src_upd);

  // This will complain if we are a multi-slot object since
  // access to anonymous context is requested.
  // NULL == anonymous context
  osssi::osss_user_process_descriptor * dest_upd = requestPermission(NULL, src_copy_ptr);

  OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Requesting lhs assign method execution [ recon := context ]");

  wait(2); // Method call overhead

  OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Recognising lhs assign method completed [ recon := context ]");

  // Return the access permission to the access controller.
  returnPermission(dest_upd);

}

template< class InterfaceT >
void
osss_recon< InterfaceT >::operator=( osss_recon< InterfaceT > & source )
{
  // Make sure the source object is single-slot
  if (1 != source.m_num_slots)
  {
    std::cerr << "ERROR! Cannot access anonymous context of a multi-slot" << std::endl
              << "       osss_recon<> object. Object is: " << source.name() << std::endl;
    OSSS_ABORT();
  }

  // Request permission on source
  osssi::osss_user_process_descriptor * src_upd = source.requestPermission();

  OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Requesting rhs read method execution [ recon := recon ]");

  wait(2); // Method call overhead

  OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Recognising rhs read method completed [ recon := recon ]");

  // Read source
  osss_object * src_copy_ptr = // source.getOSSSObjectPtr()->_getClone_();
    osssi::osss_class_proxy_registry::getClone( *source.getOSSSObjectPtr() );

  // Return permission for source
  source.returnPermission(src_upd);

  // This will complain if we are a multi-slot object since
  // access to anonymous context is requested.
  // NULL == anonymous context
  osssi::osss_user_process_descriptor * dest_upd = requestPermission(NULL, src_copy_ptr);

  OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Requesting lhs assign method execution [ recon := recon ]");

  wait(2); // Method call overhead

  OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Recognising lhs assign method completed [ recon := recon ]");

  // Return permission for destination
  returnPermission(dest_upd);

}


template< class InterfaceT >
template< class DerivedT   >
void
osss_recon< InterfaceT >::operator=( osss_recon< DerivedT > & source )
{
  // Check for assignment compatibility via RTTI.
  if (NULL == dynamic_cast< InterfaceT * >(source.getOSSSObjectPtr()))
  {
    std::cerr << "ERROR: RHS osss_recon interface type "
              << osssi::type_id::from<DerivedT>();
    std::cerr << " is not derived from or identical to LHS osss_recon interface type "
              << this->if_typename()
              << std::endl;
    OSSS_ABORT();
  }
  // Make sure the source object is single-slot
  if (1 != source.m_num_slots)
  {
    std::cerr << "ERROR! Cannot access anonymous context of a multi-slot" << std::endl
              << "       osss_recon<> object. Object is: " << source.name() << std::endl;
    OSSS_ABORT();
  }

  // request permission on source
  osssi::osss_user_process_descriptor * src_upd = source.requestPermission();

  OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Requesting rhs read method execution [ recon := recon ]");

  wait(2); // Method call overhead

  // read source
  osss_object * src_copy_ptr = // source.getOSSSObjectPtr()->_getClone_();
    osssi::osss_class_proxy_registry::getClone( *source.getOSSSObjectPtr() );

  OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Recognising rhs read method completed [ recon := recon ]");

  // Return source permission
  source.returnPermission(src_upd);

  // This will complain if we are a multi-slot object since
  // access to anonymous context is requested.
  // NULL == anonymous context
  osssi::osss_user_process_descriptor * dest_upd = requestPermission(NULL, src_copy_ptr);

  OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Requesting lhs assign method execution [ recon := recon ]");

  wait(2); // Method call overhead

  OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Recognising lhs assign method completed [ recon := recon ]");

  returnPermission(dest_upd);
}

template< class InterfaceT >
void
osss_recon< InterfaceT >::operator=( const InterfaceT & source )
{
  // Create && anonymous context: ==> creation of temporary context
  // This will complain if we are a multi-slot object since
  // access to anonymous context is requested.
  // (Remember: NULL == anonymous context)
  osss_object* source_clone = osssi::osss_class_proxy_registry::getClone( source );
  osssi::osss_user_process_descriptor * upd = requestPermission(NULL, source_clone);

  OSSS_MESSAGE(DEBUG_RECON_OBJECT)
   << "Requesting assign method execution [ recon := plain ]";

  wait(2); // Method call overhead

  OSSS_MESSAGE(DEBUG_RECON_OBJECT)
    << "Recognising assign method completed [ recon := plain ]";

  // Return access permission to access controller.
  returnPermission(upd);
}

template< class InterfaceT >
template < class DerivedT >
void
osss_recon< InterfaceT >::operator=( const osss_polymorphic< DerivedT > & source )
{
  // Check type compatibility before assignment
  if (NULL == dynamic_cast< InterfaceT * >( source.operator->()  ))
  {
    std::cerr << "ERROR: osss_polymorphic interface type "
              << osssi::type_id::from<DerivedT>();
    std::cerr << " is not derived from or identical to osss_recon interface type "
              << this->if_typename()
              << std::endl;
    OSSS_ABORT();
  }

  // This will complain if we are a multi-slot object since
  // access to anonymous context is requested.
  // NULL == anonymous context
  osss_object* source_clone = osssi::osss_class_proxy_registry::getClone( source.cast_to_const() );
  osssi::osss_user_process_descriptor * upd = requestPermission(NULL, source_clone);

  OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Requesting assign method execution [ recon := poly ]");

  wait(2); // Method call overhead

  OSSS_MESSAGE(DEBUG_RECON_OBJECT, "Recognising assign method completed [ recon := poly ]");

  // Return access permission to access controller.
  returnPermission(upd);
}


/** This method performs a default constructor call on the recon object.
    If the argument is NULL, it is done on the anonymous context.
    If it is non-NULL, the call is executed on the given context. */
template< class InterfaceT >
template< class RHSType >
void
osss_recon< InterfaceT >::construct(osssi::osss_context_base * calling_named_context_ptr)
{
  InterfaceT* invalid_rhs_type_given = (RHSType*) 0;

  osssi::osss_user_process_descriptor * upd = requestPermission(calling_named_context_ptr, NULL);

  // We rely on m_context being updated.
  // Reason: requestPermission() always communicates with
  // the access controller upon context creation.
  osssi::whichLastContext(upd)->updateContent( RHSType() );

  returnPermission( upd );
}

template< class InterfaceT >
typename osss_recon< InterfaceT >::CallWrapperClass
osss_recon< InterfaceT >::operator->()
{
  return CallWrapperClass( *this );
}

// The argument distinguishes between two use cases:
// a) The recon object is intended to be intact and shall be used.
//    Here we do not accept the content ptr to be NULL. We will
//    never return an invalid pointer.
// b) Internal use, e.g. testing, if the content was set / is valid.
//    Here we happily return NULL.
template< class InterfaceT >
InterfaceT *
osss_recon< InterfaceT >::getContentPtr(bool check_for_null)
{
  InterfaceT * if_ptr =
    dynamic_cast< InterfaceT* >( this->getOSSSObjectPtr( check_for_null ) );
  OSSS_ASSERT( !check_for_null || if_ptr );
  // All checks passed (already checked in OSSSOjectPtr) Proceed.
  return if_ptr;
}

// This method cannot provide exact timing :(
template< class InterfaceT >
inline
osss_recon< InterfaceT >::operator osss_polymorphic< InterfaceT > ()
{
  // Attempt to copy the anonymous context. "this" is the right-hand-side.
  if (1 != m_num_slots)
  {
    std::cerr << "ERROR! Cannot access anonymous context of a multi-slot reconfigurable object" << std::endl;
    OSSS_ABORT();
  }

  // We do not give an argument. Default value is the anonymous
  // context.
  osssi::osss_user_process_descriptor * upd = requestPermission();

  wait( 2 );

  // We should consume the copy-cycles here and return
  // access permission afterwards... But this would require
  // grabbing the thread of control right after our return
  // value is copied into its new destination. At that time
  // this method is left long ago :(
  // So we return permission before leaving this method.
  // This results in consumption of copy-constructor cycles
  // (if any) AFTER returning of permission....
  // Note: Copy cycles do not mean protocol cycles but
  // wait()s possibly placed in copy constructor...
  returnPermission(upd);

  // Return the value
  return ( osss_polymorphic< InterfaceT >( *getContentPtr() ) );
}

//----------------------------------------------------------------------------//

template< class InterfaceT >
template< class TargetType >
TargetType
osss_recon< InterfaceT >::cast()
{
  osss_polymorphic< InterfaceT > p = *this;
  return p.cast< TargetType >();
}

} // end namespace osss

// $Id: osss_recon.tpp 3021 2009-03-04 22:55:21Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
