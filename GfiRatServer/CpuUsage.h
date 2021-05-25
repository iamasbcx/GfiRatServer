// CpuUsage.h: interface for the CCpuUsage class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

#include <pdh.h>
#include <pdhmsg.h>

#pragma comment(lib,"PDH.lib")


#define MAX_RAW_VALUES          20

const char szCounterName[] = "\\Processor(_Total)\\% Processor Time";

typedef struct _tag_PDHCounterStruct {
    HCOUNTER hCounter;      // Handle to the counter - given to use by PDH Library
    int nNextIndex;         // element to get the next raw value
    int nOldestIndex;       // element containing the oldes raw value
    int nRawCount;          // number of elements containing raw values
    PDH_RAW_COUNTER a_RawValue[MAX_RAW_VALUES]; // Ring buffer to contain raw values
} PDHCOUNTERSTRUCT, *PPDHCOUNTERSTRUCT;


class CCpuUsage  
{
public:
	CCpuUsage();
	virtual ~CCpuUsage();
	BOOL	Init();
	int GetUsage();

protected:

	PPDHCOUNTERSTRUCT			m_pCounterStruct;
	HQUERY						m_hQuery;


};
