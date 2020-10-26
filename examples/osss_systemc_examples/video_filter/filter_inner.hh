#ifndef FILTER_INNER_INCLUDED
#define FILTER_INNER_INCLUDED

#include <systemc.h>

typedef sc_biguint<8> pixel_t;
typedef sc_bigint<8>  coeff_t;

template<typename T>
struct Vec3
{
  T x0,x1,x2;  
  
  Vec3() {}

  // Initialise the vector by a scalar
  // Implicit conversions are evil  
  explicit
  Vec3(T x)
  {
    // call overloaded operator =
    *this = x;
  }
  
  // Assign a scalar to the vector
  void operator = (const Vec3 &rhs)
  {
    x0 = rhs.x0;
    x1 = rhs.x1;
    x2 = rhs.x2; 
  }
  
  // overloaded operator =
  void operator = (T x)
  {
    x0=x;
    x1=x;
    x2=x; 
  }
  
  // Set the three elements
  void set(T x0_new, T x1_new, T x2_new)
  {
    x0 = x0_new;
    x1 = x1_new;
    x2 = x2_new; 
  }
  
  // Compare to vectors element-wise
  bool operator == (const Vec3 & sv3) const
  {
    return ((sv3.x0 == x0) &&
            (sv3.x1 == x1) &&
            (sv3.x2 == x2));
  }

  // Shift the vector elements left by i
  // Note: only 1 and 2 are meaningful values  
  void operator <<= (int i) 
  {
    if (i==1)
    {
     x2 = x1;
     x1 = x0;
    } else if (i==2)
    {
     x2 = x0; 
    }
  }
  
};

// Template specialisation for vectors of vectors, i.e. matrices
template<typename T>
struct Vec3<Vec3<T> > 
{
  // Note: due to the specialisation T is the matix' element type
  typedef T element_t;
  
  Vec3<element_t> x0,x1,x2;

  Vec3() {}

  // Initialise the matrix by a scalar
  // Implicit conversions are evil  
  explicit
  Vec3(element_t x)
  {
    // call overloaded operator =
    *this = x;
  }

  // Initialise of elements of the matrix  
  Vec3(T x0x0, T x0x1, T x0x2,
       T x1x0, T x1x1, T x1x2,
       T x2x0, T x2x1, T x2x2)
  {
    x0.x0 = x0x0; x0.x1 = x0x1 ; x0.x2 = x0x2; 
    x1.x0 = x1x0; x1.x1 = x1x1 ; x1.x2 = x1x2; 
    x2.x0 = x2x0; x2.x1 = x2x1 ; x2.x2 = x2x2; 
  }       
    
  // Assign another matrix to this
  void operator = (const Vec3 &rhs)
  {
    x0 = rhs.x0;
    x1 = rhs.x1;
    x2 = rhs.x2; 
  }
  
  // Assign a scalar to this matrix
  void operator = (element_t x)
  {
    // Call the operator = of the unspecialised vector
    x0=x;
    x1=x;
    x2=x; 
  }
  
  // Compare two matrices element-wise
  bool operator == (const Vec3 & sv3) const
  {
    return ((sv3.x0 == x0) &&
            (sv3.x1 == x1) &&
            (sv3.x2 == x2));
  }

  // Note: As opposed to the vector, here we shift each element!
  void operator <<= (int i) 
  {
    x0 <<= i;
    x1 <<= i;
    x2 <<= i;
  }
  
  // Multiply this by a pixel matrix, returning a pixel
  pixel_t operator * (Vec3<Vec3<pixel_t> > pm) const
  {
    return (x0.x0 * pm.x0.x0 + 
            x0.x1 * pm.x0.x1 +
            x0.x2 * pm.x0.x2 +
            x1.x0 * pm.x1.x0 + 
            x1.x1 * pm.x1.x1 +
            x1.x2 * pm.x1.x2 +
            x2.x0 * pm.x2.x0 + 
            x2.x1 * pm.x2.x1 +
            x2.x2 * pm.x2.x2);
  }
};

typedef Vec3<pixel_t>       pixel_vector_t;
typedef Vec3<pixel_vector_t> pixel_matrix_t;

typedef Vec3<coeff_t>       coeff_vector_t;
typedef Vec3<coeff_vector_t> coeff_matrix_t;


// Ugly SystemC stuff
template<typename T>
std::ostream & operator << (std::ostream &os, const Vec3<T> &) {return os;}

template<typename T>
void sc_trace(sc_trace_file *tf, const Vec3<T> &v, const std::string &s) 
{
  sc_trace(tf, v.x0, "x0");
  sc_trace(tf, v.x1, "x1");
  sc_trace(tf, v.x2, "x2");
}


SC_MODULE(filter_inner)
{

  //
  // port section
  //
  sc_in< bool >           rst;
  sc_in< bool >           Ho_444;
  sc_in< bool >           write_clk;
  
  sc_in< sc_biguint<8> >  red_in;
  sc_in< sc_biguint<8> >  green_in;
  sc_in< sc_biguint<8> >  blue_in;

  sc_out< sc_biguint<8> > red_out;
  sc_out< sc_biguint<8> > green_out;
  sc_out< sc_biguint<8> > blue_out;

  //
  // to chunk buffer
  //
  sc_out<bool>          read_enable;
  sc_in< sc_biguint<8> > read_red_data_line0;
  sc_in< sc_biguint<8> > read_red_data_line1;
  sc_in< sc_biguint<8> > read_red_data_line2;
  sc_in< sc_biguint<8> > read_green_data_line0;
  sc_in< sc_biguint<8> > read_green_data_line1;
  sc_in< sc_biguint<8> > read_green_data_line2;
  sc_in< sc_biguint<8> > read_blue_data_line0;
  sc_in< sc_biguint<8> > read_blue_data_line1;
  sc_in< sc_biguint<8> > read_blue_data_line2;

  
  //
  // signals
  //
  sc_signal< sc_uint<2> > line_sel_counter;

  sc_signal<pixel_matrix_t> red_filter_matrix_sig;
  sc_signal<pixel_matrix_t> green_filter_matrix_sig;
  sc_signal<pixel_matrix_t> blue_filter_matrix_sig;

  SC_HAS_PROCESS(filter_inner);

  filter_inner(sc_module_name name) :
    sc_module(name),
    rst("rst"),
    Ho_444("Ho_444"),
    write_clk("write_clk"),
    red_in("red_in"),
    green_in("green_in"),
    blue_in("blue_in"),
    red_out("red_out"),
    green_out("green_out"),
    blue_out("blue_out")
  {
    SC_CTHREAD(sel_counter_proc, Ho_444.pos());
    reset_signal_is(rst, true);

    SC_CTHREAD(shift_filter_proc, write_clk.pos());
    reset_signal_is(rst, true);

    SC_CTHREAD(apply_coeffs_proc, write_clk.pos());
    reset_signal_is(rst, true);
  }

  void sel_counter_proc();
  void shift_filter_proc();
  void apply_coeffs_proc();
};

#endif
