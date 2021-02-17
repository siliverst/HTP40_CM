#ifndef SERVICE_TIMING_H
#define SERVICE_TIMING_H

#ifdef	__cplusplus
extern "C" {
#endif

// TMOD
// биты включающие режим таймера 0	
#define M01							1
#define M00							0

//TCON1
//  бит включающий режим тактирования timer0 от sysclk
#define TCLKS0					5
// бит делителя на 12
#define TCLKP0					2
// бит включения компаратора
#define TC0						0
	
#define MS_IN_SEC					1000
#define SERVICE_FREQ                1000
#define PERIOD_8HZ                  (SERVICE_FREQ/8)
#define PERIOD_8HZ_FOR_CLOCK_PROC	(PERIOD_8HZ/2)
#define PERIOD_15MS					((15*SERVICE_FREQ)/MS_IN_SEC)
   
void service_timing_init (void);
void service_timing_proc (void);


#ifdef	__cplusplus
}
#endif

#endif