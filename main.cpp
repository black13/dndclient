/*
1. Remove all old mail client parts of the program. Title labes etc.
2. Add basic menu items.
3. Remove labels from old mail client. 
4. Consider model/view arch.
5. Do save as that saves the document.
6. Consider how to execute the underlying the xml document.
**/
#include <QtWidgets/QMainWindow>
#include <QStyle>
#include <QApplication>
#include "dndclient.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow client;
    client.show();
    return app.exec();
}
