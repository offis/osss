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

#include "matrix.hh"
#include <stdio.h>

int vec3sAreClose(const Vec3 *pVec1, const Vec3 *pVec2)
{
  return   REALS_ARE_CLOSE(pVec1->x, pVec2->x)
        && REALS_ARE_CLOSE(pVec1->y, pVec2->y)
        && REALS_ARE_CLOSE(pVec1->z, pVec2->z);
}

real_t
vec3LengthSquare(const Vec3 *pVec)
{
 return
   REAL_SUM3(
     REAL_SQUARE(pVec->x),
     REAL_SQUARE(pVec->y),
     REAL_SQUARE(pVec->z));      
}

real_t
vec3Length(const Vec3 *pVec)
{
   return REAL_SQRT(vec3LengthSquare(pVec));
}

void
vec3Normalise(Vec3 *pVec)
{
  real_t length = vec3Length(pVec);
  pVec->x = REAL_DIV(pVec->x, length);
  pVec->y = REAL_DIV(pVec->y, length);
  pVec->z = REAL_DIV(pVec->z, length);
}

void 
vec3Sub(Vec3 *dest, const Vec3 *a, const Vec3 *b)
{
  dest->x = REAL_SUB(a->x, b->x);
  dest->y = REAL_SUB(a->y, b->y);
  dest->z = REAL_SUB(a->z, b->z);
}

real_t
vec3Dot(const Vec3 *a, const Vec3 *b)
{
  return 
    REAL_SUM3(
      REAL_MULT(
        a->x,
        b->x),
      REAL_MULT(
        a->y,
        b->y),
      REAL_MULT(
        a->z,
        b->z)
    );
}

void 
vec3Cross(Vec3 *dest, const Vec3 *a, const Vec3 *b)
{
  #define CROSS_HELPER(c1, c2, c3) \
    dest->c1 = REAL_SUB( \
                 REAL_MULT( \
                   a->c2,  \
                   b->c3), \
                 REAL_MULT( \
                   a->c3, \
                   b->c2) \
               );

  CROSS_HELPER(x,y,z)
  CROSS_HELPER(y,z,x)
  CROSS_HELPER(z,x,y)

  #undef CROSS_HELPER  
               
}

void
vec3ScaleAdd(Vec3 *dest, const Vec3 *a, real_t k, const Vec3 *b)
{
  #define SCALE_HELPER(coordPart) \
    dest->coordPart = REAL_ADD( \
                        a->coordPart, \
                        REAL_MULT( \
                          k, \
                          b->coordPart) \
                      );
                      
  SCALE_HELPER(x)
  SCALE_HELPER(y)
  SCALE_HELPER(z)    
  
  #undef SCALE_HELPER          
}

void
vec3ScaleAdd2(Vec3 *dest, const Vec3 *a, real_t k1, 
              const Vec3 *b, real_t k2, const Vec3 *c)
{
  #define SCALE_HELPER(coordPart) \
    dest->coordPart = REAL_SUM3( \
                        a->coordPart, \
                        REAL_MULT( \
                          k1, \
                          b->coordPart), \
                        REAL_MULT( \
                          k2, \
                          c->coordPart) \
                      );
                      
  SCALE_HELPER(x)
  SCALE_HELPER(y)
  SCALE_HELPER(z)    
  
  #undef SCALE_HELPER          

}

void
vec3Scale(Vec3 *dest, real_t k, const Vec3 *a)
{
  dest->x=REAL_MULT(k, a->x);
  dest->y=REAL_MULT(k, a->y);
  dest->z=REAL_MULT(k, a->z);
}
