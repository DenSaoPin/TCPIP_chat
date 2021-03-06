#include "threadcontroller.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include "QThread"
#include "public/ChatClientAPI.h"

enum EMessageType
{
    eInvalide = 0x00,
    eNameRequest = 0x01,
    eDirectMessage = 0x05,
    eBroadcastMessage = 0x06,
    eResponse = 0x07,
};

MainWindow* MainWindow::m_pI = nullptr;

MainWindow::MainWindow(Client *pClient, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->pClient = pClient;
    MainWindow::m_pI = this;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnRecieveMessage(const char *szName, const int* messageType, const char *szMessage)
{
    QString name (szName);
    QString message (szMessage);

    MainWindow::m_pI->ui->textBox_Out->append(name + ": " + message + "\n");
}

void MainWindow::PrintMessage(QString& qstr)
{
    ui->textBox_Out->append(qstr + "\n");
}

void MainWindow::on_button_Send_clicked()
{
    QString text = ui->textBox_input->toPlainText();
    if(!text.trimmed().isEmpty())
    {
        PrintMessage(text);

        QByteArray ba = text.toLocal8Bit();
        const char *szMessage = ba.data();

        ClientSendMessage(nullptr, static_cast<const int>(EMessageType::eBroadcastMessage), szMessage, ba.length());

        ui->textBox_input->clear();
    }
}

void MainWindow::on_actionReconnect_triggered()
{
    //QString rec("@reconnect@");

     if (pClient->DllStatus == EClientStatus::eTerminating)
     {
     }
     else if (pClient->DllStatus == EClientStatus::eShutDown)
     {
     }
     else
     {
         ClientTerminate();
     }
     QThread::usleep(500);
     pClient->pMainLoopController->Operate();
     pClient->pStatusCheckerController->Operate();
}

void MainWindow::on_actionShutdown_triggered()
{
    if (pClient->DllStatus == EClientStatus::eTerminating)
    {
    }
    else if (pClient->DllStatus == EClientStatus::eShutDown)
    {
    }
    else
    {
        ClientTerminate();
    }
}

void MainWindow::on_actionExit_triggered()
{
    pClient->pMainLoopController->Kill();
    pClient->pStatusCheckerController->Kill();

    QCoreApplication::quit();
}
