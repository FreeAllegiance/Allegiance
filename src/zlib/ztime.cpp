#include "ztime.h"
#include "zassert.h"
#include <time.h>

tlsUINT32 Time::s_dwPauseStart;
tlsUINT32 Time::s_dwNegativeOffset;

#ifdef _DEBUG_TRAINING
  tlsUINT32 Time::s_dwLastTime;
  tlsUINT32 Time::s_dwAccumulatedTime;
  tlsINT   Time::s_iShift;
  tlsUINT32 Time::s_dwLastClockTime;
#endif

void    Time::Pause (void)
{
    assert (! IsPaused ());

  #ifdef _DEBUG_TRAINING
    // save the current time as the clock
    s_dwPauseStart = s_dwAccumulatedTime;
  #else
    // save the current time as the clock
    s_dwPauseStart = timeGetTime ();
  #endif
}

bool    Time::IsPaused (void)
{
    // the clock is paused if s_dwPauseStart is non-zero
    return (s_dwPauseStart != 0) ? true : false;
}

void    Time::Continue (void)
{
    assert (IsPaused ());

  #ifdef _DEBUG_TRAINING
    // get the current time to see how long the system was paused,
    // and accumulate that into the clock offset.
    s_dwNegativeOffset += s_dwAccumulatedTime - s_dwPauseStart;
  #else
    // get the current time to see how long the system was paused,
    // and accumulate that into the clock offset.
    s_dwNegativeOffset += timeGetTime () - s_dwPauseStart;
  #endif

    // clear the pause counter
    s_dwPauseStart = 0;
}

#ifdef _DEBUG_TRAINING
int     Time::GetShift (void)
{
    // return the current shift value
    return s_iShift;
}

void    Time::SetShift (int iShift)
{
    // limit the amount of speed change
    if (iShift > 3)
        iShift = 3;
    else if (iShift < -3)
        iShift = -3;

    // set the shift value
    s_iShift = iShift;
}
#endif

// BT - WOPR - Diagnosing timing issues for bot commands. 
// https://stackoverflow.com/questions/20370920/convert-current-time-from-windows-to-unix-timestamp-in-c-or-c
INT64 Time::getUnixTime()
{
	//Get the number of seconds since January 1, 1970 12:00am UTC
	//Code released into public domain; no attribution required.

	const INT64 UNIX_TIME_START = 0x019DB1DED53E8000; //January 1, 1970 (start of Unix epoch) in "ticks"
	const INT64 TICKS_PER_SECOND = 10000000; //a tick is 100ns

	FILETIME ft;
	GetSystemTimeAsFileTime(&ft); //returns ticks in UTC

								  //Copy the low and high parts of FILETIME into a LARGE_INTEGER
								  //This is so we can access the full 64-bits as an Int64 without causing an alignment fault
	LARGE_INTEGER li;
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;

	//Convert ticks since 1/1/1970 into seconds
	return (li.QuadPart - UNIX_TIME_START) / TICKS_PER_SECOND;
}

// Does not include milli seconds :(
const char * Time::getTimeString()
{
	char timeString[20];
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);

	strftime(timeString, sizeof(timeString), "%m/%d/%Y %HH:%MM:%S", tm);

	return timeString;
}
