#include "Nave.h"
#include "NFProfile.h"
#include <assert.h>

namespace Nave { 
	
	NFProfile::ProfileSample			NFProfile::s_stSamples[NUM_PROFILE_SAMPLES];
	NFProfile::ProfileSampleHistory		NFProfile::s_stHistory[NUM_PROFILE_SAMPLES];
	
	float								NFProfile::s_fStartProfile = 0.0f;
	float								NFProfile::s_fEndProfile = 0.0f;
	float								NFProfile::s_fFrequency = 1.0f;

	BOOL								NFProfile::s_bInitProfile = FALSE;
	BOOL								NFProfile::s_bEndProfile = FALSE;
	WCHAR								NFProfile::s_strID[256];

	NFProfile::NFProfile(WCHAR* strID)
	{
		if(!s_bInitProfile)
		{
			ProfileInit();
		}

		wcscpy(s_strID, strID);
		ProfileBegin(s_strID);
	}

	NFProfile::~NFProfile()
	{
		EndProfile();
	}

	float NFProfile::GetTime()
	{
		LARGE_INTEGER CurrentTime;

		QueryPerformanceCounter(&CurrentTime);

		return ((float) CurrentTime.QuadPart) / s_fFrequency;
	}

	void NFProfile::ProfileInit()
	{
		unsigned int i;

		for( i=0; i<NUM_PROFILE_SAMPLES; i++ ) {
			s_stSamples[i].bValid = false;
			s_stHistory[i].bValid = false;
		}

		LARGE_INTEGER CurrentFrequency;

		QueryPerformanceFrequency(&CurrentFrequency);

		s_fFrequency = (float)CurrentFrequency.QuadPart;
		s_fStartProfile = GetTime();
		s_bInitProfile = TRUE;
	}

	void NFProfile::ProfileBegin( WCHAR* name )
	{
	   unsigned int i = 0;

	   while( i < NUM_PROFILE_SAMPLES && s_stSamples[i].bValid == true ) 
	   {
		  if( wcscmp( s_stSamples[i].szName, name ) == 0 ) 
		  {
			 //Found the sample
			 s_stSamples[i].iOpenProfiles++;
			 s_stSamples[i].iProfileInstances++;
			 s_stSamples[i].fStartTime = GetTime();
			 assert( s_stSamples[i].iOpenProfiles == 1 ); //max 1 open at once
			 return;
		   }
		   i++;	
	   }

	   if( i >= NUM_PROFILE_SAMPLES ) 
	   {
		  assert( !"Exceeded Max Available Profile Samples" );
		  return;
	   }

	   wcscpy( s_stSamples[i].szName, name );
	   s_stSamples[i].bValid = true;
	   s_stSamples[i].iOpenProfiles = 1;
	   s_stSamples[i].iProfileInstances = 1;
	   s_stSamples[i].fAccumulator = 0.0f;
	   s_stSamples[i].fStartTime = GetTime();
	   s_stSamples[i].fChildrenSampleTime = 0.0f;
	}

	void NFProfile::ProfileEnd( WCHAR* name )
	{
	   unsigned int i = 0;
	   unsigned int numParents = 0;

	   while( i < NUM_PROFILE_SAMPLES && s_stSamples[i].bValid == true )
	   {
		  if( wcscmp( s_stSamples[i].szName, name ) == 0 )
		  {  //Found the sample
			 unsigned int inner = 0;
			 int parent = -1;
			 float fEndTime = GetTime();
			 s_stSamples[i].iOpenProfiles--;

			 //Count all parents and find the immediate parent
			 while( s_stSamples[inner].bValid == true ) 
			 {
				if( s_stSamples[inner].iOpenProfiles > 0 )
				{  //Found a parent (any open profiles are parents)
				   numParents++;
				   if( parent < 0 )
				   {  //Replace invalid parent (index)
					  parent = inner;
				   }
				   else if( s_stSamples[inner].fStartTime >= s_stSamples[parent].fStartTime )
				   {  //Replace with more immediate parent
					  parent = inner;
				   }
				}
				inner++;
			 }

			 //Remember the current number of parents of the sample
			 s_stSamples[i].iNumParents = numParents;

			 if( parent >= 0 )
			 {  //Record this time in fChildrenSampleTime (add it in)
				s_stSamples[parent].fChildrenSampleTime += fEndTime - s_stSamples[i].fStartTime;
			 }

			 //Save sample time in accumulator
			 s_stSamples[i].fAccumulator += fEndTime - s_stSamples[i].fStartTime;
			 return;
		  }
		  i++;	
	   }
	}

	void NFProfile::StoreProfileInHistory( WCHAR* name, float percent )
	{
	   unsigned int i = 0;
	   float oldRatio;
	   float newRatio = 0.8f * 0.001f;
	   if( newRatio > 1.0f ) 
	   {
		  newRatio = 1.0f;
	   }
	   oldRatio = 1.0f - newRatio;

	   while( i < NUM_PROFILE_SAMPLES && s_stHistory[i].bValid == true ) 
	   {
		  if( wcscmp( s_stHistory[i].szName, name ) == 0 )
		  {  //Found the sample
			 s_stHistory[i].fAve = (s_stHistory[i].fAve*oldRatio) + (percent*newRatio);

			 if( percent < s_stHistory[i].fMin ) 
				s_stHistory[i].fMin = percent;
			 else 
				s_stHistory[i].fMin = (s_stHistory[i].fMin*oldRatio) + (percent*newRatio);

			 if( s_stHistory[i].fMin < 0.0f ) 
				s_stHistory[i].fMin = 0.0f;

			 if( percent > s_stHistory[i].fMax ) 
				s_stHistory[i].fMax = percent;
			 else 
				s_stHistory[i].fMax = (s_stHistory[i].fMax*oldRatio) + (percent*newRatio);

			 return;
		  }
		  i++;
	   }

	   if( i < NUM_PROFILE_SAMPLES )
	   {  //Add to history
		  wcscpy( s_stHistory[i].szName, name );
		  s_stHistory[i].bValid = true;
		  s_stHistory[i].fAve = s_stHistory[i].fMin = s_stHistory[i].fMax = percent;
	   }
	   else 
	   {
		  assert( !"Exceeded Max Available Profile Samples!");
	   }
	}

	void NFProfile::GetProfileFromHistory( WCHAR* name, float* ave, float* min, float* max )
	{
	   unsigned int i = 0;
	   while( i < NUM_PROFILE_SAMPLES && s_stHistory[i].bValid == true ) 
	   {
		  if( wcscmp( s_stHistory[i].szName, name ) == 0 )
		  {  //Found the sample
			 *ave = s_stHistory[i].fAve;
			 *min = s_stHistory[i].fMin;
			 *max = s_stHistory[i].fMax;
			 return;
		  }
		  i++;
	   }	
	   *ave = *min = *max = 0.0f;
	}

	void NFProfile::PrintString(WCHAR* tBuf, WCHAR* sBuf, LONG& index)
	{
		int len = wcslen(sBuf);

		CopyMemory(tBuf+index, sBuf, sizeof(WCHAR)*len);
		index += len;
	}

	void NFProfile::PrintProfile(WCHAR* strBuffer, INT iMaxBuffer)
	{
		unsigned int i = 0;

		s_fEndProfile = GetTime();

		LONG iLen = 0;
		ZeroMemory(strBuffer, sizeof(WCHAR)*iMaxBuffer);

		float fTotalAve = 0.0f;

		PrintString(strBuffer, L"-----------------------------------------------------------------------------\r\n", iLen);
		PrintString(strBuffer, L"   Average :    Minimum :    Maximum :    Call : Profile Name\r\n", iLen);
		PrintString(strBuffer, L"-----------------------------------------------------------------------------\r\n", iLen);

		WCHAR line[256], name[256], indentedName[256];
		WCHAR ave[16], min[16], max[16], num[16];
		float sampleTime, percentTime, aveTime, minTime, maxTime;

		while( i < NUM_PROFILE_SAMPLES && s_stSamples[i].bValid == true ) 
		{		
			unsigned int indent = 0;

			if( s_stSamples[i].iOpenProfiles < 0 ) 
				assert( !"ProfileEnd() called without a ProfileBegin()" );	
			else if( s_stSamples[i].iOpenProfiles > 0 ) 
				assert( !"ProfileBegin() called without a ProfileEnd()" );

			sampleTime = s_stSamples[i].fAccumulator - s_stSamples[i].fChildrenSampleTime;
			percentTime = ( sampleTime / (s_fEndProfile - s_fStartProfile ) ) * 100.0f;

			aveTime = minTime = maxTime = percentTime;

			//Add new measurement into the history and get the ave, min, and max
			StoreProfileInHistory( s_stSamples[i].szName, percentTime );
			GetProfileFromHistory( s_stSamples[i].szName, &aveTime, &minTime, &maxTime );

			//Format the data
			swprintf( ave, L"%3.5f", aveTime );
			swprintf( min, L"%3.5f", minTime );
			swprintf( max, L"%3.5f", maxTime );
			swprintf( num, L"%5d", s_stSamples[i].iProfileInstances );

			wcscpy( indentedName, s_stSamples[i].szName );
			for( indent=0; indent<s_stSamples[i].iNumParents; indent++ ) 
			{
				swprintf( name, L"   %s", indentedName );
				wcscpy( indentedName, name );
			}

			fTotalAve += aveTime;

			swprintf(line,L"%10s : %10s : %10s : %7s : %s\r\n", ave, min, max, num, indentedName);
			PrintString(strBuffer, line, iLen);
			i++;
		}

		swprintf(ave, L"%3.5f", fTotalAve );
		swprintf(line,L"\r\n Total AverageTime : %s\r\n", ave);
		PrintString(strBuffer, line, iLen);

		//Reset samples for next frame
		for( i=0; i<NUM_PROFILE_SAMPLES; i++ ) 
		{
			s_stSamples[i].bValid = false;
		}
		s_fStartProfile = GetTime();
	}

	void NFProfile::EndProfile()
	{
		if(!s_bEndProfile)
		{
			ProfileEnd( s_strID );
			s_bEndProfile = TRUE;
		}
	}

} 