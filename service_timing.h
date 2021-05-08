#ifndef SERVICE_TIMING_H
#define SERVICE_TIMING_H

#ifdef	__cplusplus
extern "C" {
#endif

// T34MOD
// биты включающие режим таймера 0	
// запуск таймера 4
#define TR4								7
// делитель таймера 1= Fsys/4； 0= Fsys/12.
#define T4M								6
// режим
//T4M1，T4M0: 定时器4模式选择位；
//00= 模式0，13位定时器；
//01= 模式1，16位定时器；
//10= 模式2，8位自动重装定时器；
//11= 模式3，停止计数。
#define T4M1							5
#define T4M0							4
	
//биты 	EIE2
// бит включения прерывания от таймера 4
#define ET4								1

// флаги EIF2
// флаг прерывания таймера 4, стирается автоматически
#define EF4								1


//---------------------------------------------------------------------------------------------------------------
// определения для отсчёта по counter.t1_for_1hz
//---------------------------------------------------------------------------------------------------------------
#define f4Hz	0x01
#define f2Hz	0x02
#define f1Hz	0x04
#define f05Hz	0x08
#define f025Hz	0x10

	
#define MS_IN_SEC					1000
#define SERVICE_FREQ                1000
#define PERIOD_8HZ                  (SERVICE_FREQ/8)
#define PERIOD_8HZ_FOR_CLOCK_PROC	(PERIOD_8HZ/2)
#define PERIOD_15MS					((15*SERVICE_FREQ)/MS_IN_SEC)
   
void service_timing_init (void);
void service_timing_proc (void);
#ifdef SOUND_ON
uint8_t timer05hz_get (void);
#endif
#ifdef	__cplusplus
}
#endif

#endif