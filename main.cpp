//
// This is the main file which starts the GUI
//

#include "gb_dcc.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    gb_dcc main_window;
    if (!main_window.uart_opened)
      return 1;
    main_window.show();
    return application.exec();
} // main 
