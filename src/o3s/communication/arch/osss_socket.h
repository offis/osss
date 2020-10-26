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
// OSSS, experimental feature
// 2005-05-28, Thorsten Schubert, OFFIS

#ifndef OSSS_SOCKET_INCLUDED
#define OSSS_SOCKET_INCLUDED

#include "o3s/datatypes/osss_polymorphic.h"
#include "o3s/communication/arch/osss_shelter.h"

// - Modelling Macros ----------------------------------------------------------

/*!
 * By this macro the user can define an interface which contains ports
 * which in turn are needed by instances of the surrounding class.
 * In order to access these ports a method with the name of the interface
 * is provided.
 *
 * Example:
 * ========
 *
 * class MyClass
 * {
 *   public:
 *
 *    OSSS_REQUIRED(myInterface)
 *    {
 *      sc_in<bool>         portX;
 *      sc_out<sc_uint<8> > portY;
 *    };
 *
 *    void someMethod()
 *    {
 *      if (myInterface().portX.read() == true)
 *        myInterface().portY.write(sc_uint<8>(3));
 *    }
 * };
 *
 * \note Currently, only one interface per class is supported.
 *
 */

#define OSSS_REQUIRED(INTERFACE_NAME) \
  struct OSSS_RequiredInterface; \
  ::osss::osssi::osss_shelter<OSSS_RequiredInterface*> OSSS_currentlyConnectedInterface; \
  OSSS_RequiredInterface& \
  INTERFACE_NAME (){ \
  if (OSSS_currentlyConnectedInterface){\
    return *OSSS_currentlyConnectedInterface;\
  } else {\
    std::cerr << "ERROR: Socket interface \"" << #INTERFACE_NAME \
              << "\" is not connected" << std::endl;\
    OSSS_ABORT();\
  }}\
  struct OSSS_RequiredInterface

// -----------------------------------------------------------------------------

namespace osss {

////////////////////////////////////////////////////////////////////////////////
///
/// \brief This class implements a socket.
///
/// The class osss_socket<T> must be used in conjunction with a class T which
/// has an OSSS_REQUIRED(X) section. The ports given in X are actually located
/// at the socket and have to be bound.
/// Different objects can be plugged into a socket.
///
/// Example:
/// ========
///
/// osss_socket<MyClass>   mySocket;
/// sc_signal<bool>        signalX;
/// sc_signal<sc_uint<8> > signalY;
///
/// mySocket.get_interface().portX(signalX);
/// mySocket.get_interface().portY(signalY);
///
/// MyClass obj1;
/// mySocket = obj1;
///
////////////////////////////////////////////////////////////////////////////////

template<class ContentClass>
class osss_socket : private sc_core::sc_module
{
  public:

    typedef ContentClass ContentClass_t;
    typedef typename ContentClass_t::OSSS_RequiredInterface RequiredInterface_t;

    explicit
    osss_socket( const sc_core::sc_module_name& =
                   sc_core::sc_gen_unique_name( "osss_socket" ) );

    //! \brief Access the underlying object.
    ContentClass_t*
    operator -> ();

    //! \brief Convert to the content class.
    operator ContentClass_t& ();

    //! \brief Plug another object into this socket.
    void
    operator = ( ContentClass_t const& source);

    //! \brief Access the OSSS_REQUIRED interface.
    RequiredInterface_t&
    get_interface();

  protected:

    virtual
    void
    end_of_elaboration();

  private:

    ContentClass_t       m_contentObject;
    RequiredInterface_t  m_portBindings;

};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief This class implements a socket for polymorphic/shared objects.
///
/// \note Partial template specialisation.
///
////////////////////////////////////////////////////////////////////////////////

template<class ContentClass, template<class> class wrappingClass >
class osss_socket<wrappingClass<ContentClass> > : private sc_core::sc_module
{
  public:

    typedef ContentClass ContentClass_t;
    typedef typename ContentClass_t::OSSS_RequiredInterface RequiredInterface_t;

    explicit
    osss_socket( const sc_core::sc_module_name& =
                   sc_core::sc_gen_unique_name( "osss_socket" ) );

    //! \brief Access the inner (wrapped) object
    //!        (necessary for polymorphic objects).
    ContentClass*
    operator -> ();

    //! \brief Access the whole plugged object.
    wrappingClass<ContentClass>& plugged_object();

    //! \brief Plug another object into this socket.
    void
    operator = ( wrappingClass<ContentClass> const& source);

    //! \brief Access the OSSS_REQUIRED interface.
    RequiredInterface_t&
    get_interface();

  protected:

    virtual
    void
    end_of_elaboration();

  private:

    wrappingClass<ContentClass>  m_contentObject;
    RequiredInterface_t          m_portBindings;
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief This class implements a socket for a shared object containing a
///        polymorphic object.
///
/// \note Partial template specialisation.
///
////////////////////////////////////////////////////////////////////////////////

template<class ContentClass >
class osss_socket<osss_shared<osss_polymorphic<ContentClass> > >
  : private sc_core::sc_module
{
  public:

    typedef ContentClass ContentClass_t;
    typedef typename ContentClass_t::OSSS_RequiredInterface RequiredInterface_t;

    explicit
    osss_socket( const sc_core::sc_module_name& =
                   sc_core::sc_gen_unique_name( "osss_socket" ) );

    //! \brief Access the whole plugged object.
    osss_shared<osss_polymorphic<ContentClass> >& plugged_object();

    //! \brief Plug another object into this socket.
    template <class SourceType>
    osss_socket<osss_shared<osss_polymorphic<ContentClass> > > &
    operator = ( const SourceType & source);

    //! \brief Access the OSSS_REQUIRED interface.
    RequiredInterface_t&
    get_interface();

  protected:

    virtual
    void
    end_of_elaboration();

  private:

    osss_shared<osss_polymorphic<ContentClass> > m_contentObject;
    RequiredInterface_t                          m_portBindings;
};

} // namespace osss

#include "o3s/communication/arch/osss_socket.tpp"

#endif
// $Id: osss_socket.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
