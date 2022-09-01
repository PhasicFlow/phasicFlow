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


#ifndef __multiTriSurface_H__
#define __multiTriSurface_H__


#include "triSurface.H"
#include "VectorDuals.H"

namespace pFlow
{


class multiTriSurface
:
	public triSurface
{
protected:

	// - the index of last point of each triSurface 
	int32Field_HD 	lastPointIndex_;

	// - the index of the last vertex of each triSurface 
	int32Field_HD   lastVertexIndex_;

	// - name of each surface 
	wordField 		surfaceNames_;

	int32Field_HD 	surfaceNumPoints_;

	int32Vector_HD  pointsStartPos_;

	int32Field_HD   surfaceNumVertices_;

	int32Vector_HD  verticesStartPos_;

	int32 			numSurfaces_ = 0;

	void        calculateVars();

public:

	// - type info
	TypeNameNV("multiTriSurface");

	//// - Constructors

		// - emtpy
		multiTriSurface();

		multiTriSurface(const multiTriSurface&) = default;

		multiTriSurface& operator = (const multiTriSurface&) = default;

		multiTriSurface(multiTriSurface&&) = delete;

		multiTriSurface& operator = (multiTriSurface&&) = delete;		

		~multiTriSurface() = default;

	//// - Methods

		bool addTriSurface(const word& name, const triSurface& tSurf);

		bool addTriSurface(const word& name, const realx3x3Vector& vertices);

		int32 numSurfaces()const
		{
			return numSurfaces_;
		}

		void clear()
		{
			triSurface::clear();

			lastPointIndex_.clear();
			surfaceNames_.clear();
		}

		
		const auto& pointsStartPos()const
		{
			return pointsStartPos_;
		}

		const auto& verticesStartPos()const
		{
			return verticesStartPos_;
		}

		const auto& surfaceNumPoints()const
		{
			return surfaceNumPoints_;
		}

		auto& surfaceNumPoints()
		{
			return surfaceNumPoints_;
		}

		int32 surfNumPoints(int32 i)const
		{
			return surfaceNumPoints_[i];
		}

		int32 surfNumTriangles(int32 i)const
		{
			return surfaceNumVertices_[i];
		}

		int32 surfSize(int32 i)const
		{
			return surfNumTriangles(i);
		}

		word surfaceName(int32 i)const
		{
			return surfaceNames_[i];
		}

	//// - IO operations

		bool readMultiTriSurface(iIstream& is);

		bool writeMultiTriSurface(iOstream& os)const;

		bool read(iIstream& is)
		{
			return readMultiTriSurface(is);
		}

		bool write(iOstream& os)const
		{
			return writeMultiTriSurface(os);
		}

};

inline iIstream& operator >> (iIstream & is, multiTriSurface & tri )
{
	if(!tri.readMultiTriSurface(is))
	{
	 	ioErrorInFile(is.name(), is.lineNumber())<<
	 	"  error in reading multiTriSurface from file.\n";
	 	fatalExit;
	}
	return is;
}

inline iOstream& operator << (iOstream& os, const multiTriSurface& tri)
{
	if( !tri.writeMultiTriSurface(os) )
	{
		ioErrorInFile(os.name(), os.lineNumber())<<
	 	"  error in writing multiTriSurface to file.\n";
	 	fatalExit;
	}
	return os;
}

}


#endif
