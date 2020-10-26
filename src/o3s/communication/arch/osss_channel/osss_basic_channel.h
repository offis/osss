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

#ifndef OSSS_BASIC_CHANNEL_H_INCLUDED
#define OSSS_BASIC_CHANNEL_H_INCLUDED

#include "o3s/utils/osss_common.h"
#include "o3s/structural/arch/osss_clock_reset_if.h"

#include <vector>

namespace osss {

class osss_signal_list_element_base;
class osss_port_list_element_base;
class osss_via_list_element;
class osss_basic_channel;

namespace osssi {
class osss_object_registry;
}

template<class InterfaceType> struct getTransactorType;
#if 0
{
  typedef void* return_t;
};
#endif

namespace osssi {

std::ostream&
print_channel_graph( std::ostream&, osss_basic_channel const & );

}

//------------------------------------------------------------------------------
// osss_channel_if
//------------------------------------------------------------------------------

template<class IF = sc_core::sc_interface>
class osss_channel_if;

template<>
class osss_channel_if<sc_core::sc_interface>
  : public sc_core::sc_interface
{
public:

  virtual osssi::osss_object_registry* getObjectRegistry() = 0;
  virtual const char* kind() const = 0;

};

//------------------------------------------------------------------------------
// osss_basic_channel
//------------------------------------------------------------------------------

class osss_signal_if;

class osss_basic_channel
  : public virtual osss_channel_if<>
  , public osssi::osss_clock_reset_if
{
public:

  template<class InterfaceType, int N> friend class osss_port;
  template<class InterfaceType> friend class osss_channel_port;
  template<class InterfaceType> friend class osss_export; 
  friend class osss_signal_if;
  friend class osss_controller_if;
  friend class osss_arbiter_if;
  friend class osss_address_decoder_if;

  friend std::ostream&
    operator<<( std::ostream& , osss_basic_channel & );

  friend std::ostream&
    osssi::print_channel_graph( std::ostream&
                              , osss_basic_channel const & );

  template <class InterfaceType> static InterfaceType* create();

#if 0
  void clock_port(const sc_core::sc_signal_in_if<bool>& in_if);
  void clock_port(sc_core::sc_port<sc_core::sc_signal_in_if<bool>, 1>& in_port);
  void clock_port(sc_core::sc_port<sc_core::sc_signal_inout_if<bool>, 1>& inout_port);

  void reset_port(const sc_core::sc_signal_in_if<bool>& in_if);
  void reset_port(sc_core::sc_port<sc_core::sc_signal_in_if<bool>, 1>& in_port);
  void reset_port(sc_core::sc_port<sc_core::sc_signal_inout_if<bool>, 1>& inout_port);
#endif

  explicit
  osss_basic_channel( const std::string& name =
                      sc_core::sc_gen_unique_name("osss_channel") );

  virtual const char* kind() const
    { return "osss_basic_channel"; }

  virtual const char* name() const
    { return m_name.c_str(); }

  virtual ~osss_basic_channel();

  osssi::osss_object_registry* getObjectRegistry();

  virtual void trace(sc_core::sc_trace_file* tf);

  virtual void trace_all(sc_core::sc_trace_file* tf);

  void print_channel_graph(const std::string& file_name);

  std::vector< osss_signal_list_element_base * > *
    getSharedSignalList();

  virtual std::vector< std::vector< osss_signal_list_element_base * > * > *
    getMultiSignalList();

  virtual std::vector< osss_via_list_element * > *
    getMuxViaList();

  virtual std::vector< osss_via_list_element * > *
    getOrViaList();

  virtual std::vector< osss_via_list_element * > *
    getAndOrViaList();

  virtual std::vector< osss_via_list_element * > *
    getAndViaList();

  virtual sc_core::sc_signal<unsigned int> * getArbiterSelect();
  virtual sc_core::sc_signal<unsigned int> * getAddressDecoderSelect();

  virtual unsigned int getNumTransactors();
  void incNumTransactors();

  virtual unsigned int getNumMasters();
  virtual void incNumMasters();

  virtual unsigned int getNumSlaves();
  virtual void incNumSlaves();

  virtual void complete();

  // TODO: osss_channel_reset_module still needed???
  virtual osssi::osss_clock_reset_if* getPorts();

protected:

//  void check_clock_reset_port();

  void register_trace();
  void register_trace_all();

  void print_channel_graph_on(std::ofstream& o) const;
  void print_channel_graph_on() const;

  std::vector< osss_signal_if * > m_transactorList;

  std::vector< osss_signal_list_element_base * > m_sharedSignalList;

  osssi::osss_object_registry *m_object_registry;

  bool m_complete_method_called;

  std::string m_name;

private:
  unsigned int m_numTransactors;

#if 0
  // TODO: still needed?
  bool m_reset_module_clock_bound;
  bool m_reset_module_reset_bound;
#endif

  sc_core::sc_trace_file* m_trace_file;
  sc_core::sc_trace_file* m_trace_all_file;

  std::string m_channel_graph_file_name;

#if 0
  // TODO: check whether this reset module is still needed
  osss_channel_reset_module* m_reset_module;
#endif
};

std::ostream& operator<<( std::ostream& o,
                          osss_basic_channel & ch );

} // namespace osss

#include "o3s/communication/arch/osss_channel/osss_basic_channel.tpp"

// ------------------------------------------------------------------------
// osss_channel_completion_helper
// ------------------------------------------------------------------------

#define OSSS_CREATE_CHANNEL( PointerName, Type, ConstructorArgs)          \
   PointerName = new Type( ConstructorArgs );                             \
   ::osss::osssi::osss_channel_completion_helper                          \
      OSSS_IMPL_CONCAT_( osss_impl_channel_completion_helper_,            \
         OSSS_IMPL_CONCAT_( PointerName , __LINE__ ) )                    \
            ( PointerName )

namespace osss {
namespace osssi {

class osss_channel_completion_helper
{
public:
  explicit
  osss_channel_completion_helper( osss_basic_channel* channel )
    : m_channel_ptr(channel)
  {}
private:
  osss_basic_channel* m_channel_ptr;
};

} // namespace osssi
} // namespace osss

#endif
// $Id: osss_basic_channel.h 2842 2008-12-04 09:30:13Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
