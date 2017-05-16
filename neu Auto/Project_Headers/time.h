#ifndef __TIME_H__
#define __TIME_H__


#define I2C_WAIT_TIME (50)


extern int I2C_read_byte_from_time_module(BYTE address, BYTE data[], BYTE cnt);
extern void disable_I2C(void);
extern int I2C_write_byte_to_time_module(BYTE address, BYTE data[], BYTE cnt);
extern int I2C_write_byte_to_time_module_2(BYTE address, BYTE data[], BYTE cnt);
extern int I2C_read_byte_from_time_module_2(BYTE address, BYTE data[], BYTE cnt);
void init_I2C(void);

#endif
