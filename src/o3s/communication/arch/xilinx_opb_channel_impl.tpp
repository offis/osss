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

#define DEBUG_XILINX_OPB 0

#define DEBUG_INFO sc_time_stamp() << " : " << name() << " : "
#define ERROR_INFO __FILE__ << ":" << __LINE__ << ":"
#ifndef XILINX_OPB_CHANNEL_CS_ALGORITHM
   #error Checksum algorith not defined!
#endif   

namespace osss {
namespace xilinx {
namespace internal {

//-------------------------------------------------------------------------
// xilinx arbiter
template< class SchedulerType, unsigned int ExtraWait >
void
xilinx_opb_arbiter<SchedulerType, ExtraWait>::reset()
{
  //std::cout << "DEBUG-1-xilinx_opb_arbiter RESET" << std::endl;
  for(unsigned int i = 0; i < (unsigned int)ARB_MGrant.size(); i++) {
    ARB_MGrant.write(i, false);
  }
  ARB_timeout.write(false);
}

template< class SchedulerType, unsigned int ExtraWait >
void
xilinx_opb_arbiter<SchedulerType, ExtraWait>::arbiter()
{
  sc_core::wait(); // for reset
  
  unsigned int num_masters = getChannel()->getNumMasters();
  bool* request = new bool[num_masters];
  while(true)
  {
//     while(not M_request.event())
//       sc_core::wait();

    if((OPB_busLock.read() == false and OPB_select.read() == false)  // idle
       or
       (OPB_busLock.read() == false and OPB_xferAck.read() == true)) // overlap
    {
      bool found = false;
      for(unsigned int i = 0; i < num_masters; ++i)
      {
        request[i] = M_request.read(i);
        if(request[i])
          found = true;
      }

      if(found)
      {
        unsigned int granted_master = m_scheduler->schedule(request);

        // wait additional cycles (according to template parameter)
        if( ExtraWait ) sc_core::wait( static_cast<int>(ExtraWait) );

        for(unsigned int i = 0; i < (unsigned int)ARB_MGrant.size(); i++)
        {
          ARB_MGrant.write(i, (i == granted_master));
        }
        sc_core::wait(); // for select
        for(unsigned int i = 0; i < (unsigned int)ARB_MGrant.size(); i++)
        {
          ARB_MGrant.write(i, false);
        }
      }
    }
    sc_core::wait();
  }
}


//-------------------------------------------------------------------------
// xilinx channel
template<class ArbiterType>
inline
void
xilinx_opb_channel_impl<ArbiterType>::
reset()
{
  //std::cout << "DEBUG-1-xilinx_opb_channel.icc RESET" << std::endl;
  this->getArbiter()->reset();
} /* xilinx_opb_channel_impl::reset */

//---------------------------------------------------------------------------
// master interface
//

/*
template<class ArbiterType>
void
xilinx_opb_channel_impl<ArbiterType>::master::
trace_all(sc_trace_file *tf)
{
   //Trace signals
   sc_trace(tf, OPB_Clk,   OPB_Clk.name());
   sc_trace(tf, OPB_Rst,   OPB_Rst.name());
   sc_trace(tf, M_request, M_request.name());
   sc_trace(tf, M_busLock, M_busLock.name());
   sc_trace(tf, M_select,  M_select.name());
   sc_trace(tf, M_RNW,     M_RNW.name());
   sc_trace(tf, M_BE,      M_BE.name());
   //sc_trace(tf, M_bwXfer,  M_bwXfer.name()); // Not supported by Xilinx-developed OPB devices.
   //sc_trace(tf, M_hwXfer,  M_hwXfer.name()); // Not supported by Xilinx-developed OPB devices.
   //sc_trace(tf, M_fwXfer,  M_fwXfer.name()); // Not supported by Xilinx-developed OPB devices.
   //sc_trace(tf, M_dwXfer,  M_dwXfer.name()); // Not supported by Xilinx-developed OPB devices.
   sc_trace(tf, M_seqAddr, M_seqAddr.name());
   //sc_trace(tf, M_DBusEn,  M_DBusEn.name()); // Not supported by Xilinx-developed OPB devices.
   sc_trace(tf, M_DBus,    M_DBus.name());
   sc_trace(tf, M_ABus,    M_ABus.name());
   sc_trace(tf, OPB_MGrant,  OPB_MGrant.name());
   sc_trace(tf, OPB_xferAck, OPB_xferAck.name());
   sc_trace(tf, OPB_xferAck, OPB_xferAck.name());
   //sc_trace(tf, OPB_beAck,    OPB_beAck.name());   // Not supported by Xilinx-developed OPB devices.
   //sc_trace(tf, OPB_hwAck,    OPB_hwAck.name());   // Not supported by Xilinx-developed OPB devices.
   //sc_trace(tf, OPB_fwAck,    OPB_fwAck.name());   // Not supported by Xilinx-developed OPB devices.
   //sc_trace(tf, OPB_dwAck,    OPB_dwAck.name());   // Not supported by Xilinx-developed OPB devices.
   //sc_trace(tf, OPB_pendReqn, OPB_pendReqn.name());// Not supported by Xilinx-developed OPB devices.
   sc_trace(tf, OPB_errAck,  OPB_errAck.name());
   sc_trace(tf, OPB_retry,   OPB_retry.name());
   sc_trace(tf, OPB_timeout, OPB_timeout.name());
   sc_trace(tf, OPB_DBus,    OPB_DBus.name());
} 
*/
/* xilinx_opb_channel_impl::master::trace _all */



template<class ArbiterType>
inline
void
xilinx_opb_channel_impl<ArbiterType>::master::
reset()
{
  M_ABus.write(0);
  M_BE.write(0);
  M_busLock.write(false);
  M_DBus.write(0);
  M_request.write(false);
  M_RNW.write(false);
  M_seqAddr.write(false);
  M_select.write(false);
} /* xilinx_opb_channel_impl::master::reset */


template<class ArbiterType>
inline
bool
xilinx_opb_channel_impl<ArbiterType>::master::
write_blocking(address_type slave_base_addr, const data_chunk& data)
{ 
   sc_dt::sc_uint< 32 > data32;
   unsigned int         index;

   assert(data.size()>0); //Write at least one byte.

   // Prepare the bus.
   this->alloc_bus();
   M_request.write(false);
   M_ABus.write(slave_base_addr);
   M_select.write(true);
   M_RNW.write(false);


#if DEBUG_XILINX_OPB==1
   std::cout << DEBUG_INFO << "I'm writing " << std::dec << data.size() 
        << " byte(s) to address: 0x" << std::hex << slave_base_addr << std::endl;
#endif

   index = 0;
   while (index<data.size()) {
      data32 = 0x00000000;
#if XILINX_OPB_CHANNEL_CS_ALGORITHM==XILINX_OPB_CHANNEL_CS_NONE
      if (data.size()-index > 4){
         M_busLock.write(true); /* More transfer cycles to come. */
         M_seqAddr.write(true);
      }else{
         M_busLock.write(false); /* This is the last transfer cycle. */
         M_seqAddr.write(false);
      }
#else /* The checksum will be the last transfer cycle. */     
      M_busLock.write(true);
      M_seqAddr.write(true);
#endif /* XILINX_OPB_CHANNEL_CS_ALGORITHM */
      switch (data.size()-index) {
         case 0:
            std::cerr << ERROR_INFO << "Invalid number of bytes to send!\n";
            sc_core::sc_stop();
            break;

         case 1:
            data32.range(7,0) = data[index];
            M_BE.write(DBUS_BYTE0);
            index += 1;
            break;

         case 2:
            data32.range( 7, 0) = data[index+0];
            data32.range(15, 8) = data[index+1];
            M_BE.write(DBUS_HALFWORD0);
            index += 2;
            break;

         case 3:
            data32.range( 7, 0) = data[index+0];
            data32.range(15, 8) = data[index+1];
            data32.range(23,16) = data[index+2];
            M_BE.write(DBUS_BYTE0|DBUS_BYTE1|DBUS_BYTE2);
            index += 3;
            break;

         case 4:
         default:
            data32.range( 7, 0) = data[index+0];
            data32.range(15, 8) = data[index+1];
            data32.range(23,16) = data[index+2];
            data32.range(31,24) = data[index+3];
            M_BE.write(DBUS_WORD);
            index += 4;
            break;
      } /* switch */
      M_DBus.write(data32);
      do {
         sc_core::wait();
         if ((OPB_errAck.read()==true) || (OPB_retry.read()==true)) {
            this->reset();
            return false;
         } /* if error || retry  */
      }while(OPB_xferAck.read() != true);
   }; /* while data to send */

#if XILINX_OPB_CHANNEL_CS_ALGORITHM!=XILINX_OPB_CHANNEL_CS_NONE
   data32 = build_checksum(data);
   M_busLock.write(false);
   M_seqAddr.write(false);
   M_BE.write(DBUS_WORD);
   M_DBus.write(data32);
   do {
      sc_core::wait();
      if ((OPB_errAck.read()==true) || (OPB_retry.read()==true)) {
            this->reset();
            return false;
         } /* if error || retry  */
   }while(OPB_xferAck.read() != true);
#endif /* XILINX_OPB_CHANNEL_CS_ALGORITHM */
   M_select.write(false);

   // Transfer done. Release the bus.
   this->reset();
   return true;
} /* xilinx_opb_channel_impl::master::write_blocking */



template<class ArbiterType>
inline
bool
xilinx_opb_channel_impl<ArbiterType>::master::
write_blocking(address_type slave_base_addr, osss_serialisable_object& ser_obj)
{
  return this->write_blocking(slave_base_addr, 
                              serialisable_object_to_data_chunk(ser_obj));
}

template<class ArbiterType>
inline
bool
xilinx_opb_channel_impl<ArbiterType>::master::
write_blocking(address_type slave_base_addr, osss_serialisable_archive& ser_arch)
{
  return this->write_blocking(slave_base_addr, 
			      serialisable_archive_to_data_chunk(ser_arch));
} /* xilinx_opb_channel_impl::master::write_blocking */



template<class ArbiterType>
inline
bool
xilinx_opb_channel_impl<ArbiterType>::master::
read_blocking(address_type slave_base_addr, data_chunk& data, unsigned int size)
{
   std::vector<sc_dt::sc_uint<8> > vector8;
   sc_dt::sc_uint<32>              data32;
   unsigned int                    curByte;

   assert(size>0); //Read at least one byte.

#if DEBUG_XILINX_OPB==1
   std::cout << DEBUG_INFO << "I'm reading " << std::dec << size 
        << " Byte(s) from address 0x" << std::hex << slave_base_addr << std::endl;
#endif

   // Prepare the bus.
   this->alloc_bus();
   M_request.write(false);
   M_select.write(true);
   M_RNW.write(true);
   M_ABus.write(slave_base_addr);

   curByte = 0;
   while (curByte<size) {
#if XILINX_OPB_CHANNEL_CS_ALGORITHM==XILINX_OPB_CHANNEL_CS_NONE
      if ((size-curByte) > 4) {
         M_busLock.write(true); /* More transfercycles to perform. */
         M_seqAddr.write(true);
      }else{
         M_busLock.write(false); /* This is the last cycle. */
         M_seqAddr.write(false);
      }
#else
      M_busLock.write(false);
      M_seqAddr.write(false);
#endif /* XILINX_OPB_CHANNEL_CS_ALGORITHM */      
      switch (size-curByte) {
         case 0:
            std::cerr << ERROR_INFO << "Invalid number of bytes to read!\n";
            sc_core::sc_stop();
            break;

         case 1:
            M_BE.write(DBUS_BYTE0);
            do{
               sc_core::wait();
               if ((OPB_errAck.read()==true) || (OPB_retry.read()==true)) {
                  this->reset();
                  return false;
               }
            }while(OPB_xferAck.read() != true);
            data32 = OPB_DBus.read();
            vector8.push_back(data32.range(7,0));
            curByte += 1;
            break;

         case 2:
            M_BE.write(DBUS_HALFWORD0);
            do{
               sc_core::wait();
               if ((OPB_errAck.read()==true) || (OPB_retry.read()==true)) {
                  this->reset();
                  return false;
               }
            }while(OPB_xferAck.read() != true);
            data32 = OPB_DBus.read();
            vector8.push_back(data32.range( 7, 0));
            vector8.push_back(data32.range(15, 8));
            curByte += 2;
            break;

         case 3:
            M_BE.write(DBUS_BYTE0|DBUS_BYTE1|DBUS_BYTE2);
            do{
               sc_core::wait();
               if ((OPB_errAck.read()==true) || (OPB_retry.read()==true)) {
                  this->reset();
                  return false;
               }
            }while(OPB_xferAck.read() != true);
            data32 = OPB_DBus.read();
            vector8.push_back(data32.range( 7, 0));
            vector8.push_back(data32.range(15, 8));
            vector8.push_back(data32.range(23,16));
            curByte += 3;
            break;

         case 4:
         default:
            M_BE.write(DBUS_WORD);
            do{
               sc_core::wait();
               if ((OPB_errAck.read()==true) || (OPB_retry.read()==true)) {
                  this->reset();
                  return false;
               }
            }while(OPB_xferAck.read() != true);
            data32 = OPB_DBus.read();
            vector8.push_back(data32.range( 7, 0));
            vector8.push_back(data32.range(15, 8));
            vector8.push_back(data32.range(23,16));
            vector8.push_back(data32.range(31,24));
            curByte += 4;
            break;
      } /* switch */
   }/* while */
#if XILINX_OPB_CHANNEL_CS_ALGORITHM!=NONE
   do{
      sc_core::wait();
      if ((OPB_errAck.read()==true) || (OPB_retry.read()==true)) {
         this->reset();
         return false;
      }
   }while(OPB_xferAck.read() != true);
   data32 = OPB_DBus.read();

   data = vector8;
   this->reset();
   if (data32 != build_checksum(vector8)) {
      return false;
   }else{
      return true;
   }
#else   
   data = vector8;
   this->reset();
   return true;
#endif /* XILINX_OPB_CHANNEL_CS_ALGORITHM */
} /* xilinx_opb_channel_impl::master::read_blocking */



template<class ArbiterType>
inline
bool
xilinx_opb_channel_impl<ArbiterType>::master::
read_blocking(address_type slave_base_addr,
              osss_serialisable_object& ser_obj,
              unsigned int size)
{
  typename base_type::data_chunk data;
  bool return_value;

  return_value = this->read_blocking(slave_base_addr, data, size);
  data_chunk_to_serialisable_object(data, ser_obj);
  return return_value;
} /* return_value */

template<class ArbiterType>
inline
bool
xilinx_opb_channel_impl<ArbiterType>::master::
read_blocking(address_type slave_base_addr,
              osss_serialisable_archive& ser_arch,
              unsigned int size)
{
  typename base_type::data_chunk data;
  bool return_value;

  return_value = this->read_blocking(slave_base_addr, data, size);
  data_chunk_to_serialisable_archive(data, ser_arch);
  return return_value;
} /* read_blocking */

template<class ArbiterType>
inline
void
xilinx_opb_channel_impl<ArbiterType>::master::
alloc_bus()
{
  M_request.write(true);
  while(OPB_MGrant.read()==false) {
    sc_core::wait();
  }
} /* xilinx_opb_channel_impl::master::alloc_bus */





//-------------------------------------------------------------------------
// slave interface

/*
template<class ArbiterType>
void
xilinx_opb_channel_impl<ArbiterType>::slave::
trace_all(sc_trace_file *tf)
{
   sc_trace(tf, OPB_Clk,     OPB_Clk.name());
   sc_trace(tf, OPB_Rst,     OPB_Rst.name());

   sc_trace(tf, Sl_xferAck,  Sl_xferAck.name());
   sc_trace(tf, Sl_errAck,   Sl_errAck.name());
   sc_trace(tf, Sl_toutSup,  Sl_toutSup.name());
   sc_trace(tf, Sl_retry,    Sl_retry.name());

   sc_trace(tf, OPB_select,  OPB_select.name());
   sc_trace(tf, OPB_RNW,     OPB_RNW.name());
   sc_trace(tf, OPB_BE,      OPB_BE.name());
   sc_trace(tf, OPB_seqAddr, OPB_seqAddr.name());
   sc_trace(tf, OPB_ABus,    OPB_ABus.name());
   sc_trace(tf, OPB_DBus,    OPB_DBus.name());
} 
*/
/* slave::trace_all */



template<class ArbiterType>
inline
void
xilinx_opb_channel_impl<ArbiterType>::slave::
reset(void)
{
  Sl_xferAck.write(false);
  Sl_errAck.write(false);
  Sl_toutSup.write(false);
  Sl_retry.write(false);
  Sl_DBus.write(0x00000000);
} /* xilinx_opb_channel_impl::slave::reset */




template<class ArbiterType>
inline
void
xilinx_opb_channel_impl<ArbiterType>::slave::
wait_for_action(address_type& base_address,
		action_type& action)
{
  /* Do the default outputs. */
  this->reset();
  
  /* Wait until we got something to do. */
  do {
#if DEBUG_XILINX_OPB==1
    std::cout << DEBUG_INFO << "I'm waiting.\n";
#endif /* DEBUG_XILINX_OPB==1 */
    sc_core::wait();
  }while((OPB_select.read() == false) or
          (!this->template getAddressRange< address_type >()->is_in_range(OPB_ABus.read().to_uint()))
         );



  if (OPB_RNW.read()==true) {
     action = base_type::READ_ACTION;
#if DEBUG_XILINX_OPB==1
     std::cout << DEBUG_INFO << "I got a READ_ACTION to do.\n";
#endif /* DEBUG_XILINX_OPB==1 */
  }else{
     action = base_type::WRITE_ACTION;
#if DEBUG_XILINX_OPB==1
     std::cout << DEBUG_INFO << "I got a WRITE_ACTION to do.\n";
#endif /* DEBUG_XILINX_OPB==1 */
  }
  base_address = OPB_ABus.read().to_uint();
  Sl_toutSup.write(true);

} /* xilinx_opb_channel_impl::slave::wait_for_action */



template<class ArbiterType>
inline
bool
xilinx_opb_channel_impl<ArbiterType>::slave::
read_blocking(data_chunk& data)
{
   std::vector<sc_dt::sc_uint<8> > vector8;
   sc_dt::sc_uint<32>              data32;


#if DEBUG_XILINX_OPB==1
   std::cout << DEBUG_INFO << "Start reading from the bus.\n";
#endif

   vector8.clear();
   Sl_xferAck.write(true);
   do{   
      //Read the bytes from the bus.
      sc_core::wait();
      assert(OPB_BE.read().to_uint() > 0); // Check if at least one byte is transfered
      data32 = OPB_DBus.read();
      if ((OPB_BE.read().to_uint()&DBUS_BYTE0) == DBUS_BYTE0) {
         vector8.push_back(data32.range( 7, 0));
      }
      if ((OPB_BE.read().to_uint()&DBUS_BYTE1) == DBUS_BYTE1) {
         vector8.push_back(data32.range(15, 8));
      }
      if ((OPB_BE.read().to_uint()&DBUS_BYTE2) == DBUS_BYTE2) {
         vector8.push_back(data32.range(23,16));
      }
      if ((OPB_BE.read().to_uint()&DBUS_BYTE3) == DBUS_BYTE3) {
         vector8.push_back(data32.range(31,24));
      }
   }while(OPB_seqAddr.read()==true);
   Sl_xferAck.write(false);
   this->reset();

#if DEBUG_XILINX_OPB==1
   std::cout << DEBUG_INFO << "I read " << vector8.size() << " byte(s) from the bus.\n";
#endif

#if XILINX_OPB_CHANNEL_CS_ALGORITHM!=XILINX_OPB_CHANNEL_CS_NONE
   assert(vector8.size()>4);
   data32.range(31,24) = vector8.back();
   vector8.pop_back();
   data32.range(23,16) = vector8.back();
   vector8.pop_back();
   data32.range(15, 8) = vector8.back();
   vector8.pop_back();
   data32.range( 7, 0) = vector8.back();
   vector8.pop_back();
   data = vector8;
   if (data32 != build_checksum(vector8)) {
      return false;
   }else{
      return true;
   }
#else
   data = vector8;
   return true;
#endif /* XILINX_OPB_CHANNEL_CS_ALGORITHM */

} /* xilinx_opb_channel_impl::slave::read_blocking */



template<class ArbiterType>
inline
bool
xilinx_opb_channel_impl<ArbiterType>::slave::
read_blocking(osss_serialisable_object& ser_obj)
{
  typename base_type::data_chunk data;
  bool return_value = this->read_blocking(data);
  data_chunk_to_serialisable_object(data, ser_obj);
  return return_value;
}



template<class ArbiterType>
inline
bool
xilinx_opb_channel_impl<ArbiterType>::slave::
read_blocking(osss_serialisable_archive& ser_arch)
{
  typename base_type::data_chunk data;
  bool return_value = this->read_blocking(data);
  data_chunk_to_serialisable_archive(data, ser_arch);
  return return_value;
}



template<class ArbiterType>
inline
bool
xilinx_opb_channel_impl<ArbiterType>::slave::
write_blocking(const data_chunk& data)
{
   sc_dt::sc_uint<32> data32;
   unsigned int       curByte;

   //Write at least one byte.
   assert(data.size()>0);

   Sl_xferAck.write(true);
   curByte = 0;
   while(curByte<data.size()) {
      if(curByte>=data.size()) {
         std::cerr << ERROR_INFO << "The master tries to read more data, than offered by the slave!\n";
         sc_core::sc_stop();
         assert(false);
      }
      data32 = 0x00000000;
      if ((OPB_BE.read()&DBUS_BYTE0) == DBUS_BYTE0) {
         data32.range( 7, 0) = data[curByte];
         curByte++;
      }
      if ((OPB_BE.read()&DBUS_BYTE1) == DBUS_BYTE1) {
         data32.range(15, 8) = data[curByte];
         curByte++;
      }
      if ((OPB_BE.read()&DBUS_BYTE2) == DBUS_BYTE2) {
         data32.range(23,16) = data[curByte];
         curByte++;
      }
      if ((OPB_BE.read()&DBUS_BYTE3) == DBUS_BYTE3) {
         data32.range(31,24) = data[curByte];
         curByte++;
      }
      Sl_DBus.write(data32);
      sc_core::wait();
   } /* while */
#if XILINX_OPB_CHANNEL_CS_ALGORITHM!=XILINX_OPB_CHANNEL_CS_NONE
   data32 = build_checksum(data);
   Sl_DBus.write(data32);
   sc_core::wait();
#else
#endif /* XILINX_OPB_CHANNEL_CS_ALGORITHM */
   Sl_xferAck.write(false);

   if (curByte<data.size()) {
      std::cerr << ERROR_INFO << "The master read less data than offered by the slave!\n";
      sc_core::sc_stop();
      assert(false);
   }

   this->reset();
   return true;
} /* xilinx_opb_channel_impl::slave::write_blocking */



template<class ArbiterType>
inline
bool
xilinx_opb_channel_impl<ArbiterType>::slave::
write_blocking(osss_serialisable_object& ser_obj)
{
  return this->write_blocking(serialisable_object_to_data_chunk(ser_obj));
}



template<class ArbiterType>
inline
bool
xilinx_opb_channel_impl<ArbiterType>::slave::
write_blocking(osss_serialisable_archive& ser_arch)
{
  return this->write_blocking(serialisable_archive_to_data_chunk(ser_arch));
}

//------------------------------------------------------------------------------
// helper methods
//
template<class ArbiterType>
inline
sc_dt::sc_uint<32>
xilinx_opb_channel_impl<ArbiterType>::
build_checksum(const std::vector<sc_dt::sc_uint<8> > &data)
{
   sc_dt::sc_uint<32>  checksum;  //!< The checksum.

#if XILINX_OPB_CHANNEL_CS_ALGORITHM==XILINX_OPB_CHANNEL_CS_NONE
   checksum = 0;


#elif XILINX_OPB_CHANNEL_CS_ALGORITHM==XILINX_OPB_CHANNEL_CS_FIXED
   checksum = XILINX_OPB_CHANNEL_CS_FIXED_VALUE;


#elif XILINX_OPB_CHANNEL_CS_ALGORITHM==XILINX_OPB_CHANNEL_CS_CRC32
   const unsigned int CRC32_POLYNOM = 0xEDB88320;

   unsigned int bit;       /* The current bit from the data chunk. */
   unsigned int lowbit;    /* The lowest bit from the reverse crc. */
   unsigned int rev_crc32; /* The reverse CRC32.                   */

   rev_crc32 = 0xffffffff;
   for (unsigned int i=0; i<data.size(); ++i) { /* for each byte */
      for (unsigned int j=0; j<8; ++j) { /* for each bit */
         bit    = (data[i]>>j)&0x1;
         lowbit = rev_crc32 & 0x1;
         if (lowbit!=bit) {
            rev_crc32 = (rev_crc32>>1) ^ CRC32_POLYNOM;
         }else{
            rev_crc32 = rev_crc32>>1;
         }
         rev_crc32 = rev_crc32 & 0xFFFFFFFF; /* Assure, that the CRC is 32 bit wide. */
      } /* for j (each bit) */
   } /* for i (each byte) */
   checksum = rev_crc32 ^ 0xFFFFFFFF;


#else
   std::cerr << "Unkonwn checksum algorithm defined!\n";
   sc_core::sc_stop();
#endif

   return checksum;
} /* xilinx_opb_channel_impl::build_checksum */




template<class ArbiterType>
inline
std::vector<sc_dt::sc_uint<32> >
xilinx_opb_channel_impl<ArbiterType>::
v32_from_v8(const std::vector<sc_dt::sc_uint<8> >& data)
{
  std::vector<sc_dt::sc_uint<32> > result;
  unsigned int size = data.size();
  unsigned int index;
  sc_dt::sc_uint<32> buffer;

  //Make the vector large enough.
  //We need roundup(data.size/4) elements plus one for the size and one for the checksum.
  result.reserve((data.size()+3)/4);
  result.clear();

  index = 0;
  while(index < size)
  {
    buffer = 0;
    buffer.range(7, 0)   = data[index];
    index++;
    buffer.range(15, 8)  = (index < size) ? data[index] : sc_dt::sc_uint<8>(0);
    index++;
    buffer.range(23, 16) = (index < size) ? data[index] : sc_dt::sc_uint<8>(0);
    index++;
    buffer.range(31, 24) = (index < size) ? data[index] : sc_dt::sc_uint<8>(0);
    index++;

    result.push_back(buffer);
  }

  return result;
} /* xilinx_opb_channel_impl::v32_from_v8 */



template<class ArbiterType>
inline
std::vector<sc_dt::sc_uint<8> > 
xilinx_opb_channel_impl<ArbiterType>::
v8_from_v32(const std::vector<sc_dt::sc_uint<32> >& data, unsigned int size)
{
  std::vector<sc_dt::sc_uint<8> > result;
  unsigned int index;


  if(size > (data.size() * 4)) {
    std::cerr << "size too big in " << __func__ << std::endl;
    sc_core::sc_stop();
  }

  index = 0;
  while(result.size() < size) {
    result.push_back(data[index].range(7, 0));

    if(result.size() < size)
      result.push_back(data[index].range(15, 8));
    else
      break;

    if(result.size() < size)
      result.push_back(data[index].range(23, 16));
    else
      break;

    if(result.size() < size)
      result.push_back(data[index].range(31, 24));
    else
      break;

    index++;
  }
  return result;
} /* xilinx_opb_channel_impl::v8_from_v32 */



template<class ArbiterType>
typename xilinx_opb_channel_impl<ArbiterType>::data_chunk
xilinx_opb_channel_impl<ArbiterType>::
serialisable_object_to_data_chunk(osss_serialisable_object& ser_obj)
{
  data_chunk _chunk;

  // prepares the object ser_obj for serialisation
  ser_obj.serialise_obj();

  // splits the ser_obj into chunks of size of type 
  // base_type::data_chunk::value_type
  while (!ser_obj.complete())
  {
    _chunk.push_back(ser_obj.read_chunk<typename data_chunk::value_type>());
  }
  return _chunk;
}

template<class ArbiterType>
inline
typename xilinx_opb_channel_impl<ArbiterType>::data_chunk
xilinx_opb_channel_impl<ArbiterType>::
serialisable_archive_to_data_chunk(osss_serialisable_archive& ser_arch)
{
  data_chunk chunk_;

  // prepares the osss_serialisable_archive for serialisation
  ser_arch.prepare_read();

  // writes the whole osss_serialisable_archive in chunks of size
  // of the type base_type::data_chunk::value_type
  while (!ser_arch.complete())
  {
    chunk_.push_back(ser_arch.read_chunk<typename data_chunk::value_type>());
  }
  return chunk_;
}

template<class ArbiterType>
inline
void
xilinx_opb_channel_impl<ArbiterType>::data_chunk_to_serialisable_object
  ( data_chunk data, osss_serialisable_object& ser_obj)
{
  if (!data.empty())
  {
    while (!ser_obj.complete())
    {
      ser_obj.write_chunk<typename data_chunk::value_type>( data.front() );
      data.erase(data.begin());
    }
  
    // finallises the deserialisation of ser_obj
    ser_obj.deserialise_obj();
  }
}

template<class ArbiterType>
inline
void
xilinx_opb_channel_impl<ArbiterType>::data_chunk_to_serialisable_archive
  ( data_chunk data, osss_serialisable_archive& ser_arch )
{
  if (!data.empty())
  {
    ser_arch.prepare_write();
    while (!data.empty())
    {
      ser_arch.write_chunk<typename data_chunk::value_type>( data.front() );
      data.erase(data.begin());
    }
    ser_arch.prepare_rebuild();
  }
}

} // namespace internal
} // namespace xilinx
} // namespace osss

// $Id: xilinx_opb_channel_impl.tpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
//
