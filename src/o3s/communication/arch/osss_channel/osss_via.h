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

#ifndef OSSS_VIA_HH
#define OSSS_VIA_HH

#include "o3s/communication/arch/osss_channel/osss_channel_fwd.h"
#include "o3s/communication/arch/osss_channel/osss_signal.h"
#include "o3s/communication/arch/osss_channel/osss_port_list_element_base.h"

#include "o3s/utils/noncopyable.h"

namespace osss {

class osss_via_base
{
public:

  virtual osss_via_type type() const = 0;
  virtual std::string guard()  const = 0;
  virtual ~osss_via_base(){}
};

std::ostream&
operator<< ( std::ostream& o, osss_via_base const & vb);

//-----------------------------------------------------------------------------

namespace osssi {

class osss_no_via
  : public osss_via_base
{
public:
  std::string   guard() const;
  osss_via_type type()  const;
};

} // end namespace osssi

//-----------------------------------------------------------------------------

namespace osssi {

class osss_mux
  : public osss_via_base
{
public:
  explicit osss_mux(osss_mux_controller muxController);

  virtual osss_via_type type()  const;
  virtual std::string   guard() const;

private:
  osss_mux_controller m_muxController;
};

} // end namespace osssi

//-----------------------------------------------------------------------------

class osss_via_logic
  : public osss_via_base
  , osssi::noncopyable
{
public:
  osss_via_logic();
  explicit osss_via_logic(osss_port_list_element_base * outPort);
  explicit osss_via_logic(osss_signal_base * signal);

  virtual std::string guard() const;

  osss_port_list_element_base * port();
  osss_signal_base *            osss_signal();

  virtual ~osss_via_logic();

private:
  osss_port_list_element_base * m_port;
  osss_signal_base * m_signal;
};

//-----------------------------------------------------------------------------

namespace osssi {

class osss_or
  : public osss_via_logic
{
public:
  osss_or();

  virtual osss_via_type type() const;
};

//-----------------------------------------------------------------------------

class osss_and
  : public osss_via_logic
{
public:
  explicit osss_and(osss_port_list_element_base * outPort);
  explicit osss_and(osss_signal_base * signal);

  virtual  osss_via_type type() const;
};

//-----------------------------------------------------------------------------

class osss_and_or
  : public osss_via_logic
{
public:
  explicit osss_and_or(osss_port_list_element_base * outPort);
  explicit osss_and_or(osss_signal_base * signal);

  virtual  osss_via_type type() const;
};

} // end namespace osssi
} // namespace osss

#endif
// $Id: osss_via.h 2839 2008-12-04 09:29:59Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
