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

// 2006-04-11, Thorsten Schubert, OFFIS

#ifndef REAL_FLOAT_INCLUDED
#define REAL_FLOAT_INCLUDED

typedef   float   real_t;

#define REAL_ADD(a, b) ((a)+(b))
#define REAL_SUB(a, b) ((a)-(b))
#define REAL_MULT(a,b) ((a)*(b))
#define REAL_DIV(a,b)  real_divide(a,b)

#define REAL_SQUARE(x) REAL_MULT(x,x)
#define REAL_SQRT(x) real_sqrt(x)

#define REAL_0 0.0f
#define REAL_EPS 0.0001f
/* For adjusting things reliably */
#define REAL_SMALL 0.01f
#define REAL_MINUS_1 (-1.0f)
#define REAL_0_5 0.5f
#define REAL_1 1.0f
#define REAL_2 2.0f
#define REAL_MINUS_2 (-2.0f)
#define REAL_4 4.0f
/* Create a real from a floating point number */
#define REAL_NUM(x) ((real_t)(x))
/* Create a real from an integer type (char, short, int, whatever) */
#define REAL_FROM_INT(x) ((real_t)(x))
/* Round to nearest integer */
#define REAL_ROUND_TO_INTEGER(x, typ) ((typ)(x+0.5f))

#define REAL_IS_LOWER(x,y) ((x)<(y))
#define REAL_IS_LOWER_EQUAL(x,y) ((x)<=(y))
#define REAL_IS_NEGATIVE(x) (REAL_IS_LOWER(x,REAL_0))

#define REAL_MULT_BY_2(x) REAL_MULT(REAL_2, x)
#define REAL_MULT_BY_MINUS_2(x) REAL_MULT(REAL_MINUS_2, x)
#define REAL_MULT_BY_4(x) REAL_MULT(REAL_4, x)

#define REAL_DIV_BY_2(x) REAL_DIV(x, REAL_2)


#define REAL_SUM3(a, b, c) REAL_ADD(REAL_ADD(a,b),c)

real_t real_sqrt(real_t);
real_t real_divide(real_t, real_t);

#define REAL_ABS(r) (REAL_IS_NEGATIVE(r) ? -r : r)

#define REALS_ARE_CLOSE(x, y) (REAL_IS_LOWER(REAL_ABS(REAL_SUB(x,y)), REAL_EPS))

#endif
