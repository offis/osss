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

#include "tracer.hh"
#include <stdio.h>

/* Some "global" variables in order to avoid unnecessary parameter passing and
   hence save stack space */
static const Scene *g_scene;
static const Camera *g_camera;
static Bitmap *g_bitmap;
static const Tile *g_tile;

/* Some intermediate results which stay constant for a trace call */

/* The camera's left, up and look-at vectors in "world" space 
   g_filmBottomRight is the fictive lower right corner of the film within 
   a pin-hole camera. The film plane is locate -1.0d coordinate units 
   in g_camAtTarget direction.   
*/
static Vec3 g_camLeft,
            g_camUp,
            g_camAtTarget,
            g_filmBottomRight;

/* Determines how far we have to go in x/y-direction on the film 
   when moving by one pixel 
*/
static real_t g_xCoordUnitsPerPixel,
              g_yCoordUnitsPerPixel;

static
void initGlobals(const Scene *scene, const Camera *camera, 
                 Bitmap *dest, const Tile *tile)
{
  Vec3 filmCenter,
       camRight,
       camDown;
       
  real_t halfWidth = REAL_DIV_BY_2(camera->m_filmWidth),
         halfHeight= REAL_DIV_BY_2(camera->m_filmHeight);
  
  real_t r_numPixelsX = REAL_FROM_INT(dest->m_size.x_max - 
                                      dest->m_size.x_min + 1);
  real_t r_numPixelsY = REAL_FROM_INT(dest->m_size.y_max - 
                                      dest->m_size.y_min + 1);
  
  g_scene = scene;
  g_camera = camera;
  g_bitmap = dest;
  g_tile = tile;
  
  vec3Sub(&g_camAtTarget, &camera->m_lookAt, &camera->m_position);
  vec3Normalise(&g_camAtTarget);  
  vec3Sub(&filmCenter, &camera->m_position, &g_camAtTarget);
  vec3Cross(&g_camLeft, &g_camAtTarget, &camera->m_worldUp);
  vec3Normalise(&g_camLeft);
  vec3Cross(&camDown, &g_camAtTarget, &g_camLeft);
  vec3Normalise(&camDown);

  vec3Scale(&camRight, REAL_MINUS_1, &g_camLeft);
  vec3Scale(&g_camUp, REAL_MINUS_1, &camDown);
  
  vec3ScaleAdd2(&g_filmBottomRight, &filmCenter, halfWidth, 
                &camRight, halfHeight, &camDown);
    
  g_xCoordUnitsPerPixel = REAL_DIV(
                            camera->m_filmWidth,
                            r_numPixelsX);
  
  g_yCoordUnitsPerPixel = REAL_DIV(
                            camera->m_filmHeight,
                            r_numPixelsY);
}

/*
  Move the origin a little bit into direction in order to avoid unintended self
  intersection for rays which directly start from an object, 
  e.g. relfection rays.
 */
static
void adjustRaySlightly(Ray *ray)
{
  vec3ScaleAdd(&ray->m_origin, 
               &ray->m_origin, 
               REAL_SMALL, 
               &ray->m_direction);
}

/* Shoot a ray from the camera into the world */
static
void getInitialRay(Bitmap_coord_t x, Bitmap_coord_t y, Ray *ray)
{
  Vec3 filmPoint;

  /* Alternatively, we could pre-scale the g_camLeft/Up vectors, most probably
     gaining performance, but losing accuracy. */  
  real_t  x_coord = REAL_MULT(REAL_FROM_INT(x), g_xCoordUnitsPerPixel),
          y_coord = REAL_MULT(REAL_FROM_INT(y), g_yCoordUnitsPerPixel);

  vec3ScaleAdd2(&filmPoint, &g_filmBottomRight, x_coord, 
                &g_camLeft, y_coord, &g_camUp);

  vec3Sub(&ray->m_direction, &g_camera->m_position, &filmPoint);

  vec3Normalise(&ray->m_direction);

  ray->m_origin = g_camera->m_position;
}

/* Find the object which is hit by ray and is the closest one to the ray's 
   origin. Sets *nearestObject to NULL if there is no hit.
 */
static
void
findNearestHit(const Ray* ray, const Object **nearestHitObject, 
               IntersectionResult *ir)
{
  unsigned int       objIndex;
  
  const Object       *curObject, 
                     *nearestHit=0;
                     
  IntersectionResult  curIntersection, 
                      nearestIntersection;

                      
  for (objIndex = 0; objIndex < g_scene->m_numObjects ; ++objIndex)
  {
    curObject = &g_scene->m_objects[objIndex];
    if (curObject->m_intersectionFunction(curObject->m_geometry, ray, 
                                          &curIntersection))
    {
      if (nearestHit == 0 || REAL_IS_LOWER(curIntersection.m_distance, 
                                           nearestIntersection.m_distance))
      {
        nearestHit = curObject;
        nearestIntersection = curIntersection;
      }      
    }
  }

  *nearestHitObject = nearestHit;
  if (nearestHit)
  {
    *ir = nearestIntersection;
  }
}

/* Check if there is no obstacle between the object and the light. 
   returns 1 if there is no obstacle
 */
static
int
objectIsIlluminatedBy(const Object *object, 
                      const Vec3 *pointOnObject, 
                      const Light *light,
                      Vec3  *vecObjToLight)
{
  unsigned int        objIndex;
  const Object       *curObject;
  Ray                 rayObjToLight;
  IntersectionResult  curIntersection;
  real_t              distanceToLight;
  
  rayObjToLight.m_origin = *pointOnObject;
  vec3Sub(&rayObjToLight.m_direction, light->m_getOrigin(light->m_light), 
          pointOnObject);
  distanceToLight = vec3Length(&rayObjToLight.m_direction);
  vec3Normalise(&rayObjToLight.m_direction);
  
  adjustRaySlightly(&rayObjToLight);
  
  for (objIndex = 0; objIndex < g_scene->m_numObjects ; ++objIndex)
  {
    curObject = &g_scene->m_objects[objIndex];

    if (curObject->m_intersectionFunction(curObject->m_geometry, 
                                          &rayObjToLight, 
                                          &curIntersection)==1 )
    {
      if (curIntersection.m_distance < distanceToLight)
      {
        return 0;
      }
    }                                            
  }
  
  *vecObjToLight = rayObjToLight.m_direction;
  
  return 1;
}

/* Determine object's color depending on the light sources and the material */
static 
Color shadeObject(const Object *object, const IntersectionResult *ir)
{
  unsigned int lightIndex;
  Color   result=colorBlack(),
          lightColor;
  const Light  *curLight;        
  Vec3    objToLight;
  real_t  intensFactor;

/*
  , 
          numLightsFactor = 
            REAL_DIV(
              REAL_1, 
              REAL_FROM_INT(g_scene->m_numLights));
*/
  
  if (g_scene->m_numLights == 0)
  {
    return object->m_color;
  }
  
  for (lightIndex=0; lightIndex < g_scene->m_numLights; ++lightIndex)
  {
    if (objectIsIlluminatedBy(object, 
                              &ir->m_intersectionPoint, 
                              &g_scene->m_lights[lightIndex],
                              &objToLight))
    {
      curLight   = &g_scene->m_lights[lightIndex];
      lightColor = curLight->m_getIntensity(curLight->m_light, 
                                            &ir->m_intersectionPoint);
      /* lightColor = colorScaleSat(numLightsFactor, lightColor); */
      
      intensFactor = vec3Dot(&ir->m_normal, &objToLight);
      lightColor = colorScaleSat(intensFactor, lightColor);
      lightColor = colorMultSat(lightColor, object->m_color);
      result = colorAddSat(result, lightColor);
    }
  }
  
  return result;
}

/* Determine the reflected ray based on the incoming ray, the intersection point
   and the normal vector at the intersection point.
 */
static
void getReflectionRay(const Ray *incidentRay, 
                      const IntersectionResult *ir,
                      Ray *outgoingRay)
{
  real_t  dotProd = REAL_MULT(
                      REAL_MINUS_2,
                      vec3Dot(&incidentRay->m_direction, &ir->m_normal));

  vec3ScaleAdd(&outgoingRay->m_direction, 
               &incidentRay->m_direction, 
               dotProd,
               &ir->m_normal );
               
  vec3Normalise(&outgoingRay->m_direction);
  outgoingRay->m_origin = ir->m_intersectionPoint;
  adjustRaySlightly(outgoingRay);
}

/* Very naive ray tracer */
static
Color traceRay(const Ray *ray, unsigned int recursionsTodo)
{
 
  const Object       *nearestHit;                     
  IntersectionResult  nearestIntersection;
  Ray       reflectedRay;                      
  Color     result=colorBlack()/*g_scene->m_background*/,
            reflectedColor;
  
  findNearestHit(ray, &nearestHit, &nearestIntersection);
  
  if (nearestHit)
  {
    result = shadeObject(nearestHit, &nearestIntersection);
    if (recursionsTodo > 0)
    {
      getReflectionRay(ray, &nearestIntersection, &reflectedRay);
      reflectedColor = traceRay(&reflectedRay, recursionsTodo-1);
      /*reflectedColor = colorScaleSat(REAL_0_5, reflectedColor);*/
      /*result         = colorScaleSat(REAL_0_5, result); */
      result = colorAddSat(result, reflectedColor);
    }
  }
  
  return result;
}

void raytrace(const Scene *scene, const Camera *camera, 
              Bitmap *dest, const Tile *tile)
{
  Bitmap_coord_t x,y;
  Ray initialRay;
  
  initGlobals(scene, camera, dest, tile);
  
  for (y=tile->y_min; y <= tile->y_max; ++y)
  {
    for (x=tile->x_min; x <= tile->x_max; ++x)
    {
      getInitialRay(x,y, &initialRay);
      #if !defined(__cplusplus)
        setPixel(dest, x, y, traceRay(&initialRay, 1));
      #else
        dest->setPixel(x, y, traceRay(&initialRay, 1));
      #endif
    }
  }
}


/* 
 *  Tests
 */

#ifdef MT_TEST
int nearIntersectionResults(const IntersectionResult *ir1, 
                            const IntersectionResult *ir2)
{
  return   vec3sAreClose(&ir1->m_intersectionPoint, 
                         &ir2->m_intersectionPoint)
        && vec3sAreClose(&ir1->m_normal, &ir2->m_normal)
        && REALS_ARE_CLOSE(ir1->m_distance, ir2->m_distance);
}
#endif
