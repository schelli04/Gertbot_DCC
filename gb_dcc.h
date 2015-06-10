
#ifndef QT_DCC_H
#define QT_DCC_H

#ifdef _WIN32

extern "C" {
#include "gertbot_winserial.h"
}

#define SLEEP_MS(x) Sleep(x)

#else

extern "C" {
#include "gertbot_pi_uart.h"
}

#define SLEEP_MS(x) usleep(x*1000)

#endif


#include "assert.h"

#include <QtGui/QMainWindow>
#include "ui_gb_dcc.h"
#include <qmessagebox.h>
#include <stdint.h>
#include <string>   // Strings
#include <iomanip>  // setw(), setfill()
#include <sstream>

#include "gertbot_defines.h"

class gb_dcc : public QMainWindow
{
    Q_OBJECT

public:
    gb_dcc(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~gb_dcc();
    bool uart_opened;

private:
    Ui::gb_dcc ui;
    QMessageBox message_box;   // to pop-up error messages and warnings 
    uint32_t available_commports;
    void set_all_channels_to_dcc(unsigned char board);
    void send_2byte_command(unsigned char b1,unsigned char b2);
    void dcc_config(unsigned char,unsigned char,unsigned char);

public slots:
  void loc1_spB_slot(int nr);
  void loc2_spB_slot(int nr);
  void loc3_spB_slot(int nr);
  void loc1_slide_slot(int v);
  void loc2_slide_slot(int v);
  void loc3_slide_slot(int v);
  void loc1_stop_slot();
  void loc2_stop_slot();
  void loc3_stop_slot();
  void chn_chkBx_slot();
  void pointA_slot(bool down);
  void pointB_slot(bool down);
  void pointC_slot(bool down);
  void pointD_slot(bool down);
  void sigA_slot(bool down);
  void sigB_slot(bool down);
  void sigC_slot(bool down);
  void sigD_slot(bool down);
  void stop_all_slot();

};


#endif // QT_DCC_H
