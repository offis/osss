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

#include "objects.hh"

int sphereIntersect(const Sphere *sphere, const Ray *ray, 
                    IntersectionResult *result)
{
  
  /* A = 1 due to normalisation of the direction*/
  /* B = -2 * (Xd * (X0 - Xc) + Yd * (Y0 - Yc) + Zd * (Z0 - Zc)) */
  /*          ^^^^^^^^^^^^^^ */
  #define MULT_HELPER(coordPart) \
      REAL_MULT( \
        ray->m_direction.coordPart, \
        REAL_SUB(ray->m_origin.coordPart,  \
                 sphere->m_center.coordPart) \
      )  
  real_t  B =
    REAL_MULT_BY_MINUS_2(
      REAL_SUM3(
        MULT_HELPER(x),
        MULT_HELPER(y),
        MULT_HELPER(z))
    );

  #undef MULT_HELPER  


  /* C = (X0 - Xc)^2 + (Y0 - Yc)^2 + (Z0 - Zc)^2 - Sr^2 */
  /*     ^^^^^^^^^^^ */  
  #define SQUARE_HELPER(coordPart) \
    REAL_SQUARE( \
      REAL_SUB( \
        ray->m_origin.coordPart, \
        sphere->m_center.coordPart) \
    )

  real_t  C = 
    REAL_SUB(
      REAL_SUM3(
        SQUARE_HELPER(x),
        SQUARE_HELPER(y),
        SQUARE_HELPER(z)),
      REAL_SQUARE(sphere->m_radius));
        
  #undef SQUARE_HELPER


  real_t  discr = 
    REAL_SUB(
      REAL_SQUARE(B),
      REAL_MULT_BY_4(C));

  if (REAL_IS_NEGATIVE(discr))
  {
    /* The ray passes the sphere => no result */
    return 0;
  }
  
  
  real_t  sqRoot = REAL_SQRT(discr);
  
  real_t  t0 = REAL_DIV_BY_2(REAL_SUB(B, sqRoot));
  
  real_t  final_t;
  
  if (REAL_IS_NEGATIVE(t0))
  {
    real_t  t1 = REAL_DIV_BY_2(REAL_ADD(B, sqRoot));
    if (REAL_IS_NEGATIVE(t1))
    {
      /* The *line* intersects with the sphere, but the intersection point(s)
         are located before the *ray's* origin
       */
      return 0;
    }
    final_t = t1;
  }
  else
  {
    final_t = t0;
  }
  
  result->m_distance = final_t;
  
   /* [x0 + xd * ti ,  y0 + yd * ti,  z0 + zd * ti]  */
   /*  ^^^^^^^^^^^^ */
  #define POINT_HELPER(coordPart) \
    result->m_intersectionPoint.coordPart = \
      REAL_ADD( \
        ray->m_origin.coordPart, \
        REAL_MULT( \
          ray->m_direction.coordPart, \
          final_t) \
      );
      
  POINT_HELPER(x)
  POINT_HELPER(y)
  POINT_HELPER(z)

  #undef POINT_HELPER      
  
  /* [(xi - xc)/Sr,   (yi - yc)/Sr,   (zi - zc)/Sr] */
  /*  ^^^^^^^^^^^^ */
  #define NORMAL_HELPER(coordPart) \
    result->m_normal.coordPart =  \
      REAL_DIV( \
        REAL_SUB( \
          result->m_intersectionPoint.coordPart, \
          sphere->m_center.coordPart), \
        sphere->m_radius \
      );
    	
  NORMAL_HELPER(x)
  NORMAL_HELPER(y)
  NORMAL_HELPER(z)
    
  #undef NORMAL_HELPER    
  
  return 1;
}


void
initSphereObject(Object *dest, const Sphere *sphere, Color c)
{
  dest->m_intersectionFunction = (pIntersectionFunction) &sphereIntersect;
  dest->m_geometry = sphere;  
  dest->m_color = c;
}
