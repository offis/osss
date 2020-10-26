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
#ifndef OSSS_FIFO_H_INCLUDED_
#define OSSS_FIFO_H_INCLUDED_

#include "o3s/structural/app/osss_shared.h"
//#include "o3s/algorithms/osss_no_scheduler.h"

#include "osss_fifo_if.h"
#include "osss_fifo_buf.h"

namespace osss {
OSSS_SYNTHESISABLE_BEGIN_

/* -------------------------------------------------------------------- */

template< typename T
        , size_t Size
        // TODO: add support for omitted scheduler
        // , typename SchedulerType = osss_no_scheduler
        >
class osss_shared_fifo
  : public osss_shared< osss_fifo_buf< T, Size >
                      //, SchedulerType 
                      >
{
  typedef osss_fifo_buf< T, Size >                      if_type;
  typedef osss_shared< if_type /* , SchedulerType */ >  base_type;

public:

  osss_shared_fifo( sc_core::sc_module_name name
                      = sc_core::sc_gen_unique_name( "osss_shared_fifo" ) )
    : base_type( name ){}

  const char* kind() const
  {
    return "osss_shared_fifo";
  }
}; // osss_shared_fifo


/* -------------------------------------------------------------------- */

template< typename T >
struct osss_fifo_port
{
  typedef osss_port_to_shared< osss_fifo_if<T> >     type;
  typedef osss_port_to_shared< osss_fifo_get_if<T> > get_type;
  typedef osss_port_to_shared< osss_fifo_put_if<T> > put_type;

#ifndef OSSS_SYNTHESIS_
  typedef osss_port_to_rmi< osss_fifo_if<T> >        rmi_type;
  typedef osss_port_to_rmi< osss_fifo_get_if<T> >    get_rmi_type;
  typedef osss_port_to_rmi< osss_fifo_put_if<T> >    put_rmi_type;
#else
  typedef void rmi_type;
  typedef void get_rmi_type;
  typedef void put_rmi_type;
#endif

private:
  osss_fifo_port();                              // deliberately undefined
  virtual void please_use_nested_typedefs() = 0; // improve error message
  virtual ~osss_fifo_port(){}                    // shut up stupid GCC
};

/* -------------------------------------------------------------------- */

template< typename T >
struct osss_fifo_in
  : private osss_fifo_port<T>
{
  typedef typename osss_fifo_port<T>::get_type     type;
  typedef typename osss_fifo_port<T>::get_rmi_type rmi_type;
};

/* -------------------------------------------------------------------- */

template< typename T >
struct osss_fifo_out
  : private osss_fifo_port<T>
{
  typedef typename osss_fifo_port<T>::put_type     type;
  typedef typename osss_fifo_port<T>::put_rmi_type rmi_type;
};

/* -------------------------------------------------------------------- */

OSSS_SYNTHESISABLE_END_

OSSS_SYNTHESISABLE_EXPORT_(osss_shared_fifo );
OSSS_SYNTHESISABLE_EXPORT_(osss_fifo_port);
OSSS_SYNTHESISABLE_EXPORT_(osss_fifo_in);
OSSS_SYNTHESISABLE_EXPORT_(osss_fifo_out);

} // namespace osss

#endif // OSSS_FIFO_H_INCLUDED_
// :flavour: (osss,H) (osss_recon,h) (osss_full,h)
// // Taf!
