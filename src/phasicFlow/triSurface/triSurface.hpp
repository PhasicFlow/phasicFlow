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

class triSurface
:
	public IOobject
{
private:

	/// points of triangles
	realx3Field_D 		points_;

	/// vectices indices of triangles 
	uint32x3Field_D 	vertices_;

	/// area of each triangle 
	realField_D  		area_;

	/// normal vector of triangles 
	realx3Field_D 		normals_;

protected:

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

		bool appendTriSurface(const realx3x3Field_H& triangles);

		bool appendTriSurface(const realx3x3Vector& triangles);

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

		const auto& points() const
		{
			return points_;
		}

		auto& points()
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

		const auto& vertices() const
		{
			return vertices_;
		}

		auto& vertices()
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
