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

#ifndef OSSS_VIA_LIST_ELEMENT_HH
#define OSSS_VIA_LIST_ELEMENT_HH

#include "o3s/utils/osss_process_handle.h"
#include "o3s/communication/arch/osss_channel/osss_via.h"
#include "o3s/communication/arch/osss_channel/osss_signal_list_element.h"

#include <vector>

namespace osss {

class osss_via_list_element
  : public sc_core::sc_module
{

  SC_HAS_PROCESS(osss_via_list_element);

protected:

  typedef osssi::osss_method_handle      method_handle;
  typedef osss_signal_list_element_base* signal_handle;
  typedef std::vector< signal_handle >   signal_list;

  osss_via_list_element( sc_core::sc_module_name const & viaName
                       , signal_handle selectSignal
                       , signal_handle outputSignal );

  /// make name() protected, to avoid confusion with via_name()
  using sc_module::name;

public:
  // ------------------------------------------------------------- //

  /// deprecated API
  signal_handle getOutputSignal() const
    { return output_signal(); }

  signal_handle getSelectSignal() const
    { return select_signal(); }

  bool addInputSignal( signal_handle inputSignal )
    { return add_input( inputSignal ); }

  signal_list const & getInputSignals() const
    { return input_signals(); }

  //bool addSelectSignal( signal_handle selectSignal )
  //  { return add_select( selectSignal ); }

  // ------------------------------------------------------------- //

  /// equivalent to sc_module::basename()
  std::string via_name() const
    { return sc_core::sc_module::basename(); }

  virtual osss_via_type type() const = 0;
  virtual const char*   kind() const = 0;
  virtual void          process()    = 0;

  signal_handle      output_signal() const;
  signal_handle      select_signal() const;
  signal_list const& input_signals() const;

  bool add_input ( signal_handle );
  bool add_select( signal_handle );


  void print( std::ostream& ) const;

  void trace_all( sc_core::sc_trace_file* tf
                , std::string const & channel_name ) const;

  virtual ~osss_via_list_element();

protected:

  virtual void trace_inputs( sc_core::sc_trace_file* tf
                           , std::string const &     channel_name ) const;

  virtual void trace_select( sc_core::sc_trace_file* tf
                           , std::string const &     channel_name ) const;

  virtual void trace_output( sc_core::sc_trace_file* tf
                           , std::string const &     channel_name ) const;

          void trace_signal( sc_core::sc_trace_file* tf
                           , std::string const &     channel_name
                           , signal_handle           signal
                           , const char * type, int index = -1    ) const;

  virtual void print_inputs( std::ostream& ) const;
  virtual void print_select( std::ostream& ) const;
  virtual void print_output( std::ostream& ) const;

protected:

  signal_handle m_outputSignal;
  signal_handle m_selectSignal;
  signal_list   m_inputSignals;

  method_handle via_proc_handle;

};

//-----------------------------------------------------------------------------

class osss_or_via_list_element
  : public osss_via_list_element
{
public:

  osss_or_via_list_element( sc_core::sc_module_name viaName
                          , signal_handle           outputSignal );

  virtual void          process();
  virtual osss_via_type type() const;
  virtual const char*   kind() const;

};

//-----------------------------------------------------------------------------

// only usable to guard a single input signal
// only usable with an osss_multi_signal as output signal 
// ==> can possibly be deleted !!!
class osss_and_via_list_element
  : public osss_via_list_element
{
public:

  osss_and_via_list_element( sc_core::sc_module_name viaName
                           , signal_handle           selectSignal
                           , signal_handle           outputSignal );

  virtual void          process();
  virtual osss_via_type type() const;
  virtual const char*   kind() const;

};

//-----------------------------------------------------------------------------

class osss_and_or_via_list_element
    : public osss_via_list_element
{
public:

  osss_and_or_via_list_element( sc_core::sc_module_name viaName
                              , signal_handle           outputSignal );


  /// deprecated
  bool addSelectSignal( signal_handle selectSignal )
    { return add_select( selectSignal ); }

  virtual void          process();
  virtual osss_via_type type() const;
  virtual const char*   kind() const;

  bool add_select( signal_handle selectSignal );
  signal_list const& select_signals() const;

protected:

  virtual void print_select( std::ostream& ) const;

  virtual void trace_select( sc_core::sc_trace_file* tf
                           , std::string const & channel_name ) const;

private:

  signal_list m_selectSignals;

};

//-----------------------------------------------------------------------------

class osss_mux_via_list_element
  : public osss_via_list_element
{
public:
  typedef sc_core::sc_signal<unsigned int>* select_signal_handle;

  osss_mux_via_list_element( sc_core::sc_module_name viaName
                           , select_signal_handle    selectSignal
                           , signal_handle           outputSignal );

  virtual void          process();
  virtual osss_via_type type() const;
  virtual const char*   kind() const;

  select_signal_handle  mux_select_signal() const;

protected:

  virtual void print_select( std::ostream& ) const;

  virtual void trace_select( sc_core::sc_trace_file* tf
                           , std::string const & channel_name ) const;

private:

  select_signal_handle m_muxSelectSignal;

};

//------------------------------------------------------------------------------

std::ostream& operator<< (std::ostream& o, 
                          osss_via_list_element& vle);

} // namespace osss

#endif
// $Id: osss_via_list_element.h 2839 2008-12-04 09:29:59Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
