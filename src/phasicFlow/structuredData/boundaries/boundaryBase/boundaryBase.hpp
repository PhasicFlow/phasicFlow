
#ifndef __boundaryBase_hpp__
#define __boundaryBase_hpp__


#include "VectorSingles.hpp"
#include "plane.hpp"



namespace pFlow
{

// forward 

class internalPoints;

class boundaryBase
{
public:

	enum DIRECTION: int8
	{
		
	};

protected:

	plane 		boundaryPlane_;

	word 		name_;

	word 		type_;

	uint32 		mirrorProcessoNo_;

	/// list of particles indices on device 
	uint32Vector_D 	indexList_;

	/// a reference to 
	internalPoints& internal_;

public:

	TypeInfo("boundaryBase");

	boundaryBase(
		const plane& 		bplane,
		uint32 				mirrorProc, 
		const word& 		name,
		const word& 		type,
		internalPoints& 	internal);

	boundaryBase(const boundaryBase&) = default;

	boundaryBase& operator=(const boundaryBase&) = default;

	boundaryBase(boundaryBase&&) = delete;

	boundaryBase& operator=(boundaryBase&&) = delete;

	virtual ~boundaryBase() = default;

	virtual bool update() = 0 ;

	virtual bool iterate(uint32 iterNum, real t) = 0;

	
};

}


#endif //__boundaryBase_hpp__