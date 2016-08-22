#include "Nave.h"
#include "NFRandom.h"

namespace Nave { 

	static NFRandomInt s_RandomInt;
	static NFRandomDouble s_RandomFloat;

	NFRandomInt::NFRandomInt() 
	{
		SetSeed(timeGetTime());
	}

	NFRandomInt::NFRandomInt(unsigned long seed) : random(seed) 
	{
	}

	void NFRandomInt::SetSeed(unsigned long seed)
	{
		random.SetSeed(seed);
	}

	unsigned int NFRandomInt::Next() 
	{ 
		return random.Next(); 
	}

	unsigned int NFRandomInt::Next(unsigned int excludedMax) 
	{ 
		if(excludedMax == 0)
			return excludedMax;

		return Next(0, excludedMax); 
	}

	unsigned int NFRandomInt::Next(unsigned int includedMin, unsigned int excludedMax) 
	{ 
		if(includedMin == excludedMax)
			return includedMin;

		return random.Next(includedMin, excludedMax); 
	}


	NFRandomDouble::NFRandomDouble() 
	{
		SetSeed(timeGetTime());
	}

	NFRandomDouble::NFRandomDouble(unsigned long seed) : random(seed) 
	{
	}

	void NFRandomDouble::SetSeed(unsigned long seed)
	{
		random.SetSeed(seed);
	}

	double NFRandomDouble::Next() 
	{ 
		return random.Next(); 
	}


} 