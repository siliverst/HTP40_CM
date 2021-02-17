#include <SH79F084B.h>
#include "globalvar.h"
#include "pin_manager.h"

void pin_manager_init (void)
{
	//EVA_TEMP|FC_TEMP|LED|REED|NC|TRIM|-|-
	P1 = 	bin(00000000);
	P1M0 = 	bin(11010100);
	P1M1 = 	bin(00101000);
	
	//LAMP_PWM|-|-|-|HEATER|PHASE|TXD|RXD
	P3 = 	bin(10001010);
	P3M0 = 	bin(00000100);
	P3M1 = 	bin(10001010);
	
	//-|-|-|-|-|FAN|COMPRESSOR|RESET
	P4 = 	bin(00000110);
	P4M0 = 	bin(00000000);
	P4M1 = 	bin(00000110);
}

/*
PxM0n PxM1n Description

0
0
Quasi-Bi mode

0
1
Push-Pull mode

1
0
Input-Only mode(high impedance)

1
1
Open-Drain output mode
*/