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

/*******************************************************************************
Description:
------------

A simple test case for the osss_software_timing.h

Two tasks are using the OSSS_EET and OSSS_RET macros (without any further useful
computation).

There is no notion of concurrency, i.e. it is assumed that each task has its
own CPU. Furthermore there are no CPU-objects on which the tasks are mapped.
(They simply run on their own)

The tasks can be enabled via an integer command line paramter (see sc_main).

*******************************************************************************/

#define OSSS_BLUE

#include <osss.h>

#include "Testbench.h"

#include <iostream>
#include <sstream>

// A kind of assertion which checks that the given time is "now"
// This macro does not have a meaning for the model -- 
// it just checks the correct behaviour of the timing annotations.
#define EXPECTED_TIME(time) \
  {if ((time) != sc_time_stamp()) \
     std::cerr << "Erroneous simulation behaviour @ " \
               <<  sc_time_stamp() << ": " \
               << name() \
               << "(line: " << __LINE__ << ")" \
               << ", expected time : " << (time) << std::endl; }
                               

OSSS_SOFTWARE_TASK(task1)
{

  OSSS_SOFTWARE_CTOR(task1)
  {  
  }

  void methodX()
  {
    PRINT_MSG("Beginning methodX");   
    OSSS_EET(sc_time(3.0, SC_US)) {
      /* do something else */
    }
    PRINT_MSG("Completed methodX");   
  }
  
  void main()
  {
    OSSS_EET(sc_time(2.0, SC_US)) {
      /* do something */
      PRINT_MSG("Doing something");
    }
    PRINT_MSG("Communication with some other module");

    EXPECTED_TIME(sc_time(110.0, SC_NS)+ //reset time
		  sc_time(2.0, SC_US));

    // Note: The execution time of the initialisation of i and for checking the 
    //       condition (at least the first time) is neglected here
    for (int i=0; i<3; ++i)
    {
      OSSS_EET(sc_time(5.0, SC_US)) {
        PRINT_MSG("For loop, iteration " << i);                  

        if (i%2 == 0)
        {
          // will be called for i==0 and i==2
          methodX();
        }
      }
      
      EXPECTED_TIME(sc_time(110.0, SC_NS)+ //reset time
	            sc_time(2.0, SC_US)+
                    sc_time(5.0, SC_US)*static_cast<double>(i+1)+
                    sc_time(3.0, SC_US)*((i==2) ? 2.0 : 1.0)
                   );
    }
  }  
};

OSSS_SOFTWARE_TASK(task2)
{

  OSSS_SW_CTOR(task2)
  {  
  }
  
  void methodY()
  {
    OSSS_RET(sc_time(6.0, SC_US)) {
    
      OSSS_EET(sc_time(4.0, SC_US))   {
        
      }
    }
  }

  void main()
  {
    PRINT_MSG("Beginning time critical calculation");
    OSSS_RET(sc_time(10.0, SC_US)) {

      OSSS_RET(sc_time(4.0, SC_US)) {

        PRINT_MSG("Beginning time critical sub-calculation 1");
        OSSS_EET(sc_time(2.0, SC_US))   {
          /* do something */       
        }
        PRINT_MSG("Completed time critical sub-calculation 1");

        EXPECTED_TIME(sc_time(110.0, SC_NS)+ //reset time
		      sc_time(2.0, SC_US));

      }

      EXPECTED_TIME(sc_time(110.0, SC_NS)+ //reset time
		    sc_time(2.0, SC_US));

      PRINT_MSG("Beginning time critical sub-calculation 2");
      OSSS_EET(sc_time(2.0, SC_US))   {
        /* do something */       
      }
      PRINT_MSG("Completed time critical sub-calculation 2");

      EXPECTED_TIME(sc_time(110.0, SC_NS)+ //reset time
		    sc_time(4.0, SC_US));

    }
    
    EXPECTED_TIME(sc_time(110.0, SC_NS)+ //reset time
		  sc_time(4.0, SC_US));

    PRINT_MSG("Completed time critical calculation");

    PRINT_MSG("Beginning time critical calculation 2 (which will fail)");
    OSSS_RET(sc_time(3.0, SC_US)) {

      for (int i=0; i<3; ++i)
      {
        OSSS_EET(sc_time(2.0, SC_US)) {
          PRINT_MSG("For loop, iteration " << i);                  
        }
        EXPECTED_TIME(sc_time(110.0, SC_NS)+ //reset time
		      sc_time(4.0, SC_US)+
                      sc_time(2.0, SC_US)*static_cast<double>(i+1));

      }
    }
    
    // The previous RET is intentionally violated by inner EETs. Hence we expect
    // now == 10.0 us instead of 7.0 us
    EXPECTED_TIME(sc_time(110.0, SC_NS)+ //reset time
		  sc_time(10.0, SC_US));

    PRINT_MSG("Completed time critical calculation 2");

    PRINT_MSG("Beginning time critical calculation 3 (which is inconsistently constrained)");
    OSSS_RET(sc_time(5.0, SC_US)) {
      methodY();
    }

    // The previous RET is intentionally violated by an inner RET. Hence we 
    // expect now == 10.0 us instead of 7.0 us
    EXPECTED_TIME(sc_time(110.0, SC_NS)+ //reset time
		  sc_time(14.0, SC_US));

    PRINT_MSG("Completed time critical calculation 3");
  }
};

class Top : public sc_module
{
public:

  sc_in<bool>   pi_bClk;
  sc_in<bool>   pi_bReset;

  task1* t1;
  task2* t2;

 Top(sc_module_name name, int enableKey) : sc_module(name)
  {  
    if (enableKey & 1)
    {
      t1 = new task1("task1");
      t1->clock_port(pi_bClk);
      t1->reset_port(pi_bReset);
    }
  
    if (enableKey & 2)
    {
      t2 = new task2("task2");
      t2->clock_port(pi_bClk);
      t2->reset_port(pi_bReset); 
    }
  }

};

int sc_main(int argc, char *argv[])
{
  int enableKey = 255;

  // The bits of the optionally given integer determine which tasks 
  // are intantiated.
  if (argc == 2)
  {
    std::istringstream is(argv[1]);
    is >> enableKey;
  }

  Top top("top", enableKey);
  Testbench test("test");
  
  sc_signal<bool> s_clock;
  sc_signal<bool> s_reset;
  
  test.po_bClk(s_clock);
  test.po_bReset(s_reset);
  
  top.pi_bClk(s_clock);
  top.pi_bReset(s_reset);
    
  osss_start(30000, SC_NS );

  return 0;
}
