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

#ifndef OSSS_TRANSACTOR_ACCESSOR_HH_INCLUDED
#define OSSS_TRANSACTOR_ACCESSOR_HH_INCLUDED

#include "o3s/communication/arch/osss_channel/osss_basic_channel.h"
#include "o3s/communication/arch/osss_rmi/osss_rmi_types.h"
#include "o3s/communication/arch/osss_rmi/osss_rmi_channel_client_if.h"
#include "o3s/communication/arch/osss_rmi/osss_rmi_channel_server_if.h"

namespace osss {

  //--------------------------------------------------------
  // transactor_accessor base
  //--------------------------------------------------------

  class transactor_accessor_base
  {
  public:
    
    transactor_accessor_base() :
      m_channel(NULL),
      m_simple_trace(false),
      m_simple_trace_name(),
      m_trace(false),
      m_trace_file(NULL),
      m_trace_name()
    {} 

    virtual ~transactor_accessor_base() {}

    transactor_accessor_base(const transactor_accessor_base& tab)
    {
      // call operator=(...)
      *this = tab;
    }

    transactor_accessor_base& operator=(const transactor_accessor_base& tab)
    {
      if (this == &tab)
        return *this;

      this->m_channel = tab.m_channel;
      this->m_simple_trace = tab.m_simple_trace;
      this->m_simple_trace_name = tab.m_simple_trace_name;
      this->m_trace = tab.m_trace;
      this->m_trace_file = tab.m_trace_file;
      this->m_trace_name = tab.m_trace_name;

      return *this;
    }
    
    virtual void set_channel(osss_basic_channel* ch) { m_channel = ch; }
    virtual osss_basic_channel* get_channel() const { return m_channel; }

    virtual void trace(std::string name)
    {
      m_simple_trace = true;
      m_simple_trace_name = name;
    }
    virtual bool simple_trace_requested() const { return m_simple_trace; }
    virtual std::string get_simple_trace_name() const 
    { return m_simple_trace_name; }
    
    virtual void trace(sc_core::sc_trace_file* tf, std::string name)
    {
      m_trace = true;
      m_trace_file = tf;
      m_trace_name = name;
    }
    virtual bool trace_requested() const { return m_trace; }
    virtual std::string get_trace_name() const { return m_trace_name; }
    virtual sc_core::sc_trace_file* get_trace_file() const 
    { return m_trace_file; }

  protected:
    osss_basic_channel*           m_channel;
    bool                          m_simple_trace;
    std::string                   m_simple_trace_name;
    bool                          m_trace;
    sc_core::sc_trace_file*       m_trace_file;
    std::string                   m_trace_name;
  };

  //--------------------------------------------------------
  // client_transactor_accessor
  //--------------------------------------------------------

  class client_transactor_accessor : public transactor_accessor_base
  {
  public:
    typedef transactor_accessor_base base_type;
    
    client_transactor_accessor() : 
      transactor_accessor_base(),
      m_prio(0),
      m_transactor(NULL)
    {}

    explicit client_transactor_accessor(const client_transactor_accessor& cta)
    {
      // call operator=(...)
      *this = cta;
    }

    client_transactor_accessor& operator=(const client_transactor_accessor& cta)
    {
      if (this == &cta)
        return *this;
      
      base_type::operator=(cta);
      this->m_prio = cta.m_prio;
      this->m_transactor = cta.m_transactor;

      return *this;
    }

    void set_priority(unsigned int prio) { m_prio = prio; }
    unsigned int get_priority() const { return m_prio; }

    void set_transactor(osss_rmi_channel_client_if* tr) { m_transactor = tr; }
    osss_rmi_channel_client_if* get_transactor() const { return m_transactor; }

  protected:
    unsigned int m_prio;
    osss_rmi_channel_client_if* m_transactor;
  };

  //--------------------------------------------------------
  // server_transactor_accessor
  //--------------------------------------------------------

  class server_transactor_accessor : public transactor_accessor_base
  {
  public:
    typedef transactor_accessor_base base_type;
    
    server_transactor_accessor() : 
      transactor_accessor_base(),
      m_base_addr(0),
      m_high_addr(0),
      m_transactor(NULL)
    {}

    server_transactor_accessor(const server_transactor_accessor& sta)
    {
      // call operator=(...)
      *this = sta;
    }

    server_transactor_accessor& operator=(const server_transactor_accessor& sta)
    {
      if (this == &sta)
        return *this;
      
      base_type::operator=(sta);
      this->m_base_addr = sta.m_base_addr;
      this->m_high_addr = sta.m_high_addr;
      this->m_transactor = sta.m_transactor;

      return *this;
    }

    void set_address_range(unsigned long long base,
                           unsigned long long high)
    { m_base_addr = base; m_high_addr = high; }
    unsigned long long get_base_address() const { return m_base_addr; }
    unsigned long long get_high_address() const { return m_high_addr; }

    void set_transactor(osss_rmi_channel_server_if* tr) { m_transactor = tr; }
    osss_rmi_channel_server_if* get_transactor() const { return m_transactor; }

  protected:
    unsigned long long          m_base_addr, m_high_addr;
    osss_rmi_channel_server_if* m_transactor;
  };

} // namespace osss

#endif // OSSS_TRANSACTOR_ACCESSOR_HH_INCLUDED
// $Id: osss_transactor_accessor.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
