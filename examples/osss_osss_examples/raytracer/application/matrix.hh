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

#ifndef MATRIX_INCLUDED
#define MATRIX_INCLUDED

#include "real.hh"

typedef struct
{
  real_t  x;
  real_t  y;
  real_t  z; 
} Vec3;

#define VEC3_INIT(x,y,z) {REAL_NUM(x), REAL_NUM(y), REAL_NUM(z)}

int vec3sAreClose(const Vec3 *pVec1, const Vec3 *pVec2);

real_t
vec3LengthSquare(const Vec3 *pVec);

real_t
vec3Length(const Vec3 *pVec);

void
vec3Normalise(Vec3 *pVec);

/* dest = a - b */
void 
vec3Sub(Vec3 *dest, const Vec3 *a, const Vec3 *b);

real_t
vec3Dot(const Vec3 *a, const Vec3 *b);

/* dest = a cross b. 
   Note: dest must not point to the same object as a or b */
void 
vec3Cross(Vec3 *dest, const Vec3 *a, const Vec3 *b);

void
vec3Scale(Vec3 *dest, real_t k, const Vec3 *a);

/* dest = a + k*b */
void
vec3ScaleAdd(Vec3 *dest, const Vec3 *a, real_t k, const Vec3 *b);

/* dest = a + k1*b + k2*c */
void
vec3ScaleAdd2(Vec3 *dest, const Vec3 *a, real_t k1, 
              const Vec3 *b, real_t k2, const Vec3 *c);

#endif
