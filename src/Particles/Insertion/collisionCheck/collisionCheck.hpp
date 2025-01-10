
#ifndef __collisionCheck_hpp__
#define __collisionCheck_hpp__

#include "Vectors.hpp"
#include "VectorSingles.hpp"
#include "box.hpp"


namespace pFlow
{

class collisionCheck
{
private:

	box                           searchBox_;

	real                          dx_;

	int32x3                       nCells_;

    const realx3Vector&          position_;

	const realVector&            diameters_;
    
	uint32Vector                  next_;

	ViewType3D<uint32, HostSpace> head_;

    int32x3 pointIndex(const realx3& p)const
	{
		return int32x3( (p - searchBox_.minPoint())/dx_ );
	}

    bool build();

public:

	collisionCheck(
	  box                 sBox,
	  real                dx,
	  const realx3Vector& pos,
	  const realVector&   diam
	);

    bool checkPoint(const realx3& p, const real d)const;

    bool mapLastAddedParticle();
};

}

#endif //__collisionCheck_hpp__