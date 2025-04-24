/*------------------------------- phasicFlow ---------------------------------
      O        C enter of
     O O       E ngineering and
    O   O      M ultiscale modeling of
   OOOOOOO     F luid flow
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------
Licence:
  This file is part of phasicFlow code. It is a free software for simulating
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions.

  phasicFlow is distributed to help others in their research in the field of
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/

#ifndef __fieldFunctions_hpp__
#define __fieldFunctions_hpp__

#include "types.hpp"
#include "span.hpp"

namespace pFlow::postprocessData
{

template<typename T>
inline
void funcCast(span<T> src, span<real> dst)
{
    for(uint32 i=0; i<src.size(); i++)
    {
        dst[i] = static_cast<real>(src[i]);
    }
}

template<typename T>
inline
void funcAbs(span<T> src, span<real> dst)
{
    for(uint32 i=0; i<src.size(); i++)
    {
        dst[i] = abs(src[i]);
    }
}

template<typename T>
inline
void funcSquare(span<T> src, span<real> dst)
{
    for( uint32 i=0; i<src.size(); i++)
    {
        dst[i] = pow(static_cast<real>(src[i]),2);
    }
}

template<typename T>
inline
void funcCube(span<T> src, span<real> dst)
{
    for( uint32 i=0; i<src.size(); i++)
    {
        dst[i] = pow(static_cast<real>(src[i]),3);
    }
}

template<typename T>
inline
void funcSquareRoot(span<T> src, span<real> dst)
{
    for( uint32 i=0; i<src.size(); i++)
    {
        dst[i] = sqrt(static_cast<real>(src[i]));
    }
}

template<VectorType T> 
inline
void funcMagnitude(span<T> src, span<real> dst)
{
    for( uint32 i=0; i<src.size(); i++)
    {
        dst[i] = src[i].length();
    }
}

template<VectorType T>
inline
void funcMagnitudeSquare(span<T> src, span<real> dst)
{
    for( uint32 i=0; i<src.size(); i++)
    {
        dst[i] = dot(src[i],src[i]);
    }
}

template<VectorType T>
inline
void funcMagnitudeCube(span<T> src, span<real> dst)
{
    for( uint32 i=0; i<src.size(); i++)
    {
        dst[i] = pow(src[i].length(),3);
    }
}

template<VectorType T>
inline 
void funcMagnitudeSquareRoot(span<T> src, span<real> dst)
{
    for( uint32 i=0; i<src.size(); i++)
    {
        dst[i] = sqrt(src[i].length());
    }
}

inline
void funcComponent(span<realx3> src, span<real> dst, char component)
{
    for( uint32 i=0; i<src.size(); i++)
    {
        switch(component)
        {
            case 'x':
                dst[i] = src[i].x();
                break;
            case 'y':
                dst[i] = src[i].y();
                break;
            case 'z':
                dst[i] = src[i].z();
                break;
        }
    }
}

inline
void funcComponent(span<realx4> src, span<real> dst, char component)
{
    for( uint32 i=0; i<src.size(); i++)
    {
        switch(component)
        {
            case 'x':
                dst[i] = src[i].x();
                break;
            case 'y':
                dst[i] = src[i].y();
                break;
            case 'z':
                dst[i] = src[i].z();
                break;
            case 'w':
                dst[i] = src[i].w();
                break;
        }
    }
}

} // namespace pFlow

#endif //__fieldFunctions_hpp__