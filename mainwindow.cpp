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

    m_serverClient->start();
}

MainWindow::~MainWindow()
{
    InstanceManager::deleteInstance();
    m_serverClient->close();
    delete m_serverClient;
    delete ui;
}
