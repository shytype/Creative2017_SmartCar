#ifndef __MAG_H__
#define __MAG_H__

extern WORD mag_left;
extern WORD mag_right;
extern WORD mag_left_old;
extern WORD mag_right_old;

extern void init_ADC(void);
extern void control_steer_helm(void);
extern void mag_read(void);
extern void mag_TX(void);
extern int is_on_mag_line(void);

#endif