//
// Raspberry-Pi UART access from C
//
// Code based on
//  http://www.raspberrypi.org/phpBB3/viewtopic.php?t=7500&p=93257
//  mixed with minicom code
//

#include <unistd.h>   
#include <fcntl.h>    
#include <termios.h>  

#include <stdio.h>

int uart0_filestream = -1;

//
// Setup Raspberry-Pi UART
// return 0 on success
// return 1 on failure
//
// Code based on http://www.raspberrypi.org/phpBB3/viewtopic.php?t=7500&p=93257
// mixed with minicom code 
//
// 'port' is for compatibility with windows
//
// todo: save settings 
//
int open_uart(int port)
{ 
  struct termios options;
  if (uart0_filestream!=-1)
    return 0; // assume already opened
  uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);  //Open in non blocking read/write mode
  if (uart0_filestream == -1)
  {
    fprintf(stderr,"Error - Unable to open UART. Ensure it is not in use by another application\n");
    fflush(stderr);
    return 1;
  }

  // CONFIGURE THE UART
  // The flags (defined in /usr/include/termios.h -
  // see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
  tcgetattr(uart0_filestream, &options);
  cfsetospeed(&options,B57600);
  cfsetispeed(&options,B57600);
  options.c_cflag =  (options.c_cflag & ~CSIZE) | CS8; // 8 bits
  options.c_cflag |=  CLOCAL | CREAD;  // ignore mode status, enable rec.
  options.c_cflag &=  ~(PARENB | PARODD | CSTOPB); // No parity, 1 stop bit
 
  options.c_iflag = IGNBRK;
  options.c_iflag &= ~(IXON|IXOFF|IXANY);

  options.c_oflag = 0;
  options.c_lflag = 0;
  tcsetattr(uart0_filestream, TCSANOW, &options); // set the options NOW
  return 0;
} // open_uart

// 
// Close uart
// todo: restore settings 
//
void close_uart()
{
  if (uart0_filestream!=-1)
    close(uart0_filestream);
} // close_uart

//
// transmit data to uart
//
// return 0 on failure
// return 1 on success
//
int write_uart(unsigned char *data,int bytes)
{ int txed;
  while (bytes)
  {
    txed = write(uart0_filestream, data, bytes);
    if (txed==-1)
    { fprintf(stderr,"UART WRITE ERRROR!!\n");
      return 0;
    }
    bytes -= txed;
//    printf("a=%d\n",a);
    data += txed;
  }
  tcdrain(uart0_filestream);
  return 1;
} // write_uart

//
// Try to read 'len' characters from uart 
// 
// return the actual bytes read 
// 
int  read_uart(unsigned char *data,int len)
{ int rec,rep,tot;
  rep = len;  // use lenght as repeat 
  tot = 0;
  while (len>0 && rep)
  {
    rec = read(uart0_filestream,(void *)data,len);
    if (rec<=0)   // if we got no data

    { usleep(5000); // Wait 5 milli seconds 
      rep--; // time out after a while 
    }
    else
    { tot += rec;
      len -= rec;
      data += rec;  // move pointer to after what we already have 
    } 
  } 
  return tot;
} // read_uart
