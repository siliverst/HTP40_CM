#ifndef TRIM_H
#define	TRIM_H

#ifdef	__cplusplus
extern "C" {
#endif


#define TR_GIST		60
	
enum tr_sec {
	TR_SEC_m16,
	TR_SEC_m17,
	TR_SEC_m18,
	TR_SEC_m19,
	TR_SEC_m20,
	TR_SEC_m21,
	TR_SEC_m22,
	TR_SEC_m23,
	TR_SEC_m24,
	TR_SEC_SF,
	TR_SECTORS //10
};


void trim_init (void);	
void tirm_8hz_proc (void);

#ifdef	__cplusplus
}
#endif

#endif	/* TRIM_H */