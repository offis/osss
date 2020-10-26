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

// Reference documents:

// [1] IBM On-Chip Peripheral Bus Architecture Specifications
//     Version 2.1 SA-14-2528-02

// [2] Xilinx On-Chip Peripheral Bus V2.0 with OPB Arbiter (v1.10c)
//     DS401 June 7, 2005

// [3] Xilinx Processor IP Reference Guide
//     Chapter 1: OPB Usage in Xilinx FPGAs
//     Version 3.3, February 2005

#ifndef XILINX_OPB_CHANNEL_IMPL_HH_INCLUDED
#define XILINX_OPB_CHANNEL_IMPL_HH_INCLUDED

#include "o3s/communication/arch/osss_channel/osss_vector_port.h"
#include "o3s/communication/arch/osss_channel/osss_vector_port_list_element.h"

/*
 * Define the checksumalgorithm.
 *
 */
#define XILINX_OPB_CHANNEL_CS_NONE  0 //!< Deactivate the usage of an checksum algorithm.
#define XILINX_OPB_CHANNEL_CS_CRC32 1 //!< Use the CRC32-Algorithm.
#define XILINX_OPB_CHANNEL_CS_FIXED 2 //!< Use the value from \c XILINX_OPB_CHANNEL_CS_FIXED_VALUE as checksum.
#define XILINX_OPB_CHANNEL_CS_FIXED_VALUE 0x00000000 //!< The checksum value used by \c XILINX_OPB_CHANNEL_CS_FIXED_VALUE

// Determin if the checksum algorithm is defined
#ifndef XILINX_OPB_CHANNEL_CS_ALGORITHM
#warning Checksum algorithm not defined! Please define XILINX_OPB_CHANNEL_CS_ALGORITHM. Deactivating the checksum.
#define XILINX_OPB_CHANNEL_CS_ALGORITHM XILINX_OPB_CHANNEL_CS_NONE
#endif


namespace osss {
namespace xilinx {
namespace internal {

//-------------------------------------------------------------------------
// Internal xilinx arbiter class. Please use xilinx_opb_channel.
template< class SchedulerType, unsigned int extra_wait >
class xilinx_opb_arbiter
  : public osss_arbiter_if
{
public:
  osss_vector_out< bool > ARB_MGrant;
  sc_core::sc_out< bool > ARB_timeout;

  osss_vector_in< bool > M_request;
  sc_core::sc_in< bool > OPB_select;
  sc_core::sc_in< bool > OPB_xferAck;
  sc_core::sc_in< bool > OPB_busLock;
  sc_core::sc_in< bool > OPB_toutSup;

  OSSS_GENERATE {
    osss_connect(osss_reg_vector(ARB_MGrant),
                 osss_multi_signal("OPB_MGrant"));
    osss_connect(osss_reg_port(ARB_timeout),
                 osss_shared_signal("OPB_timeout"));
    osss_connect(osss_multi_signal("OPB_MRequest"),
                 osss_reg_vector(M_request));
    osss_connect(osss_shared_signal("OPB_select"),
                 osss_reg_port(OPB_select));
    osss_connect(osss_shared_signal("OPB_xferAck"),
                 osss_reg_port(OPB_xferAck));
    osss_connect(osss_shared_signal("OPB_busLock"),
                 osss_reg_port(OPB_busLock));
    osss_connect(osss_shared_signal("OPB_toutSup"),
                 osss_reg_port(OPB_toutSup));
  } /* OSSS_GENERATE */

  void reset();
  void arbiter();

  OSSS_CONTROLLER_CTOR(xilinx_opb_arbiter)
  {
    m_scheduler = new SchedulerType(getChannel()->getNumMasters());

    OSSS_UNIQUE_CTHREAD(arbiter, clock_port().pos());
    reset_signal_is(reset_port(), true);
  }

  virtual const char* kind() const { return "xilinx_opb_arbiter"; }

private:
  osss_scheduler* m_scheduler;
}; /* class xilinx_opb_arbiter */

//-------------------------------------------------------------------------
// Internal base class. Please use xilinx_opb_channel.
template<class ArbiterType>
class xilinx_opb_channel_impl
  : public osss_abstract_channel<ArbiterType>
{
  // The base type of this class.
  typedef osss_abstract_channel<ArbiterType> base_type;
  
  // Used width of the busses.
  typedef sc_dt::sc_uint< 32 > OPB_ABus_t;
  typedef sc_dt::sc_uint< 4 >  OPB_BE_t;
  typedef sc_dt::sc_uint< 32 > OPB_DBus_t;

  // The bits of the M_BE signal. A one means that the lane is active.
  static const unsigned int DBUS_BYTE0     = 1 << 3; //Bits  0 to  7
  static const unsigned int DBUS_BYTE1     = 1 << 2; //Bits  8 to 15
  static const unsigned int DBUS_BYTE2     = 1 << 1; //Bits 16 to 23
  static const unsigned int DBUS_BYTE3     = 1 << 0; //Bits 24 to 31
  static const unsigned int DBUS_HALFWORD0 = DBUS_BYTE0|DBUS_BYTE1; //Bits  0 to 15
  static const unsigned int DBUS_HALFWORD1 = DBUS_BYTE2|DBUS_BYTE3; //Bits 16 to 31
  static const unsigned int DBUS_WORD      = DBUS_HALFWORD0|DBUS_HALFWORD1; //Bits 0 to 31

public:
  typedef typename base_type::data_chunk   data_chunk;
  typedef typename base_type::data_type    data_type;
  typedef typename base_type::address_type address_type;
  typedef typename base_type::action_type  action_type;

  /**
   * The constructor.
   *
   * @param name The name of this channel.
   */
  explicit xilinx_opb_channel_impl(std::string name)
    : base_type(name)
  { } /* constructor */



  /**
   * Resets the complete channel.
   */
  virtual void
  reset();


  /**
   * Traces all signals of this channel.
   *
   * The signals of the master and slave transactors are also traced.
   *
   * @param tf The tracefile to b  
   */
/*
  virtual void
  trace_all(sc_core::sc_trace_file *tf);
*/



  /**
   * Is called if the complete channel is descriped.
   *
   * We need to write our own complete, because we need to connect the master
   * and slave translators to the global clock an reset signals.
   */
/*
  virtual void
  complete();
*/



  /**
   * This module translates the functions calls performed by a bus master into
   * signals of the bus.
   */
  class master : 
    public base_type::abstract_master_if, 
    public osss_method_if,
    public sc_core::sc_module
  {
  public:

    // I/O according to [3], Table 1-1
    //sc_core::sc_in< bool >        OPB_Clk; // OPB_Clk provided by osss_channel base class
    //sc_core::sc_in< bool >        OPB_Rst; // OPB_Rst provided by osss_channel base class

    sc_core::sc_out< bool >       M_request;
    sc_core::sc_out< bool >       M_busLock;
    sc_core::sc_out< bool >       M_select;
    sc_core::sc_out< bool >       M_RNW;
    sc_core::sc_out< OPB_BE_t >   M_BE;
    //sc_core::sc_out< bool >       M_bwXfer; // Not supported by Xilinx-developed OPB devices.
    //sc_core::sc_out< bool >       M_hwXfer; // Not supported by Xilinx-developed OPB devices.
    //sc_core::sc_out< bool >       M_fwXfer; // Not supported by Xilinx-developed OPB devices.
    //sc_core::sc_out< bool >       M_dwXfer; // Not supported by Xilinx-developed OPB devices.
    sc_core::sc_out< bool >       M_seqAddr; 
    //sc_core::sc_out< bool >       M_DBusEn; // Not supported by Xilinx-developed OPB devices.
    sc_core::sc_out< OPB_DBus_t > M_DBus;
    sc_core::sc_out< OPB_ABus_t > M_ABus;

    sc_core::sc_in< bool >        OPB_MGrant;
    sc_core::sc_in< bool >        OPB_xferAck;
    //sc_core::sc_in< bool >        OPB_beAck;   // Not supported by Xilinx-developed OPB devices.
    //sc_core::sc_in< bool >        OPB_hwAck;   // Not supported by Xilinx-developed OPB devices.
    //sc_core::sc_in< bool >        OPB_fwAck;   // Not supported by Xilinx-developed OPB devices.
    //sc_core::sc_in< bool >        OPB_dwAck;   // Not supported by Xilinx-developed OPB devices.
    //sc_core::sc_in< bool >        OPB_pendReqn;// Not supported by Xilinx-developed OPB devices.
    sc_core::sc_in< bool >        OPB_errAck;
    sc_core::sc_in< bool >        OPB_retry;
    sc_core::sc_in< bool >        OPB_timeout;
    sc_core::sc_in< OPB_DBus_t >  OPB_DBus;

    
    /**
     * Resets the master transactor.
     *
     * As the master does not hav an internal state, only the ports are driven
     * with the default values.
     */
    virtual void reset();


   
    /** 
     *  Writes a datachunk to the slave.
     *
     *  This method blocks the process until the chunk had been written.
     * 
     * @param slave_base_addr The address where the chunk should be written to.
     * @param data The datachunk to be written.
     * 
     * @return
     * If the transfer was successfully, \c true is returned. Otherwise \c false
     * will be returned.
     */
    virtual bool write_blocking(address_type slave_base_addr,
                                const data_chunk& data);

    /** 
     * Writes a serialisable object to the slave.
     *
     * This method block the calling process until the chunk had been written.
     * 
     * @param slave_base_addr The address to write the object to.
     * @param ser_obj The object to be written.
     * 
     * @return 
     * If the transfer was successfuly \c true is returned. Otherwise the
     * method wil return \c false.
     *
     * @see write_blocking(address_type, data_chunk)
     */
    virtual bool write_blocking(address_type slave_base_addr,
                                osss_serialisable_object& ser_obj);

    /** 
     * Writes a serialisable_archive to the slave.
     *
     * This method blocks until the archive had been written.
     * 
     * @param slave_base_addr The address to write the archive to.
     * @param ser_arch The archive to be written.
     * 
     * @return 
     * If the transfer was successfull \c true is returned. If the transfer
     * fails \c false will be returned.
     *
     * @see write_blocking(address_type, data_chunk)
     */
    virtual bool write_blocking(address_type slave_base_addr,
                                osss_serialisable_archive& ser_arch);


    
    /**
     * Read a data chunk from the slave.
     *
     * @param slave_base_addr The address where to read the chunk from.
     * @param data Where to store the data. Must be empty.
     * @param size The number of bytes to read from the slave.
     *
     * @return \c true if the read operation was successfull, \c false else.
     */
    virtual bool read_blocking(address_type slave_base_addr,
                               data_chunk& data,
                               unsigned int size);

    /** 
     * Reads a serialisable_object from the slave.
     *
     * This method blocks the calling process until the object had been read.
     * 
     * @param slave_base_addr The address to read the object from.
     * @param ser_obj The object, that had been read.
     * @param size
     * 
     * @return 
     * If the transfer was successful \c true is returnes. Otherwise this
     * method returns \c false.
     *
     * @ read_blocking(address_type, data_chunk, unsigned int)
     */
    virtual bool read_blocking(address_type slave_base_addr,
                               osss_serialisable_object& ser_obj,
                               unsigned int size);

    /** 
     * Reads a serialisable_archive from the slave.
     *
     * This method block the calling process until the archive had been read.
     * 
     * @param slave_base_addr 
     * @param ser_arch 
     * @param size
     * 
     * @return 
     *
     * @ read_blocking(address_type, data_chunk, unsigned int)
     */
    virtual bool read_blocking(address_type slave_base_addr,
                               osss_serialisable_archive& ser_arch,
                               unsigned int size);

    /**
     * Trace all internal signals.
     *
     * @param tf The tracefile to be used.
     */
/*
    virtual void trace_all(sc_core::sc_trace_file *tf);
*/



    /**
     * The custom SystemC-Module constructor for the master_transactor.
     */
    SC_HAS_PROCESS(master);
    explicit
    master( sc_core::sc_module_name name
              = sc_core::sc_gen_unique_name("MasterTransactor"))
     : base_type::abstract_master_if()
     , sc_core::sc_module(name)
     , M_request("M_request")
     , M_busLock("M_busLock")
     , M_select("M_select")
     , M_RNW("M_RNW")
     , M_BE("M_BE")
     , M_seqAddr("M_seqAddr")
     , M_DBus("M_DBus")
     , M_ABus("M_ABus")
     , OPB_MGrant("OPB_MGrant")
     , OPB_xferAck("OPB_xferAck")
     , OPB_errAck("OPB_errAck")
     , OPB_retry("OPB_retry")
     , OPB_timeout("OPB_timeout")
     , OPB_DBus("OPB_DBus")
    { } /* master constructor */

    OSSS_GENERATE {
      // this M_select has to be connected _before_ it is used as a
      // gate in an osss_and_or()
      
      // outgoing 
      this->osss_connect(osss_reg_port(M_request),
                         this->osss_multi_signal("OPB_MRequest"));

      this->osss_connect(osss_reg_port(M_busLock), 
                         this->osss_or(),
                         this->osss_shared_signal("OPB_busLock"));

      this->osss_connect(osss_reg_port(M_select),  
                         this->osss_or(), 
                         this->osss_shared_signal("OPB_select"));

      this->osss_connect(osss_reg_port(M_RNW),
                         this->osss_and_or(osss_reg_port(M_select)), 
                         this->osss_shared_signal("OPB_RNW"));

      this->osss_connect(osss_reg_port(M_BE), 
                         this->osss_and_or(osss_reg_port(M_select)), 
                         this->osss_shared_signal("OPB_BE"));
      
      // Not supported by Xilinx-developed OPB devices.
      //this->osss_connect(osss_reg_port(M_byXfer),
      //                   this->osss_and_or(osss_reg_port(M_select)),
      //                   this->osss_shared_signal("OPB_beXfer"));

      //this->osss_connect(osss_reg_port(M_hwXfer),
      //                   this->osss_and_or(osss_reg_port(M_select)),
      //                   this->osss_shared_signal("OPB_hwXfer"));

      //this->osss_connect(osss_reg_port(M_fwXfer),
      //                   this->osss_and_or(osss_reg_port(M_select)),
      //                   this->osss_shared_signal("OPB_fwXfer"));

      //this->osss_connect(osss_reg_port(M_dwXfer),
      //                   this->osss_and_or(osss_reg_port(M_select)),
      //                   this->osss_shared_signal("OPB_dwXfer"));

      this->osss_connect(osss_reg_port(M_seqAddr),
                         this->osss_and_or(osss_reg_port(M_select)), 
                         this->osss_shared_signal("OPB_seqAddr"));
      
      // M_DBusEn is only used for gating the M_DBus!
      
      this->osss_connect(osss_reg_port(M_ABus), 
                         this->osss_and_or(osss_reg_port(M_select)), 
                         this->osss_shared_signal("OPB_ABus"));

      this->osss_connect(osss_reg_port(M_DBus), 
                         this->osss_or(), // Normally this is gated by M_select, in Xilinx not! It must be set to zero.
                         this->osss_shared_signal("OPB_DBus"));

      // incomming
      this->osss_connect(this->osss_multi_signal("OPB_MGrant"),
                         osss_reg_port(OPB_MGrant));
      
      this->osss_connect(this->osss_shared_signal("OPB_xferAck"),
                         osss_reg_port(OPB_xferAck));
      
      // Not supported by Xilinx-developed OPB devices.
      //this->osss_connect(this->osss_shared_signal("OPB_beAck"),
      //                   osss_reg_port(OPB_beAck));

      //this->osss_connect(this->osss_shared_signal("OPB_hwAck"),
      //                   osss_reg_port(OPB_hwAck));

      //this->osss_connect(this->osss_shared_signal("OPB_fwAck"),
      //                   osss_reg_port(OPB_fwAck));

      //this->osss_connect(this->osss_shared_signal("OPB_dwAck"),
      //                   osss_reg_port(OPB_dwAck));

      //this->osss_connect(this->osss_shared_signal("OPB_pendReqn"),
      //                   osss_reg_port(OPB_pendReqn));

      this->osss_connect(this->osss_shared_signal("OPB_errAck"),
                         osss_reg_port(OPB_errAck));

      this->osss_connect(this->osss_shared_signal("OPB_retry"),
                         osss_reg_port(OPB_retry));

      this->osss_connect(this->osss_shared_signal("OPB_timeout"),
                         osss_reg_port(OPB_timeout));

      this->osss_connect(this->osss_shared_signal("OPB_DBus"),
                         osss_reg_port(OPB_DBus));
    } /* OSSS_GENERATE */
    
  private:

    /**
     * Allocates the bus.
     *
     * The request signal is set. This method returns if the bus is granted
     * this master. The master can start the transfer imediately after this
     * method has returned.
     *
     * @attention
     * The request signal must be reset by the user. It is not done by this
     * method.
     */
    void alloc_bus();
  }; /* class xilinx_opb_channel_impl::master */



  /** 
   * The slave transactor.
   */
  class slave : 
    public base_type::abstract_slave_if, 
    public osss_method_if,
    public sc_core::sc_module
  {
  public:
     /** 
      * Resets all signals.
      */
    virtual void reset();



    /** 
     * Wait to be accessed by a master. 
     * 
     * @param base_address The address the master tries to access.
     * @param action The action, the master want to perform.
     */
    virtual void wait_for_action(address_type& base_address, action_type& action);


    /** 
     * Reads a data chunk from the bus.
     * 
     * @param data The chunk, that had been read.
     * 
     * @return \c true if the reading was successful, else \c false is returned.
     */
    virtual bool read_blocking(data_chunk& data);



    /** 
     * Reads a serialisable object from the bus.
     * 
     * @param ser_obj The object that had been read.
     * 
     * @return \c true if the reading was successful, else \c false is returned.
     */
    virtual bool read_blocking(osss_serialisable_object& ser_obj);



    /** 
     * Reads a serialisable archive from the bus.
     * 
     * @param ser_arch The archive, that had been read.
     * 
     * @return \c true if the reading was successful, else \c false is returned.
     */
    virtual bool read_blocking(osss_serialisable_archive& ser_arch);



    /** 
     * Writes a data chunk to the bus.
     * 
     * @param data The data chunk to be written.
     * 
     * @return Always \c true.
     */
    virtual bool write_blocking(const data_chunk& data);



    /** 
     * Writes a serialisable object to the bus.
     * 
     * @param ser_obj The object to be written.
     * 
     * @return Always \c true.
     */
    virtual bool write_blocking(osss_serialisable_object& ser_obj);



    /** 
     * Writes a serialisable archive to the bus.
     * 
     * @param ser_arch The archive to be written.
     * 
     * @return Always \c true.
     */
    virtual bool write_blocking(osss_serialisable_archive& ser_arch);


    
    /**
     * Trace all internal signals.
     *
     * @param tf The tracefile to be used.
     */
/*
    virtual void trace_all(sc_core::sc_trace_file *tf);
*/

  public:
    // I/O according to [3], Table 1-2
//    sc_core::sc_in< bool >      OPB_Clk;// OPB_Clk provided by osss_channel base class
//    sc_core::sc_in< bool >      OPB_Rst;// OPB_Rst provided by osss_channel base class.
    
    sc_core::sc_out< bool >       Sl_xferAck;
    //sc_core::sc_out< bool >       Sl_beAck;  // Not supported by Xilinx-developed OPB devices.
    //sc_core::sc_out< bool >       Sl_hwAck;  // Not supported by Xilinx-developed OPB devices.
    //sc_core::sc_out< bool >       Sl_fwAck;  // Not supported by Xilinx-developed OPB devices.
    //sc_core::sc_out< bool >       Sl_dwAck;  // Not supported by Xilinx-developed OPB devices.
    sc_core::sc_out< bool >       Sl_errAck;  
    sc_core::sc_out< bool >       Sl_toutSup; 
    sc_core::sc_out< bool >       Sl_retry;   
    //sc_core::sc_out< bool >       Sl_DBusEn; // Not supported by Xilinx-developed OPB devices.
    sc_core::sc_out< OPB_DBus_t > Sl_DBus;

    sc_core::sc_in< bool >       OPB_select;
    sc_core::sc_in< bool >       OPB_RNW;
    sc_core::sc_in< OPB_BE_t >   OPB_BE;  
    //sc_core::sc_in< bool >       OPB_beXfer; // Not supported by Xilinx-developed OPB devices.
    //sc_core::sc_in< bool >       OPB_hwXfer; // Not supported by Xilinx-developed OPB devices.
    //sc_core::sc_in< bool >       OPB_fwXfer; // Not supported by Xilinx-developed OPB devices.
    //sc_core::sc_in< bool >       OPB_dwXfer; // Not supported by Xilinx-developed OPB devices.
    sc_core::sc_in< bool >       OPB_seqAddr;
    sc_core::sc_in< OPB_ABus_t > OPB_ABus;
    sc_core::sc_in< OPB_DBus_t > OPB_DBus;

    /* Constructor. */
    explicit
    slave(sc_core::sc_module_name name
             = sc_core::sc_gen_unique_name("SlaveTransactor"))
      : base_type::abstract_slave_if()
      , sc_core::sc_module(name)
//      , OPB_Clk("OPB_Clk")
//      , OPB_Rst("OPB_Rst")
      , Sl_xferAck("Sl_xferAck")
      , Sl_errAck("Sl_errAck")
      , Sl_toutSup("Sl_toutSup")
      , Sl_retry("Sl_retry")
      , Sl_DBus("Sl_DBus")
      , OPB_select("OPB_select")
      , OPB_RNW("OPB_RNW")
      , OPB_BE("OPB_BE")
      , OPB_seqAddr("OPB_seqAddr")
      , OPB_ABus("OPB_ABus")
      , OPB_DBus("OPB_DBus")
    { }

    OSSS_GENERATE {
      this->osss_connect(osss_reg_port(Sl_DBus),
                         this->osss_or(),
                         this->osss_shared_signal("OPB_DBus"));

      this->osss_connect(osss_reg_port(Sl_errAck),
                         this->osss_or(),
                         this->osss_shared_signal("OPB_errAck"));

      this->osss_connect(osss_reg_port(Sl_retry),
                         this->osss_or(),
                         this->osss_shared_signal("OPB_retry"));

      this->osss_connect(osss_reg_port(Sl_toutSup),
                         this->osss_or(),
                         this->osss_shared_signal("OPB_toutSup"));

      this->osss_connect(osss_reg_port(Sl_xferAck),
                         this->osss_or(),
                         this->osss_shared_signal("OPB_xferAck"));

      this->osss_connect(this->osss_shared_signal("OPB_ABus"),
                         osss_reg_port(OPB_ABus));

      this->osss_connect(this->osss_shared_signal("OPB_BE"),
                         osss_reg_port(OPB_BE));  

      this->osss_connect(this->osss_shared_signal("OPB_DBus"),
                         osss_reg_port(OPB_DBus));

      this->osss_connect(this->osss_shared_signal("OPB_RNW"),
                         osss_reg_port(OPB_RNW));

      this->osss_connect(this->osss_shared_signal("OPB_select"),
                         osss_reg_port(OPB_select));

      this->osss_connect(this->osss_shared_signal("OPB_seqAddr"),
                         osss_reg_port(OPB_seqAddr));
    } /* OSSS_GENERATE */

  }; /* class xilinx_opb_channel_impl::slave */

private:
  /**
   * Build the checksum for a given vector.
   *
   * @param data The vector to build the checksum for.
   *
   * @return The checksum.
   */
  static sc_dt::sc_uint<32>
  build_checksum(const std::vector<sc_dt::sc_uint<8> > &data);

  /**
   * Creates a vector of 32Bit values which should be send via the bus.
   *
   * @param data The 8Bit vector to be converted.
   *
   * @return A 32Bit vector that could be send.
   */
  static std::vector<sc_dt::sc_uint<32> > 
  v32_from_v8(const std::vector<sc_dt::sc_uint<8> >& data);


  /**
   * Creates a vector of 8Bit values from the values that had been received via the bus.
   *
   * @param data The vector holding 32Bit values received from the bus.
   * @param size The number of valid bytes in \a data.
   *
   * @return A 8Bit vector containing all the received bytes.
   *
   * @attention data.size must be > size*4!
   */
  static std::vector<sc_dt::sc_uint<8> > 
  v8_from_v32(const std::vector<sc_dt::sc_uint<32> >& data, unsigned int size);


  /** 
   * Converts a serialisable object into a data chunk.
   * 
   * @param ser_obj The object to be converted.
   * 
   * @return The object as data chunk.
   */
  static typename xilinx_opb_channel_impl<ArbiterType>::data_chunk
  serialisable_object_to_data_chunk(osss_serialisable_object& ser_obj);

  /** 
   * Converts a serialisable archive into a data chunk.
   * 
   * @param ser_arch The archive to be converted.
   * 
   * @return The archive as data chunk.
   */
  static typename xilinx_opb_channel_impl<ArbiterType>::data_chunk
  serialisable_archive_to_data_chunk(osss_serialisable_archive& ser_arch);

  /** 
   * Converts a data chunk into a serialisable object.
   * 
   * @param data The data chunk to be converted.
   * @param ser_obj The data chunk as object.
   */
  static void
  data_chunk_to_serialisable_object(
     typename xilinx_opb_channel_impl<ArbiterType>::data_chunk data,
     osss_serialisable_object& ser_obj);

  /** 
   * Converts a data chunk into a serialisable archive.
   * 
   * @param data The data chunk to be converted.
   * @param ser_arch The data chunk as archive.
   */
  static void
  data_chunk_to_serialisable_archive(
     typename xilinx_opb_channel_impl<ArbiterType>::data_chunk data,
     osss_serialisable_archive& ser_arch);

};  /* class xilinx_opb_channel_impl */

} // namespace internal
} // namespace xilinx
} // namespace osss

#include "o3s/communication/arch/xilinx_opb_channel_impl.tpp"

#endif // XILINX_OPB_CHANNEL_IMPL_HH_INCLUDED
// $Id: xilinx_opb_channel_impl.h 2840 2008-12-04 09:30:05Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)

