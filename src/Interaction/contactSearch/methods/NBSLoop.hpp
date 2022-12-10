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
	auto d_m = sizeRatio_* diameter_[m];
	
	// the same cell
	n = this->next_(m);
	
	while(n >-1)
	{
		
		auto p_n = this->pointPosition_[n];
		auto d_n = sizeRatio_*diameter_[n];
		
		if( sphereSphereCheck(p_m, p_n, d_m, d_n) )
		{
			auto ln = n;
			auto lm = m;

			if(lm>ln) Swap(lm,ln);
			if( auto res = pairs.insert(lm,ln); res <0)
			{
				getFullUpdate++;
			}
		}
		
		n = this->next_(n);
	}

	// neighbor cells
	CellType neighborCell;
	for(int32 ni=0; ni<13; ni++)
	{
		if(ni==0)		neighborCell = currentCell + CellType( 0, 0,-1);
		else if(ni==1)	neighborCell = currentCell + CellType(-1, 0,-1);
		else if(ni==2)	neighborCell = currentCell + CellType(-1, 0, 0); 
		else if(ni==3)	neighborCell = currentCell + CellType(-1, 0, 1); 
		else if(ni==4)	neighborCell = currentCell + CellType( 0,-1,-1); 
		else if(ni==5)	neighborCell = currentCell + CellType( 0,-1, 0); 
		else if(ni==6)	neighborCell = currentCell + CellType( 0,-1, 1); 
		else if(ni==7)	neighborCell = currentCell + CellType(-1,-1,-1); 
		else if(ni==8)	neighborCell = currentCell + CellType(-1,-1, 0); 
		else if(ni==9)	neighborCell = currentCell + CellType(-1,-1, 1);
		else if(ni==10)	neighborCell = currentCell + CellType( 1,-1,-1);  
		else if(ni==11)	neighborCell = currentCell + CellType( 1,-1, 0);  
		else if(ni==12)	neighborCell = currentCell + CellType( 1,-1, 1);  
		 
		if( this->isInRange(neighborCell) )
		{
			
			n = this->head_(neighborCell.x(), neighborCell.y(), neighborCell.z());
			while( n>-1)
			{
				
				auto p_n = this->pointPosition_[n];
				auto d_n = sizeRatio_*diameter_[n];
				
				if(sphereSphereCheck(p_m, p_n, d_m, d_n))
				{
					auto ln = n;
					auto lm = m;
					if(lm>ln) Swap(lm,ln);
					if( auto res = pairs.insert(lm,ln); res <0)
					{
						getFullUpdate++;
					}
				}
				n = this->next_[n];
			}
		}

	}
	m = this->next_[m];
}

