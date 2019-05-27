void ascii_ctrl_bit_set(void);
void ascii_ctrl_bit_clear(void); 
void ascii_write_controller(unsigned char command);
unsigned char ascii_read_controller(void);
void ascii_write_cmd(unsigned char command);
void ascii_write_data(unsigned char data);
unsigned char ascii_read_status(void);
unsigned char ascii_read_data(void); 
void ascii_init(void);
void ascii_write_char(unsigned char c);
void ascii_gotoxy(int x, int y);
  