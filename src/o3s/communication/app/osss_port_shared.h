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

#ifndef OSSS_PORT_SHARED_HH
#define OSSS_PORT_SHARED_HH

#include "o3s/structural/app/osss_shared_if.h"
#include <map>

#if defined(USE_DUST)
#  include <dust/xml_log/tr_model/sc_dust_port.h>
#endif

namespace osss {

////////////////////////////////////////////////////////////////////////////
///
/// \brief This class implements a port to a shared object.
///
/// The shared port has to be used to access the method interface
/// provided by the user defined class inside a shared object.
///
/// \note Partial specialised Template Class.
///       The template parameter specifies the interface of the user defined
///       class which should be accessed by this port
///
////////////////////////////////////////////////////////////////////////////

template <class ObjectType_if>
class osss_port<osss_shared_if<ObjectType_if> >
#if !defined(USE_DUST)
  : public sc_core::sc_port<osss_shared_if<>, 1 >
#else
  : public sc_dust_port<osss_shared_if<>, 1 >
#endif
{
public:

  // typedefs:
  //----------

  typedef osss_shared_if<>                          if_type;
#if !defined(USE_DUST)
  typedef sc_core::sc_port<if_type, 1>              base_type;
#else
  typedef sc_dust_port<if_type, 1>                  base_type;
#endif
  typedef osss_port<osss_shared_if<ObjectType_if> > this_type;

public:

  // constructors:
  //--------------

  osss_port();
  explicit osss_port( const char* name_ );
  explicit osss_port( if_type& interface_ );
  osss_port( const char* name_, if_type& interface_ );
  explicit osss_port( base_type& parent_ );
  osss_port( const char* name_, base_type& parent_ );
  explicit osss_port( this_type& parent_ );
  osss_port( const char* name_, this_type& parent_ );


  virtual ~osss_port();


  // interface access shortcut methods:
  //-----------------------------------

  //!  \brief Sets the priority of this port for accessing the
  //!         shared arbiter through its scheduler.
  //!
  //! This method allows to pass
  //! an argument of type osss_priority to specify a certain priority for
  //! a shared port representing a client of the shared object.
  //! More than one port can have the same priority. The
  //! priority determines the way, the clients are ordered in the internal
  //! buffers of the shared object's arbiter - the higher the priority,
  //! the higher the index in the buffer.
  //! The way the priority is interpreted depends on the scheduler itself.
  void setPriority( osss_priority priority );

  //!  \brief Returns the priority of this port for accessing the
  //!         shared arbiter through its scheduler.
  //!
  osss_priority getPriority();

  //! \brief This operator must be used for calling methods on a shared object.
  //!
  //! More precisely this operator must be used for calling methods
  //! on the user defined class inside the shared object.
  //! Only methods specified in the interface of the user defined class
  //! (this interface has to be specified by the template parameter of
  //! this port) can be called.
  //! A call to the method of a shared object is always blocking. It returns
  //! after the shared object has been granted permission and the method has
  //! been executed.
  //!
  //! Accessing a shared object through a single osss_port by
  //! more than one process might lead to an inconsistent state of the
  //! shared object. To avoid this, the operator->() checks whether
  //! it is always used by the same process. If not, an error message will
  //! be generated and the simulation is stopped.
  ObjectType_if * operator->();

  //! \brief Returns the arbiter of the shared object this port is bound to.
  osss_arbiter_shared_base * getArbiter();

  //! Overwritten from SystemC.
  //! Specifies the name (osss_port) of this port.
  virtual const char* kind() const;

protected:

  //! A hook in the SystemC kernel which is called when the elaboration
  //! phase is done.
  //! This hook is used to check whether we are accessing a valid interface
  //! of the user defined class inside the shared object.
  //! Additionally the port is registered at the shared object.
  virtual void end_of_elaboration();

  bool                     m_firstAccess;
  osssi::OSSS_Process_ID_t m_proc_id;
  osss_priority            m_priority;
  bool                     m_isPolymorphic;

private:

  // disabled copy-constructor
  osss_port( const this_type& )
  {
    std::cerr << "ERROR! " << __func__ << "() may not be used" << std::endl;
  }

};

} // namespace osss

#include "o3s/communication/app/osss_port_shared.tpp"

#endif
// $Id: osss_port_shared.h 3024 2009-03-04 22:55:28Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
