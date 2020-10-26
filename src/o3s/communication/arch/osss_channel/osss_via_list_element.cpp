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

#include "osss_via_list_element.h"

#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/osss_macros.h"
#include "o3s/utils/osss_report.h"
#include "o3s/utils/osss_assert.h"

namespace osss {

osss_via_list_element::
  osss_via_list_element( sc_core::sc_module_name const & viaName
                       , signal_handle selectSignal
                       , signal_handle outputSignal )
  : sc_core::sc_module( viaName )
  , m_outputSignal( outputSignal )
  , m_selectSignal( selectSignal )
{
  OSSS_ASSERT( outputSignal != NULL );

  SC_METHOD( process );
  via_proc_handle = osssi::osss_process_handle::last_created();
}

//-------------------------------------------------------------------------

osss_via_list_element::signal_handle
osss_via_list_element::output_signal() const
{
  return m_outputSignal;
}

osss_via_list_element::signal_handle
osss_via_list_element::select_signal() const
{
  return m_selectSignal;
}

osss_via_list_element::signal_list const &
osss_via_list_element::input_signals() const
{
  return m_inputSignals;
}

bool
osss_via_list_element::add_input( signal_handle inputSignal )
{
  //check if the input signal is of the same type as the output signal
  if ( !m_outputSignal->doTypecheck(inputSignal) )
    return false;

  sensitive << via_proc_handle;
  inputSignal->addToSensitivity(&sensitive);
  m_inputSignals.push_back(inputSignal);

  return true;
}

/// TODO: drop this?
bool
osss_via_list_element:: add_select( signal_handle /* unused */ )
{
  return true;
}

osss_via_list_element::~osss_via_list_element()
{
  /* empty */
}

//-------------------------------------------------------------------------

void
osss_via_list_element::print( std::ostream & os ) const
{
  os << "name: " << this->basename() << std::endl
     << "type: ";
  switch( this->type() )
  {
    case OSSS_NO_VIA :
      os << "OSSS_NO_VIA";
      break;

    case OSSS_MUX_VIA :
      os << "OSSS_MUX_VIA";
      break;

    case OSSS_OR_VIA :
      os << "OSSS_OR_VIA";
      break;

    case OSSS_AND_VIA :
      os << "OSSS_AND_VIA";
      break;

    case OSSS_AND_OR_VIA :
      os << "OSSS_AND_OR_VIA";
      break;
  }
  os << std::endl;

  print_inputs( os );
  print_select( os );
  print_output( os );

}

void
osss_via_list_element::print_inputs( std::ostream & os ) const
{
  os << "input signals: " << std::endl;

  typedef signal_list::size_type size_type;
  size_type size = m_inputSignals.size();

  if( size == 0 ) {
    os << "none" << std::endl;
  } else {
    for( size_type i = 0; i<size; ++i )
    {
      os << "  " << i << ": "
         << "(alias) " << m_inputSignals[i]->getAlias()
         << std::endl;
    }
  }
}

void
osss_via_list_element::print_select( std::ostream & os ) const
{
  os << "select signal: ";

  if( !m_selectSignal ) {
    os << "none" << std::endl;
  } else {
    os << "(alias) " << m_selectSignal->getAlias()
       << std::endl;
  }
}


void
osss_via_list_element::print_output( std::ostream & os ) const
{
  os << "output signal: "
     << "(alias) " << m_outputSignal->getAlias() << std::endl;
}

//-------------------------------------------------------------------------

void
osss_via_list_element::
  trace_all( sc_core::sc_trace_file* tf
           , std::string const & channel_name ) const
{
  trace_inputs( tf, channel_name );
  trace_select( tf, channel_name );
  trace_output( tf, channel_name );
}

void
osss_via_list_element::
  trace_signal( sc_core::sc_trace_file* tf
              , std::string const &     channel_name
              , signal_handle           signal
              , const char *            type
              , int                     index /* = -1 */ ) const
{
  // if no signal given, do nothing
  if( ! signal )
    return;

  // build name
  std::ostringstream ostr;

  ostr << channel_name
       << "_" << signal->getAlias();

  if( index >= 0 ) { // append index, if given
    ostr << "_" << index;
  }

  ostr << "-(" << type << ")->"
       << this->basename();

  // trace signal with given name
  signal->trace( tf, ostr.str() );
}


void
osss_via_list_element::
  trace_inputs( sc_core::sc_trace_file* tf
              , std::string const & channel_name ) const
{
  typedef signal_list::const_iterator iterator;

  int idx = 0;
  for( iterator i = m_inputSignals.begin(), end = m_inputSignals.end();
       i != end; ++i )
  {
    trace_signal( tf, channel_name, *i, "in", idx++ );
  }
}

void
osss_via_list_element::
  trace_select( sc_core::sc_trace_file* tf
              , std::string const & channel_name ) const
{
  trace_signal( tf, channel_name, m_selectSignal, "sel" );
}

void
osss_via_list_element::
  trace_output( sc_core::sc_trace_file* tf
              , std::string const & channel_name ) const
{
  trace_signal( tf, channel_name, m_outputSignal, "out" );
}

//-------------------------------------------------------------------------

osss_or_via_list_element::
  osss_or_via_list_element( sc_core::sc_module_name viaName
                          , signal_handle           outputSignal )
  : osss_via_list_element(viaName, 0, outputSignal)
{}

const char*
osss_or_via_list_element::kind() const
{
  return "osss_or_via_list_element";
}

osss_via_type
osss_or_via_list_element::type() const
{
  return OSSS_OR_VIA;
}

void
osss_or_via_list_element::process()
{
  if (!m_inputSignals.empty())
  {
    /// TODO: Fix signal implementation (drop pointers)
    m_outputSignal->bitwise_or(&m_inputSignals);
  }
}

//-----------------------------------------------------------------------------

// only usable to guard a single input signal
// only usable with an osss_multi_signal as output signal 
// ==> can possibly be deleted !!!

osss_and_via_list_element::
  osss_and_via_list_element( sc_core::sc_module_name viaName
                           , signal_handle           selectSignal
                           , signal_handle           outputSignal )
  : osss_via_list_element( viaName, selectSignal, outputSignal )
{
  OSSS_ASSERT( selectSignal != NULL );

  if( !selectSignal->isBoolean() )
  {
    OSSS_REPORT_ERROR( report::channel_select_signal_invalid )
      % "osss_and_via";
  }
}

const char*
osss_and_via_list_element::kind() const
{
  return "osss_and_via_list_element";
}

osss_via_type
osss_and_via_list_element::type() const
{
  return OSSS_AND_VIA;
}

void
osss_and_via_list_element::process()
{
  if (m_inputSignals.size() == 1)
  {
    if (m_selectSignal->getBoolean())
      m_outputSignal->assign(m_inputSignals[0]);
    /// TODO: else?
    ///       maybe: assign_all_zero?
  }
}

//-----------------------------------------------------------------------------

osss_and_or_via_list_element::
  osss_and_or_via_list_element( sc_core::sc_module_name viaName
                              , signal_handle           outputSignal )
  : osss_via_list_element(viaName, 0, outputSignal)
{}

const char*
osss_and_or_via_list_element::kind() const
{
  return "osss_and_or_via_list_element";
}

osss_via_type
osss_and_or_via_list_element::type() const
{
  return OSSS_AND_OR_VIA;
}

bool
osss_and_or_via_list_element::add_select( signal_handle selectSignal )
{
  OSSS_ASSERT( selectSignal );

  //check if the selectSignal is of type boolean
  if (!selectSignal->isBoolean())
    return false;

  sensitive << via_proc_handle;
  selectSignal->addToSensitivity(&sensitive);

  m_selectSignals.push_back(selectSignal);
  return true;
}

osss_via_list_element::signal_list const &
osss_and_or_via_list_element::select_signals() const
{
  return m_selectSignals;
}

void
osss_and_or_via_list_element::process()
{
  /// TODO: assertion here?
  ///       Is the no-op really a good idea?
  if ((!m_inputSignals.empty()) && 
      (m_selectSignals.size() == m_inputSignals.size()))
  {
    signal_list tmp_selectedSignals;

    // find signals which have to be switched to the or matrix
    typedef signal_list::iterator iterator;

    for( iterator sel_iter = m_selectSignals.begin(),
                  inp_iter = m_inputSignals.begin();
         sel_iter != m_selectSignals.end();
         ++sel_iter, ++inp_iter )
    {
      if( (*sel_iter)->getBoolean() )
        tmp_selectedSignals.push_back( *inp_iter );
    }

    if (!tmp_selectedSignals.empty())
      m_outputSignal->bitwise_or(&tmp_selectedSignals);
    else
      m_outputSignal->assign_all_zero();
  }
}

void
osss_and_or_via_list_element::print_select( std::ostream & os ) const
{
  typedef signal_list::size_type size_type;
  size_type size = m_selectSignals.size();

  os << "select signal: ";

  if ( m_selectSignals.empty() ) {
    os << "none" << std::endl;
  } else {
    os << std::endl;

    for( size_type i = 0; i<size; ++i )
    {
      os << "  " << i << ": "
         << "(alias) " << m_selectSignals[i]->getAlias()
         << std::endl;
    }
  }
}


void
osss_and_or_via_list_element::
  trace_select( sc_core::sc_trace_file* tf
              , std::string const & channel_name ) const
{
  typedef signal_list::const_iterator iterator;

  int idx = 0;
  for( iterator i = m_selectSignals.begin(), end = m_selectSignals.end();
       i != end; ++i )
  {
    trace_signal( tf, channel_name, *i, "sel", idx++ );
  }
}

//-----------------------------------------------------------------------------

osss_mux_via_list_element::
  osss_mux_via_list_element( sc_core::sc_module_name viaName
                           , select_signal_handle    selectSignal
                           , signal_handle           outputSignal )
  : osss_via_list_element( viaName, 0, outputSignal )
  , m_muxSelectSignal( selectSignal )
{
  OSSS_ASSERT( selectSignal != NULL );

  sensitive << via_proc_handle;
  sensitive << *selectSignal;
}

const char*
osss_mux_via_list_element::kind() const
{
  return "osss_mux_via_list_element";
}

osss_via_type
osss_mux_via_list_element::type() const
{
  return OSSS_MUX_VIA;
}

void
osss_mux_via_list_element::process()
{
  //std::cout << "mux process called" << std::endl;
  if (//(m_muxSelectSignal->read() >= 0) && 
      (m_muxSelectSignal->read() < m_inputSignals.size()))
  {
    /*
    std::cout << "value changed at " << m_name << std::endl;
    if (m_outputSignal->assign(m_inputSignals[m_muxSelectSignal->read()]))
      std::cout << "value written through mux "
                << m_name << std::endl << std::endl; 
    */
    m_outputSignal->assign(m_inputSignals[m_muxSelectSignal->read()]);
  }
}

osss_mux_via_list_element::select_signal_handle
osss_mux_via_list_element::mux_select_signal() const
{
  return m_muxSelectSignal;
}

void
osss_mux_via_list_element::print_select( std::ostream& os ) const
{
  os << "select signal: ";

  if( m_muxSelectSignal != 0 ) {
    os << " (kind) " << m_muxSelectSignal->kind()
       << std::endl;
  } else {
    os << "none" << std::endl;
  }
}

void
osss_mux_via_list_element::
  trace_select( sc_core::sc_trace_file* tf
              , std::string const & channel_name ) const
{
  if (m_muxSelectSignal != 0)
  {
    std::ostringstream ostr;
    ostr << channel_name << "_" 
         << "mux_select" << "-(sel)->" << this->basename();
    sc_trace(tf, *m_muxSelectSignal, ostr.str());
  }
}

//-------------------------------------------------------------------------

std::ostream& operator<< ( std::ostream& os
                         , osss_via_list_element& vle )
{
  vle.print( os );
  return os;
}

//-------------------------------------------------------------------------

} // namespace osss

// $Id: osss_via_list_element.cpp 2834 2008-12-04 09:29:26Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
