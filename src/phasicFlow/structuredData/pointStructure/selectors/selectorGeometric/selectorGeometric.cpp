


template<typename GeomType>
bool
pFlow::selectorGeometric<GeomType>::selectPoints()
{
	
	selectedPoints_.clear();

	auto maskH = pStruct().activePointsMaskHost();
	const auto aRange = maskH.activeRange();
	auto pPos = pStruct().pointPositionHost();
	
	for(uint32 i=aRange.start(); i<aRange.end();i++)
	{
		if(maskH.isActive(i)&& pRegion_.isInside( pPos[i] ))
		{
			selectedPoints_.push_back(i);
		}
	}

	return true;
}

template<typename GeomType>
pFlow::selectorGeometric<GeomType>::selectorGeometric(
  const pointStructure& pStruct,
  const dictionary&     dict
)
  : pStructSelector(pStruct, dict),
    type_(dict.getVal<word>("selector")),
    pRegion_(type_, dict.subDict(type_ + "Info"))
{

	if(!selectPoints())
	{
		fatalExit;
	}
}

