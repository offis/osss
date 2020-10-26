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

#ifndef O3S_UTILS_OSSS_START_H_INCLUDED
#define O3S_UTILS_OSSS_START_H_INCLUDED

#define OSSS_GEN_ELABORATION_CODE \
  if (!sc_core::sc_start_of_simulation_invoked()) \
    sc_core::sc_start(sc_core::SC_ZERO_TIME);

namespace osss {

  /**
   * A replacement for sc_start that can be used to trigger the elaboration
   * only.
   * This can either be done by setting the global OSSS_ELABORATE_ONLY define
   * or by using the elaborate_only parameter.
   * \param elaborate_only controls whether elaboration only or elaboration
   *        with simulation is performed
   *
   * \see OSSS_ELABORATE_ONLY
   */
  inline
  void osss_start(sc_core::sc_clock &clock,
                  const sc_core::sc_time &duration,
                  bool elaborate_only = false)
  {
#ifdef OSSS_ELABORATE_ONLY
    OSSS_GEN_ELABORATION_CODE
#else
    if (elaborate_only) 
    {
      OSSS_GEN_ELABORATION_CODE
    }
    else
      sc_core::sc_start(clock, duration);
#endif
  }


  /**
   * A replacement for sc_start that can be used to trigger the elaboration
   * only.
   * This can either be done by setting the global OSSS_ELABORATE_ONLY define
   * or by using the elaborate_only parameter.
   * \param elaborate_only controls whether elaboration only or elaboration
   *        with simulation is performed
   *
   * \see OSSS_ELABORATE_ONLY
   */
  inline
  void osss_start(sc_core::sc_clock &clock,
                  double v,
                  sc_core::sc_time_unit tu,
                  bool elaborate_only = false)
  {
#ifdef OSSS_ELABORATE_ONLY
    OSSS_GEN_ELABORATION_CODE
#else
    if (elaborate_only)
    {
      OSSS_GEN_ELABORATION_CODE
    }
    else
      sc_core::sc_start(clock, v, tu);
#endif
  }


  /**
   * A replacement for sc_start that can be used to trigger the elaboration
   * only.
   * This can either be done by setting the global OSSS_ELABORATE_ONLY define
   * or by using the elaborate_only parameter.
   * \param elaborate_only controls whether elaboration only or elaboration
   *        with simulation is performed
   *
   * \warning This function is deprecated and should not be used anymore!
   *
   * \see OSSS_ELABORATE_ONLY
   */
  inline
  void osss_start(sc_core::sc_clock &clock,
                  double duration = -1,
                  bool elaborate_only = false)
  {
#ifdef OSSS_ELABORATE_ONLY
    OSSS_GEN_ELABORATION_CODE
#else
    if (elaborate_only)
    {
      OSSS_GEN_ELABORATION_CODE
    }
    else
      sc_core::sc_start(clock, duration);
#endif
  }
  
  
  /**
   * A replacement for sc_start that can be used to trigger the elaboration
   * only.
   * This can either be done by setting the global OSSS_ELABORATE_ONLY define
   * or by using the elaborate_only parameter.
   * \param elaborate_only controls whether elaboration only or elaboration
   *        with simulation is performed
   *
   * \see OSSS_ELABORATE_ONLY
   */
  inline
  void osss_start(bool elaborate_only = false)
  {
#ifdef OSSS_ELABORATE_ONLY
    OSSS_GEN_ELABORATION_CODE
#else
    if (elaborate_only)
    {
      OSSS_GEN_ELABORATION_CODE
    }
    else
      sc_core::sc_start();
#endif
  }


  /**
   * A replacement for sc_start that can be used to trigger the elaboration
   * only.
   * This can either be done by setting the global OSSS_ELABORATE_ONLY define
   * or by using the elaborate_only parameter.
   * \param elaborate_only controls whether elaboration only or elaboration
   *        with simulation is performed
   *
   * \see OSSS_ELABORATE_ONLY
   */
  inline
  void osss_start(const sc_core::sc_time &duration,
                  bool elaborate_only = false)
  {
#ifdef OSSS_ELABORATE_ONLY
    OSSS_GEN_ELABORATION_CODE
#else
    if (elaborate_only)
    {
      OSSS_GEN_ELABORATION_CODE
    }
    else
      sc_core::sc_start(duration);
#endif
  }


  /**
   * A replacement for sc_start that can be used to trigger the elaboration
   * only.
   * This can either be done by setting the global OSSS_ELABORATE_ONLY define
   * or by using the elaborate_only parameter.
   * \param elaborate_only controls whether elaboration only or elaboration
   *        with simulation is performed
   *
   * \warning This function is deprecated and should not be used anymore!
   *
   * \see OSSS_ELABORATE_ONLY
   */
  inline
  void osss_start(double duration,
                  bool elaborate_only = false)
  {
#ifdef OSSS_ELABORATE_ONLY
    OSSS_GEN_ELABORATION_CODE
#else
    if (elaborate_only)
    {
      OSSS_GEN_ELABORATION_CODE
    }
    else
      sc_core::sc_start(duration);
#endif
  }


  /**
   * A replacement for sc_start that can be used to trigger the elaboration
   * only.
   * This can either be done by setting the global OSSS_ELABORATE_ONLY define
   * or by using the elaborate_only parameter.
   * \param elaborate_only controls whether elaboration only or elaboration
   *        with simulation is performed
   *
   * \see OSSS_ELABORATE_ONLY
   */
  inline
  void osss_start(double duration,
                  sc_core::sc_time_unit time_unit,
                  bool elaborate_only = false)
  {
#ifdef OSSS_ELABORATE_ONLY
    OSSS_GEN_ELABORATION_CODE
#else
    if (elaborate_only)
    {
      OSSS_GEN_ELABORATION_CODE
    }
    else
      sc_core::sc_start(duration, time_unit);
#endif
  }

}

#undef OSSS_GEN_ELABORATION_CODE

#endif // O3S_UTILS_OSSS_START_H_INCLUDED
// $Id: osss_start.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
