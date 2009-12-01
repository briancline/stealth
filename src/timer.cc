#include <time.h>
#include "timer.h"
#include "stealth.h"


Timer::Timer()
	: t_interval( 1 ), t_last( time( NULL ) ), t_next( t_last + t_interval ),
	  t_handler( NULL )
{

}


Timer::Timer( void (*handlerFunction)() )
	: t_interval( 1 ), t_last( time( NULL ) ), t_next( t_last + t_interval ),
	  t_handler( handlerFunction )
{

}


Timer::Timer( void (*handlerFunction)(), time_t interval )
	: t_interval( interval ), t_last( time( NULL ) ), t_next( t_last + t_interval ),
	  t_handler( handlerFunction )
{

}


void Timer::Run()
{
	(t_handler)();
	t_last = time( NULL );
	t_next = t_last + t_interval;
}


time_t Timer::GetLastRunTS()
{
	return t_last;
}


time_t Timer::GetNextRunTS()
{
	return t_next;
}


time_t Timer::GetInterval()
{
	return t_interval;
}


void Timer::SetInterval( time_t newInterval )
{
	t_interval = newInterval;
}

