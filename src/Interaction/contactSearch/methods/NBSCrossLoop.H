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


int32 m = this->head_(i,j,k);
CellType currentCell(i,j,k);
int32 n = -1;

while( m > -1 )
{
	
	auto p_m = this->pointPosition_[m];	
	auto d_m = this->sizeRatio_* this->diameter_[m];
	
	int32x3 crossIndex = mapIndexLevels(
		int32x3(i,j,k),
		level_, 
		upperLevel.level());

	
	for(int32 ii = -1; ii<2; ii++)
	{
		for(int32 jj=-1; jj<2; jj++)
		{
			int32 kk=-1;
			while( kk < 2)
			{
				int32x3 nghbrCI = crossIndex + int32x3(ii,jj,kk);
				
				if( upperLevel.isInRange(nghbrCI) )
				{
					n = upperLevel.head_(
						nghbrCI.x(),
						nghbrCI.y(),
						nghbrCI.z());

					while( n >-1)
					{
						auto p_n = this->pointPosition_[n];
						auto d_n = this->sizeRatio_*this->diameter_[n];

						if( sphereSphereCheck(p_m, p_n, d_m, d_n) )
						{
							auto ln = n;
							auto lm = m;
							if(lm>ln) this->Swap(lm,ln);
							if( auto res = pairs.insert(lm,ln); res <0)
							{
								getFullUpdate++;
							}
						}

						n = this->next_[n];
					}

				}

				kk++;
			}
		}
	}

	m = this->next_[m];
}

