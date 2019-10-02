#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QRegExpValidator>
#include <QAbstractButton>
#include "clientsettings.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(ClientSettings &settings,QWidget *parent = 0);
    ~SettingsDialog();

private slots:

    void on_button_Ok_clicked();

    void on_button_Cancel_clicked();

private:
    Ui::SettingsDialog *ui;

    ClientSettings m_settings;

    QRegExpValidator m_nameValidator;
    QRegExpValidator m_adressValidator;
    QRegExpValidator m_portValidator;
};

#endif // SETTINGSDIALOG_H
