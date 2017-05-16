#ifndef __SUPERSONIC_H__
#define __SUPERSONIC_H__

extern int ss_left;
extern int ss_right;

/* 超声 */
#ifdef __SUPERSONIC_C_
union
{
	DWORD R;
	struct {
		BYTE byte_0;
		BYTE byte_1;
		BYTE byte_2;
		BYTE byte_3;
	} B;
}tmp_time;
#else
extern union
{
	DWORD R;
	struct {
		BYTE byte_0;
		BYTE byte_1;
		BYTE byte_2;
		BYTE byte_3;
	} B;
}tmp_time;
#endif

#ifdef __SUPERSONIC_C_
union
{
	DWORD R;
	struct {
		BYTE byte_0;
		BYTE byte_1;
		BYTE byte_2;
		BYTE byte_3;
	} B;
}tmp_time2;
#else
extern union
{
	DWORD R;
	struct {
		BYTE byte_0;
		BYTE byte_1;
		BYTE byte_2;
		BYTE byte_3;
	} B;
}tmp_time2;
#endif

#define SS_DIS_20CM (0x00000000)

extern void init_supersonic_trigger_0(void);
extern void init_supersonic_trigger_1(void);
extern void init_supersonic_trigger_2(void);
extern void init_supersonic_trigger_3(void);

extern void trigger_supersonic_0(void);
extern void trigger_supersonic_1(void);
extern void trigger_supersonic_2(void);
extern void trigger_supersonic_3(void);

extern void init_supersonic_receive_0(void);
extern void init_supersonic_receive_1(void);
extern void init_supersonic_receive_2(void);
extern void init_supersonic_receive_3(void);

extern void get_supersonic_time_0(void);
extern void get_supersonic_time_1(void);
extern void get_supersonic_time_2(void);
extern void get_supersonic_time_3(void);
extern void supersonic(void);
#endif
