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
#ifndef __pointStructure_hpp__
#define __pointStructure_hpp__

#include "IOobject.hpp"
#include "demComponent.hpp"
#include "internalPoints.hpp"
#include "simulationDomain.hpp"
#include "boundaryList.hpp"
#include "streams.hpp"



namespace pFlow
{


class pointStructure
:
    public IOobject,
    public demComponent,
	public internalPoints
{
protected:

	//// - data members 
	uniquePtr<simulationDomain> simulationDomain_ = nullptr;

	boundaryList 				boundaries_;
    

    bool setupPointStructure(const realx3Vector& points);

    bool initializePoints(const realx3Vector& points);

public:

		
	// - type info
	TypeInfo("pointStructure");
	

	//// - Constructors 

		/// an empty pointStructure, good for reading from file 
		pointStructure(systemControl& control);

		/// construct from point positions, assume all points are active
		pointStructure(
            systemControl& control,
			const realx3Vector& posVec);

		pointStructure(const pointStructure&) = delete;

		// - no move construct 
		pointStructure(pointStructure&&) = delete;

		
		pointStructure& operator=(const pointStructure&) = delete;

		// - no move assignment 
		pointStructure& operator=(pointStructure&&) = delete;

		// - destructor 
		virtual ~pointStructure() = default;

	// - Iteration methods

		/// In the time loop before iterate
		bool beforeIteration() override;

		/// @brief This is called in time loop. Perform the main calculations 
		/// when the component should evolve along time.
		bool iterate() override;

		/// This is called in time loop, after iterate.
		bool afterIteration() override;

    // - Access methods 

        auto& boundaries()
        {
            return boundaries_;
        }

        const auto& boundaries()const
        {
            return boundaries_;
        }

        auto& boundary(size_t i )
        {
            return boundaries_[i];
        }

        auto& boundary(size_t i)const
        {
            return boundaries_[i];
        }

		const auto& simDomain()const
		{
			return simulationDomain_();
		}

	// - IO methods 

		/// @brief read the point structure from the input stream.
		/// @param is: input stream
		/// @param iop: type of input pattern
		/// @return true on success
		bool read(iIstream& is, const IOPattern& iop) override;
		

		/// Write 
		bool write(iOstream& os, const IOPattern& iop)const override;
        
		
		
		
		



};


/*inline iOstream& operator << (iOstream& os, const pointStructure& ps )
{	
	if( !ps.write(os, IOPattern::AllProcessorsDifferent) )
	{
		ioErrorInFile(os.name(), os.lineNumber());
		fatalExit;
	}
	return os; 
}*/

} // pFlow




#endif //__pointStructure_hpp__
