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

/*
 * Brief description:
 *
 * This simple example connects a camera and a filter module through an 
 * osss channel which enables a simple point-to-point communication. The
 * camera continuously writes image objects to the channel, which are read by
 * the filter module.
 * While the camera and the filter module perform a method based communication
 * with the channel, transactors inside the channel enable the definition of
 * user defined communication protocols using SystemC signal ports.
 * In this example the channel is used to serialise a "big" image object in
 * order to transfer it via an 8 bit signal inside the channel.
 *
 */

#ifndef _TOP_INCLUDE_
#define _TOP_INCLUDE_

#include <systemc.h>

#include "Channel.h"
#include "Camera.h"
#include "Filter.h"


//-----------------------------------------------------------------------------
// top module declaration
//   ( the declaration of a top module is very important, because the
//     it embeds the channel inside a module and thus enables the usage of
//     wait() statements inside transactors )
//-----------------------------------------------------------------------------

SC_MODULE(Top)
{
  
  sc_in<bool>   pi_bClk;
  sc_in<bool>   pi_bReset;

  Camera *cam;
  Filter *filter;
  Channel channel;  // channel instantiation

  SC_CTOR( Top )
  {  
    channel.clock_port(pi_bClk);
    channel.reset_port(pi_bReset);

    cam = new Camera( "cam" );
    cam->clock(pi_bClk);
    cam->reset(pi_bReset);
    
    // binds the cam module to the channel
    //
    // This binding generates the Channel::Transactor_write transactor because
    // it has been registered for the method_write_if through the
    // OSSS_REGISTER_TRANSACTOR(Channel::Transactor_write, method_write_if)
    // macro. Additionally this binding connects the generated transactor to
    // the shared signals inside the channel as denoted inside the OSSS_GENERATE
    // block of the signal_write_if
    cam->output(channel);
  
    filter = new Filter( "filter" );
    filter->clock(pi_bClk);
    filter->reset(pi_bReset);
    
    // binds the filter module to the channel
    //
    // This binding generates the Channel::Transactor_read transactor because
    // it has been registered for the method_read_if through the
    // OSSS_REGISTER_TRANSACTOR(Channel::Transactor_read, method_read_if)
    // macro. Additionally this binding connects the generated transactor to
    // the shared signals inside the channel as denoted inside the OSSS_GENERATE
    // block of the signal_read_if
    filter->input(channel);
  }
};

#endif
