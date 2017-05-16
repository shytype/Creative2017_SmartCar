#ifndef __UART_H__
#define __UART_H__


extern int g_serial_port_0_f;
extern BYTE g_serial_port_0_data;
extern int g_serial_port_1_f;
extern BYTE g_serial_port_1_data;

extern void init_serial_port_0(void);
extern void serial_port_0_TX(BYTE data);
extern void serial_port_0_TX_array(const BYTE data[], WORD n);
extern void intc_serial_port_0_RX(void);

extern void init_serial_port_1(void);
extern void serial_port_1_TX(BYTE data);
extern void serial_port_1_TX_array(const BYTE data[], WORD n);
extern void intc_serial_port_1_RX(void);

extern void init_serial_port_2(void);
extern void serial_port_2_TX(BYTE data);
extern void serial_port_2_TX_array(const BYTE data[], WORD n);
extern void intc_serial_port_2_RX(void);

extern void init_serial_port_3(void);
extern void serial_port_3_TX(BYTE data);
extern void serial_port_3_TX_array(const BYTE data[], WORD n);
extern void intc_serial_port_3_RX(void);


#endif