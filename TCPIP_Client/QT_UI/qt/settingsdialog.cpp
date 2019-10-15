#include "settingsdialog.h"
#include "QRegExp"
#include "QMessageBox"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(ClientSettings *settings ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    m_nameValidator(QRegExp("[A-Za-z 0-9_]+") ,this),
    m_adressValidator(QRegExp("[A-Za-z0-9-.]+") ,this),
    m_portValidator(QRegExp("[0-9]+") ,this)
{
    this->m_settings = settings;

    ui->setupUi(this);

    ui->line_Name->setMaxLength(20);
    ui->line_Name->setValidator(&m_nameValidator);

    ui->line_Port->setMaxLength(5);
    ui->line_Port->setValidator(&m_portValidator);

    ui->line_Adress->setMaxLength(255);
    ui->line_Adress->setValidator(&m_adressValidator);

    QObject::connect(ui->button_Cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_button_Ok_clicked()
{
    if((ui->line_Name->text().length() != 0) &&
        ui->line_Adress->text().length() != 0 &&
        ui->line_Port->text().length() != 0)
    {
        m_settings->SetName(ui->line_Name->text());
        m_settings->SetAdress(ui->line_Adress->text());
        m_settings->SetPort(ui->line_Port->text());

        this->accept();
        this->close();
    }
    else
    {
        QMessageBox::warning(this, "Warning","You should fill all fields");
    }
}

void SettingsDialog::on_button_Cancel_clicked()
{
    this->close();
}
