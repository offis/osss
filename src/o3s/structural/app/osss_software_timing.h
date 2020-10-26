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

#ifndef OSSS_SOFTWARE_TIMING_HH
#define OSSS_SOFTWARE_TIMING_HH

#include "o3s/utils/block_wrapper.h"
#include "o3s/structural/app/osss_software_task.h"
#include "o3s/structural/app/osss_sw_shared_if.h"

namespace osss {

// -----------------------------------------------------------------------------
// OSSS internal class declarations. Not to be used directly in an OSSS model!
// -----------------------------------------------------------------------------

// Helper struct which stores the source location of the user's timing annotations
// (for meaningful error messages).
struct osss_timing_annotation_source_info
{
  osss_timing_annotation_source_info( 
    const char * const fileName,
    unsigned int lineNumber,
    osss_sw_object_if * const sw_obj
  );

  std::ostream& print(std::ostream &os) const;
  
  sc_core::sc_time const     m_timeOfCreation;
  const char * const         m_fileName;
  unsigned int const         m_lineNumber;
  osss_sw_object_if  * const m_sw_obj;
};

// The implementation of the RET as described in D11
class osss_required_time
  : public osss::osssi::block_wrapper
{
public:

  osss_required_time( osss_timing_annotation_source_info const &otasi,
                      sc_core::sc_time                   const &duration);

  ~osss_required_time();

private:
    
  osss_timing_annotation_source_info const  m_otasi;
  sc_core::sc_time                          m_duration;

};

// The implementation of EET as described in D11
class osss_waste_of_time 
  : public osss::osssi::block_wrapper
{
public:
  
  osss_waste_of_time(osss_timing_annotation_source_info const &otasi,
                     sc_core::sc_time                   const &duration);
  ~osss_waste_of_time();

private:
  osss_timing_annotation_source_info const  m_otasi;
  sc_core::sc_time                          m_duration;
};

// =============================================================================
// OSSS-internal method bodies
// =============================================================================

inline
osss_timing_annotation_source_info::
osss_timing_annotation_source_info( const char * const fileName,
                                    unsigned int lineNumber,
                                    osss_sw_object_if * const sw_obj )
  : m_timeOfCreation(sc_core::sc_time_stamp()),
    m_fileName(fileName),
    m_lineNumber(lineNumber),
    m_sw_obj(sw_obj)
{
  if (m_sw_obj == NULL)
  {
    std::cerr << "OSSS internal error: illegal software object during "
              << "the creation of an osss_timing_annotation_source_info. "
              << fileName << ':' << lineNumber << std::endl;
  }
}

inline
std::ostream&
osss_timing_annotation_source_info::print(std::ostream &os) const
{
  os << '[';

  std::string _swObjKind = m_sw_obj->swObjectKind();

  if (_swObjKind == std::string("osss_software_task"))
  {
    osss_software_task* m_task = 
      dynamic_cast<osss_software_task*>(m_sw_obj);

    os << m_task->name();
  }
  else if (_swObjKind == std::string("osss_sw_shared"))
  {
    os << _swObjKind; // <---- to do
  }
  
  os << ", " << m_fileName << ':' << m_lineNumber
     << ", created : "
     << m_timeOfCreation << ']';
  return os;
}

// -----------------------------------------------------------------------------

inline
osss_required_time::
osss_required_time( osss_timing_annotation_source_info const &otasi,
                    sc_core::sc_time                   const &duration )
  : m_otasi(otasi),
    m_duration(duration)
{     
}

inline
osss_required_time::
~osss_required_time() 
{ 
  /*some computation to calculate the deadline and if the deadline 
    is already exceeded */
    sc_core::sc_time now = sc_core::sc_time_stamp(),
    deadline = m_otasi.m_timeOfCreation + m_duration;
  
  /*information about an deadline violation, or otherwise a waste of time*/
  if (now > deadline)
  {
    std::cout << "OSSS_RET violation ";
    m_otasi.print(std::cout);
    std::cout << " : duration: " << m_duration
              << ", deadline: " << deadline
              << ", now: " << now
              << std::endl;
  }
}

// -----------------------------------------------------------------------------

inline
osss_waste_of_time::
    osss_waste_of_time(osss_timing_annotation_source_info const &otasi,
                       sc_core::sc_time                   const &duration)
      :  m_otasi(otasi),
         m_duration(duration)
{}

inline
osss_waste_of_time::
~osss_waste_of_time() 
{ 
  sc_core::sc_time _period = m_otasi.m_sw_obj->getPeriod();
  // this cast might be dangerous !!!
  int _num_clock_cycles = (int)ceil(m_duration/_period); 

  //std::cout << "--------------------->" 
  //     << osssi::osss_process_handle::current()->get_parent()->kind() << std::endl;

  std::string _swObjKind = m_otasi.m_sw_obj->swObjectKind();

  if (_swObjKind == std::string("osss_software_task"))
  {
    //std::cout << "--------------------->" 
    //     << "SW_TASK" << std::endl;

    osss_software_task* m_task = 
      dynamic_cast<osss_software_task*>(m_otasi.m_sw_obj);

    for(int i=0; i<_num_clock_cycles; i++)
    {
      m_task->base_type::wait();
      m_task->handleInterrupts();
    }

  }
  else if (_swObjKind == std::string("osss_sw_shared"))
  {
    //std::cout << "--------------------->" 
    //     << "SW_SHARED" << std::endl;

    sc_core::wait(_num_clock_cycles);
  }
  else
  {
    std::cerr << "OSSS internal error: illegal software object. "
              << m_otasi.m_fileName << ':' 
              << m_otasi.m_lineNumber << std::endl;
  }
}

} // namespace osss

#endif
// $Id: osss_software_timing.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
