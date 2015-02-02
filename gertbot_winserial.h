//
// Serial comm port SW
// Simplified system using local handle 
// and blocking (non overlapped) operation
// 
// G.J. van Loo, 30 April, 2014
//

#include <windows.h>
#include <stdint.h>

#define SLEEP_MS(x) Sleep(x)

//
// open comm port <n>
//
// Return 0 on success
// return 1 on error
//
int  open_uart(int number);

//
// Set up comm port using settings string
// e.g. "9600,n,8,1"
// or "baud=19200 parity=n data=8 stop=1"
// port must previoulsy been opened
//
// Return 0 on success
// return 1 on error
//
// Needs TCHAR pointer which can be obtained by using
// setup_comm_port ( L "...");
//                   ^!!
int  setup_comm_port(TCHAR *com_format);

// Closes comm port 
void close_uart();

// Read bytes from comm port
// returns actual number of bytes read
int read_uart(unsigned char *read_buffer,int bytes);

// Write bytes to comm port
// returns actual number of bytes written
int write_uart(unsigned char *write_buffer,int bytes);

/** Windows has some extra routines **/
int  setup_comm_port(TCHAR *com_format);

//
// Returns 32-bit integer with a bit set for each comm port found
// Thus limited to comm ports 1-32
//
uint32_t  available_com_ports(void);

// Find bit 'p' which is set 
int index_comport(int available_ports,int p);

