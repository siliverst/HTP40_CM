/*--------------------------------------------------------------------------
  CMS80F231x.H
 Registers definition for CMS80F231x

Copyright CMS Semicon Co., Ltd
 All rights reserved.
-------------------------------------------------------------------------- */
#ifndef __CMS80F231X_H
#define __CMS80F231X_H
 
#define     __I     volatile const   /*!< defines 'read only' permissions     */
#define     __O     volatile         /*!< defines 'write only' permissions    */
#define     __IO    volatile         /*!< defines 'read / write' permissions  */

/*!< Signed integer types  */
//typedef   signed char     int8_t;
//typedef   signed short    int16_t;
//typedef   signed long     int32_t;

/*!< Unsigned integer types  */
//typedef unsigned char     uint8_t;
//typedef unsigned short    uint16_t;
//typedef unsigned long     uint32_t;

typedef enum
{
	FALSE = 0, 
	TRUE = 1
}bool;

typedef enum
{
	RESET = 0, 
	SET = 1
}FlagStatus, ITStatus, BitStatus, BitAction;

typedef enum{DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONALSTATE_OK(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

sfr        P0            =   0x0080;   
sfr        SP            =   0x0081;   
sfr        DPL0            =   0x0082;   
sfr        DPH0            =   0x0083;   
sfr        DPL1            =   0x0084;   
sfr        DPH1            =   0x0085;   
sfr        DPS            =   0x0086;   
sfr        PCON            =   0x0087;   
sfr        TCON            =   0x0088;   
sfr        TMOD            =   0x0089;   
sfr        TL0            =   0x008A;   
sfr        TL1            =   0x008B;   
sfr        TH0            =   0x008C;   
sfr        TH1            =   0x008D;   
sfr        CKCON            =   0x008E;   
sfr        CLKDIV            =   0x008F;   
sfr        P1            =   0x0090;   
sfr        FUNCCR            =   0x0091;   
sfr        DPX0            =   0x0093;   
sfr        DPX1            =   0x0095;   
sfr        TA            =   0x0096;   
sfr        WDCON            =   0x0097;   
sfr        SCON0            =   0x0098;   
sfr        SBUF            =   0x0099;   
sfr        P0TRIS            =   0x009A;   
sfr        P2            =   0x00A0;   
sfr        P1TRIS            =   0x00A1;   
sfr        P2TRIS            =   0x00A2;   
sfr        P3TRIS            =   0x00A3;   
sfr        IE            =   0x00A8;   
sfr        EIE2            =   0x00AA;   
sfr        P0EXTIE            =   0x00AC;   
sfr        P1EXTIE            =   0x00AD;   
sfr        P2EXTIE            =   0x00AE;   
sfr        P3EXTIE            =   0x00AF;   
sfr        P3            =   0x00B0;   
sfr        EIF2            =   0x00B2;   
sfr        P0EXTIF            =   0x00B4;   
sfr        P1EXTIF            =   0x00B5;   
sfr        P2EXTIF            =   0x00B6;   
sfr        P3EXTIF            =   0x00B7;   
sfr        IP            =   0x00B8;   
sfr        EIP1            =   0x00B9;   
sfr        EIP2            =   0x00BA;   
sfr        WUTCRL            =   0x00BC;   
sfr        WUTCRH            =   0x00BD;   
sfr        BUZDIV            =   0x00BE;   
sfr        BUZCON            =   0x00BF;   
sfr        CCL1            =   0x00C2;   
sfr        CCH1            =   0x00C3;   
sfr        CCL2            =   0x00C4;   
sfr        CCH2            =   0x00C5;   
sfr        CCL3            =   0x00C6;   
sfr        CCH3            =   0x00C7;   
sfr        T2CON            =   0x00C8;   
sfr        T2IF            =   0x00C9;   
sfr        RLDL            =   0x00CA;   
sfr        RLDH            =   0x00CB;   
sfr        TL2            =   0x00CC;   
sfr        TH2            =   0x00CD;   
sfr        CCEN            =   0x00CE;   
sfr        T2IE            =   0x00CF;   
sfr        PSW            =   0x00D0;   
sfr        ADCMPC            =   0x00D1;   
sfr        T34MOD            =   0x00D2;   
sfr        ADDLYL            =   0x00D3;   
sfr        ADCMPL            =   0x00D4;   
sfr        ADCMPH            =   0x00D5;      
sfr        TL3            =   0x00DA;   
sfr        TH3            =   0x00DB;   
sfr        ADRESL            =   0x00DC;   
sfr        ADRESH            =   0x00DD;   
sfr        ADCON1            =   0x00DE;   
sfr        ADCON0            =   0x00DF;   
sfr        ACC            =   0x00E0;   
sfr        TL4            =   0x00E2;   
sfr        TH4            =   0x00E3;   
sfr        ADCON2            =   0x00E9;   
sfr        SCON1            =   0x00EA;   
sfr        SBUF1            =   0x00EB;   
sfr        SPCR            =   0x00EC;   
sfr        SPSR            =   0x00ED;   
sfr        SPDR            =   0x00EE;   
sfr        SSCR            =   0x00EF;   
sfr        B            =   0x00F0;   
sfr        I2CSADR            =   0x00F1;   
sfr        I2CSCR            =   0x00F2;   
sfr        I2CSBUF            =   0x00F3;   
sfr        I2CMSA            =   0x00F4;   
sfr        I2CMCR            =   0x00F5;   
sfr        I2CMBUF            =   0x00F6;   
sfr        I2CMTP            =   0x00F7;
sfr		   MLOCK			 =	 0x00FB;   
sfr        MADRL            =   0x00FC;   
sfr        MADRH            =   0x00FD;   
sfr        MDATA            =   0x00FE;   
sfr        MCTRL            =   0x00FF;   
#define	P00CFG	*(volatile unsigned char xdata *)	0xF000 
#define	P01CFG	*(volatile unsigned char xdata *)	0xF001 
#define	P02CFG	*(volatile unsigned char xdata *)	0xF002 
#define	P03CFG	*(volatile unsigned char xdata *)	0xF003 
#define	P04CFG	*(volatile unsigned char xdata *)	0xF004 
#define	P05CFG	*(volatile unsigned char xdata *)	0xF005 
#define	P0OD	*(volatile unsigned char xdata *)	0xF009 
#define	P0UP	*(volatile unsigned char xdata *)	0xF00A 
#define	P0RD	*(volatile unsigned char xdata *)	0xF00B 
#define	P0DR	*(volatile unsigned char xdata *)	0xF00C 
#define	P0SR	*(volatile unsigned char xdata *)	0xF00D 
#define	P0DS	*(volatile unsigned char xdata *)	0xF00E 
#define	P13CFG	*(volatile unsigned char xdata *)	0xF013 
#define	P14CFG	*(volatile unsigned char xdata *)	0xF014 
#define	P15CFG	*(volatile unsigned char xdata *)	0xF015 
#define	P16CFG	*(volatile unsigned char xdata *)	0xF016 
#define	P17CFG	*(volatile unsigned char xdata *)	0xF017 
#define	P1OD	*(volatile unsigned char xdata *)	0xF019 
#define	P1UP	*(volatile unsigned char xdata *)	0xF01A 
#define	P1RD	*(volatile unsigned char xdata *)	0xF01B 
#define	P1DR	*(volatile unsigned char xdata *)	0xF01C 
#define	P1SR	*(volatile unsigned char xdata *)	0xF01D 
#define	P1DS	*(volatile unsigned char xdata *)	0xF01E
#define	P21CFG	*(volatile unsigned char xdata *)	0xF021
#define	P22CFG	*(volatile unsigned char xdata *)	0xF022
#define	P23CFG	*(volatile unsigned char xdata *)	0xF023
#define	P24CFG	*(volatile unsigned char xdata *)	0xF024
#define	P25CFG	*(volatile unsigned char xdata *)	0xF025
#define	P26CFG	*(volatile unsigned char xdata *)	0xF026
#define	P2OD	*(volatile unsigned char xdata *)	0xF029
#define	P2UP	*(volatile unsigned char xdata *)	0xF02A
#define	P2RD	*(volatile unsigned char xdata *)	0xF02B
#define	P2DR	*(volatile unsigned char xdata *)	0xF02C
#define	P2SR	*(volatile unsigned char xdata *)	0xF02D
#define	P2DS	*(volatile unsigned char xdata *)	0xF02E
#define	P30CFG	*(volatile unsigned char xdata *)	0xF030
#define	P31CFG	*(volatile unsigned char xdata *)	0xF031
#define	P32CFG	*(volatile unsigned char xdata *)	0xF032
#define	P35CFG	*(volatile unsigned char xdata *)	0xF035
#define	P36CFG	*(volatile unsigned char xdata *)	0xF036
#define	P3OD	*(volatile unsigned char xdata *)	0xF039
#define	P3UP	*(volatile unsigned char xdata *)	0xF03A
#define	P3RD	*(volatile unsigned char xdata *)	0xF03B
#define	P3DR	*(volatile unsigned char xdata *)	0xF03C
#define	P3SR	*(volatile unsigned char xdata *)	0xF03D
#define	P3DS	*(volatile unsigned char xdata *)	0xF03E
#define	P00EICFG	*(volatile unsigned char xdata *)	0xF080
#define	P01EICFG	*(volatile unsigned char xdata *)	0xF081
#define	P02EICFG	*(volatile unsigned char xdata *)	0xF082
#define	P03EICFG	*(volatile unsigned char xdata *)	0xF083
#define	P04EICFG	*(volatile unsigned char xdata *)	0xF084
#define	P05EICFG	*(volatile unsigned char xdata *)	0xF085
#define	P13EICFG	*(volatile unsigned char xdata *)	0xF08B
#define	P14EICFG	*(volatile unsigned char xdata *)	0xF08C
#define	P15EICFG	*(volatile unsigned char xdata *)	0xF08D
#define	P16EICFG	*(volatile unsigned char xdata *)	0xF08E
#define	P17EICFG	*(volatile unsigned char xdata *)	0xF08F
#define	P21EICFG	*(volatile unsigned char xdata *)	0xF091
#define	P22EICFG	*(volatile unsigned char xdata *)	0xF092
#define	P23EICFG	*(volatile unsigned char xdata *)	0xF093
#define	P24EICFG	*(volatile unsigned char xdata *)	0xF094
#define	P25EICFG	*(volatile unsigned char xdata *)	0xF095
#define	P26EICFG	*(volatile unsigned char xdata *)	0xF096
#define	P30EICFG	*(volatile unsigned char xdata *)	0xF098
#define	P31EICFG	*(volatile unsigned char xdata *)	0xF099
#define	P32EICFG	*(volatile unsigned char xdata *)	0xF09A
#define	P35EICFG	*(volatile unsigned char xdata *)	0xF09D
#define	P36EICFG	*(volatile unsigned char xdata *)	0xF09E
#define	PS_INT0	*(volatile unsigned char xdata *)	0xF0C0
#define	PS_INT1	*(volatile unsigned char xdata *)	0xF0C1
#define	PS_T0	*(volatile unsigned char xdata *)	0xF0C2
#define	PS_T0G	*(volatile unsigned char xdata *)	0xF0C3
#define	PS_T1	*(volatile unsigned char xdata *)	0xF0C4
#define	PS_T1G	*(volatile unsigned char xdata *)	0xF0C5
#define	PS_T2	*(volatile unsigned char xdata *)	0xF0C6
#define	PS_T2EX	*(volatile unsigned char xdata *)	0xF0C7
#define	PS_CAP0	*(volatile unsigned char xdata *)	0xF0C8
#define	PS_CAP1	*(volatile unsigned char xdata *)	0xF0C9
#define	PS_CAP2	*(volatile unsigned char xdata *)	0xF0CA
#define	PS_CAP3	*(volatile unsigned char xdata *)	0xF0CB
#define	PS_ADET	*(volatile unsigned char xdata *)	0xF0CC
#define	PS_FB	*(volatile unsigned char xdata *)	0xF0CD
#define	PWMCON	*(volatile unsigned char xdata *)	0xF120
#define	PWMOE	*(volatile unsigned char xdata *)	0xF121
#define	PWMPINV	*(volatile unsigned char xdata *)	0xF122
#define	PWM01PSC	*(volatile unsigned char xdata *)	0xF123
#define	PWM23PSC	*(volatile unsigned char xdata *)	0xF124
#define	PWM45PSC	*(volatile unsigned char xdata *)	0xF125
#define	PWMCNTE	*(volatile unsigned char xdata *)	0xF126
#define	PWMCNTM	*(volatile unsigned char xdata *)	0xF127
#define	PWMCNTCLR	*(volatile unsigned char xdata *)	0xF128
#define	PWMLOADEN	*(volatile unsigned char xdata *)	0xF129
#define	PWM0DIV	*(volatile unsigned char xdata *)	0xF12A
#define	PWM1DIV	*(volatile unsigned char xdata *)	0xF12B
#define	PWM2DIV	*(volatile unsigned char xdata *)	0xF12C
#define	PWM3DIV	*(volatile unsigned char xdata *)	0xF12D
#define	PWM4DIV	*(volatile unsigned char xdata *)	0xF12E
#define	PWM5DIV	*(volatile unsigned char xdata *)	0xF12F
#define	PWMP0L	*(volatile unsigned char xdata *)	0xF130
#define	PWMP0H	*(volatile unsigned char xdata *)	0xF131
#define	PWMP1L	*(volatile unsigned char xdata *)	0xF132
#define	PWMP1H	*(volatile unsigned char xdata *)	0xF133
#define	PWMP2L	*(volatile unsigned char xdata *)	0xF134
#define	PWMP2H	*(volatile unsigned char xdata *)	0xF135
#define	PWMP3L	*(volatile unsigned char xdata *)	0xF136
#define	PWMP3H	*(volatile unsigned char xdata *)	0xF137
#define	PWMP4L	*(volatile unsigned char xdata *)	0xF138
#define	PWMP4H	*(volatile unsigned char xdata *)	0xF139
#define	PWMP5L	*(volatile unsigned char xdata *)	0xF13A
#define	PWMP5H	*(volatile unsigned char xdata *)	0xF13B
#define	PWMD0L	*(volatile unsigned char xdata *)	0xF140
#define	PWMD0H	*(volatile unsigned char xdata *)	0xF141
#define	PWMD1L	*(volatile unsigned char xdata *)	0xF142
#define	PWMD1H	*(volatile unsigned char xdata *)	0xF143
#define	PWMD2L	*(volatile unsigned char xdata *)	0xF144
#define	PWMD2H	*(volatile unsigned char xdata *)	0xF145
#define	PWMD3L	*(volatile unsigned char xdata *)	0xF146
#define	PWMD3H	*(volatile unsigned char xdata *)	0xF147
#define	PWMD4L	*(volatile unsigned char xdata *)	0xF148
#define	PWMD4H	*(volatile unsigned char xdata *)	0xF149
#define	PWMD5L	*(volatile unsigned char xdata *)	0xF14A
#define	PWMD5H	*(volatile unsigned char xdata *)	0xF14B
#define	PWMDD0L	*(volatile unsigned char xdata *)	0xF150
#define	PWMDD0H	*(volatile unsigned char xdata *)	0xF151
#define	PWMDD1L	*(volatile unsigned char xdata *)	0xF152
#define	PWMDD1H	*(volatile unsigned char xdata *)	0xF153
#define	PWMDD2L	*(volatile unsigned char xdata *)	0xF154
#define	PWMDD2H	*(volatile unsigned char xdata *)	0xF155
#define	PWMDD3L	*(volatile unsigned char xdata *)	0xF156
#define	PWMDD3H	*(volatile unsigned char xdata *)	0xF157
#define	PWMDD4L	*(volatile unsigned char xdata *)	0xF158
#define	PWMDD4H	*(volatile unsigned char xdata *)	0xF159
#define	PWMDD5L	*(volatile unsigned char xdata *)	0xF15A
#define	PWMDD5H	*(volatile unsigned char xdata *)	0xF15B
#define	PWMDTE	*(volatile unsigned char xdata *)	0xF160
#define	PWM01DT	*(volatile unsigned char xdata *)	0xF161
#define	PWM23DT	*(volatile unsigned char xdata *)	0xF162
#define	PWM45DT	*(volatile unsigned char xdata *)	0xF163
#define	PWMMASKE	*(volatile unsigned char xdata *)	0xF164
#define	PWMMASKD	*(volatile unsigned char xdata *)	0xF165
#define	PWMFBKC	*(volatile unsigned char xdata *)	0xF166
#define	PWMFBKD	*(volatile unsigned char xdata *)	0xF167
#define	PWMPIE	*(volatile unsigned char xdata *)	0xF168
#define	PWMZIE	*(volatile unsigned char xdata *)	0xF169
#define	PWMUIE	*(volatile unsigned char xdata *)	0xF16A
#define	PWMDIE	*(volatile unsigned char xdata *)	0xF16B
#define	PWMPIF	*(volatile unsigned char xdata *)	0xF16C
#define	PWMZIF	*(volatile unsigned char xdata *)	0xF16D
#define	PWMUIF	*(volatile unsigned char xdata *)	0xF16E
#define	PWMDIF	*(volatile unsigned char xdata *)	0xF16F
#define	C0CON0	*(volatile unsigned char xdata *)	0xF500
#define	C0CON1	*(volatile unsigned char xdata *)	0xF501
#define	C0CON2	*(volatile unsigned char xdata *)	0xF502
#define	C1CON0	*(volatile unsigned char xdata *)	0xF503
#define	C1CON1	*(volatile unsigned char xdata *)	0xF504
#define	C1CON2	*(volatile unsigned char xdata *)	0xF505
#define	CNVRCON	*(volatile unsigned char xdata *)	0xF506
#define	CNFBCON	*(volatile unsigned char xdata *)	0xF507
#define	CNIE	*(volatile unsigned char xdata *)	0xF508
#define	CNIF	*(volatile unsigned char xdata *)	0xF509
#define	C0ADJE	*(volatile unsigned char xdata *)	0xF50A
#define	C1ADJE	*(volatile unsigned char xdata *)	0xF50B
#define	C0HYS	*(volatile unsigned char xdata *)	0xF50C
#define	C1HYS	*(volatile unsigned char xdata *)	0xF50D
#define	OP0CON0	*(volatile unsigned char xdata *)	0xF520
#define	OP0CON1	*(volatile unsigned char xdata *)	0xF521
#define	OP1CON0	*(volatile unsigned char xdata *)	0xF523
#define	OP1CON1	*(volatile unsigned char xdata *)	0xF524
#define	OP0ADJE	*(volatile unsigned char xdata *)	0xF526
#define	OP1ADJE	*(volatile unsigned char xdata *)	0xF527
#define	PGACON0	*(volatile unsigned char xdata *)	0xF529
#define	PGACON1	*(volatile unsigned char xdata *)	0xF52A
#define	PGACON2	*(volatile unsigned char xdata *)	0xF52B
#define	PGACON3	*(volatile unsigned char xdata *)	0xF52C
#define	PGAADJE	*(volatile unsigned char xdata *)	0xF52D
#define	BRTCON 	*(volatile unsigned char xdata *)	0xF5C0
#define	BRTDL  	*(volatile unsigned char xdata *)	0xF5C1
#define	BRTDH	*(volatile unsigned char xdata *)	0xF5C2
#define	LVDCON 	*(volatile unsigned char xdata *)	0xF690
#define	BOOTCON	*(volatile unsigned char xdata *)	0xF691
#define	ADCLDO 	*(volatile unsigned char xdata *)	0xF692
#define	LSECRL 	*(volatile unsigned char xdata *)	0xF694
#define	LSECRH	*(volatile unsigned char xdata *)	0xF695
#define	LSECON 	*(volatile unsigned char xdata *)	0xF696
#define	UID0   	*(volatile unsigned char xdata *)	0xF5E0
#define	UID1   	*(volatile unsigned char xdata *)	0xF5E1
#define	UID2   	*(volatile unsigned char xdata *)	0xF5E2
#define	UID3   	*(volatile unsigned char xdata *)	0xF5E3
#define	UID4   	*(volatile unsigned char xdata *)	0xF5E4
#define	UID5   	*(volatile unsigned char xdata *)	0xF5E5
#define	UID6   	*(volatile unsigned char xdata *)	0xF5E6
#define	UID7   	*(volatile unsigned char xdata *)	0xF5E7
#define	UID8   	*(volatile unsigned char xdata *)	0xF5E8
#define	UID9   	*(volatile unsigned char xdata *)	0xF5E9
#define	UID10  	*(volatile unsigned char xdata *)	0xF5EA
#define	UID11  	*(volatile unsigned char xdata *)	0xF5EB

           
           
           
           
/* Definitions for P0 register */           
sbit  P0_7   =  P0^7;           
sbit  P0_6   =  P0^6;           
sbit  P0_5   =  P0^5;           
sbit  P0_4   =  P0^4;           
sbit  P0_3   =  P0^3;           
sbit  P0_2   =  P0^2;           
sbit  P0_1   =  P0^1;           
sbit  P0_0   =  P0^0;           
           
           
/* Definitions for TCON register */           
sbit  TF1   =  TCON^7;           
sbit  TR1   =  TCON^6;           
sbit  TF0   =  TCON^5;           
sbit  TR0   =  TCON^4;           
sbit  IE1   =  TCON^3;           
sbit  IT1   =  TCON^2;           
sbit  IE0   =  TCON^1;           
sbit  IT0   =  TCON^0;           
           
           
/* Definitions for P1 register */           
sbit  P1_7   =  P1^7;           
sbit  P1_6   =  P1^6;           
sbit  P1_5   =  P1^5;           
sbit  P1_4   =  P1^4;           
sbit  P1_3   =  P1^3;           
sbit  P1_2   =  P1^2;           
sbit  P1_1   =  P1^1;           
sbit  P1_0   =  P1^0;           
           
           
/* Definitions for SCON0 register */           
sbit  U0SM0   =  SCON0^7;           
sbit  U0SM1   =  SCON0^6;           
sbit  U0SM2   =  SCON0^5;           
sbit  U0REN   =  SCON0^4;           
sbit  U0TB8   =  SCON0^3;           
sbit  U0RB8   =  SCON0^2;           
sbit  TI0   =  SCON0^1;           
sbit  RI0   =  SCON0^0;           
           
           
/* Definitions for P2 register */           
sbit  P2_7   =  P2^7;           
sbit  P2_6   =  P2^6;           
sbit  P2_5   =  P2^5;           
sbit  P2_4   =  P2^4;           
sbit  P2_3   =  P2^3;           
sbit  P2_2   =  P2^2;           
sbit  P2_1   =  P2^1;           
sbit  P2_0   =  P2^0;           
           
           
/* Definitions for IE register */           
sbit  EA   =  IE^7;           
sbit  ES1   =  IE^6;           
sbit  ET2   =  IE^5;           
sbit  ES0   =  IE^4;           
sbit  ET1   =  IE^3;           
sbit  EX1   =  IE^2;           
sbit  ET0   =  IE^1;           
sbit  EX0   =  IE^0;           
           
           
/* Definitions for P3 register */           
sbit  P3_7   =  P3^7;           
sbit  P3_6   =  P3^6;           
sbit  P3_5   =  P3^5;           
sbit  P3_4   =  P3^4;           
sbit  P3_3   =  P3^3;           
sbit  P3_2   =  P3^2;           
sbit  P3_1   =  P3^1;           
sbit  P3_0   =  P3^0;           
           
           
/* Definitions for IP register */           
sbit  PS1   =  IP^6;           
sbit  PT2   =  IP^5;           
sbit  PS0   =  IP^4;           
sbit  PT1   =  IP^3;           
sbit  PX1   =  IP^2;           
sbit  PT0   =  IP^1;           
sbit  PX0   =  IP^0;           
           
           
/* Definitions for T2CON register */           
sbit  T2PS   =  T2CON^7;           
sbit  I3FR   =  T2CON^6;           
sbit  T2R1   =  T2CON^4;           
sbit  T2R0   =  T2CON^3;           
sbit  T2CM   =  T2CON^2;           
sbit  T2I1   =  T2CON^1;           
sbit  T2I0   =  T2CON^0;           
           
           
/* Definitions for PSW register */           
sbit  CY   =  PSW^7;           
sbit  AC   =  PSW^6;           
sbit  F0   =  PSW^5;           
sbit  RS1   =  PSW^4;           
sbit  RS0   =  PSW^3;           
sbit  OV   =  PSW^2;           
sbit  P   =  PSW^0;           
           
           
/* Definitions for ACC register */           
sbit  ACC_7   =  ACC^7;           
sbit  ACC_6   =  ACC^6;           
sbit  ACC_5   =  ACC^5;           
sbit  ACC_4   =  ACC^4;           
sbit  ACC_3   =  ACC^3;           
sbit  ACC_2   =  ACC^2;           
sbit  ACC_1   =  ACC^1;           
sbit  ACC_0   =  ACC^0;           
           
           
/* Definitions for B register */           
sbit  B_7   =  B^7;           
sbit  B_6   =  B^6;           
sbit  B_5   =  B^5;           
sbit  B_4   =  B^4;           
sbit  B_3   =  B^3;           
sbit  B_2   =  B^2;           
sbit  B_1   =  B^1;           
#endif
