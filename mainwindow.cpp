#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "instancemanager.h"
#include "tcpserverclients.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_serverClient(new TcpServerClients())
{
    ui->setupUi(this);
    new InstanceManager();

    if(InstanceManager::instance()->checkInstanceExeExists())
        m_serverClient->start();
    else
        qCritical() << __PRETTY_FUNCTION__ << "Instance exe not found";
}

MainWindow::~MainWindow()
{
    InstanceManager::deleteInstance();

    if(m_serverClient->isRunning())
        m_serverClient->close();
    delete m_serverClient;

    delete ui;
}
