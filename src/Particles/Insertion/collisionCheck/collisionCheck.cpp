#include "collisionCheck.hpp"

bool
pFlow::collisionCheck::build()
{
	fill(head_, static_cast<uint32>(-1));
	fill(next_, static_cast<uint32>(-1));

	for (auto i = 0uL; i < position_.size(); i++)
	{
		if(!searchBox_.isInside(position_[i]))
        {
            fatalErrorInFunction<<
            "Point "<< position_[i]<< "is not in search box"<<endl;
        }

        const auto ind                   = pointIndex(position_[i]);
		next_[i]                         = head_(ind.x(), ind.y(), ind.z());
		head_(ind.x(), ind.y(), ind.z()) = static_cast<uint32>(i);
	}

    return true;
}

pFlow::collisionCheck::collisionCheck(
  box                 sBox,
  real                dx,
  const realx3Vector& pos,
  const realVector&   diam
)
  : searchBox_(sBox),
    dx_(dx),
    nCells_((sBox.maxPoint() - sBox.minPoint()) / dx + realx3(1.0)),
    position_(pos),
    diameters_(diam),
    next_("next", pos.size()),
    head_("head", nCells_.x(), nCells_.y(), nCells_.z())
{
	if(!build())
    {
        fatalExit;
    }
}

bool
pFlow::collisionCheck::checkPoint(const realx3& p, const real d) const
{
    
    if(!searchBox_.isInside(p)) return false;

    const auto ind = pointIndex(p);
    
    uint32 n = head_(ind.x(), ind.y(), ind.z());
    while( n != -1)
    {
        if( (position_[n]-p).length() - 0.5*(diameters_[n]+d )<0.0 )
        {
            return false;
        }
        n = next_[n];
    }
	return true;
}

bool
pFlow::collisionCheck::mapLastAddedParticle()
{
    size_t n = position_.size()-1;
    if( next_.size() != n )
    {
        fatalErrorInFunction<<
        "size mismatch of next and position"<<endl;
        return false;
    }
    next_.push_back(-1);
    const auto& p = position_[n];

    if(!searchBox_.isInside(p))
    {
        fatalErrorInFunction<< 
        "Point "<< p <<" is not inside the search box"<<endl;
        return false;
    }

    const auto ind = pointIndex(p);

    next_[n]       = head_(ind.x(), ind.y(), ind.z());
    head_(ind.x(), ind.y(), ind.z()) = static_cast<uint32>(n);

	return true;
}
