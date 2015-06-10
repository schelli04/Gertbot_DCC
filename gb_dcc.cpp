//
// Demo code QT interface to gertbot running as DCC server
//
// This code has been written to be easy to read, not to run optimally
// You will also see that I have  implemented the same function using different
// code as a teaching tool of 'you can do it this waay as well'
// 
// I am NOT using the gerbot drivers 
// For DCC 99% of the driver routines are not required
// (Unless you want to use th other channels for DC railway control)
// At some point I might write DCC drivers only
//

#include <stdio.h>

#include "gb_dcc.h"
#include "gertbot_defines.h"

bool uart_opened;

bool chn0 = true, chn1 = true, chn2 = true, chn3 = true;

unsigned char loc_nr1 = 1, loc_nr2 = 2, loc_nr3 = 3;

gb_dcc::gb_dcc(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{ 
#ifdef _WIN32
  int b;
#endif
  QString str;
  ui.setupUi(this);
//  setWindowFlags(this->windowFlags() | Qt::WindowContextHelpButtonHint);
  
  uart_opened = false;

#ifdef _WIN32
  // Running on windows we need to do some nasty 'stuff'
  // to find a commport and use it to talk to the Gertbot
  // The code below is simplified: it uses the first available commport
  // For better code see the gertbot gui
  available_commports = available_com_ports();
  if (available_commports==0)
  { message_box.setText("No comm ports available, exiting...");
    message_box.exec();         
    // exit program 
    return;
  }
  else
  { // To keep things simple take the first available one
    for (b=0; b<32 && !uart_opened; b++)
    { if (available_commports & (1<<b))
      { // try to open comm port
        if (open_uart(b+1)==0)
          uart_opened = true;
        else
        { str.sprintf("Could not open COM %d",b+1);
          message_box.setText(str);
          message_box.exec();         
        }
      }
    }
    if (!uart_opened)
    { message_box.setText("Could not open any COM port. Exiting...");
      message_box.exec();        
      return; 
    }
  }
#else
  if (open_uart(0)==0)
    uart_opened = true;
  else
  { message_box.setText("Could not open UART. Check availability (enable_uart cf). Exiting...");
    message_box.exec();         
    // exit program 
    return;
  }
#endif

 
  // Connect up the controls
  connect(ui.pb_Stop_loc1 ,SIGNAL(clicked(bool)), this, SLOT(loc1_stop_slot()));
  connect(ui.pb_Stop_loc2 ,SIGNAL(clicked(bool)), this, SLOT(loc2_stop_slot()));
  connect(ui.pb_Stop_loc3 ,SIGNAL(clicked(bool)), this, SLOT(loc3_stop_slot()));
  
  connect(ui.spB_loco1 ,SIGNAL(valueChanged(int)), this, SLOT(loc1_spB_slot(int)));
  connect(ui.spB_loco2 ,SIGNAL(valueChanged(int)), this, SLOT(loc2_spB_slot(int)));
  connect(ui.spB_loco3 ,SIGNAL(valueChanged(int)), this, SLOT(loc3_spB_slot(int)));
  
  connect(ui.chkBx_Chn0, SIGNAL(stateChanged()), this, SLOT(chn_chkBx_slot()));
  connect(ui.chkBx_Chn1, SIGNAL(stateChanged()), this, SLOT(chn_chkBx_slot()));
  connect(ui.chkBx_Chn2, SIGNAL(stateChanged()), this, SLOT(chn_chkBx_slot()));
  connect(ui.chkBx_Chn3, SIGNAL(stateChanged()), this, SLOT(chn_chkBx_slot()));
  
  connect(ui.sld_loc1,SIGNAL(valueChanged(int)), this, SLOT(loc1_slide_slot(int)));
  connect(ui.sld_loc2,SIGNAL(valueChanged(int)), this, SLOT(loc2_slide_slot(int)));
  connect(ui.sld_loc3,SIGNAL(valueChanged(int)), this, SLOT(loc3_slide_slot(int)));

  connect(ui.pb_pointA ,SIGNAL(toggled(bool)), this, SLOT(pointA_slot(bool)));
  connect(ui.pb_pointB ,SIGNAL(toggled(bool)), this, SLOT(pointB_slot(bool)));
  connect(ui.pb_pointC ,SIGNAL(toggled(bool)), this, SLOT(pointC_slot(bool)));
  connect(ui.pb_pointD ,SIGNAL(toggled(bool)), this, SLOT(pointD_slot(bool)));

  connect(ui.pb_signalA ,SIGNAL(toggled(bool)), this, SLOT(sigA_slot(bool)));
  connect(ui.pb_signalB ,SIGNAL(toggled(bool)), this, SLOT(sigB_slot(bool)));
  connect(ui.pb_signalC ,SIGNAL(toggled(bool)), this, SLOT(sigC_slot(bool)));
  connect(ui.pb_signalD ,SIGNAL(toggled(bool)), this, SLOT(sigD_slot(bool)));

  connect(ui.pb_stop_all,SIGNAL(clicked(bool)), this, SLOT(stop_all_slot()));

  // 'cute' trains as slider handle
  ui.sld_loc1->setStyleSheet(
    "QSlider::handle:vertical {image: url(:/gb_dcc/Resources/train.bmp) 1; }");
  ui.sld_loc2->setStyleSheet(
    "QSlider::handle:vertical {image: url(:/gb_dcc/Resources/train.bmp) 1; }");
  ui.sld_loc3->setStyleSheet(
    "QSlider::handle:vertical {image: url(:/gb_dcc/Resources/train.bmp) 1; }");

  set_all_channels_to_dcc(0); // assume only board 0 for now 
  dcc_config(16,8,0); 
} // constructor

// when exiting it is good practice to
// close the UART connection
gb_dcc::~gb_dcc()
{
  if (uart_opened)
  {
    close_uart();
    uart_opened = false;
  }
} // ~gb_dcc

//
// Send STOP command to decoder
//
#define DCC_STOP_CMND 0x40

// ab hier neu
void gb_dcc::loc1_spB_slot(int nr)
{
  loc_nr1 = nr;
}

void gb_dcc::loc2_spB_slot(int nr)
{
  loc_nr2 = nr;
}

void gb_dcc::loc3_spB_slot(int nr)
{
  loc_nr3 = nr;
}


void gb_dcc::loc1_stop_slot()
{ 
  send_2byte_command(loc_nr1,DCC_STOP_CMND);
  // Set slider back to the middle
  ui.sld_loc1->setValue(0);
} // loc1_stop_slot

void gb_dcc::loc2_stop_slot()
{ send_2byte_command(loc_nr2,DCC_STOP_CMND);
  // Set slider back to the middle
  ui.sld_loc2->setValue(0);
} // loc2_stop_slot

void gb_dcc::loc3_stop_slot()
{ send_2byte_command(loc_nr3,DCC_STOP_CMND);
  // Set slider back to the middle
  ui.sld_loc3->setValue(0);
} // loc3_stop_slot

//
// I use a simple 28 step speed system 
// But the sliders in the GUI are set to go from -30...30
// This is done so I have a bit more leeway in the middle
// to make sure the value is set to STOP
// (Not very slowly crawling)
//
// This is a simple set-up where the systen can flood
// the Gertbot with a lot of changes
// To see a more sophisticated system where the computer
// is prevented from sending too fast see the gertbot
// code, specifically the "gertbot_dcc.cpp" file
//

// 28-value slider (speed) tables
// with 3 zero points 
//
static int slide_28_table[31] = { 
    0x40,   0x40,   0x40,    // tripple zero 
    0x42,0x52,    0x43,0x53,
    0x44,0x54,    0x45,0x55,
    0x46,0x56,    0x47,0x57,
    0x48,0x58,    0x49,0x59,
    0x4A,0x5A,    0x4B,0x5B,
    0x4C,0x5C,    0x4D,0x5D,
    0x4E,0x5E,    0x4F,0x5F
};

// In my case the trains go forward if the direction bit is SET
// Thus I set the bit on positive slider value 
#define DCC_DIRECTION_BIT 0x20

void gb_dcc::loc1_slide_slot(int v)
{ unsigned char speed_byte;
  if (v<0)
  { // negative: reverse direction
    v = -v; // make positive 
    speed_byte = 0;
  }
  else
  { // Postive = forward: set direction bit
    speed_byte = DCC_DIRECTION_BIT;
  }
  // Now add the table value 
  speed_byte = speed_byte | slide_28_table[v];
  send_2byte_command(loc_nr1,speed_byte);
} // loc1_slide_slot

//
// Second slide
// Note could have a different speed table here....
//
void gb_dcc::loc2_slide_slot(int v)
{ unsigned char speed_byte;
  if (v<0)
  { v = -v; // make positive 
    speed_byte = 0;
  }
  else
  {
    speed_byte = DCC_DIRECTION_BIT;
  }
  // Now add the table value 
  speed_byte = speed_byte | slide_28_table[v];
  send_2byte_command(loc_nr2,speed_byte);
} // loc2_slide_slot

//
// Second slide
// Note could have a different speed table here....
//
void gb_dcc::loc3_slide_slot(int v)
{ unsigned char speed_byte;
  if (v<0)
  { v = -v; // make positive 
    speed_byte = 0;
  }
  else
  {
    speed_byte = DCC_DIRECTION_BIT;
  }
  // Now add the table value 
  speed_byte = speed_byte | slide_28_table[v];
  send_2byte_command(loc_nr3,speed_byte);
} // loc3_slide_slot

void gb_dcc::chn_chkBx_slot()
{
    chn0 = ui.chkBx_Chn0->isChecked();
    chn1 = ui.chkBx_Chn1->isChecked();
    chn2 = ui.chkBx_Chn2->isChecked();
    chn3 = ui.chkBx_Chn3->isChecked();
}

//
// Send 2 byte command 
// (All commands in this demo are 2 byte DCC commands)
//
void gb_dcc::send_2byte_command(unsigned char b1,unsigned char b2)
{ unsigned char message[10];
  QString str_1;
  std::stringstream ss;
  message[0] = CMD_START_VAL;// Start of Message, comes from gertbot_defines.h
  message[1] = CMD_DCC_MESS; // comes from gertbot_defines.h
  message[2] = 0;            // Assuming only one board (0) connected 
  message[3] = 0xF0 |        // Send to all channels 
               2  ;          // Size is two bytes 
  message[4] = b1;           // First byte
  message[5] = b2;           // Second byte 
  // Here we have 3 unused bytes 
  message[6] = 0x00;         // Unused: set to zero 
  message[7] = 0x00;         // Unused: set to zero 
  message[8] = 0x00;         // Unused: set to zero 
  message[9] = CMD_STOP_VAL; // End of Message, comes from gertbot_defines.h 
  // Send message out
  write_uart(message,10);

  for(int i=0;i<10;++i)
    ss << std::hex
       << std::uppercase
       << std::setw(2)
       << std::setfill('0')
       << (int)message[i];

  str_1 = QString::fromStdString(ss.str());
  for(int i=0;i<10;i++)
    str_1.insert(2+3*i, ":");
  ui.te_log->appendPlainText(str_1);
} // send_2byte_command

//
// Reverse engineered Point decoder
// When I press the 'learn' button it picks up on the lowest
// address which in this case is 0x80 0x08
// The next address switches the same point over
// I have not been able to match this up with the DCC standard....
//
//
#define DCC_POINTA_ADRS 0x8008 // Decoder address point A
#define DCC_POINTB_ADRS 0x800A // Decoder address point B
#define DCC_POINTC_ADRS 0x800C // Decoder address point C
#define DCC_POINTD_ADRS 0x800E // Decoder address point D

#define DCC_SIGA_ADRS 0x8018 // Decoder address point A
#define DCC_SIGB_ADRS 0x801A // Decoder address point B
#define DCC_SIGC_ADRS 0x801C // Decoder address point C
#define DCC_SIGD_ADRS 0x801E // Decoder address point D

// This converts a function number to a bit 
#define FUNCTION_TO_BIT(x) (1<<(x-1))

void gb_dcc::pointA_slot(bool down)
{ unsigned char cmnd1,cmnd2;
  cmnd1 = (DCC_POINTA_ADRS>>8) & 0xFF; // MS byte
  cmnd2 = (DCC_POINTA_ADRS   ) & 0xFF; // LS byte 

  // We have been passed the state of A : down (or not)
  if (!down)
    cmnd2 |=1; // One LS address further 

  send_2byte_command(cmnd1,cmnd2);
} // pointA_slot


void gb_dcc::pointB_slot(bool down)
{ unsigned char cmnd1,cmnd2;
  cmnd1 = (DCC_POINTB_ADRS>>8) & 0xFF; // MS byte
  cmnd2 = (DCC_POINTB_ADRS   ) & 0xFF; // LS byte 

  // We have been passed the state of B : down (or not)
  if (!down)
    cmnd2 |=1; // One LS address further 

  send_2byte_command(cmnd1,cmnd2);
} // pointB_slot


void gb_dcc::pointC_slot(bool down)
{ unsigned char cmnd1,cmnd2;
  cmnd1 = (DCC_POINTC_ADRS>>8) & 0xFF; // MS byte
  cmnd2 = (DCC_POINTC_ADRS   ) & 0xFF; // LS byte 

  // We have been passed the state of C : down (or not)
  if (!down)
    cmnd2 |=1; // One LS address further 

  send_2byte_command(cmnd1,cmnd2);
} // pointC_slot


void gb_dcc::pointD_slot(bool down)
{ unsigned char cmnd1,cmnd2;
  cmnd1 = (DCC_POINTD_ADRS>>8) & 0xFF; // MS byte
  cmnd2 = (DCC_POINTD_ADRS   ) & 0xFF; // LS byte 

  // We have been passed the state of D : down (or not)
  if (!down)
    cmnd2 |=1; // One LS address further 

  send_2byte_command(cmnd1,cmnd2);
} // pointD_slot



void gb_dcc::sigA_slot(bool down)
{ unsigned char cmnd1,cmnd2;
  cmnd1 = (DCC_SIGA_ADRS>>8) & 0xFF; // MS byte
  cmnd2 = (DCC_SIGA_ADRS   ) & 0xFF; // LS byte 

  // We have been passed the state of A : down (or not)
  if (!down)
    cmnd2 |=1; // One LS address further 

  send_2byte_command(cmnd1,cmnd2);
} // sigA_slot


void gb_dcc::sigB_slot(bool down)
{ unsigned char cmnd1,cmnd2;
  cmnd1 = (DCC_SIGB_ADRS>>8) & 0xFF; // MS byte
  cmnd2 = (DCC_SIGB_ADRS   ) & 0xFF; // LS byte 

  // We have been passed the state of B : down (or not)
  if (!down)
    cmnd2 |=1; // One LS address further 

  send_2byte_command(cmnd1,cmnd2);
} // sigB_slot


void gb_dcc::sigC_slot(bool down)
{ unsigned char cmnd1,cmnd2;
  cmnd1 = (DCC_SIGC_ADRS>>8) & 0xFF; // MS byte
  cmnd2 = (DCC_SIGC_ADRS   ) & 0xFF; // LS byte 

  // We have been passed the state of C : down (or not)
  if (!down)
    cmnd2 |=1; // One LS address further 

  send_2byte_command(cmnd1,cmnd2);
} // sigC_slot


void gb_dcc::sigD_slot(bool down)
{ unsigned char cmnd1,cmnd2;
  cmnd1 = (DCC_SIGD_ADRS>>8) & 0xFF; // MS byte
  cmnd2 = (DCC_SIGD_ADRS   ) & 0xFF; // LS byte 

  // We have been passed the state of D : down (or not)
  if (!down)
    cmnd2 |=1; // One LS address further 

  send_2byte_command(cmnd1,cmnd2);
} // sigD_slot

//
// Stop all button pressed
// Send stop-all DCC command
// 
#define DCC_BROADCAST_ADRS 0x00
#define DCC_STOP_ALL_CMND  0x50

void gb_dcc::stop_all_slot()
{ send_2byte_command(DCC_BROADCAST_ADRS,DCC_STOP_ALL_CMND);
  // Set all sliders to the stop position 
  ui.sld_loc1->setValue(0);
  ui.sld_loc2->setValue(0);
  ui.sld_loc3->setValue(0);
} // stop_all_slot

//
// Setup the Gertbot
// All four channels are set to DCC
// Also disable short-circuit shut-down 
//
void gb_dcc::set_all_channels_to_dcc(unsigned char board)
{ unsigned char message[5],chan;

  // first send the don't shut down on short messages 
  for (chan=0; chan<4; chan++)
  {
    message[0] = CMD_START_VAL;// Start of Message, comes from gertbot_defines.h
    message[1] = CMD_STOPSHORT;// Set endtops/short mode command
    message[2] = (board<<2) | chan;  // Board  & channel
    message[3] = ENDSTOP_OFF|ENB_ERR_IGNORE; // No endstops, ignore shorts
    message[4] = CMD_STOP_VAL; // End of Message, comes from gertbot_defines.h 
    write_uart(message,5);
  }

  // Now set the channels to DCC mode 
  for (chan=0; chan<4; chan++)
  {
    message[0] = CMD_START_VAL;// Start of Message, comes from gertbot_defines.h
    message[1] = CMD_OPMODE;   // Set channel mode command
    message[2] = (board<<2) | chan;  // Board  & channel
    message[3] = MOT_MODE_DCC; // DCC mode
    message[4] = CMD_STOP_VAL; // End of Message, comes from gertbot_defines.h 
    write_uart(message,5);
  }
} // set_all_channels_to_dcc

//
// Send a DCC configure command
//
void gb_dcc::dcc_config(unsigned char preamble, // # preamble bits
                        unsigned char repeat,   // packet repeat
                        unsigned char flags     // 0 (1 = for debug only) 
                        )
{ unsigned char message[10];
  message[0] = CMD_START_VAL;// Start of Message, comes from gertbot_defines.h
  message[1] = CMD_DCC_CONFIG; // comes from gertbot_defines.h
  message[2] = 0;              // board 0, channel 0 
  message[3] = repeat;         // The speed byte 
  message[4] = preamble;       // Address of loc
  message[5] = 0x00;         // DC is always kept at zero  
  message[6] = flags;        // Flags
  message[7] = CMD_STOP_VAL; // End of Message, comes from gertbot_defines.h 
  // Send message out
  write_uart(message,8);
} // send_2byte_command

