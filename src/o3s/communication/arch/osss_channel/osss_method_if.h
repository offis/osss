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

#ifndef OSSS_METHOD_IF_HH
#define OSSS_METHOD_IF_HH

namespace osss {

class osss_method_if
{
public:

  /**
   * Prototype for a reset method that needs to be implemented inside
   * the Channel's transactors.
   * In the reset method all output signal ports should be driven with
   * proper initialisation values.
   *
   * This reset method needs to be called from outside of the Channel.
   * That is the module containing the port that makes use of the
   * Channel's transactor.
   * When using SC_CTHREADs the call of the reset method should be used as
   * follows:
   *
   * void my_cthread()
   * {
   *   // reset part
   *   port_to_channel->reset();
   *   [... other module specific reset code]
   *   
   *   while(true)
   *   {
   *     [...]
   *     // communicate through the channel
   *     port_to_channel->read(...)/write(...);     
   *     [...]
   *   }
   * }
   * 
   */
  virtual void reset() = 0;

  virtual ~osss_method_if() {}

};

} // namespace osss

#endif
// $Id: osss_method_if.h 2923 2008-12-16 10:32:25Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
