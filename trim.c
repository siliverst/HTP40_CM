#include "globalvar.h"
#include "trim.h"

//static void calc_borders (uint8_t segment);

static uint16_t trtable[] =
{				// -16
  1637,        	
				// -17
  3274,        	
				// -18
  4910,        	
				// -19
  6547,        	
				// -20
  8184,       	
				// -21
  9821,       	
				// -22
  11458,       	
				// -23
  13094,       	
				// -24
  14731,       	
				// SF
  16368        	
};

/*static void calc_borders (uint8_t segment)
{
	gvar.tr_up_bord = trtable[segment];
	if (segment) {
		gvar.tr_down_bord = trtable[segment-1];
	}else{
		gvar.tr_down_bord = 0;
	}
}*/

void trim_init (void)
{
	gvar.tr_sector = 0xFF;
}

void tirm_8hz_proc (void)
{
uint8_t i;
static uint16_t tr_prev = 0xFFFF;
static uint16_t tr_prev_filtred;
	
	if (tr_prev != gvar.tr_sensor){
		tr_prev = gvar.tr_sensor;
	}else{
		tr_prev_filtred = tr_prev;
		
		for (i=0; i<TR_SECTORS; i++){
			if (tr_prev_filtred <= trtable[i]){
				if (gvar.tr_sector != i){
					if (gvar.tr_sector == 0xFF){
						gvar.tr_sector = i;
					}
					else if (tr_prev_filtred > trtable[gvar.tr_sector]){
						if ((tr_prev_filtred - trtable[gvar.tr_sector])>TR_GIST){
							gvar.tr_sector = i;
						}
					}else if (gvar.tr_sector){
						if (tr_prev_filtred < trtable[gvar.tr_sector-1]){
							if ((trtable[gvar.tr_sector-1] - tr_prev_filtred)>TR_GIST){
								gvar.tr_sector = i;
							}
						}
					}
				}
				//calc_borders(gvar.tr_sector);
				break;
			}
		}
	}
}


