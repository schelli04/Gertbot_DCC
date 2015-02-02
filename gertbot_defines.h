//
//
// gertbot project
// 
// Defines for interfacing
// 
//

#ifndef GERTBOT_DEFINES_H
#define GERTBOT_DEFINES_H

#define CMD_START_VAL  0xA0  // Serial protocol start flag
#define CMD_STOP_VAL   0x50  // Serial protocol end flag
#define CMD_DIRECT1    0xA1  // Serial protocol direct flag 
#define CMD_DIRECT4    0xA4  // Serial protocol direct flag 
#define CMD_MAXVAL     0x2F  // Highest possible command byte 

#define CMD_OPMODE     0x01 // <ID> <mode>
#define CMD_STOPSHORT 0x02 // <ID> <stopmask> 
#define CMD_BRD_STATUS 0x03 // <ID> 
#define CMD_LINFREQ    0x04 // <ID> <MS><LS> 
#define CMD_LINDC      0x05 // <ID> <MS><LS>
#define CMD_LINON      0x06 // <ID> <dir>
#define CMD_GET_ERROR  0x07 // <ID> 
#define CMD_STEP       0x08 // <ID> <MS><MM><LS>
#define CMD_STEPFREQ   0x09 // <ID> <MS><MM><LS>
#define CMD_STOPALL    0x0A // 0x81
#define CMD_STOP2ND    0x81 
#define CMD_OD         0x0B // <ID> <on/off>
#define CMD_DAC        0x0C // <ID> <MS><LS>
#define CMD_GET_ADC    0x0D // <ID> <MS><LS>
#define CMD_READIO     0x0E // <ID> <MS><MM><LS>
#define CMD_WRITEIO    0x0F // <ID> <MS><MM><LS>
#define CMD_SETIO      0x10 // <ID> <MS><MM><LS>
#define CMD_SETADCDAC  0x11 // <ID> <ADC><DAC>
#define CMD_CONFIGURE  0x12 // <ID> <MS><LS>
#define CMD_VERSION    0x13 // <ID>
#define CMD_MOT_STATUS 0x14 // <ID>
#define CMD_SYNC       0x15 // 0x18
#define CMD_POLL       0x16 // <ID>
#define CMD_PWR_OFF    0x17 // 0x81
#define CMD_IO_STATUS  0x18 // <ID>
#define CMD_DCC_MESS   0x19 // <ID> <format> <d0> <d1> <d2> <d3> <d4>
#define CMD_DCC_CONFIG 0x1A // <ID> <repeat> <preamble> <dc> 0
#define CMD_MOT_CONFIG 0x1B // <ID> 
#define CMD_MOT_MISSED 0x1C // <ID>
#define CMD_SET_RAMP   0x1D // <ID> <up|down> <hlt>
#define CMD_UNUSED1E   0x1E // -
#define CMD_UNUSED1F   0x1F // -
#define CMD_REFLASH    0x20 // Upload new image 
#define CMD_UNUSED21   0x21 // -
#define CMD_UNUSED22   0x22 // -
#define CMD_UNUSED23   0x23 // -
#define CMD_UNUSED24   0x24 // -
#define CMD_UNUSED25   0x25 // -
#define CMD_UNUSED26   0x26 // -
#define CMD_UNUSED27   0x27 // -
#define CMD_UNUSED28   0x28 // -
#define CMD_UNUSED29   0x29 // -
#define CMD_UNUSED2A   0x2A // -
#define CMD_UNUSED2B   0x2B // -
#define CMD_UNUSED2C   0x2C // -
#define CMD_UNUSED2D   0x2D // -
#define CMD_UNUSED2E   0x2E // -
#define CMD_UNUSED2F   0x2F // -

#define MAX_COMMAND    0x2F

// Mode is coded so bits 3 & 4 are stepper codes
#define MOT_MODE_OFF         0x00
#define MOT_MODE_BRUSH       0x01
#define MOT_MODE_DCC         0x02
#define MOT_MODE_STEP_GRAY   0x08
#define MOT_MODE_STEP_PULSE  0x09
//#define MOT_MODE_STEP_WAVE   0x0A
//#define MOT_MODE_STEP_USER   0x0B
#define MOT_MODE_STEP_MASK   0x0C // Bits  stepper mode 
#define MOT_MODE_STEP_HOLD   0x10 // Flag to hold power on stepper when done

// Motor movement
#define MOT_PWR_OFF    0x00 // not moving, no power
#define MOT_MOVE_A     0x01 // move towards A
#define MOT_MOVE_B     0x02 // move towards B
#define MOT_MOVE_OFF   0x03 // powered stop for stepper
// May need RAMP_UP, RAMP_DOWN,RAMP_HALT states! 

// End-stop configure
#define ENDSTOP_OFF    0x00
#define ENDSTOP_A      0x01
#define ENDSTOP_B      0x02
#define ENDSTOP_A_LOW  0x00
#define ENDSTOP_A_HIGH 0x01
#define ENDSTOP_B_LOW  0x00
#define ENDSTOP_B_HIGH 0x02

// Enable (error) modes
#define ENB_ERR_IGNORE 0x00 // Ignore
#define ENB_ERR_THIS   0x01 // Switch off this channel only
#define ENB_ERR_PAIR   0x02 // Switch off pair (A&B or C&D) 
#define ENB_ERR_BOARD  0x03 // Switch off A,B,C & D
#define ENB_ERR_SYSTEM 0x04 // Switch off all boards (halt line)

// Command size defines 
// The first byte is not counted
// thus a stop-all <0x0A 0x18> has size 1 
// 0 is disabled 
#define CMD_CL00 0
#define CMD_CL01 2
#define CMD_CL02 2 
#define CMD_CL03 1
#define CMD_CL04 3
#define CMD_CL05 3
#define CMD_CL06 2 
#define CMD_CL07 1
#define CMD_CL08 4
#define CMD_CL09 4
#define CMD_CL0A 1 // 0A 18
#define CMD_CL0B 2
#define CMD_CL0C 3
#define CMD_CL0D 1
#define CMD_CL0E 1
#define CMD_CL0F 4
#define CMD_CL10 4
#define CMD_CL11 3
#define CMD_CL12 4
#define CMD_CL13 1
#define CMD_CL14 1
#define CMD_CL15 1
#define CMD_CL16 1
#define CMD_CL17 1
#define CMD_CL18 1
#define CMD_CL19 7 // 19 <id> <format> <d0> <d1> <d2> <d3> <d4> <d5>
#define CMD_CL1A 5  // 5 1A <id> <repeat> <preamble> <dc_ms> <dc_ls>
#define CMD_CL1B 1
#define CMD_CL1C 1
#define CMD_CL1D 3
#define CMD_CL1E 0
#define CMD_CL1F 0
#define CMD_CL20 3
#define CMD_CL21 0
#define CMD_CL22 0
#define CMD_CL23 0
#define CMD_CL24 0
#define CMD_CL25 0
#define CMD_CL26 0
#define CMD_CL27 0
#define CMD_CL28 0
#define CMD_CL29 0
#define CMD_CL2A 0
#define CMD_CL2B 0
#define CMD_CL2C 0
#define CMD_CL2D 0
#define CMD_CL2E 0
#define CMD_CL2F 0

// System error values 
#define ERROR_INP_OVERFLOW 0x0001 // Command input buffer overflow 
#define ERROR_FATALSYSTEM  0x0002 // Program internal error 
#define ERROR_MODEERR      0x0003 // Mode command value error 
#define ERROR_ENDSTOP      0x0004 // End stop parameter wrong 
#define ERROR_LINFREQ      0x0005 // Lin. freq error: Motor was not in linear mode 
#define ERROR_LINDC        0x0006 // Duty cycle error: Motor was not in linear mode
#define ERROR_ILLPWMFREQ   0x0007 // Illegal linear freq given
#define ERROR_ILLPWMDC     0x0008 // Illegal duty cycle given 
#define ERROR_NOLINFREQ    0x0009 // Brushed motor start but not given freq.
#define ERROR_NOSTEPFREQ   0x000A // Stepper motor start but not given freq.
#define ERROR_STEP         0x000B // Step command: Motor was not in step mode
#define ERROR_STEPFREQ     0x000C // Step freq. command: Motor was not in step mode
//#define ERROR_ENDSTOPERR   0x000D // Illegal end stop command 
//#define ERROR_MODECONFL    0x000E // Command conflicts with mode 
//#define ERROR_ENDSPOL      0x000F // End stop polarity command error 
#define ERROR_HALTACTIVE   0x0010 // Start command given with halt active 
#define ERROR_TQ_EMPTY     0x0011 // Timer queue pool empty 
#define ERROR_SEROUTOVFL   0x0012 // Serial output queue overflow 
#define ERROR_ILLSTEPFREQ  0x0013 // Illegal stepper frequency 
#define ERROR_DACDIS       0x0014 // Write to DAC which is disabled 
#define ERROR_ADCDIS       0x0015 // Read from ADC which is disabled 
#define ERROR_ADCMASK      0x0016 // Enable ADC illegal mask bits
#define ERROR_DACMASK      0x0017 // Enable DAC illegal mask bits
#define ERROR_LINON        0x0018 // Linear on/off direction but not a lin. motor
#define ERROR_ILLCOMMAND   0x0019 // Illegal command
#define ERROR_HALTSEEN     0x001A // HALT was active
#define ERROR_A_SEEN       0x001B // Enable A was active
#define ERROR_B_SEEN       0x001C // Enable B was active
#define ERROR_C_SEEN       0x001D // Enable c was active
#define ERROR_D_SEEN       0x001E // Enable D was active
#define ERROR_DCC_OVFL     0x001F // DCC message queue overflow 
#define ERROR_DCC_MESS     0x0020 // DCC illegal message (length)

#endif // GERTBOT_DEFINES_H



