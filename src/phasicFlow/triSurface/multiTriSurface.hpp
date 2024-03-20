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


#ifndef __multiTriSurface_hpp__
#define __multiTriSurface_hpp__

#include "subscriber.hpp"
#include "triSurface.hpp"
#include "subSurface.hpp"

namespace pFlow
{

class multiTriSurface
:
	public triSurface,
	public subscriber
{
private:

	subSurfaceList subSurfaces_;

public:

	// - type info
	TypeInfo("multiTriSurface");

	//// - Constructors

		/// @brief  Construct from objectFile and owner repository.
		/// This is mainly used for reading from file.
		/// @param obj object file 
		/// @param owner owner repository 
		multiTriSurface(const objectFile& obj, repository* owner);

		/// @brief Construct from another multiTriSurface 
		multiTriSurface(
			const objectFile& objf, 
			repository* owner, 
			const multiTriSurface& surf);

		/// @brief Copy construct (default)
		multiTriSurface(const multiTriSurface&) = default;

		/// @brief Copy assignment (default)
		multiTriSurface& operator = (const multiTriSurface&) = default;

		/// @brief  No move construct
		multiTriSurface(multiTriSurface&&) = delete;

		/// @brief No move assignment 
		multiTriSurface& operator = (multiTriSurface&&) = delete;

		/// @brief  default destructor 
		~multiTriSurface() override = default;

	//// - Methods

		bool appendSurface(
			const word& name, 
			const realx3x3Vector& vertices);

		uint32 numSurfaces()const
		{
			return static_cast<uint32>(subSurfaces_.size());
		}

		const subSurfaceList& subSurfaces()const
		{
			return subSurfaces_;
		}
		
		rangeU32 subSurfaceRange(uint32 nSub)const
		{
			if( !(nSub < numSurfaces() ) )
			{
				fatalErrorInFunction;
				fatalExit;
			}
			return {subSurfaces_[nSub].start(),subSurfaces_[nSub].end()};
		}

		rangeU32 subSurfacePointRange(uint32 nSub)const
		{
			if( !(nSub < numSurfaces() ) )
			{
				fatalErrorInFunction;
				fatalExit;
			}
			return {
				subSurfaces_[nSub].pointStart(),
				subSurfaces_[nSub].pointEnd()};
		}

		/// @brief Clear the content of object
		void clear()
		{
			triSurface::clear();
			subSurfaces_.clear();
		}

		
		uint32 subSurfaceSize(uint32 nSub)const
		{
			if(nSub<numSurfaces())
			{
				return subSurfaceRange(nSub).numElements();
			}
			else
			{
				return 0;
			}
		}

		uint32 subSurfaceNumPoints(uint32 nSub)const
		{
			if(nSub<numSurfaces())
			{
				return subSurfacePointRange(nSub).numElements();
			}
			else
			{
				return 0;
			}
		}
		
		word subSurfaceName(uint32 nSub)const
		{
			if(nSub<numSurfaces())
			{
				return subSurfaces_[nSub].name();
			}
			else
			{
				return "";
			}
		}

	//// - IO operations

		

		bool read(iIstream& is, const IOPattern& iop)override;


		bool write(iOstream& os, const IOPattern& iop)const override;
		

};


inline iOstream& operator << (iOstream& os, const multiTriSurface& tri)
{
	if( !tri.write(os, IOPattern::AllProcessorsDifferent) )
	{
		ioErrorInFile(os.name(), os.lineNumber())<<
	 	"  error in writing multiTriSurface to file.\n";
	 	fatalExit;
	}
	return os;
}

} // pFlow


#endif
