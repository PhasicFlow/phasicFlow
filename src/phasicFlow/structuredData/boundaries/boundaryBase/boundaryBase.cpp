


#include "boundaryBase.hpp"


pFlow::boundaryBase::boundaryBase
(
	const plane& 		bplane,
	uint32 				mirrorProc, 
	const word& 		name,
	const word& 		type,
	internalPoints& 	internal
)
:
boundaryPlane_(bplane),
name_(name),
type_(type),
mirrorProcessoNo_(mirrorProc),
internal_(internal)
{
	
}