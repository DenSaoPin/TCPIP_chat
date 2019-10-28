#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "public/ChatClientAPI.h"

enum EMessageType
{
    eInvalide = 0x00,
    eNameRequest = 0x01,
    eDirectMessage = 0x05,
    eBroadcastMessage = 0x06,
    eResponse = 0x07,
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnRecieveMessage(const char *szName, const int* messageType, const char *szMessage)
{
    QString name (szName);
    QString message (szMessage);

    ui->textBox_Out->append(name + ": " + message + "\n");
}

void MainWindow::PrintMessage(QString& qstr)
{
    ui->textBox_Out->append(qstr + "\n");
}

void MainWindow::on_button_Send_clicked()
{
    QString text = ui->textBox_input->toPlainText();
    PrintMessage(text);

    QByteArray ba = text.toLocal8Bit();
    const char *szMessage = ba.data();

    ClientSendMessage(nullptr, (const int)EMessageType::eBroadcastMessage, szMessage, ba.length());

    ui->textBox_input->clear();
}

void MainWindow::on_actionReconnect_triggered()
{
    QString rec("@reconnect@");
/*
    if (m_clientStatus == EClientStatus::eTerminating)
    {
    }
    else if (m_clientStatus == EClientStatus::eShutDown)
    {
    }
    else
    {
        ClientTerminate();
    }
    QThread::usleep(500);
*/
    //m_clientDllMainThread = std::async(std::launch::async, ClientMainLoop);
}
