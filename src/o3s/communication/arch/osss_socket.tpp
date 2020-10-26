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
#include <iostream>

namespace osss {

//----------------------------------------------------------------------------//

template<class ContentClass>
osss_socket<ContentClass>::osss_socket( const sc_core::sc_module_name& name )
  : sc_module( name )
{
  m_contentObject.OSSS_currentlyConnectedInterface = &m_portBindings;
}

//----------------------------------------------------------------------------//

template<class ContentClass>
typename osss_socket<ContentClass>::ContentClass_t*
osss_socket<ContentClass>::operator -> ()
{
  return &m_contentObject;
}

//----------------------------------------------------------------------------//

template<class ContentClass>
osss_socket<ContentClass>::
operator typename osss_socket<ContentClass>::ContentClass_t& ()
{
  return m_contentObject;
}

//----------------------------------------------------------------------------//

template<class ContentClass>
void
osss_socket<ContentClass>::operator = ( ContentClass_t const& source)
{
  m_contentObject = source;
  m_contentObject.OSSS_currentlyConnectedInterface = &m_portBindings;
}

//----------------------------------------------------------------------------//

template<class ContentClass>
typename osss_socket<ContentClass>::RequiredInterface_t&
osss_socket<ContentClass>::get_interface()
{
  return m_portBindings;
}

//----------------------------------------------------------------------------//

template<class ContentClass>
void
osss_socket<ContentClass>::end_of_elaboration()
{
  std::cout << "Warning: osss_socket<ContentClass> "
	    << "is currently not synthesisable." 
	    << std::endl << std::endl;
}

// -----------------------------------------------------------------------------
// Partial specilisation for polymorphic/shared objects
// -----------------------------------------------------------------------------

template<class ContentClass, template<class> class wrappingClass >
osss_socket<wrappingClass<ContentClass> >::
osss_socket( const sc_core::sc_module_name& name )
  : sc_module( name )
{
  m_contentObject.plugged_object().OSSS_currentlyConnectedInterface = 
    &m_portBindings;
}

//----------------------------------------------------------------------------//

template<class ContentClass, template<class> class wrappingClass >
ContentClass *
osss_socket<wrappingClass<ContentClass> >::operator -> ()
{
  return m_contentObject.operator -> ();
}

//----------------------------------------------------------------------------//

template<class ContentClass, template<class> class wrappingClass >
wrappingClass<ContentClass>&
osss_socket<wrappingClass<ContentClass> >::plugged_object()
{
  return m_contentObject;
}

//----------------------------------------------------------------------------//

template<class ContentClass, template<class> class wrappingClass >
void
osss_socket<wrappingClass<ContentClass> >::
operator = ( wrappingClass<ContentClass> const& source)
{
  m_contentObject = source;
  m_contentObject->OSSS_currentlyConnectedInterface = &m_portBindings;
}

//----------------------------------------------------------------------------//

template<class ContentClass, template<class> class wrappingClass >
typename osss_socket<wrappingClass<ContentClass> >::RequiredInterface_t&
osss_socket<wrappingClass<ContentClass> >::get_interface()
{
  return m_portBindings;
}

//----------------------------------------------------------------------------//

template<class ContentClass, template<class> class wrappingClass >
void
osss_socket<wrappingClass<ContentClass> >::end_of_elaboration()
{
  std::cout << "Warning: osss_socket<wrappingClass<ContentClass> > " 
	    << "is currently not synthesisable." 
	    << std::endl << std::endl;
}

// -----------------------------------------------------------------------------
// Partial specialisation for a shared object containing a polymorphic object
// -----------------------------------------------------------------------------

template<class ContentClass>
osss_socket<osss_shared<osss_polymorphic<ContentClass> > >::
osss_socket( const sc_core::sc_module_name& name )
  : sc_module( name )
{
  m_contentObject.plugged_object()->
    OSSS_currentlyConnectedInterface = &m_portBindings;
}

//----------------------------------------------------------------------------//

template<class ContentClass>
osss_shared<osss_polymorphic<ContentClass> >&
osss_socket<osss_shared<osss_polymorphic<ContentClass> > >::plugged_object()
{
  return ( m_contentObject );
}

//----------------------------------------------------------------------------//

template<class ContentClass>
template<class SourceType>
osss_socket<osss_shared<osss_polymorphic<ContentClass> > > &
osss_socket<osss_shared<osss_polymorphic<ContentClass> > >::
operator = ( const SourceType & source)
{
  m_contentObject = source;
  m_contentObject.plugged_object()->OSSS_currentlyConnectedInterface = 
    &m_portBindings;
  return ( *this );
}

//----------------------------------------------------------------------------//

template<class ContentClass>
typename 
osss_socket<osss_shared<osss_polymorphic<ContentClass> > >::RequiredInterface_t&
osss_socket<osss_shared<osss_polymorphic<ContentClass> > >::get_interface()
{
  return m_portBindings;
}

//----------------------------------------------------------------------------//

template<class ContentClass>
void
osss_socket<osss_shared<osss_polymorphic<ContentClass> > >::end_of_elaboration()
{
  std::cout << "Warning: " 
	    << "osss_socket<osss_shared<osss_polymorphic<ContentClass> > >: " 
	    << "is currently not synthesisable." 
	    << std::endl << std::endl;
}

} /* namespace osss */

// $Id: osss_socket.tpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
