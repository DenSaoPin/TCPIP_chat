#include "mainwindow.h"
#include "settingsdialog.h"
#include "clientsettings.h"
#include <QApplication>
#include <ChatClientAPI.h>
#include <QThread>
#include "worker.h"
#include "mainloopworker.h"
#include "statuscheckerworker.h"
#include "threadcontroller.h"
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    callbackMessageReceivedFunc(OnRecieveMessage);

    MainWindow w;
    Client *cl = new Client(&w);
    SettingsDialog dialog(cl->pSettings);

    int result = dialog.exec();
    if(result == QDialog::Rejected)
    {
          return 0;
    }

    cl->pSettings->SetupToDll();

    MainLoopWorker mainWorker;
    ThreadController MainController(mainWorker, &w);
    QString str;
    //MainController.operate(str);

    StatusCheckerWorker statusWorker;
    ThreadController StatusController(statusWorker, &w);

    int i = 12;

    w.show();

    return a.exec();
}
