#include "mainwindow.h"
#include "settingsdialog.h"
#include "clientsettings.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ClientSettings settings;

    SettingsDialog dialog(settings);

    int result = dialog.exec();

    if(result == QDialog::Rejected)
    {
          return 0;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
