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

#include "o3s/utils/type_id.h"

namespace osss {

template<class ObjectType_if>
inline
osss_port<osss_shared_if<ObjectType_if> >::osss_port()
  : base_type(),
    m_firstAccess( true ),
    m_proc_id( 0 ),
    m_priority( 0 ),
    m_isPolymorphic( false )
{
  std::cout << "Warning: You are using the default constructor of "
            << "osss_port<osss_shared_if<...> >." << std::endl
            << "For synthesis it is required that the name of this port "
            << "matches its identifier. " << std::endl
            << "Please use osss_port( const char* ) constructor for osss_port "
            << name() << " instead."<< std::endl
            << std::endl;
}

//----------------------------------------------------------------------------//

template<class ObjectType_if>
inline
osss_port<osss_shared_if<ObjectType_if> >::osss_port( const char* name_ )
  : base_type(name_),
    m_firstAccess( true ),
    m_proc_id( 0 ),
    m_priority( 0 ),
    m_isPolymorphic( false )
{}

//----------------------------------------------------------------------------//

template<class ObjectType_if>
inline
osss_port<osss_shared_if<ObjectType_if> >::osss_port( if_type& interface_ )
  : base_type(interface_),
    m_firstAccess( true ),
    m_proc_id( 0 ),
    m_priority( 0 ),
    m_isPolymorphic( false )
{
  std::cout << "Warning: You are using the osss_port( osss_shared_if<>& ) "
            << "constructor of osss_port<osss_shared_if<...> >." << std::endl
            << "For synthesis it is required that the name of this port "
            << "matches its identifier. " << std::endl
            << "Please use osss_port( const char*, osss_shared_if<>& ) "
            << "constructor for osss_port " << name() << " instead."<< std::endl
            << std::endl;
}

//----------------------------------------------------------------------------//

template<class ObjectType_if>
inline
osss_port<osss_shared_if<ObjectType_if> >::
osss_port( const char* name_, if_type& interface_ )
  : base_type(name_, interface_),
    m_firstAccess( true ),
    m_proc_id( 0 ),
    m_priority( 0 ),
    m_isPolymorphic( false )
{}

//----------------------------------------------------------------------------//

template<class ObjectType_if>
inline
osss_port<osss_shared_if<ObjectType_if> >::osss_port( base_type& parent_ )
  : base_type(parent_),
    m_firstAccess( true ),
    m_proc_id( 0 ),
    m_priority( 0 ),
    m_isPolymorphic( false )
{
  std::cout << "Warning: You are using the "
            << "osss_port( sc_core::sc_port<osss_shared_if<>, 1>& ) "
            << "constructor of osss_port<osss_shared_if<...> >." << std::endl
            << "For synthesis it is required that the name of this port "
            << "matches its identifier. " << std::endl
            << "Please use "
            << "osss_port( const char*, sc_core::sc_port<osss_shared_if<>, 1>& ) "
            << "constructor for osss_port " << name() << " instead."<< std::endl
            << std::endl;
}

//----------------------------------------------------------------------------//

template<class ObjectType_if>
inline
osss_port<osss_shared_if<ObjectType_if> >::
osss_port( const char* name_, base_type& parent_ )
  : base_type(name_, parent_),
    m_firstAccess( true ),
    m_proc_id( 0 ),
    m_priority( 0 ),
    m_isPolymorphic( false )
{}

//----------------------------------------------------------------------------//

template<class ObjectType_if>
inline
osss_port<osss_shared_if<ObjectType_if> >::osss_port( this_type& parent_ )
  : base_type(parent_),
    m_firstAccess( true ),
    m_proc_id( 0 ),
    m_priority( 0 ),
    m_isPolymorphic( false )
{
  std::cout << "Warning: You are using the "
            << "osss_port( osss_port<osss_shared_if<...> >& ) "
            << "constructor of osss_port<osss_shared_if<...> >." << std::endl
            << "For synthesis it is required that the name of this port "
            << "matches its identifier. " << std::endl
            << "Please use "
            << "osss_port( const char*, osss_port<osss_shared_if<...> >& ) "
            << "constructor for osss_port " << name() << " instead."<< std::endl
            << std::endl;
}

//----------------------------------------------------------------------------//

template<class ObjectType_if>
inline
osss_port<osss_shared_if<ObjectType_if> >::
osss_port( const char* name_, this_type& parent_ )
  : base_type(name_, parent_),
    m_firstAccess( true ),
    m_proc_id( 0 ),
    m_priority( 0 ),
    m_isPolymorphic( false )
{}

//----------------------------------------------------------------------------//

template<class ObjectType_if>
inline
osss_port<osss_shared_if<ObjectType_if> >::~osss_port()
{}

//----------------------------------------------------------------------------//

template<class ObjectType_if>
inline
void
osss_port<osss_shared_if<ObjectType_if> >::
setPriority( osss_priority priority )
{
  m_priority = priority;
}

//----------------------------------------------------------------------------//

template<class ObjectType_if>
inline
osss_priority
osss_port<osss_shared_if<ObjectType_if> >::getPriority()
{
  return m_priority;
}

//----------------------------------------------------------------------------//

template<class ObjectType_if>
inline
ObjectType_if *
osss_port<osss_shared_if<ObjectType_if> >::operator->()
{

  if (m_firstAccess)
  {
    m_proc_id = osssi::osss_process_handle::current().get_proc_id();

    base_type::operator->()->registerPortUsage(
      this, m_proc_id);

    m_firstAccess = false;
  }
  else
  {
    if ( m_proc_id != osssi::osss_process_handle::current().get_proc_id())
    {
      std::cerr << "Error: This " << kind() << " "
	   << base_type::name()
	   << " has been accessed from within another process before. "
	   << "This might lead to an inconsistent state of the shared object."
	   << std::endl;
      // stop simulation due to error
      sc_core::sc_stop();
    }
  }
  // check has been performed at end_of_elaboration
  if (!m_isPolymorphic)
    return dynamic_cast<ObjectType_if*>(
      base_type::operator->()->get_interface());
  else
    return dynamic_cast<ObjectType_if*>
      (dynamic_cast<osss_polymorphic_if*>(
	base_type::operator->()->get_interface())
       ->get_interface());
}

//----------------------------------------------------------------------------//

template<class ObjectType_if>
inline
osss_arbiter_shared_base *
osss_port<osss_shared_if<ObjectType_if> >::getArbiter()
{
  return base_type::operator->()->getArbiter();
}

//----------------------------------------------------------------------------//

template<class ObjectType_if>
inline
const char*
osss_port<osss_shared_if<ObjectType_if> >::kind() const
{
  return "osss_port_shared";
}

//----------------------------------------------------------------------------//

template<class ObjectType_if>
inline
void
osss_port<osss_shared_if<ObjectType_if> >::end_of_elaboration()
{
  if (NULL == dynamic_cast<sc_core::sc_interface*>(base_type::operator->()->get_interface()))
  {
    std::cerr << this->kind()
         << ": " << base_type::name()
         << " bound to a shared object not implementing sc_interface"
         << std::endl;
    OSSS_ABORT();
  }

  // check if this port is bound to a shared object
  // which contains an osss_polymorphic
  if (dynamic_cast<osss_polymorphic_if*>(
	base_type::operator->()->get_interface()) == NULL)
  {
    // does not contain osss_polymorphic
    m_isPolymorphic = false;
    // check whether the requested interface is implemented
    // by the object inside the shared object
    if (dynamic_cast<ObjectType_if*>(
	  base_type::operator->()->get_interface()) == NULL)
    {
      std::cerr << this->kind()
	   << ": " << base_type::name()
	   << " bound to a shared object containing an object"
	   << " implementing a wrong interface" << std::endl;
      std::cerr << "osss_port_shared interface type is \""
           << osssi::type_id::from<ObjectType_if>()
           << "\"" << std::endl;
      std::cerr << "osss_shared interface type is \""
           << osssi::type_id(*(base_type::operator->()->get_interface()))
           << "\"" << std::endl;
      OSSS_ABORT();
    }
  }
  else
  {
    // does contain osss_polymorphic
    m_isPolymorphic = true;
    // check whether the requested interface is implemented
    // by the object inside the shared object
    if (dynamic_cast<ObjectType_if*>(dynamic_cast<osss_polymorphic_if*>(
				       base_type::operator->()->
				         get_interface())->
				     get_interface()) == NULL)
    {
      std::cerr << this->kind()
	   << ": " << base_type::name()
	   << " bound to a shared object containing a polymorphic object"
	   << " implementing a wrong interface" << std::endl;
      OSSS_ABORT();
    }
  }
  base_type::operator->()->subscribe( this, m_priority );

  // debug information
  /*
    std::cout << "name: " << base_type::name()
    << " port address: " << this
    << " priority: " << m_priority
    << std::endl;
  */
}

//----------------------------------------------------------------------------//

} // namespace osss

// $Id: osss_port_shared.tpp 3024 2009-03-04 22:55:28Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
