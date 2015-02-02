//
//
// Pi UART interface header file 
//
//


//
// Open UART in correct mode for Gertbot:
// 8 bits, 1 start, 1 stop, no parity
// No HW flow control non blocking
// 
// if low use 57600 baud else use 115K2 baud
int open_uart(int low);

// close uart
// No return status
// (if this fails we are in big trouble anway)
void close_uart();

//
// transmit data to uart 
//
// return 0 on failure
// return 1 on success
//
int write_uart(unsigned char *data,int count);

//
// Try to read at least 'len' characters from uart (len<=32) 
// Beware: this routine can return more then asked for, up to 64 bytes
// Thus data should point to area of 64 unsigned chars!!!!
//
// return the actual number of bytes read 
// 
int read_uart(unsigned char *data,int len);

// Try to find if board is present
// using 'get revision' command 
// This code is specific written to re-sync on startup
// or after communication errors
//
// return 1 if OK 
// return 0 if fails
// If OK version numbers (MS/LS) is set
//
int find_board(int b,int *ver_ms,int *ver_ls);

//
// Standard 'get reply' routine
//
// *data must point to at least 64 bytes
// return 1 on OK
// return 0 on failure
//
// If OK *data holds received data guaranteed
// to start with <id> <command>
// 
int get_reply(unsigned char *data,int byte_count,unsigned char id,unsigned char command);

