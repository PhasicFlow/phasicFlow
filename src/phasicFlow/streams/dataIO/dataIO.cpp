
#include "dataIO.hpp"


pFlow::uniquePtr<pFlow::dataIO> pFlow::dataIO::create
(
	IOPattern::IOType iotype, 
	word exeMode
)
{
	auto ioMode = angleBracketsNames("dataIO", exeMode);

	if( wordvCtorSelector_.search(ioMode) )
	{
		auto objPtr = wordvCtorSelector_[ioMode] 
			(
				iotype,
				exeMode
			);
		return objPtr;
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< Yellow_Text(ioMode) << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			wordvCtorSelector_
		);
		fatalExit;
	}
	return nullptr;
}
