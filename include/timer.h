#ifndef __TIMER_H__
#define __TIMER_H__ "$Id: timer.h,v 1.5 2002/03/13 06:41:37 bcline Exp $"

#include <time.h>

class Network;

class Timer
{
	public:

		Timer();
		Timer( void (*handlerFunction)() );
		Timer( void (*handlerFunction)(), time_t interval );

		void Run();
		time_t GetLastRunTS();
		time_t GetNextRunTS();
		time_t GetInterval();
		void SetInterval( time_t newInterval );

		
	private:

		time_t t_interval;
		time_t t_last;
		time_t t_next;
		void (*t_handler)();
	
};


#endif // __TIMER_H__
