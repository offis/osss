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

#include "bmpWriter.hh"

#include "stdio.h"

typedef enum { BigEndian=0, LittleEndian=1, Native, Invalid } Endianness;

static Endianness g_nativeEndianness = Invalid;

static
Endianness
getNativeEndianness()
{
  int   i = 0x1234;
  char  *p = (char*)&i;
  
  return  (*p == 0x34) ? LittleEndian 
                       : BigEndian;
}

void swapEndianess(void *vData, int size)
{
  char *data = (char*)vData;
  char tmp;
  int i;
  for (i=0; i<size/2; ++i)
  {
    tmp = data[i];
    data[i] = data[size-1-i];
    data[size-i] = tmp;    
  }
}

void setEndianness(void *vData, int size, Endianness e)
{
  if (e != g_nativeEndianness)
  {
    swapEndianess(vData, size);
  }
}

static void writeByte(FILE *file, void *p)
{
  char *x = (char*)p;
  fwrite(x, 1, 1, file);
}

static void writeShort(FILE *file, short x, Endianness e)
{
  setEndianness(&x, sizeof(x), e);
  fwrite(&x, sizeof(x), 1, file);   
}

static void writeInt(FILE *file, int x, Endianness e)
{
  setEndianness(&x, sizeof(x), e);
  fwrite(&x, sizeof(x), 1, file);   
}

void writeBmpFile(const Bitmap *b, const char *fileName)
{
  FILE *bmpFile = fopen (fileName, "wb");
  Endianness fileEndianness = LittleEndian;
  int   width, height, fileSize;
  Bitmap_coord_t x,y;
  Color tmpColor;
  Color_base_t rVal,gVal,bVal;

  g_nativeEndianness = getNativeEndianness();
  
  width = b->m_size.x_max - b->m_size.x_min + 1;
  height = b->m_size.y_max - b->m_size.y_min + 1;
  fileSize = width * height * 3 + 54;

  /* File Header */
  /* "BM"               */ writeShort(bmpFile, 0x4d42, fileEndianness);
  /* file size in bytes */ writeInt(bmpFile, fileSize ,fileEndianness);
  /* reserved1          */ writeShort(bmpFile, 0, fileEndianness);
  /* reserved2          */ writeShort(bmpFile, 0, fileEndianness);
  /* bitmap offset      */ writeInt(bmpFile, 54,fileEndianness);

  /* Bitmap info header */
    
  /* header size        */ writeInt(bmpFile, 40 ,fileEndianness);
  /* width              */ writeInt(bmpFile, width ,fileEndianness);
  /* height             */ writeInt(bmpFile, height ,fileEndianness);
  /* number of planes   */ writeShort(bmpFile, 1 ,fileEndianness);
  /* bits per pixel     */ writeShort(bmpFile, 24 ,fileEndianness);
  /* compression        */ writeInt(bmpFile, 0 ,fileEndianness);
  /* image size         */ writeInt(bmpFile, 0 ,fileEndianness);
  /* pixel per meter x  */ writeInt(bmpFile, 0 ,fileEndianness);
  /* pixel per meter y  */ writeInt(bmpFile, 0 ,fileEndianness);
  /* # colormap entires */ writeInt(bmpFile, 0 ,fileEndianness);
  /* significant colors */ writeInt(bmpFile, 0 ,fileEndianness);

  for (y=b->m_size.y_min; y <= b->m_size.y_max; ++y)
  {
    for (x=b->m_size.x_min; x <= b->m_size.x_max; ++x)
    {
      /* BMP is stored upside down */
      #if !defined(__cplusplus)
        tmpColor = getPixel(b, x, b->m_size.y_max - y);
      #else
	tmpColor = b->getPixel(x, b->m_size.y_max - y);
      #endif
      colorSplit24(tmpColor, &rVal, &gVal, &bVal);
      writeByte(bmpFile, &bVal);
      writeByte(bmpFile, &gVal);
      writeByte(bmpFile, &rVal);
    }
  }
  
  fclose(bmpFile);
}
