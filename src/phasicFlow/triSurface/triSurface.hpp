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


#include "Vectors.hpp"
#include "Fields.hpp"
#include "IOobject.hpp"

namespace pFlow
{


class iIstream;
class iOstream;
class repository;


class triangleAccessor
{
private:
	uint32 		numPoints_;
	
	uint32 		numTriangles_;

	deviceViewType1D<realx3> dPoints_;
	
	deviceViewType1D<uint32x3>  dVectices_;
public:

	INLINE_FUNCTION_H
	triangleAccessor(
		uint32 	numPoints,
		deviceViewType1D<realx3> points,
		uint32 	numTrianlges,
		deviceViewType1D<uint32x3> vertices )
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
	realx3x3 triangle(uint32 i)const	{
		auto v = dVectices_[i];
		return realx3x3(
				dPoints_[v.x_],
				dPoints_[v.y_],
				dPoints_[v.z_]);
	}

	INLINE_FUNCTION_HD
	realx3x3 operator()(uint32 i)const { return triangle(i);	}

	INLINE_FUNCTION_HD
	realx3x3 operator[](uint32 i)const { return triangle(i);	}

	INLINE_FUNCTION_HD
	uint32 numPoints()const { return numPoints_; }

	INLINE_FUNCTION_HD
	uint32 numTrianlges()const { return numTriangles_;}
};



class triSurface
:
	public IOobject
{
private:

	/// points of triangles
	realx3Field_D 		points_{"points", "points"};

	/// vectices indices of triangles 
	uint32x3Field_D 	vertices_{"vertices", "vertices"};

	/// area of each triangle 
	realField_D  		area_{"area", "area"};

	/// normal vector of triangles 
	realx3Field_D 		normals_{"normals", "normals"};

protected:
	
	bool calculateNormals();

	bool calculateArea();

	bool append(const realx3x3Field_H& triangles);

	bool append(const realx3x3Vector& triangles);


	// to be used by multiTriSurface
	triSurface(const objectFile& obj, repository* owner);

public:

	// - type info
	TypeInfo("triSurface");

	triSurface(
		const objectFile& objf, 
		repository* owner, 
		const triSurface& surf);

	//// - Constructors
		
		// - construct from vertices of triangles 
		explicit triSurface(const realx3x3Field_H& triangles, repository* owner=nullptr);

		// - construct from components
		//triSurface(const realx3Vector& points, const uint32x3Vector& vertices);		

		~triSurface() override = default;


	//// - Methods
		uint32 numPoints() const
		{
			return points_.size();
		}

		uint32 size()const
		{
			return vertices_.size();
		}

		uint32 capacity()const
		{
			return vertices_.capacity();
		}

		const realx3Field_D& points() const
		{
			return points_;
		}

		realx3Field_D& points()
		{
			return points_;
		}

		const auto& area()const
		{
			return area_;
		}

		auto& area()
		{
			return area_;
		}		

		const uint32x3Field_D& vertices() const
		{
			return vertices_;
		}

		uint32x3Field_D& vertices()
		{
			return vertices_;
		}

		auto& normals()
		{
			return normals_;
		}

		const auto& normals()const
		{
			return normals_;
		}

		void clear()
		{
			points_.clear();
			vertices_.clear();
			area_.clear();
			normals_.clear();
		}

		/// Obtain an object for accessing triangles 
		auto getTriangleAccessor()const
		{
			return triangleAccessor( 
				points_.size(),
				points_.deviceViewAll(),
				vertices_.size(),
				vertices_.deviceViewAll());
		}
		
	//// - IO operations 
		
		bool read(iIstream& is, const IOPattern& iop) override;

		bool write(iOstream& os, const IOPattern& iop)const override;
};

inline iIstream& operator >> (iIstream & is, triSurface & tri )
{
	if(!tri.read(is, IOPattern::AllProcessorsDifferent))
	{
	 	ioErrorInFile(is.name(), is.lineNumber())<<
	 	"  error in reading triSurface from file.\n";
	 	fatalExit;
	}
	return is;
}

inline iOstream& operator << (iOstream& os, const triSurface& tri)
{
	if( !tri.write(os, IOPattern::AllProcessorsDifferent) )
	{
		ioErrorInFile(os.name(), os.lineNumber())<<
	 	"  error in writing triSurface to file.\n";
	 	fatalExit;
	}
	return os;
}


} // pFlow



#endif
