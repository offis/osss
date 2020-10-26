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

#ifndef TRACER_INCLUDED
#define TRACER_INCLUDED

#include "matrix.hh"
#include "color.hh"
#include "bitmap.hh"

typedef struct
{
  Vec3    m_origin;
  /* Must be normalised. */
  Vec3    m_direction;
} Ray;

typedef struct
{
  Vec3  m_intersectionPoint;
  /* The surface's normal at the intersection point. Must be normalised. */
  Vec3  m_normal;

  /* Distance from the ray origin to the intersection point */  
  real_t  m_distance;
  
} IntersectionResult;

/* Test if the two intersection results are equal up to REAL_EPS */
int nearIntersectionResults(const IntersectionResult *ir1, 
                            const IntersectionResult *ir2);

/* Geometric objects */

typedef int (*pIntersectionFunction)(const void *geometry, 
                                     const Ray *ray, IntersectionResult *ir);

typedef struct
{
  pIntersectionFunction   m_intersectionFunction;
  
  const void             *m_geometry;
  
  /* TODO: more sophisticated materials */
  Color   m_color;
} Object;


/* Light sources */

typedef const Vec3* (*pGetVecFunction)(const void *x);
typedef Color (*pGetLightIntensFunction)(const void *light, const Vec3 *p);

typedef struct
{
  pGetLightIntensFunction   m_getIntensity;
  pGetVecFunction           m_getOrigin;
  
  const void *m_light;
  
} Light;

#define MAX_OBJECTS 5
#define MAX_LIGHTS 5

typedef struct
{
  unsigned int  m_numObjects;
  unsigned int  m_numLights;
  Object  m_objects[MAX_OBJECTS];
  Light   m_lights[MAX_LIGHTS];

  Color   m_background;

} Scene;

/* The camera uses a left-handed coordinate system: 
   Looking from the camera into the world, x is right, y is up and z 
   goes out of the camera into the world.
 */
typedef struct
{
  Vec3    m_position;
  Vec3    m_lookAt;
  Vec3    m_worldUp;
  real_t  m_filmWidth;
  real_t  m_filmHeight;

} Camera;

void raytrace(const Scene *scene, const Camera *camera, 
              Bitmap *dest, const Tile *tile);

#endif
