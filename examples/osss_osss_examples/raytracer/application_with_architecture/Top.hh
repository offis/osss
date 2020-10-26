/*------------------------------------------------------------------------------
 Copyright (c) 2005 Kuratorium OFFIS e.V., Oldenburg, Germany
  
 Name of the Library: OSSS (A library for synthesisable system level models 
                            in SystemC) 

 Created for Project: ICODES (http://icodes.offis.de)

 Created by: See AUTHORS (file)

 
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Contact:
 Kuratorium OFFIS e.V. (http://offis.de)
 Escherweg 2
 D-26121 Oldenburg
 PHONE: *49 (441) 9722-0
 FAX: *49 (441) 9722-102
 -----------------------------------------------------------------------------*/
#ifndef TOP_INCLUDED
#define TOP_INCLUDED

#include <systemc.h>

#define OSSS_GREEN // Virtual Target Architecture Layer Model

#include "Raytrace_Task.hh"
#include "Monitor_Task.hh"

#include "Bitmap_Manager.hh"

class Top : 
#ifdef OSSS_INCLUDE_XILINX
  public xilinx_system
#else
  public osss_system
#endif
{
public:

  sc_in<bool>   pi_bClk;
  sc_in<bool>   pi_bReset;
  
  Raytrace_Task *m_tracer0;
  Tile           tile0;

  Raytrace_Task *m_tracer1;
  Tile           tile1;

  typedef 
  osss_rmi_channel<xilinx_opb_channel<false, false> > processor_bus_type;
  
  processor_bus_type* m_processor_bus;
  
  osss_object_socket<
    osss_shared<Bitmap_Manager<2>, 
                osss_modified_round_robin > >* m_bitmap_manager;

  Monitor_Task *m_monitor;

  osss_processor *m_processor0;
  osss_processor *m_processor1;
  osss_processor *m_processor2;
  
  Top(sc_core::sc_module_name name) :
#ifdef OSSS_INCLUDE_XILINX
    xilinx_system(name)
#else
    osss_system(name)
#endif
  {
    //--------------------------------------------
    // RMI OSSS-Channel
    //--------------------------------------------
    m_processor_bus = new processor_bus_type("m_processor_bus");
    m_processor_bus->clock_port(pi_bClk);
    m_processor_bus->reset_port(pi_bReset);

    //--------------------------------------------
    // Shared Object
    //--------------------------------------------
    m_bitmap_manager = 
      new osss_object_socket<osss_shared<Bitmap_Manager<2>, 
                             osss_modified_round_robin > >("m_bitmap_manager");
    m_bitmap_manager->clock_port(pi_bClk);
    m_bitmap_manager->reset_port(pi_bReset);
    m_bitmap_manager->bind(*m_processor_bus);

    //--------------------------------------------
    // Software Tasks
    //--------------------------------------------
    tile0.x_min = 0;
    tile0.y_min = 0;
    tile0.x_max = BITMAP_SIZE_X-1;
    tile0.y_max = 99; //BITMAP_SIZE_Y-1;
    m_tracer0 = new Raytrace_Task("m_tracer0", tile0);
    m_tracer0->output(*m_bitmap_manager);

    tile1.x_min = 0;
    tile1.y_min = 100; //0;
    tile1.x_max = BITMAP_SIZE_X-1;
    tile1.y_max = BITMAP_SIZE_Y-1;
    m_tracer1 = new Raytrace_Task("m_tracer1", tile1);
    m_tracer1->output(*m_bitmap_manager);
    
    m_monitor = new Monitor_Task("m_monitor");
    m_monitor->input(*m_bitmap_manager);

    //--------------------------------------------
    // Processors
    //--------------------------------------------
#ifdef OSSS_INCLUDE_XILINX
    m_processor0 = new xilinx_microblaze("m_processor0");
#else
    m_processor0 = new osss_processor("m_processor0");
#endif
    m_processor0->clock_port(pi_bClk);
    m_processor0->reset_port(pi_bReset);
    m_processor0->rmi_client_port(*m_processor_bus);
    m_processor0->add_sw_task(m_tracer0);

#ifdef OSSS_INCLUDE_XILINX
    m_processor1 = new xilinx_microblaze("m_processor1");
#else
    m_processor1 = new osss_processor("m_processor1"); 
#endif
    m_processor1->clock_port(pi_bClk);
    m_processor1->reset_port(pi_bReset);
    m_processor1->rmi_client_port(*m_processor_bus);
    m_processor1->add_sw_task(m_tracer1);

#ifdef OSSS_INCLUDE_XILINX
    m_processor2 = new xilinx_microblaze("m_processor2");
#else
    m_processor2 = new osss_processor("m_processor2");
#endif
    m_processor2->clock_port(pi_bClk);
    m_processor2->reset_port(pi_bReset);
    m_processor2->rmi_client_port(*m_processor_bus);
    m_processor2->add_sw_task(m_monitor);
  }
  
};

#endif
