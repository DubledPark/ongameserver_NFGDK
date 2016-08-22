/** 
 *  @file		NFPseudoRandom.h
 *  @brief		랜덤 생성기
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include <windows.h>

namespace Nave { namespace Private {
	
	class NFMersenneTwister 
	{ 
	public:
		// default constructor: uses default seed only if this is the first instance
		NFMersenneTwister(BOOL useCurrentTime=TRUE);
		
		// constructor with 32 bit int as seed
		NFMersenneTwister(unsigned long s) { SetSeed(s); }
		// constructor with array of size 32 bit ints as seed
		NFMersenneTwister(const unsigned long* array, int size) { SetSeed(array, size); }
		// the two seed functions
		void SetSeed(unsigned long); // seed with 32 bit integer
		void SetSeed(const unsigned long*, int size); // seed with array
		// overload operator() to make this a generator (functor)
		//unsigned long operator()() { return GenerateRandom(); }

		//unsigned long Next() { return (*this)(); }
		~NFMersenneTwister() {} // destructor

	protected: // used by derived classes, otherwise not accessible; use the ()-operator
		unsigned long GenerateRandom() 
		{ // generate 32 bit random integer
			if (position == Constant1) GenerateState(); // new state vector needed
			// GenerateState() is split off to be non-inline, because it is only called once
			// in every 624 calls and otherwise irand() would become too big to get inlined
			unsigned long x = state[position++];
			x ^= (x >> 11);
			x ^= (x << 7) & 0x9D2C5680UL;
			x ^= (x << 15) & 0xEFC60000UL;
			return x ^ (x >> 18);
		}

	private:
		enum { Constant1 = 624, Constant2 = 397 };
		
		unsigned long state[Constant1]; // state vector array
		int position; // position in state array
		
		// private functions used to generate the pseudo random numbers
		unsigned long Twiddle(unsigned long u, unsigned long v) 
		{
			return (((u & 0x80000000UL) | (v & 0x7FFFFFFFUL)) >> 1)	^ ((v & 1UL) ? 0x9908B0DFUL : 0x0UL);
		} // used by GenerateState()
		void GenerateState(); // generate new state
		// make copy constructor and assignment operator unavailable, they don't make sense
		NFMersenneTwister(const NFMersenneTwister&); // copy constructor not defined
		void operator=(const NFMersenneTwister&); // assignment operator not defined
	};

	class NFPseudoRandomInt : public NFMersenneTwister 
	{
	public:
		NFPseudoRandomInt(BOOL useCurrentTime=TRUE): NFMersenneTwister(useCurrentTime) {}
		NFPseudoRandomInt(unsigned long seed) : NFMersenneTwister(seed) {}
		NFPseudoRandomInt(const unsigned long* seed, int size) : NFMersenneTwister(seed, size) {}

		unsigned long operator()() { return GenerateRandom(); }

		unsigned long Next() { return (*this)(); }
		
		unsigned long Next(unsigned long minval, unsigned long maxval) 
		{
			return minval+(Next()%(maxval-minval));
		}
	
	};

	// generates double floating point numbers in the half-open interval [0, 1)
	class NFPseudoRandomFloat : public NFMersenneTwister 
	{
	public:
		NFPseudoRandomFloat(BOOL useCurrentTime=TRUE) : NFMersenneTwister(useCurrentTime) {}
		NFPseudoRandomFloat(unsigned long seed) : NFMersenneTwister(seed) {}
		NFPseudoRandomFloat(const unsigned long* seed, int size) : NFMersenneTwister(seed, size) {}
		~NFPseudoRandomFloat() {}
		double operator()() 
		{
			return static_cast<double>(GenerateRandom()) * (1. / 4294967296.); 
		} // divided by 2^32

		double Next() { return (*this)(); }
		double Next(double mul) { return Next()*mul; }

	private:
		NFPseudoRandomFloat(const NFPseudoRandomFloat&); // copy constructor not defined
		void operator=(const NFPseudoRandomFloat&); // assignment operator not defined
	};

} } 