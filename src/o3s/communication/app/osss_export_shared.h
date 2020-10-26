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
#ifndef OSSS_EXPORT_SHARED_INCLUDED
#define OSSS_EXPORT_SHARED_INCLUDED

#include "o3s/structural/app/osss_shared_if.h"

namespace osss {

//=============================================================================
//  CLASS : osss_export
//
//  Generic export class for other export classes. This
//  class provides a binding point for access to an interface.
//=============================================================================
template<class ObjectType_if>
class osss_export<osss_shared_if<ObjectType_if> > 
  : public sc_core::sc_export<osss_shared_if<> >
{
  typedef osss_export<osss_shared_if<ObjectType_if> > this_type;
  typedef sc_core::sc_export<osss_shared_if<> >       base_type;

public: // constructors:
  osss_export() : base_type() 
  { 
    std::cout << "Warning: You are using the default constructor of "
              << "osss_export<osss_shared_if<...> >." << std::endl
              << "For synthesis it is required that the name of this export "
              << "matches its identifier. " << std::endl
              << "Please use osss_export( const char* ) constructor for " 
              << "osss_export "<< name() << " instead."<< std::endl
              << std::endl;
  }

  explicit osss_export( const char* name_ ) : base_type(name_)
  {}

  explicit osss_export( osss_shared_if<>& interface_ ) : base_type()
  { 
    std::cout << "Warning: You are using the osss_export( osss_shared_if<>& ) "
              << "constructor of osss_export<osss_shared_if<...> >." << std::endl
              << "For synthesis it is required that the name of this export "
              << "matches its identifier. " << std::endl
              << "Please use osss_export( const char*, osss_shared_if<>& ) "
              << "constructor for osss_export " << name() << " instead."
              << std::endl << std::endl;

    base_type::m_interface_p = &interface_;
  }

  osss_export( const char* name_, osss_shared_if<>& interface_ ) 
    : base_type(name_)
  {
    base_type::m_interface_p = &interface_;
  }

  explicit osss_export( this_type& child_ ) : base_type()
  {
    base_type::m_interface_p = child_.m_interface_p;
  }

public: // destructor:
  virtual ~osss_export() 
    { }

public: // identification:
  virtual const char* kind() const 
  { 
    return "osss_export_shared"; 
  }

void end_of_elaboration()
{
  // check if this port is bound to a shared object
  // which contains an osss_polymorphic
  if (dynamic_cast<osss_polymorphic_if*>(
        base_type::operator->()->get_interface()) == NULL)
  {
    // check whether the requested interface is implemented
    // by the object inside the shared object
    if (dynamic_cast<ObjectType_if*>(
          base_type::operator->()->get_interface()) == NULL)
    {
      std::cerr << this->kind() 
                << ": " << base_type::name() 
                << " bound to a shared object containing an object"
                << " implementing a wrong interface" << std::endl;
      OSSS_ABORT();
    }
  }
  else
  {
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
}

private: // disabled
  osss_export( const this_type& );
  this_type& operator = ( const this_type& );

};

} // namespace osss

#endif

// $Id: osss_export_shared.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
