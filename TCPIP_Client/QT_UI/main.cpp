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
    ClientSettings settings;
    SettingsDialog dialog(&settings);
    settings.SetupToDll();

    int result = dialog.exec();
    if(result == QDialog::Rejected)
    {
          return 0;
    }
    settings.SetupToDll();

    Client *cl = new Client(&settings);
    setCallbackMessageReceived(MainWindow::OnRecieveMessage);

    cl->Window.show();
    cl->pMainLoopController->Operate();
    cl->pStatusCheckerController->Operate();

    return a.exec();
}
