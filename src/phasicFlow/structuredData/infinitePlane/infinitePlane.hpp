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

#ifndef __infinitePlane_hpp__
#define __infinitePlane_hpp__

#include "types.hpp"
//#include "dictionary.hpp"
#include "iIstream.hpp"
#include "iOstream.hpp"

namespace pFlow
{

class  infinitePlane
{
protected:
	
	/// normal vector 
	realx3 	normal_{1,0,0};
	
	/// distance value 
	real  	d_ = 0;

public:

	// - type info
	TypeInfoNV("infinitePlane");

	//// - Constructors 
		
		/// Default
		INLINE_FUNCTION_HD 
		infinitePlane(){}

		/// From components
		INLINE_FUNCTION_HD 
		infinitePlane(const realx3& normal, const real& d)
		:
			normal_(normal),
			d_(d)
		{}

		INLINE_FUNCTION_HD
		infinitePlane(const realx3& normal, const realx3& p)
		:
			normal_(normal),
			d_(-dot(normal,p))
		{}

		/// From 3 points
		infinitePlane(const realx3& p1, const realx3& p2, const realx3& p3);
		
		FUNCTION_HD
		infinitePlane(const infinitePlane&) = default;

		FUNCTION_HD
		infinitePlane(infinitePlane&&) = default;

		FUNCTION_HD
		infinitePlane& operator=(const infinitePlane&) = default;

		FUNCTION_HD
		infinitePlane& operator=(infinitePlane&&) = default;

		~infinitePlane()=default;

		/*FUNCTION_H
		infinitePlane(const dictionary& dict);

		FUNCTION_H
		infinitePlane(iIstream& is);*/
	

	//// - Methods

		INLINE_FUNCTION_HD
		real pointFromPlane(const realx3& p)const
		{
			return dot(normal_, p) + d_;
		}

		INLINE_FUNCTION_HD
		bool pointInPositiveSide(const realx3& p)const
		{
			return pointFromPlane(p)>=0;
		}

		INLINE_FUNCTION_HD
		bool inPositiveDistance(const realx3& p, real dist)const
		{	
			real d = pointFromPlane(p);
			return d >= 0.0 && d <= dist;
		}

		INLINE_FUNCTION_HD
		bool inNegativeDistance(const realx3& p, real dist)const
		{	
			real d = pointFromPlane(p);
			return d < 0.0 && d <= -dist;
		}


		INLINE_FUNCTION_HD
		bool pointInNegativeSide(const realx3& p)const
		{
			return pointFromPlane(p)<0;
		}

		INLINE_FUNCTION_HD
		bool pointOnPlane(const realx3& p)const
		{
			return equal(pointFromPlane(p),0.0);
		}

		INLINE_FUNCTION_HD
		realx3 projectPoint(const realx3& p)const
		{
			real t = -(dot(normal_, p) + d_);
			return t*normal_ + p;
		}

        INLINE_FUNCTION_HD
        bool parallel(const infinitePlane& pln)const
        {
            return  equal(normal_, pln.normal_) ||
                    equal(normal_,-pln.normal_);
        }

        INLINE_FUNCTION_HD
        bool parallelTouch(const infinitePlane& pln)const
        {
            if(equal(normal_, pln.normal_) && equal(d_, pln.d_))return true;
            if(equal(normal_,-pln.normal_) && equal(d_,-pln.d_))return true;
            return false;
        }

        INLINE_FUNCTION_HD
        const auto& normal()const
        {
            return normal_;
        }

        INLINE_FUNCTION_HD
        const auto& d()const
        {
            return d_;
        }

	//// - IO operation 

		FUNCTION_H
		bool write(iOstream& os)const;

		bool read(iIstream & is);

		/*FUNCTION_H
		bool read(iIstream & is);

		FUNCTION_H
		bool write(iOstream& os)const;

		FUNCTION_H
		bool read(const dictionary& dict);

		FUNCTION_H
		bool write(dictionary& dict)const;*/

		static bool validPlane3(
			const realx3& p1, 
			const realx3& p2, 
			const realx3& p3);
};

/*FUNCTION_H
iIstream& operator >>(iIstream& is, box& b);

FUNCTION_H
iOstream& operator << (iOstream& os, const box& b);

INLINE_FUNCTION_HD
box extendBox(const box& b, const realx3& dl)
{
	return box(b.minPoint()-dl , b.maxPoint()+dl);
}*/

}


#endif // __infinitePlane_hpp__
