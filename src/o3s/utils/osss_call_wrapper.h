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

#ifndef OSSS_CALL_WRAPPER_H
#define OSSS_CALL_WRAPPER_H

namespace osss {

/// forward declarations for "friend" statements
template< class T > class osss_recon;
template< class T > class osss_context;

/**
 * \brief call wrapper
 *
 * Wrapper around a container class,
 * used to prepare an up-coming access of the inner object.
 *
 * \note This is now essentially a no-op.
 */
template< class ContainerType >
ContainerType& osss_call( ContainerType & container )
{
  return container;
}

namespace osssi {

/** T is the interface class type,
    C is the container type (e.g. osss_context<T>) */
template< class ClassT >
class osss_call_wrapper
{
public:
    typedef ClassT   ContainerClass;
    typedef typename ContainerClass::InterfaceClass  InterfaceClass;

    // We cannot use "friend class ClassT" here, see
    // http://gcc.gnu.org/bugzilla/show_bug.cgi?id=21498
    // The type may not be an "elaborated" type.
    friend class osss::osss_recon< InterfaceClass >;
    friend class osss::osss_context< InterfaceClass >;

    inline InterfaceClass * operator -> ();
    inline ~osss_call_wrapper();

private:
    inline explicit
    osss_call_wrapper( ContainerClass & );

    ContainerClass                      & m_container;
    osssi::osss_user_process_descriptor * m_upd;

};

}
}

#include "osss_call_wrapper.tpp"
#endif // OSSS_CALL_WRAPPER_H

// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
