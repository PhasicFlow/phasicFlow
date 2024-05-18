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


uint32 m = cellIter.start(i,j,k);
int32x3 currentCell(i,j,k);
uint32 n = mapperNBS::NoPos;

while( m !=  mapperNBS::NoPos)
{
	
	auto p_m = pointPos[m];	
	auto d_m = sizeRatio* diameter[m];
	
	// the same cell
	n = cellIter.getNext(m);
	
	while(n != mapperNBS::NoPos)
	{
		auto p_n = pointPos[n];
		auto d_n = sizeRatio*diameter[n];
		
		if( sphereSphereCheck(p_m, p_n, d_m, d_n) )
		{
			auto ln = n;
			auto lm = m;

			if(lm>ln) Swap(lm,ln);
			if( pairs.insert(lm,ln) == static_cast<uint32>(-1))
			{
				getFullUpdate++;
			}
		}
		
		n = cellIter.getNext(n);;
	}

	// neighbor cells
	int32x3 neighborCell;
	for(uint32 ni=0u; ni<13; ni++)
	{
		if(ni==0)		neighborCell = currentCell + int32x3( 0, 0,-1);
		else if(ni==1)	neighborCell = currentCell + int32x3(-1, 0,-1);
		else if(ni==2)	neighborCell = currentCell + int32x3(-1, 0, 0); 
		else if(ni==3)	neighborCell = currentCell + int32x3(-1, 0, 1); 
		else if(ni==4)	neighborCell = currentCell + int32x3( 0,-1,-1); 
		else if(ni==5)	neighborCell = currentCell + int32x3( 0,-1, 0); 
		else if(ni==6)	neighborCell = currentCell + int32x3( 0,-1, 1); 
		else if(ni==7)	neighborCell = currentCell + int32x3(-1,-1,-1); 
		else if(ni==8)	neighborCell = currentCell + int32x3(-1,-1, 0); 
		else if(ni==9)	neighborCell = currentCell + int32x3(-1,-1, 1);
		else if(ni==10)	neighborCell = currentCell + int32x3( 1,-1,-1);  
		else if(ni==11)	neighborCell = currentCell + int32x3( 1,-1, 0);  
		else if(ni==12)	neighborCell = currentCell + int32x3( 1,-1, 1);  
		 
		if( neighborCell.x()>=0 && neighborCell.y()>=0 && neighborCell.z()>=0 &&
			neighborCell.x()<nCells.x() && neighborCell.y()<nCells.y() && neighborCell.z()<nCells.z() )
		{
			
			n = cellIter.start(neighborCell.x(), neighborCell.y(), neighborCell.z());
			while(n != mapperNBS::NoPos)
			{
				
				auto p_n = pointPos[n];
				auto d_n = sizeRatio*diameter[n];
				
				if(sphereSphereCheck(p_m, p_n, d_m, d_n))
				{
					auto ln = n;
					auto lm = m;
					if(lm>ln) Swap(lm,ln);
					if( pairs.insert(lm,ln) == static_cast<uint32>(-1))
					{
						getFullUpdate++;
					}
				}
				n = cellIter.next(n);
			}
		}
	}
	m = cellIter.next(m);
}

