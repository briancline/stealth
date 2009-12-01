#ifndef __SIGNALS_H__
#define __SIGNALS_H__ "$Id: signals.h,v 1.1 2002/03/14 05:31:35 bcline Exp $"


void RegisterSignals();
void SignalHandler( int sigType );
void SigInterrupt();
void SigSegfault();
void SigTerminate();
void SigHangup();


#endif // __SIGNALS_H__
