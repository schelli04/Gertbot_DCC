//
// Serial comm port SW
// For windows 
//

#include <stdio.h> 

#include "gertbot_winserial.h"

static HANDLE hComm=0; // Handle for com port

#define KEEP_RECEIVE

//
// open comm port <n>
// Todo: code to open again
//
//
// return 0 on success
// return 1 on failure
//
int open_uart(int com_port_number)
{
  TCHAR comm_name[16]; // need only 10 positions
  COMMTIMEOUTS comm_timing;

  wsprintf(comm_name,L"\\\\.\\COM%d",com_port_number);
//  wsprintf(comm_name,L"\\\\.\\COM5"); // PATCH!!!!!!!!!!!!!!!!!!
  hComm = CreateFile( comm_name, // L"\\\\.\\COM1",  // L".." converts string to long_pointer_to_wide_string 
                      GENERIC_READ | GENERIC_WRITE, 
                      0, 
                      0, 
                      OPEN_EXISTING,
                      0, // FILE_FLAG_OVERLAPPED,
                      0);
  if (hComm == INVALID_HANDLE_VALUE)
  { fprintf(stderr,"Error: Could not open com port '%d'\n",com_port_number);
     return 1;
  }

  // set timeouts on the comm port ( in milli seconds)
  // These are for 19200 baud, 10 bits/symbol  is 1/19200 = ~0.5 milli sec / byte
  comm_timing.ReadIntervalTimeout = 2;           // wait after receiving byte
  comm_timing.ReadTotalTimeoutMultiplier = 2;    // Wait per byte requested
  comm_timing.ReadTotalTimeoutConstant = 10;     // Minimum read wait time
  comm_timing.WriteTotalTimeoutMultiplier = 2;   // Same again for writing 
  comm_timing.WriteTotalTimeoutConstant = 10;    // Except there is no interval
  if (!SetCommTimeouts(hComm, &comm_timing))
  { fprintf(stderr,"Error: Could set com port timing\n");
    return 2;
  }
  return 0;
} // open_uart

void close_uart()
{
  CloseHandle(hComm);
}


//
// Read bytes from comm port
// sloppy: conversions between DWORD and integer
//
#ifdef KEEP_RECEIVE
#define BBS 512
int big_buffer[BBS];
int fill = 0;
int fill_cpy;
#endif 

//
// read bytes from UART
// Give it three tries 
//
int read_uart(unsigned char *read_buffer,int bytes)
{ DWORD num_read;
  int   remain,total_read,retry;
  remain = bytes;
  retry  = 3;
  total_read = 0;
  do { 
    ReadFile(hComm, (void *)read_buffer, bytes, &num_read, NULL);
    remain -= num_read;
    total_read  += num_read;
    if (remain)
    { // more to come
      read_buffer += num_read;
      SLEEP_MS(10); // @57600 baud this is time for 57 bytes! 
    }
  } while (remain && retry--);

#ifdef KEEP_RECEIVE
  // used for debugging 
  if (fill==0)
  { for (fill=0; fill<BBS; fill++)
      big_buffer[fill]=0;
    fill=1;
  }
  big_buffer[fill++] = total_read | 0x40000000;
  if (fill>=BBS)  fill=1;
  for (fill_cpy=0; fill_cpy<total_read; fill_cpy++)
  { big_buffer[fill++] = read_buffer[fill_cpy];
    if (fill>=BBS)  fill=1;
  }
#endif 
  return total_read; 
} // read_uart

//
// Write bytes to comm port
// sloppy: conversions between DWORD and integer
//
int write_uart(unsigned char *write_buffer,int bytes)
{ DWORD num_write;
  WriteFile(hComm, (void *)write_buffer, bytes, &num_write, NULL);
  return num_write; 
} // write_uart

/**************************\
 *                        *           
 * Windows extra routines *
 *                        *
\**************************/

//
// Returns integer with a bit set for each comm port found
// Limited to comm ports 1-32
//
// Stupid but effective method: tries to open each
// 
uint32_t available_com_ports(void)
{  
  TCHAR comm_name[16]; // need only 10 positions
  int com_port_number;
  uint32_t com_vector;
  HANDLE hComm; // Handle for com port
  com_vector = 0;
  for (com_port_number=1; com_port_number<=32; com_port_number++)
  {
    wsprintf(comm_name,L"\\\\.\\COM%d",com_port_number);
    hComm = CreateFile( comm_name, // L"\\\\.\\COM1",  // L".." converts string to long_pointer_to_wide_string 
                        GENERIC_READ | GENERIC_WRITE, 
                        0, 
                        0, 
                        OPEN_EXISTING,
                        0, // FILE_FLAG_OVERLAPPED,
                        0);
    if (hComm != INVALID_HANDLE_VALUE)
    { com_vector |= 1<<(com_port_number-1);
      CloseHandle(hComm);
    }
  } // for test port 1-32
  return com_vector;
} // available_com_ports

//
// Find 'comm' number of port <n> in bit vector
// e.g. if you have comports COM1, COM3, COM9, COM12
// p=2 returns '9' 
// 0 : no ports available 
//
int index_comport(int available_ports,int p)
{ int b;
  for (b=0; b<32; b++)
    if (available_ports & (1<<b))
    { p--;
      if (p==0)
        return b+1;
    }
  return 0;
} // index_comport


//
// Set up comm port using settings string
// e.g. "9600,n,8,1"
// or "baud=19200 parity=n data=8 stop=1"
//
// Return 0 on success
// return 1 on error
//
int  setup_comm_port(TCHAR *com_format)
{
  DCB port;     // Settings of com port
  // get the current DCB, and adjust a few bits to our liking.
  memset(&port, 0, sizeof(port));
  port.DCBlength = sizeof(port);
  if ( !GetCommState(hComm, &port))
  { fprintf(stderr,"Error: Could not get status of comm port\n");
    return 1;
  }
  if (!BuildCommDCB(com_format, &port))
  { fprintf(stderr,"Error: Illegal comm port settings\n");
    return 1;
  }
  if (!SetCommState(hComm, &port))
  { fprintf(stderr,"Error: Could not write comm port settings\n");
    return 1;
  }
  return 0;
} // setup_comm_port

