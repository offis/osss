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

#ifdef OSSS_GREEN // this file is only required on architecture layer

#ifndef OSSS_FIFO_STUB_INCLUDED_H_
#define OSSS_FIFO_STUB_INCLUDED_H_

#include "o3s/communication/arch/osss_rmi/osss_object_stub.h"
#include "o3s/communication/arch/osss_rmi/osss_rmi_if.h"
#include "o3s/communication/arch/osss_rmi/osss_port_to_rmi.h"
#include "osss_fifo_if.h"

//namespace osss {

template<typename T>
struct osss_rmi_if< osss::osss_fifo_put_if<T> >
  : osss::osss_fifo_put_if<T>
{
  typedef osss::osss_fifo_put_if<T>   if_type;
  typedef typename if_type::item_type item_type;

  OSSS_OBJECT_STUB_CTOR(if_type);

  // Store an item in the FIFO if it is not full
  OSSS_METHOD_VOID_STUB(put, OSSS_PARAMS(1, item_type, item));

  // Check whether the FIFO is full
  OSSS_METHOD_STUB(bool, is_full, OSSS_PARAMS(0));
};

template<typename T>
struct osss_rmi_if< osss::osss_fifo_get_if<T> >
  : osss::osss_fifo_get_if<T>
{
  typedef osss::osss_fifo_get_if<T>   if_type;
  typedef typename if_type::item_type item_type;

  OSSS_OBJECT_STUB_CTOR(if_type);

  // Get the head item if the FIFO is not empty
  OSSS_METHOD_STUB(item_type, get, OSSS_PARAMS(0));

  // Check whether the FIFO is empty
  OSSS_METHOD_STUB(bool, is_empty, OSSS_PARAMS(0));

};

template<typename T>
struct osss_rmi_if< osss::osss_fifo_if<T> >
  : osss_rmi_if< osss::osss_fifo_put_if<T> >
  , osss_rmi_if< osss::osss_fifo_get_if<T> >
{
  typedef osss::osss_fifo_get_if<T>         if_type;
  typedef typename if_type::item_type item_type;

  OSSS_OBJECT_STUB_CTOR(if_type);

};

// } // namespace osss

#endif // OSSS_FIFO_STUB_INCLUDED_H_

#endif // OSSS_GREEN

// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
// Taf!
