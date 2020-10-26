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
#ifndef RAYTRACE_TASK_INCLUDED
#define RAYTRACE_TASK_INCLUDED

#include <systemc.h>
#include <osss.h>

// sw raytracer libraries
#include "tracer.hh"
#include "objects.hh"
#include "lights.hh"
#include "bmpWriter.hh"

#include "Bitmap_Manager.hh"

Scene   g_Scene;

Sphere  mySphere1 = { VEC3_INIT(0   ,0,0), REAL_NUM(10.0) },
        mySphere2 = { VEC3_INIT(20.0,0.0,-20.0), REAL_NUM(3.0) },
        mySphere3 = { VEC3_INIT(0.0,-30.0,30.0), REAL_NUM(30.0) },
        mySphere4 = { VEC3_INIT(300.0,-100.0,1000.0), REAL_NUM(500.0) },
        mySphere5 = { VEC3_INIT(0.0,0.0,-500), REAL_NUM(200.0) };

Camera  myCamera = { VEC3_INIT(0,0,-100.0),
                     VEC3_INIT(0,0,0.0),
                     VEC3_INIT(0,1.0,0),
                     REAL_NUM(1.0),
                     REAL_NUM(1.0)
                   };

Omni    myOmni = { VEC3_INIT(200.0,0.0,-200.0), Color(0,0,0) },
        myOmni2 = { VEC3_INIT(-100.0,0.0,-200.0), Color(0,0,0) },
        myOmni3 = { VEC3_INIT(-40.0,-80.0,-200.0), Color(0,0,0) };


class Raytrace_Task : public osss_software_task
{
public:

  // connection to the shared object
  osss_port_to_shared< Bitmap_Manager_if > output;

  Raytrace_Task() : 
    osss_software_task(sc_gen_unique_name("Raytrace_Task")), 
    output("output"),
    resultFrame()
  { 
    m_tile = resultFrame.m_size;
  }

  Raytrace_Task(sc_module_name name) : 
    osss_software_task(name), 
    output("output"),
    resultFrame()
  { 
    m_tile = resultFrame.m_size;
  }

  Raytrace_Task(sc_module_name name, const Tile& t) : 
    osss_software_task(name), 
    output("output"),
    resultFrame()
  { 
    m_tile = t;
  }

  // has to override the virtual main()
  void main();

  Tile   m_tile;
  Bitmap resultFrame;
 
};

#include "Raytrace_Task.icc"

#endif
