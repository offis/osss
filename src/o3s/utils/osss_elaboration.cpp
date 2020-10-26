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

#include "osss_elaboration.h"
#include "o3s/structural/arch/system/osss_system.h"
#include "o3s/datatypes/osss_vector.h"
#include "osss_elaboration_helper.h"

#include <iostream>

namespace osss {
namespace osssi {

osss_elaboration_helper::osss_elaboration_helper( const osss_system& sys )
  : m_system( &sys ),
    m_externalPorts( sys.m_externalPorts ),
    m_custom_ip_vec( sys.m_custom_ip_vec ),
    m_software_subsystem_vec( sys.m_software_subsystem_vec ),
    m_channel_ip_vec( sys.m_channel_ip_vec )
{}

void
osss_elab_base::elaborate( const osss_system& sys )
{
  // use pointer to reduce coupling with osss_system, etc
  helper_ = new osss_elaboration_helper( sys );
  this->elaborate();
  // clear helper
  delete helper_; helper_ = 0;
}

osss_elab_base::~osss_elab_base() {
  // just to be sure
  delete helper_;
}

} // namespace osssi

osss_elaboration&
osss_elaboration::instance()
{
  // there can only be one ...
  static osss_elaboration* mc_leod = new osss_elaboration;
  return *mc_leod;
}

osss_elaboration::osss_elaboration()
  : done_( false ),
    elabs_()
{}

osss_elaboration::~osss_elaboration()
{
  if( !done_ ) {
    /// TODO: print some warning?
  }
  release();
}

void
osss_elaboration::elaborate( const osss_system& sys)
{
  done_=true;
  if ( ! elabs_.empty() ) {
//    std::cout << "Calling registered elaboration hooks for top-level: "
//              << sys.basename() << std::endl;
    for( iterator_t it = elabs_.begin(); it != elabs_.end(); ++it ) {
      (*it)->elaborate(sys);
    }
  }
}


void
osss_elaboration::append( elab_t& e )
{
  // shall we take a copy?
  // should not be needed here, since all instances
  // are meant to be registered statically
  elabs_.push_back( &e );
}

void
osss_elaboration::release()
{
  // clear up elaborators
  iterator_t it = elabs_.begin();
  while( it != elabs_.end() ) {
    delete *it;
    it = elabs_.erase( it );
  }
}

} // namespace osss

// $Id: osss_elaboration.cpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
