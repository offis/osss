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

// 2006-04-12, Thorsten Schubert, OFFIS

#include "color.hh"

#define MAX_VALUE_N_BITS(n) ((1 << (n))-1)

#define SCALE_FACTOR_N_TO_M(n, m) \
  REAL_DIV( \
    REAL_FROM_INT(MAX_VALUE_N_BITS(m)), \
    REAL_FROM_INT(MAX_VALUE_N_BITS(n)) \
  )

#define COLOR_FROM_N_TO_M(x, n, m) \
  REAL_ROUND_TO_INTEGER( \
    REAL_MULT( \
      REAL_FROM_INT(x), \
      SCALE_FACTOR_N_TO_M(n,m)), \
    Color_base_t)
 
#define COLOR_SPLIT_24(c, rx, gx, bx) \
  rx = c.r; rx = COLOR_FROM_N_TO_M(rx, NUM_BITS_RED, 8);\
  gx = c.g; gx = COLOR_FROM_N_TO_M(gx, NUM_BITS_GREEN, 8);\
  bx = c.b; bx = COLOR_FROM_N_TO_M(bx, NUM_BITS_BLUE, 8); 

#define COLOR_JOIN_24(c, rx, gx, bx) \
  (c).r = COLOR_FROM_N_TO_M(rx, 8, NUM_BITS_RED); \
  (c).g = COLOR_FROM_N_TO_M(gx, 8, NUM_BITS_GREEN); \
  (c).b = COLOR_FROM_N_TO_M(bx, 8, NUM_BITS_BLUE);

#define MAX_CHANNEL_r COLOR_MAX_RED
#define MAX_CHANNEL_g COLOR_MAX_GREEN
#define MAX_CHANNEL_b COLOR_MAX_BLUE

#define CHANNEL_NUM_BITS_r NUM_BITS_RED
#define CHANNEL_NUM_BITS_g NUM_BITS_GREEN
#define CHANNEL_NUM_BITS_b NUM_BITS_BLUE

void colorSplit24(Color c, Color_base_t *r, Color_base_t *g, Color_base_t *b)
{
  COLOR_SPLIT_24(c, *r, *g, *b)
}

Color colorJoin24(Color_base_t r, Color_base_t g, Color_base_t b)
{
  Color result;
  COLOR_JOIN_24(result, r, g, b)
  return result;
}

/*
static
Color_base_t
getMaxChannel(const char *channel) 
{  
  switch (*channel)
  {
    case 'r' : return COLOR_MAX_RED;
    case 'g' : return COLOR_MAX_GREEN;
    case 'b' : return COLOR_MAX_BLUE;
  }
}
*/

static
real_t
clipToMax(real_t val, real_t max)
{
  if (REAL_IS_LOWER_EQUAL(val, max))
  {
    return val;
  }
  
  return max;
}

Color colorScaleSat(real_t k, Color c)
{
  Color result;
  Color_base_t tmp1;
  real_t       tmp2;

  if (REAL_IS_NEGATIVE(k))
  {
    return colorBlack();
  }
  
  #define COLOR_HELPER(channel) \
    tmp1 = c.channel; \
    tmp2 = REAL_MULT(k, REAL_FROM_INT(tmp1)); \
    tmp2 = clipToMax(tmp2, REAL_FROM_INT(MAX_CHANNEL_##channel)); \
    tmp1 = REAL_ROUND_TO_INTEGER(tmp2, Color_base_t); \
    result.channel = tmp1;
    
  COLOR_HELPER(r)
  COLOR_HELPER(g)
  COLOR_HELPER(b)
  
  #undef COLOR_HELPER
  
  return result;
}

Color colorAddSat(Color a, Color b)
{
  Color result;
  int tmp;
  
  #define COLOR_HELPER(channel) \
    tmp  = a.channel; \
    tmp += b.channel; \
    if (tmp > (MAX_CHANNEL_##channel)) \
    { \
      tmp = (MAX_CHANNEL_##channel); \
    } \
    result.channel = (Color_base_t)(tmp);

  COLOR_HELPER(r)
  COLOR_HELPER(g)
  COLOR_HELPER(b)
  
  #undef COLOR_HELPER
  
  return result;
}

Color colorMultSat(Color a, Color b)
{
  Color result;
  unsigned int tmp;
  
  #define COLOR_HELPER(channel) \
    tmp  = a.channel; \
    tmp *= (unsigned int)(b.channel); \
    tmp >>= CHANNEL_NUM_BITS_##channel; \
    if (tmp > (MAX_CHANNEL_##channel)) \
    { \
      tmp = (MAX_CHANNEL_##channel); \
    } \
    result.channel = (Color_base_t)(tmp);

  COLOR_HELPER(r)
  COLOR_HELPER(g)
  COLOR_HELPER(b)
  
  #undef COLOR_HELPER
  
  return result;
}

Color colorBlack()
{
  Color c;
  c.r=0;
  c.g=0;
  c.b=0;
  return c;
}

Color colorWhite()
{
  Color c;
  c.r=COLOR_MAX_RED;
  c.g=COLOR_MAX_GREEN;
  c.b=COLOR_MAX_BLUE;
  return c;
}

Color colorRed()
{
  Color c;
  c.r=COLOR_MAX_RED;
  c.g=0;
  c.b=0;
  return c;
}

Color colorGreen()
{
  Color c;
  c.r=0;
  c.g=COLOR_MAX_GREEN;
  c.b=0;
  return c;
}

Color colorBlue()
{
  Color c;
  c.r=0;
  c.g=0;
  c.b=COLOR_MAX_BLUE;
  return c;
}
