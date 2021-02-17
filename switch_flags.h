#ifndef _SWITCH_FLAGS_H_
#define _SWITCH_FLAGS_H_

// 72 градуса, верхняя граничная температура на датчике испарителя
#define TEMP_HEATER_OVERHEAT			72.0

void switch_flags_init (void);
void switch_flags_fc_sensor_proc ( void );
void switch_flags_eva_sensor_proc ( void );
void switch_flags_cepb_proc (void);
void switch_flags_cepb_dis (void);
void switch_flags_proc (void);

#endif
