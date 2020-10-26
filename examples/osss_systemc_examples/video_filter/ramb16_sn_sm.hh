#ifndef RAMB16_SN_SM_INCLUDED
#define RAMB16_SN_SM_INCLUDED

#include <systemc.h>

typedef enum {NO_CHANGE, READ_FIRST, WRITE_FIRST} wmode_t;

#ifndef FPGA
#define FPGA 0
#endif

// template for ramb16_sn_sm modules without parity

template<int awidth_a, int dwidth_a, int awidth_b, int dwidth_b> 
struct ramb16_sn_sm_p0 : sc_module 
{
  // global set/reset port
  sc_in<bool>                gsr;

  // port a
  sc_in_clk                  clk_a;
  sc_in<bool>                en_a;
  sc_in<bool>                ssr_a;
  sc_in<bool>                we_a;
  sc_in<sc_biguint<awidth_a> >  addr_a;
  sc_in<sc_biguint<dwidth_a> >  di_a; 

  sc_out<sc_biguint<dwidth_a> > d_a; 

  // configuration port a
  wmode_t WMODE_A;
  sc_biguint<dwidth_a> INIT_A;
  sc_biguint<dwidth_a> SRVAL_A;

  // port b
  sc_in_clk                  clk_b;
  sc_in<bool>                en_b;
  sc_in<bool>                ssr_b;
  sc_in<bool>                we_b;
  sc_in<sc_biguint<awidth_b> >  addr_b;
  sc_in<sc_biguint<dwidth_b> >  di_b; 

  sc_out<sc_biguint<dwidth_b> > d_b; 

  // configuration port b
  wmode_t WMODE_B;
  sc_biguint<dwidth_b> INIT_B;
  sc_biguint<dwidth_b> SRVAL_B;

  // data memory cells
  sc_bv<16384> dmem;

  // clocked threads
  void sync_a();
  void sync_b();

  // used by systemc macros, normaly set by SC_CTOR macro
  typedef ramb16_sn_sm_p0 SC_CURRENT_USER_MODULE;

  // default constructor
  ramb16_sn_sm_p0(sc_core::sc_module_name mname) : 
    sc_module(mname), gsr("gsr"),
    clk_a("clk_a"), 
    en_a("en_a"), 
    ssr_a("ssr_a"), 
    we_a("we_a"), 
    addr_a("addr_a"), 
    di_a("di_a"), 
    d_a("d_a"), 
    WMODE_A(WRITE_FIRST), INIT_A(0), SRVAL_A(0),
    clk_b("clk_b"), 
    en_b("en_b"), 
    ssr_b("ssr_b"), 
    we_b("we_b"), 
    addr_b("addr_b"), 
    di_b("di_b"), 
    d_b("d_b"), 
    WMODE_B(WRITE_FIRST), INIT_B(0), SRVAL_B(0)
  {
    SC_CTHREAD(sync_a, clk_a.pos());
    reset_signal_is(gsr, true);

    SC_CTHREAD(sync_b, clk_b.pos());
    reset_signal_is(gsr, true);
  }

  // constructor for given write modes
  ramb16_sn_sm_p0(sc_core::sc_module_name mname, 
                  wmode_t WMODE_A, wmode_t WMODE_B ) : 
    sc_module(mname), 
    gsr("gsr"), 
    clk_a("clk_a"), 
    en_a("en_a"), 
    ssr_a("ssr_a"), 
    we_a("we_a"), 
    addr_a("addr_a"), 
    di_a("di_a"), 
    d_a("d_a"), 
    WMODE_A(WMODE_A), INIT_A(0), SRVAL_A(0),
    clk_b("clk_b"), 
    en_b("en_b"), 
    ssr_b("ssr_b"), 
    we_b("we_b"), 
    addr_b("addr_b"), 
    di_b("di_b"), 
    d_b("d_b"), 
    WMODE_B(WMODE_B), INIT_B(0), SRVAL_B(0)
  {
    SC_CTHREAD(sync_a, clk_a.pos());
    reset_signal_is(gsr, true);

    SC_CTHREAD(sync_b, clk_b.pos());
    reset_signal_is(gsr, true);
  }

  // constructor for given write modes and set/reset values
  ramb16_sn_sm_p0(sc_core::sc_module_name mname, 
                  wmode_t WMODE_A, sc_biguint<dwidth_a> INIT_A, 
                  sc_biguint<dwidth_a> SRVAL_A , 
                  wmode_t WMODE_B, sc_biguint<dwidth_b> INIT_B, 
                  sc_biguint<dwidth_b> SRVAL_B) : 
    sc_module(mname), 
    gsr("gsr"),
    clk_a("clk_a"), 
    en_a("en_a"), 
    ssr_a("ssr_a"), 
    we_a("we_a"), 
    addr_a("addr_a"), 
    di_a("di_a"), 
    d_a("d_a"), 
    WMODE_A(WMODE_A), INIT_A(INIT_A), SRVAL_A(SRVAL_A),
    clk_b("clk_b"), 
    en_b("en_b"), 
    ssr_b("ssr_b"), 
    we_b("we_b"), 
    addr_b("addr_b"), 
    di_b("di_b"), 
    d_b("d_b"), 
    WMODE_B(WMODE_B), INIT_B(INIT_B), SRVAL_B(SRVAL_B)
  {
    SC_CTHREAD(sync_a, clk_a.pos());
    reset_signal_is(gsr, true);

    SC_CTHREAD(sync_b, clk_b.pos());
    reset_signal_is(gsr, true);
  }

  // memory initialization
  // use to simulate fpga configuration
  // or to build configuration interface
#if FPGA > 0
  inline void init(const sc_bv<16384> INIT_XX) {
    dmem = INIT_XX;
  }
#endif

};

// template for ramb16_sn_sm module with parity in and out on port b

template<int awidth_a, int dwidth_a, int awidth_b, int dwidth_b, int pwidth_b> 
struct ramb16_sn_sm_p1 : sc_module 
{
  // global set/reset port
  sc_in<bool>                   gsr;

  // port a
  sc_in_clk                     clk_a;
  sc_in<bool>                   en_a;
  sc_in<bool>                   ssr_a;
  sc_in<bool>                   we_a;
  sc_in<sc_biguint<awidth_a> >     addr_a;
  sc_in<sc_biguint<dwidth_a> >     di_a; 

  sc_out<sc_biguint<dwidth_a> >    d_a; 

  // configuration port a
  wmode_t WMODE_A;
  sc_biguint<dwidth_a> INIT_A;
  sc_biguint<dwidth_a> SRVAL_A;

  // port b
  sc_in_clk                     clk_b;
  sc_in<bool>                   en_b;
  sc_in<bool>                   ssr_b;
  sc_in<bool>                   we_b;
  sc_in<sc_biguint<awidth_b> >     addr_b;
  sc_in<sc_biguint<dwidth_b> >     di_b; 
  sc_in<sc_biguint<pwidth_b> >     dpi_b; 

  sc_out<sc_biguint<dwidth_b> >    d_b; 
  sc_out<sc_biguint<pwidth_b> >    dp_b; 

  // configuration port b
  wmode_t WMODE_B;
  sc_biguint<dwidth_b + pwidth_b> INIT_B;
  sc_biguint<dwidth_b + pwidth_b> SRVAL_B;

  // data and parity memory cells
  sc_bv<16384> dmem;
  sc_bv<2048>  pmem;

  // clocked threads
  void sync_a();
  void sync_b();

  // used by systemc macros, normaly set by SC_CTOR macro
  typedef ramb16_sn_sm_p1 SC_CURRENT_USER_MODULE; 

  // default constructor
  ramb16_sn_sm_p1(sc_core::sc_module_name mname) : 
    sc_module(mname), 
    gsr("gsr"),
    clk_a("clk_a"), 
    en_a("en_a"), 
    ssr_a("ssr_a"), 
    we_a("we_a"), 
    addr_a("addr_a"), 
    di_a("di_a"),
    d_a("d_a"), 
    WMODE_A(WRITE_FIRST), INIT_A(0), SRVAL_A(0),
    clk_b("clk_b"), 
    en_b("en_b"), 
    ssr_b("ssr_b"), 
    we_b("we_b"), 
    addr_b("addr_b"), 
    di_b("di_b"), 
    dpi_b("dpi_b"), 
    d_b("d_b"), 
    dp_b("dp_b"), 
    WMODE_B(WRITE_FIRST), INIT_B(0), SRVAL_B(0)
  {
    SC_CTHREAD(sync_a, clk_a.pos());
    reset_signal_is(gsr, true);

    SC_CTHREAD(sync_b, clk_b.pos());
    reset_signal_is(gsr, true);
  }

  // constructor for given write modes
  ramb16_sn_sm_p1(sc_core::sc_module_name mname, 
                  wmode_t WMODE_A, wmode_t WMODE_B ) : 
    sc_module(mname), 
    gsr("gsr"),
    clk_a("clk_a"), 
    en_a("en_a"), 
    ssr_a("ssr_a"), 
    we_a("we_a"), 
    addr_a("addr_a"), 
    di_a("di_a"), 
    d_a("d_a"), 
    WMODE_A(WMODE_A), INIT_A(0), SRVAL_A(0),
    clk_b("clk_b"), 
    en_b("en_b"), 
    ssr_b("ssr_b"), 
    we_b("we_b"), 
    addr_b("addr_b"), 
    di_b("di_b"), 
    dpi_b("dpi_b"), 
    d_b("d_b"), 
    dp_b("dp_b"), 
    WMODE_B(WMODE_B), INIT_B(0), SRVAL_B(0)
  {
    SC_CTHREAD(sync_a, clk_a.pos());
    reset_signal_is(gsr, true);

    SC_CTHREAD(sync_b, clk_b.pos());
    reset_signal_is(gsr, true);
  }

  // constructor for given write modes and set/reset values
  ramb16_sn_sm_p1(sc_core::sc_module_name mname, 
                  wmode_t WMODE_A, 
                  sc_biguint<dwidth_a> INIT_A, 
                  sc_biguint<dwidth_a> SRVAL_A , 
                  wmode_t WMODE_B, 
                  sc_biguint<dwidth_b> INIT_B, 
                  sc_biguint<dwidth_b> SRVAL_B) : 
    sc_module(mname), 
    gsr("gsr"), 
    clk_a("clk_a"), 
    en_a("en_a"), 
    ssr_a("ssr_a"), 
    we_a("we_a"), 
    addr_a("addr_a"), 
    di_a("di_a"), 
    d_a("d_a"), 
    WMODE_A(WMODE_A), INIT_A(INIT_A), SRVAL_A(SRVAL_A), 
    clk_b("clk_b"), 
    en_b("en_b"), 
    ssr_b("ssr_b"), 
    we_b("we_b"), 
    addr_b("addr_b"), 
    di_b("di_b"), 
    dpi_b("dpi_b"), 
    d_b("d_b"), 
    dp_b("dp_b"), 
    WMODE_B(WMODE_B), INIT_B(INIT_B), SRVAL_B(SRVAL_B)
  {
    SC_CTHREAD(sync_a, clk_a.pos());
    reset_signal_is(gsr, true);

    SC_CTHREAD(sync_b, clk_b.pos());
    reset_signal_is(gsr, true);
  }

  // memory initialization
  // use to simulate fpga configuration
  // or to build configuration interface
#if FPGA > 0
  inline void init(const sc_bv<16384> INIT_XX, const sc_bv<2048> INITP_XX) {
    dmem = INIT_XX;
    pmem = INITP_XX;
  }
#endif

};

// template for ramb16_sn_sm modules with parity in and out on both ports

template<int awidth_a, int dwidth_a, int pwidth_a, int awidth_b, int dwidth_b, int pwidth_b> 
struct ramb16_sn_sm_p2 : sc_module 
{
  // global set/reset port
  sc_in<bool>                   gsr;

  // port a
  sc_in_clk                     clk_a;
  sc_in<bool>                   en_a;
  sc_in<bool>                   ssr_a;
  sc_in<bool>                   we_a;
  sc_in<sc_biguint<awidth_a> >     addr_a;
  sc_in<sc_biguint<dwidth_a> >     di_a; 
  sc_in<sc_biguint<pwidth_a> >     dpi_a; 

  sc_out<sc_biguint<dwidth_a> >    d_a; 
  sc_out<sc_biguint<pwidth_a> >    dp_a; 

  // configuration port a
  wmode_t WMODE_A;
  sc_biguint<dwidth_a + pwidth_a> INIT_A;
  sc_biguint<dwidth_a + pwidth_a> SRVAL_A;

  // port b
  sc_in_clk                     clk_b;
  sc_in<bool>                   en_b;
  sc_in<bool>                   ssr_b;
  sc_in<bool>                   we_b;
  sc_in<sc_biguint<awidth_b> >     addr_b;
  sc_in<sc_biguint<dwidth_b> >     di_b; 
  sc_in<sc_biguint<pwidth_b> >     dpi_b; 

  sc_out<sc_biguint<dwidth_b> >    d_b; 
  sc_out<sc_biguint<pwidth_b> >    dp_b; 

  // configuration port b
  wmode_t WMODE_B;
  sc_biguint<dwidth_b + pwidth_b> INIT_B;
  sc_biguint<dwidth_b + pwidth_b> SRVAL_B;

  // data and parity memory
  sc_bv<16384> dmem;
  sc_bv<2048>  pmem;

  // clocked threads
  void sync_a();
  void sync_b();

  // used by systemc macros, normaly set by SC_CTOR macro
  typedef ramb16_sn_sm_p2 SC_CURRENT_USER_MODULE; 

  // default constructor
  ramb16_sn_sm_p2( ::sc_core::sc_module_name mn) : 
    sc_module(mn), 
    gsr("gsr"),
    clk_a("clk_a"), 
    en_a("en_a"), 
    ssr_a("ssr_a"), 
    we_a("we_a"), 
    addr_a("addr_a"), 
    di_a("di_a"), 
    dpi_a("dpi_a"), 
    d_a("d_a"), 
    dp_a("dp_a"), 
    WMODE_A(WRITE_FIRST), INIT_A(0), SRVAL_A(0),
    clk_b("clk_b"), 
    en_b("en_b"), 
    ssr_b("ssr_b"), 
    we_b("we_b"), 
    addr_b("addr_b"), 
    di_b("di_b"), 
    dpi_b("dpi_b"), 
    d_b("d_b"), 
    dp_b("dp_b"), 
    WMODE_B(WRITE_FIRST), INIT_B(0), SRVAL_B(0)
  {
    SC_CTHREAD(sync_a, clk_a.pos());
    reset_signal_is(gsr, true);

    SC_CTHREAD(sync_b, clk_b.pos());
    reset_signal_is(gsr, true);
  }

  // constructor for given write modes
  ramb16_sn_sm_p2( ::sc_core::sc_module_name mn, 
                   wmode_t WMODE_A, wmode_t WMODE_B ) : 
    sc_module(mn), 
    gsr("gsr"),
    clk_a("clk_a"), 
    en_a("en_a"), 
    ssr_a("ssr_a"), 
    we_a("we_a"), 
    addr_a("addr_a"), 
    di_a("di_a"), 
    dpi_a("dpi_a"), 
    d_a("d_a"), 
    dp_a("dp_a"), 
    WMODE_A(WMODE_A), INIT_A(0), SRVAL_A(0),
    clk_b("clk_b"), 
    en_b("en_b"), 
    ssr_b("ssr_b"), 
    we_b("we_b"), 
    addr_b("addr_b"), 
    di_b("di_b"), 
    dpi_b("dpi_b"), 
    d_b("d_b"), 
    dp_b("dp_b"), 
    WMODE_B(WMODE_B), INIT_B(0), SRVAL_B(0)
  {
    SC_CTHREAD(sync_a, clk_a.pos());
    reset_signal_is(gsr, true);

    SC_CTHREAD(sync_b, clk_b.pos());
    reset_signal_is(gsr, true);
  }

  // constructor for given write modes and set/reset values
  ramb16_sn_sm_p2(sc_core::sc_module_name mname, 
                  wmode_t WMODE_A, sc_biguint<dwidth_a> INIT_A, 
                  sc_biguint<dwidth_a> SRVAL_A , 
                  wmode_t WMODE_B, sc_biguint<dwidth_b> INIT_B, 
                  sc_biguint<dwidth_b> SRVAL_B) : 
    sc_module(mname), 
    gsr("gsr"),
    clk_a("clk_a"), 
    en_a("en_a"), 
    ssr_a("ssr_a"), 
    we_a("we_a"), 
    addr_a("addr_a"), 
    di_a("di_a"), 
    dpi_a("dpi_a"), 
    d_a("d_a"), 
    dp_a("dp_a"), 
    WMODE_A(WMODE_A), INIT_A(INIT_A), SRVAL_A(SRVAL_A),
    clk_b("clk_b"), 
    en_b("en_b"), 
    ssr_b("ssr_b"), 
    we_b("we_b"), 
    addr_b("addr_b"), 
    di_b("di_b"), 
    dpi_b("dpi_b"), 
    d_b("d_b"), 
    dp_b("dp_b"), 
    WMODE_B(WMODE_B), INIT_B(INIT_B), SRVAL_B(SRVAL_B)
  {
    SC_CTHREAD(sync_a, clk_a.pos());
    reset_signal_is(gsr, true);

    SC_CTHREAD(sync_b, clk_b.pos());
    reset_signal_is(gsr, true);
  }

  // memory initialization
  // use to simulate fpga configuration
  // or to build configuration interface
#if FPGA > 0
  inline void init(const sc_bv<16384> INIT_XX, const sc_bv<2048> INITP_XX) {
    dmem = INIT_XX;
    pmem = INITP_XX;
  }

  inline void init_write_a(sc_biguint<awidth_a> addr_a,
                           sc_biguint<dwidth_a> di_a, 
                           sc_biguint<pwidth_a> dpi_a)
  {
//cout << "Writing to " << addr_a << ", data == " << di_a
//     << ", parity == " << dpi_a << endl;

    dmem((addr_a + 1) * dwidth_a - 1, addr_a * dwidth_a) = di_a;
    pmem((addr_a + 1) * pwidth_a - 1, addr_a * pwidth_a) = dpi_a;
//cout << "Memory is \n" << dmem << endl;    
    
  }
  
  inline 
  sc_biguint<dwidth_a + pwidth_a>
  init_read_a(sc_biguint<awidth_a> addr_a)
  {
//cout << "Writing to " << addr_a << ", data == " << di_a
//     << ", parity == " << dpi_a << endl;

    sc_biguint<dwidth_a> d = dmem((addr_a + 1) * dwidth_a - 1, addr_a * dwidth_a).to_uint();
    sc_biguint<pwidth_a> p = pmem((addr_a + 1) * pwidth_a - 1, addr_a * pwidth_a).to_uint();
//cout << "Memory is \n" << dmem << endl;    

    return concat(d,p);    
  }
  
#endif

};


// available ramb16_sn_sm modules

typedef ramb16_sn_sm_p0<14,  1, 14,  1>       ramb16_s1_s1;
typedef ramb16_sn_sm_p0<14,  1, 13,  2>       ramb16_s1_s2;
typedef ramb16_sn_sm_p0<14,  1, 12,  4>       ramb16_s1_s4;
typedef ramb16_sn_sm_p1<14,  1, 11,  8, 1>    ramb16_s1_s9;
typedef ramb16_sn_sm_p1<14,  1, 10, 16, 2>    ramb16_s1_s18;
typedef ramb16_sn_sm_p1<14,  1,  9, 32, 4>    ramb16_s1_s36;

typedef ramb16_sn_sm_p0<13,  2, 13,  2>       ramb16_s2_s2;
typedef ramb16_sn_sm_p0<13,  2, 12,  4>       ramb16_s2_s4;
typedef ramb16_sn_sm_p1<13,  2, 11,  8, 1>    ramb16_s2_s9;
typedef ramb16_sn_sm_p1<13,  2, 10, 16, 2>    ramb16_s2_s18;
typedef ramb16_sn_sm_p1<13,  2,  9, 32, 4>    ramb16_s2_s36;

typedef ramb16_sn_sm_p0<12,  4, 12,  4>       ramb16_s4_s4;
typedef ramb16_sn_sm_p1<12,  4, 11,  8, 1>    ramb16_s4_s9;
typedef ramb16_sn_sm_p1<12,  4, 10, 16, 2>    ramb16_s4_s18;
typedef ramb16_sn_sm_p1<12,  4,  9, 32, 4>    ramb16_s4_s36;

typedef ramb16_sn_sm_p2<11,  8, 1, 11,  8, 1> ramb16_s9_s9;
typedef ramb16_sn_sm_p2<11,  8, 1, 10, 16, 2> ramb16_s9_s18;
typedef ramb16_sn_sm_p2<11,  8, 1,  9, 32, 4> ramb16_s9_s36;

typedef ramb16_sn_sm_p2<10, 16, 2, 10, 16, 2> ramb16_s18_s18;
typedef ramb16_sn_sm_p2<10, 16, 2,  9, 32, 4> ramb16_s18_s36;

typedef ramb16_sn_sm_p2< 9, 32, 4,  9, 32, 4> ramb16_s36_s36;


/*
 * Implementations
 */

// ramb16_sn_sm modules without parity

// clocked thread for port a
template<int awidth_a, int dwidth_a, int awidth_b, int dwidth_b> 
void 
ramb16_sn_sm_p0<awidth_a, dwidth_a, awidth_b, dwidth_b>::sync_a () 
{
  if (gsr.read()) {
    d_a = SRVAL_A;
  }
  else {
    d_a = INIT_A;
  }
  while (true) {
    if (en_a.read()) {
      if (ssr_a.read()) {
	d_a = SRVAL_A;
	if (we_a.read()) {
	  dmem((addr_a.read() + 1) * dwidth_a - 1, addr_a.read() * dwidth_a) = di_a.read();
	}
      }
      else {
	if (we_a.read()) {
	  switch (WMODE_A) {
            case NO_CHANGE:
              dmem((addr_a.read() + 1) * dwidth_a - 1, addr_a.read() * dwidth_a) = di_a.read();
              break;
            case READ_FIRST:
              d_a = dmem((addr_a.read() + 1) * dwidth_a - 1, addr_a.read() * dwidth_a).to_uint();
              dmem((addr_a.read() + 1) * dwidth_a - 1, addr_a.read() * dwidth_a) = di_a.read();
              break;
            case WRITE_FIRST:
              dmem((addr_a.read() + 1) * dwidth_a - 1, addr_a.read() * dwidth_a) = di_a.read();
              d_a = dmem((addr_a.read() + 1) * dwidth_a - 1, addr_a.read() * dwidth_a).to_uint();
              break;
	  }
	}
	else {
	  d_a = dmem((addr_a.read() + 1) * dwidth_a - 1, addr_a.read() * dwidth_a).to_uint();
	}
      }
    }
    wait();
  }
}

// clocked thread for port b
template<int awidth_a, int dwidth_a, int awidth_b, int dwidth_b> 
void 
ramb16_sn_sm_p0<awidth_a, dwidth_a, awidth_b, dwidth_b>::sync_b () 
{
  if (gsr.read()) {
    d_b = SRVAL_B;
  }
  else {
    d_b = INIT_B;
  }
  while (true) {
    if (en_b.read()) {
      if (ssr_b.read()) {
	d_b = SRVAL_B;
	if (we_b.read()) {
	  dmem((addr_b.read() + 1) * dwidth_b - 1, addr_b.read() * dwidth_b) = di_b.read();
	}
      }
      else {
	if (we_b.read()) {
	  switch (WMODE_B) {
            case NO_CHANGE:
              dmem((addr_b.read() + 1) * dwidth_b - 1, addr_b.read() * dwidth_b) = di_b.read();
              break;
            case READ_FIRST:
              d_b = dmem((addr_b.read() + 1) * dwidth_b - 1, addr_b.read() * dwidth_b).to_uint();

              dmem((addr_b.read() + 1) * dwidth_b - 1, addr_b.read() * dwidth_b) = di_b.read();
              break;
            case WRITE_FIRST:
              dmem((addr_b.read() + 1) * dwidth_b - 1, addr_b.read() * dwidth_b) = di_b.read();
              d_b = dmem((addr_b.read() + 1) * dwidth_b - 1, addr_b.read() * dwidth_b).to_uint();
              break;
	  }
	}
	else {
	  d_b = dmem((addr_b.read() + 1) * dwidth_b - 1, addr_b.read() * dwidth_b).to_uint();
	}
      }
    }
    wait();
  }
}


// ramb16_sn_sm modules with parity in and out on port b

// clocked thread for port a
template<int awidth_a, int dwidth_a, int awidth_b, int dwidth_b, int pwidth_b> 
void 
ramb16_sn_sm_p1<awidth_a, dwidth_a, awidth_b, dwidth_b, pwidth_b>::sync_a () 
{
  if (gsr.read()) {
    d_a  = SRVAL_A;
  }
  else {
    d_a  = INIT_A;
  }
  while (true) {
    if (en_a.read()) {
      if (ssr_a.read()) {
	d_a  = SRVAL_A;

	if (we_a.read()) {
	  dmem((addr_a.read() + 1) * dwidth_a - 1, addr_a.read() * dwidth_a) = di_a.read();
	}
      }
      else {
	if (we_a.read()) {
	  switch (WMODE_A) {
            case NO_CHANGE:
              dmem((addr_a.read() + 1) * dwidth_a - 1, addr_a.read() * dwidth_a) = di_a.read();
              break;
            case READ_FIRST:
              d_a  = dmem((addr_a.read() + 1) * dwidth_a - 1, addr_a.read() * dwidth_a).to_uint();
              dmem((addr_a.read() + 1) * dwidth_a - 1, addr_a.read() * dwidth_a) = di_a.read();
              break;
            case WRITE_FIRST:
              dmem((addr_a.read() + 1) * dwidth_a - 1, addr_a.read() * dwidth_a) = di_a.read();
              d_a  = dmem((addr_a.read() + 1) * dwidth_a - 1, addr_a.read() * dwidth_a).to_uint();
              break;
	  }
	}
	else {
	  d_a  = dmem((addr_a.read() + 1) * dwidth_a - 1, addr_a.read() * dwidth_a).to_uint();
	}
      }
    }
    wait();
  }
}

// clocked thread for port b
template<int awidth_a, int dwidth_a, int awidth_b, int dwidth_b, int pwidth_b> 
void 
ramb16_sn_sm_p1<awidth_a, dwidth_a, awidth_b, dwidth_b, pwidth_b>::sync_b () 
{
  if (gsr.read()) {
    d_b  = SRVAL_B.range(dwidth_b - 1, 0);
    dp_b = SRVAL_B.range(dwidth_b + pwidth_b - 1, dwidth_b);
  }
  else {
    d_b  = INIT_B.range(dwidth_b - 1, 0);
    dp_b = INIT_B.range(dwidth_b + pwidth_b - 1, dwidth_b);
  }
  while (true) {
    if (en_b.read()) {
      if (ssr_b.read()) {
	d_b  = SRVAL_B.range(dwidth_b - 1, 0);
	dp_b = SRVAL_B.range(dwidth_b + pwidth_b - 1, dwidth_b);

	if (we_b.read()) {
	  dmem((addr_b.read() + 1) * dwidth_b - 1, addr_b.read() * dwidth_b) = di_b.read();
	  pmem((addr_b.read() + 1) * pwidth_b - 1, addr_b.read() * pwidth_b) = dpi_b.read();
	}
      }
      else {
	if (we_b.read()) {
	  switch (WMODE_B) {
            case NO_CHANGE:
              dmem((addr_b.read() + 1) * dwidth_b - 1, addr_b.read() * dwidth_b) = di_b.read();
              pmem((addr_b.read() + 1) * pwidth_b - 1, addr_b.read() * pwidth_b) = dpi_b.read();
              break;
            case READ_FIRST:
              d_b  = dmem((addr_b.read() + 1) * dwidth_b - 1, addr_b.read() * dwidth_b).to_uint();
              dp_b = pmem((addr_b.read() + 1) * pwidth_b - 1, addr_b.read() * pwidth_b).to_uint();
              dmem((addr_b.read() + 1) * dwidth_b - 1, addr_b.read() * dwidth_b) = di_b.read();
              pmem((addr_b.read() + 1) * pwidth_b - 1, addr_b.read() * pwidth_b) = dpi_b.read();
              break;
            case WRITE_FIRST:
              dmem((addr_b.read() + 1) * dwidth_b - 1, addr_b.read() * dwidth_b) = di_b.read();
              pmem((addr_b.read() + 1) * pwidth_b - 1, addr_b.read() * pwidth_b) = dpi_b.read();
              d_b  = dmem((addr_b.read() + 1) * dwidth_b - 1, addr_b.read() * dwidth_b).to_uint();
              dp_b = pmem((addr_b.read() + 1) * pwidth_b - 1, addr_b.read() * pwidth_b).to_uint();
              break;
	  }
	}
	else {
	  d_b  = dmem((addr_b.read() + 1) * dwidth_b - 1, addr_b.read() * dwidth_b).to_uint();
	  dp_b = pmem((addr_b.read() + 1) * pwidth_b - 1, addr_b.read() * pwidth_b).to_uint();
	}
      }
    }
    wait();
  }
}


// ramb16_sn_sm modules with parity in and out on both ports

// clocked thread for port a
template<int awidth_a, 
         int dwidth_a, 
         int pwidth_a, 
         int awidth_b, 
         int dwidth_b, 
         int pwidth_b> 
void 
ramb16_sn_sm_p2<awidth_a, 
                dwidth_a, 
                pwidth_a, 
                awidth_b, 
                dwidth_b, 
                pwidth_b>::sync_a () 
{
  if (gsr.read()) 
  {
    d_a  = SRVAL_A.range(dwidth_a - 1, 0);
    dp_a = SRVAL_A.range(dwidth_a + pwidth_a - 1, dwidth_a);
  }
  else 
  {
    d_a  = INIT_A.range(dwidth_a - 1, 0);
    dp_a = INIT_A.range(dwidth_a + pwidth_a - 1, dwidth_a);
  }
  
  while (true) 
  {
    if (en_a.read()) 
    {
      if (ssr_a.read()) 
      {
        d_a  = SRVAL_A.range(dwidth_a - 1, 0);
        dp_a = SRVAL_A.range(dwidth_a + pwidth_a - 1, dwidth_a);

        if (we_a.read()) 
        {
          dmem(((addr_a.read() + 1) * dwidth_a - 1).to_int()
               , (addr_a.read() * dwidth_a).to_int() ) = di_a.read();

          pmem(((addr_a.read() + 1) * pwidth_a - 1).to_int()
               , (addr_a.read() * pwidth_a).to_int() ) = dpi_a.read();
        }
      }
      else 
      {
        if (we_a.read()) 
        {        
        
// cout << "Writing data == " << di_a.read().to_string(SC_BIN)
//      << ", parity == " << dpi_a.read().to_string(SC_BIN)
//      << ", addr == " << addr_a.read()
//      << endl;
             
          switch (WMODE_A) 
          {
            case NO_CHANGE:
              dmem(((addr_a.read() + 1) * dwidth_a - 1).to_int()
                   , (addr_a.read() * dwidth_a).to_int() ) = di_a.read();
                   
              pmem(((addr_a.read() + 1) * pwidth_a - 1).to_int()
                   ,(addr_a.read() * pwidth_a).to_int() ) = dpi_a.read();
              break;
            case READ_FIRST:
              d_a  = dmem(((addr_a.read() + 1) * dwidth_a - 1).to_int()
                          , (addr_a.read() * dwidth_a).to_int() ).to_uint();
                          
              dp_a = pmem(((addr_a.read() + 1) * pwidth_a - 1).to_int()
                          , (addr_a.read() * pwidth_a).to_int() ).to_uint();
                          
              dmem(((addr_a.read() + 1) * dwidth_a - 1).to_int()
                   , (addr_a.read() * dwidth_a).to_int() ) = di_a.read();
                   
              pmem(((addr_a.read() + 1) * pwidth_a - 1).to_int()
                   , (addr_a.read() * pwidth_a).to_int() ) = dpi_a.read();
              break;
            case WRITE_FIRST:
              dmem(((addr_a.read() + 1) * dwidth_a - 1).to_int()
                   , (addr_a.read() * dwidth_a).to_int() ) = di_a.read();
                   
              pmem(((addr_a.read() + 1) * pwidth_a - 1).to_int()
                   , (addr_a.read() * pwidth_a).to_int() ) = dpi_a.read();
                   
              d_a  = dmem(((addr_a.read() + 1) * dwidth_a - 1).to_int()
                          , (addr_a.read() * dwidth_a).to_int() ).to_uint();
                          
              dp_a = pmem(((addr_a.read() + 1) * pwidth_a - 1).to_int()
                          , (addr_a.read() * pwidth_a).to_int() ).to_uint();
              break;
          }
        }
        else 
        {
//          cout << "reading from " << addr_a.read() << ": ";
//if (addr_a.read() == 0) cout << dmem;          
          d_a  = dmem(((addr_a.read() + 1) * dwidth_a - 1).to_int()
                      , (addr_a.read() * dwidth_a).to_int()).to_uint();
                      
          dp_a = pmem(((addr_a.read() + 1) * pwidth_a - 1).to_int()
                      , (addr_a.read() * pwidth_a).to_int()).to_uint();
                      
//           cout << "data is " << dmem((addr_a.read() + 1) * dwidth_a - 1
//                                      , addr_a.read() * dwidth_a).to_string(SC_BIN)
//                << ", parity is: " << pmem((addr_a.read() + 1) * pwidth_a - 1
//                                           , addr_a.read() * pwidth_a).to_string(SC_BIN)
//                << endl;
        }
      }
    }
    wait();
  }
}

// clocked thread for port b
template<int awidth_a, int dwidth_a, int pwidth_a, int awidth_b, int dwidth_b, int pwidth_b> 
void 
ramb16_sn_sm_p2<awidth_a, dwidth_a, pwidth_a, awidth_b, dwidth_b, pwidth_b>::sync_b () 
{
  if (gsr.read()) {
    d_b  = SRVAL_B.range(dwidth_b - 1, 0);
    dp_b = SRVAL_B.range(dwidth_b + pwidth_b - 1, dwidth_b);
  }
  else {
    d_b  = INIT_B.range(dwidth_b - 1, 0);
    dp_b = INIT_B.range(dwidth_b + pwidth_b - 1, dwidth_b);
  }
  while (true) {
    if (en_b.read()) {
      if (ssr_b.read()) {
	d_b  = SRVAL_B.range(dwidth_b - 1, 0);
	dp_b = SRVAL_B.range(dwidth_b + pwidth_b - 1, dwidth_b);

	if (we_b.read()) {
	  dmem(((addr_b.read() + 1) * dwidth_b - 1).to_int(), 
               (addr_b.read() * dwidth_b).to_int() ) = di_b.read();
	  pmem(((addr_b.read() + 1) * pwidth_b - 1).to_int(), 
               (addr_b.read() * pwidth_b).to_int()) = dpi_b.read();
	}
      }
      else {
	if (we_b.read()) {
	  switch (WMODE_B) {
            case NO_CHANGE:
              dmem(((addr_b.read() + 1) * dwidth_b - 1).to_int(), 
                   (addr_b.read() * dwidth_b).to_int()) = di_b.read();
              pmem(((addr_b.read() + 1) * pwidth_b - 1).to_int(), 
                   (addr_b.read() * pwidth_b).to_int()) = dpi_b.read();
              break;
            case READ_FIRST:
              d_b  = dmem(((addr_b.read() + 1) * dwidth_b - 1).to_int(), 
                          (addr_b.read() * dwidth_b).to_int()).to_uint();
              dp_b = pmem(((addr_b.read() + 1) * pwidth_b - 1).to_int(), 
                          (addr_b.read() * pwidth_b).to_int()).to_uint();
              dmem(((addr_b.read() + 1) * dwidth_b - 1).to_int(), 
                   (addr_b.read() * dwidth_b).to_int()) = di_b.read();
              pmem(((addr_b.read() + 1) * pwidth_b - 1).to_int(), 
                   (addr_b.read() * pwidth_b).to_int()) = dpi_b.read();
              break;
            case WRITE_FIRST:
              dmem(((addr_b.read() + 1) * dwidth_b - 1).to_int(), 
                   (addr_b.read() * dwidth_b).to_int()) = di_b.read();
              pmem(((addr_b.read() + 1) * pwidth_b - 1).to_int(), 
                   (addr_b.read() * pwidth_b).to_int()) = dpi_b.read();
              d_b  = dmem(((addr_b.read() + 1) * dwidth_b - 1).to_int(), 
                          (addr_b.read() * dwidth_b).to_int()).to_uint();
              dp_b = pmem(((addr_b.read() + 1) * pwidth_b - 1).to_int(), 
                          (addr_b.read() * pwidth_b).to_int()).to_uint();
              break;
	  }
	}
	else {
	  d_b  = dmem(((addr_b.read() + 1) * dwidth_b - 1).to_int(), 
                      (addr_b.read() * dwidth_b).to_int()).to_uint();
	  dp_b = pmem(((addr_b.read() + 1) * pwidth_b - 1).to_int(), 
                      (addr_b.read() * pwidth_b).to_int()).to_uint();
	}
      }
    }
    wait();
  }
}

#endif
