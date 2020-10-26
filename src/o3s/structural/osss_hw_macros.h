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
#ifndef OSSS_HW_MACROS_INCLUDED
#define OSSS_HW_MACROS_INCLUDED

//==============================================================================
// Declare base classes for a class (indirectly) derived from osss_object
//==============================================================================

#define OSSS_BASE_CLASS( Class )                                           \
{                                                                          \
  osss::osssi::osss_class_proxy_interface * _this_type_ =                  \
    osss::osssi::osss_class_proxy_registry::instance().addProxy(this);     \
  osss::osssi::osss_class_proxy_interface * _base_type_ =                  \
    osss::osssi::osss_class_proxy_registry::instance().addProxy< Class >();\
  _this_type_->addBaseClass( _base_type_ );                                \
} /* OSSS_BASE_CLASS( Class ) */

//==============================================================================
// template helpers
//==============================================================================

#define _OSSS_TEMPLATE0

#define _OSSS_TEMPLATE1( p1 ) \
p1

#define _OSSS_TEMPLATE2( p1, p2 ) \
p1, p2

#define _OSSS_TEMPLATE3( p1, p2, p3 ) \
p1, p2, p3

#define _OSSS_TEMPLATE4( p1, p2, p3, p4 ) \
p1, p2, p3, p4

#define _OSSS_TEMPLATE5( p1, p2, p3, p4, p5 ) \
p1, p2, p3, p4, p5

#define _OSSS_TEMPLATE6( p1, p2, p3, p4, p5, p6 ) \
p1, p2, p3, p4, p5, p6

#define _OSSS_TEMPLATE7( p1, p2, p3, p4, p5, p6, p7 ) \
p1, p2, p3, p4, p5, p6, p7

#define _OSSS_TEMPLATE8( p1, p2, p3, p4, p5, p6, p7, p8 ) \
p1, p2, p3, p4, p5, p6, p7, p8

#define _OSSS_TEMPLATE9( p1, p2, p3, p4, p5, p6, p7, p8, p9 ) \
p1, p2, p3, p4, p5, p6, p7, p8, p9

#define _OSSS_TEMPLATE10( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 ) \
p1, p2, p3, p4, p5, p6, p7, p8, p9, p10

#define _OSSS_TEMPLATE11( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11 ) \
p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11

#define _OSSS_TEMPLATE12( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12 ) \
p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12

#define _OSSS_TEMPLATE13( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13 ) \
p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13

#define _OSSS_TEMPLATE14( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14 ) \
p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14

#define _OSSS_TEMPLATE15( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15 ) \
p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15

#define _OSSS_TEMPLATE16( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16 ) \
p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16

#define _OSSS_TEMPLATE17( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17 ) \
p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17

#define _OSSS_TEMPLATE18( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18 ) \
p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18

#define _OSSS_TEMPLATE19( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19 ) \
p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19

#define _OSSS_TEMPLATE20( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20 ) \
p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20

#define OSSS_TEMPLATE(N, ...) \
  _OSSS_TEMPLATE##N( __VA_ARGS__ )

#endif
// $Id: osss_hw_macros.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(all,h)
