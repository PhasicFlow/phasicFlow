#include "vibratingMotion.hpp"

void pFlow::vibratingMotion::impl_setTime
(
	uint32 iter, 
	real t, 
	real dt
)const
{
	auto motion = motionComponents_.deviceViewAll();
	Kokkos::parallel_for(
		"vibratingMotion::impl_setTime",
		deviceRPolicyStatic(0, numComponents_),
		LAMBDA_HD(uint32 i){
			motion[i].setTime(t);
		});
	Kokkos::fence();
}

pFlow::vibratingMotion::vibratingMotion
(
    const objectFile &objf,
    repository *owner
) 
: 
	fileDictionary(objf, owner)
{
    if(! getModel().impl_readDictionary(*this) )
	{
		fatalErrorInFunction;
		fatalExit;
	}
}

pFlow::vibratingMotion::vibratingMotion
(
	const objectFile &objf, 
	const dictionary &dict, 
	repository *owner
)
:
	fileDictionary(objf,dict,owner)
{
	if(! getModel().impl_readDictionary(*this) )
	{
		fatalErrorInFunction;
		fatalExit;
	}
}

bool pFlow::vibratingMotion::write(iOstream &os, const IOPattern &iop) const
{
    // a global dictionary
	dictionary newDict(fileDictionary::dictionary::name(), true);
	if( iop.thisProcWriteData() )
	{
		if( !getModel().impl_writeDictionary(newDict) || 
			!newDict.write(os))
		{
			fatalErrorInFunction<<
			" error in writing to dictionary "<< newDict.globalName()<<endl;
			return false;
		}
	}
    return true;
}
