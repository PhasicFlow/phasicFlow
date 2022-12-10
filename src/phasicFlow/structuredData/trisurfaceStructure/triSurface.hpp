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


#ifndef __triSurface_hpp__
#define __triSurface_hpp__

#include "eventSubscriber.hpp"
#include "types.hpp"
#include "Vectors.hpp"
#include "VectorSingles.hpp"
#include "Fields.hpp"

namespace pFlow
{


class iIstream;
class iOstream;

class triSurface
:
	public eventSubscriber
{
public:
	
	class triangleAccessor
	{
	public:

	protected:
		int32 		numPoints_;
		
		int32 		numTriangles_;

		deviceViewType1D<realx3> dPoints_;
		
		deviceViewType1D<int32x3>  dVectices_;
	public:

		INLINE_FUNCTION_H
		triangleAccessor(
			int32 	numPoints,
			deviceViewType1D<realx3> points,
			int32 	numTrianlges,
			deviceViewType1D<int32x3> vertices )
		:
			numPoints_(numPoints),
			numTriangles_(numTrianlges),
			dPoints_(points),
			dVectices_(vertices)
		{}

		INLINE_FUNCTION_HD
		triangleAccessor(const triangleAccessor&)= default;

		INLINE_FUNCTION_HD
		triangleAccessor& operator=(const triangleAccessor&)= default;

		INLINE_FUNCTION_HD
		triangleAccessor(triangleAccessor&&)= default;

		INLINE_FUNCTION_HD
		triangleAccessor& operator=(triangleAccessor&&)= default;

		INLINE_FUNCTION_HD
		~triangleAccessor()=default;

		INLINE_FUNCTION_HD
		realx3x3 triangle(int32 i)const	{
			auto v = dVectices_[i];
			return realx3x3(
					dPoints_[v.x_],
					dPoints_[v.y_],
					dPoints_[v.z_]);
		}

		INLINE_FUNCTION_HD
		realx3x3 operator()(int32 i)const { return triangle(i);	}

		INLINE_FUNCTION_HD
		realx3x3 operator[](int32 i)const {	return triangle(i);	}

		INLINE_FUNCTION_HD
		int32 numPoints()const { return numPoints_; }

		INLINE_FUNCTION_HD
		int32 numTrianlges()const { return numTriangles_;}
	};

protected:

	/// points of triangles
	realx3Field_D points_;

	/// vectices indices of triangles 
	int32x3Field_D vertices_;

	/// area of each triangle 
	realField_D  	area_;

	int32 maxIndex_ = -1; 

	
	// protected methods 


		int32 addTriangle(const realx3x3 & tri, realx3Vector& points, int32x3Vector& vertices);

		// - check if the range of indices in vectors match 
		bool check();

public:

	// - type info
	TypeInfo("triSurface");

	//// - Constructors

		// - empty
		triSurface();

		// - construct from components
		triSurface(const realx3Vector& points, const int32x3Vector& vertices);

		// - construct from vertices of triangles 
		triSurface(const realx3x3Vector& triangles);

		virtual ~triSurface() = default;

	//// - Methods
		size_t numPoints() const
		{
			return points_.size();
		}

		size_t numTriangles()const
		{
			return vertices_.size();
		}

		size_t size()const
		{
			return numTriangles();
		}

		size_t capacity()const
		{
			return vertices_.capacity();
		}

		int32 maxIndex()const
		{
			return maxIndex_;
		}

		auto getTriangleAccessor()const
		{
			return triangleAccessor(
				numPoints(),
				points_.deviceVectorAll(),
				numTriangles(),
				vertices_.deviceVectorAll()
				);
		}

		const realx3Vector_D& points() const
		{
			return points_;
		}

		realx3Vector_D& points()
		{
			return points_;
		}

		const realVector_D& area()const
		{
			return area_;
		}

		realVector_D& area()
		{
			return area_;
		}		

		const realx3* pointsData_D()const
		{
			return points_.deviceVectorAll().data();
		}

		realx3* pointsData_D()
		{
			return points_.deviceVectorAll().data();
		}

		const int32x3Vector_D& vertices() const
		{
			return vertices_;
		}

		int32x3Vector_D& vertices()
		{
			return vertices_;
		}

		int32x3* verticesData_D()
		{
			return vertices_.deviceVectorAll().data();
		}

		const int32x3* verticesData_D()const
		{
			return vertices_.deviceVectorAll().data();
		}

		void clear()
		{
			points_.clear();
			vertices_.clear();
			area_.clear();
		}

		int32 calcMaxIndex()const;
		
	//// - IO operations 

		bool readTriSurface(iIstream& is);

		bool writeTriSurface(iOstream& os)const;

		bool read(iIstream& is)
		{
			return readTriSurface(is);
		}

		bool write(iOstream& os)const
		{
			return writeTriSurface(os);
		}

};

inline iIstream& operator >> (iIstream & is, triSurface & tri )
{
	if(!tri.readTriSurface(is))
	{
	 	ioErrorInFile(is.name(), is.lineNumber())<<
	 	"  error in reading triSurface from file.\n";
	 	fatalExit;
	}
	return is;
}

inline iOstream& operator << (iOstream& os, const triSurface& tri)
{
	if( !tri.writeTriSurface(os) )
	{
		ioErrorInFile(os.name(), os.lineNumber())<<
	 	"  error in writing triSurface to file.\n";
	 	fatalExit;
	}
	return os;
}


} // pFlow



#endif
