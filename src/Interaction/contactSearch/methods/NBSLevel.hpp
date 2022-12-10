#ifndef __NBSLevel_hpp__
#define __NBSLevel_hpp__


#include "NBSLevel0.hpp"


namespace pFlow
{

INLINE_FUNCTION_HD
int32x3 mapIndexLevels(const int32x3& ind, int32 lowerLevel, int32 upperLevel);

template<typename executionSpace>
class
NBSLevel
:
	public NBSLevel0<executionSpace>
{
public:

	using NBSLevel0Type	= NBSLevel0<executionSpace>;

	using cellIterator 	= typename NBSLevel0Type::cellIterator;

	using IdType 		= typename NBSLevel0Type::IdType;
	
	using IndexType 	= typename NBSLevel0Type::IndexType;

	using Cells 		= typename NBSLevel0Type::Cells;

	using CellType 		= typename Cells::CellType;

	using execution_space 	= typename NBSLevel0Type::execution_space;

	using memory_space 		= typename NBSLevel0Type::memory_space;

	using mdrPolicyFindPairs = typename NBSLevel0Type::mdrPolicyFindPairs;

	using HeadType 			= typename NBSLevel0Type::HeadType;

	using NextType 			= typename NBSLevel0Type::NextType;

	template<typename exeSpace>
	friend class NBSLevels;

protected:

	int32 									level_ = 0;

	
public:
	
	TypeInfoNV("NBSLevel0");

	INLINE_FUNCTION_HD
	NBSLevel(){}

	NBSLevel(
		int32 	lvl,
		const 	box& domain,
		real 	cellSize,
		real 	sizeRatio,
		const 	ViewType1D<realx3, memory_space>& position,
		const 	ViewType1D<real, memory_space>& diam)
	:
		NBSLevel0Type(
			domain,
			cellSize,
			sizeRatio,
			position,
			diam,
			lvl==0),
		level_(lvl)
	{}

	INLINE_FUNCTION_HD
	NBSLevel(const NBSLevel&) = default;
	
	INLINE_FUNCTION_HD
	NBSLevel& operator = (const NBSLevel&) = default;

	INLINE_FUNCTION_HD
	~NBSLevel() = default;

	INLINE_FUNCTION_HD
	auto level()const
	{
		return level_;
	}

	template<typename PairsContainer>	
	INLINE_FUNCTION_H
	int32 findPairsCountCross(PairsContainer& pairs, NBSLevel& upperLevel)
	{

		mdrPolicyFindPairs
			mdrPolicy(
				{0,0,0},
				{this->nx(),this->ny(),this->nz()} );

		int32 notInsertedPairs;

		Kokkos::parallel_reduce	(
			"NBSLevel::findPairsCountCross",
			mdrPolicy,
			CLASS_LAMBDA_HD(int32 i, int32 j, int32 k, int32& getFullUpdate){
				#include "NBSCrossLoop.hpp"
			}, notInsertedPairs);
			
		return notInsertedPairs;	
	}
	 
	
};

INLINE_FUNCTION_HD
int32x3 mapIndexLevels( const int32x3& ind, int32 lowerLevel, int32 upperLevel)
{
	int32 a = pow(2, static_cast<int32>(upperLevel-lowerLevel));
	return ind/a;
}


}

#endif
