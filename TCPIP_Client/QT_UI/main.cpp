#include "qt/mainwindow.h"
#include "qt/settingsdialog.h"
#include "qt/clientsettings.h"
#include <QApplication>
#include <public/ChatClientAPI.h>
#include <QThread>
#include <QString>
#include "qt/worker.h"
#include "qt/mainloopworker.h"
#include "qt/statuscheckerworker.h"
#include "qt/threadcontroller.h"
#include "qt/client.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //callbackMessageReceivedFunc(OnRecieveMessage);

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

    StatusCheckerWorker statusWorker;
    ThreadController StatusController(statusWorker, &w);

    w.show();

    QString str("Test message");
    w.PrintMessage(str);

    return a.exec();
}
