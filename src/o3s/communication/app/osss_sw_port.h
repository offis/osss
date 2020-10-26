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

#ifndef OSSS_SW_PORT_HH
#define OSSS_SW_PORT_HH

#include "o3s/communication/app/osss_sw_port_if.h"

namespace osss {

/****************************************************************************
CLASS
      osss_sw_port


IMPORTANT NOTES
      

****************************************************************************/

template <class ObjectType_if>
class osss_sw_port
  : public sc_core::sc_port<osss_sw_shared_if<>, 1 >,
    public osss_sw_port_if
{
public:

  // typedefs
  
  typedef osss_sw_shared_if<>          if_type;
  typedef sc_core::sc_port<if_type, 1> base_type;
  typedef osss_sw_port<ObjectType_if>  this_type;

public:

  // constructors

  osss_sw_port()
    : base_type(), 
      m_priority( 0 )
    {}

  explicit osss_sw_port( const char* name_ )
    : base_type(), 
      m_priority( 0 )
    {}

  explicit osss_sw_port( if_type& interface_ )
    : base_type(), 
      m_priority( 0 )
    {}

  osss_sw_port( const char* name_, if_type& interface_ )
    : base_type(), 
      m_priority( 0 )
    {}

  explicit osss_sw_port( base_type& parent_ )
    : base_type(), 
      m_priority( 0 )
    {}

  osss_sw_port( const char* name_, base_type& parent_ )
    : base_type(), 
      m_priority( 0 )
    {}

  explicit osss_sw_port( this_type& parent_ )
    : base_type(), 
       m_priority( 0 )
    {}

  osss_sw_port( const char* name_, this_type& parent_ )
    : base_type(), 
      m_priority( 0 )
    {}

  // destructor
  virtual ~osss_sw_port()
    {}


  // interface access shortcut methods:
  //-----------------------------------

  void setPriority( osss_priority priority )
  {
    m_priority = priority;
  }

  osss_priority getPriority()
  {
    return m_priority;
  }

  void setPeriod(const sc_core::sc_time period)
  {
    base_type::operator->()->setPeriod(period);
  }

  ObjectType_if * operator->()
  {
    return dynamic_cast<ObjectType_if*>(
      base_type::operator->()->get_interface());
  }

  bool pendingRequest()
  {
    return base_type::operator->()->pendingRequest();
  }

  void grantAccess(bool access)
  {
    base_type::operator->()->grantAccess(access);
  }

  virtual const char* kind() const
  { 
    return "osss_sw_port"; 
  }

protected:

  // called when elaboration is done
  /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
  /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

  virtual void end_of_elaboration()
  {
    if (dynamic_cast<ObjectType_if*>(
	  base_type::operator->()->get_interface()) == NULL)
    {
      std::cerr << this->kind() 
                << ": " << base_type::name() 
                << " bound to a software shared object containing an object"
                << " implementing a wrong interface" << std::endl;
      OSSS_ABORT();
    }

    base_type::operator->()->registerSWTask(m_priority);

    // debug information
    /*
    std::cout << "name: " << base_type::name() 
              << " port address: " << this 
              << " priority: " << m_priority 
              << std::endl;
    */
  }

  osss_priority m_priority;

private:

  // disabled copy-constructor
  osss_sw_port( const this_type& );

};

} // namespace osss

#endif
// $Id: osss_sw_port.h 2915 2008-12-15 18:25:28Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
