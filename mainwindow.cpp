#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
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

    ui->listView_Users->setModel(&m_modelUsers);
    connect(m_serverClient, &TcpServerClients::newUserConnected, &m_modelUsers, &ModelListUsers::addNewUser);
    connect(m_serverClient, &TcpServerClients::userDisconnected, &m_modelUsers, &ModelListUsers::removeAUser);

    ui->tableView_Games->setModel(&m_modelGames);
    connect(InstanceManager::instance(), &InstanceManager::newGameCreated, &m_modelGames, &ModelTableGames::addNewGame);
    connect(InstanceManager::instance(), &InstanceManager::gameRemoved, &m_modelGames, &ModelTableGames::removeAGame);
    connect(ui->tableView_Games, &QTableView::doubleClicked, this, &MainWindow::onDClicked_TableView_Games);
}

MainWindow::~MainWindow()
{
    InstanceManager::deleteInstance();

    if(m_serverClient->isRunning())
        m_serverClient->close();
    delete m_serverClient;

    delete ui;
}

/************************************************************
*****			  FONCTIONS SLOT PRIVEES				*****
************************************************************/
void MainWindow::onDClicked_TableView_Games(const QModelIndex& index)
{
    if(index.column() == static_cast<int>(ModelTableGames::ROLE_BUTTON_DELETE))
    {
        unsigned int idGame = m_modelGames.data(m_modelGames.index(index.row(), static_cast<int>(ModelTableGames::ROLE_ID))).value<unsigned int>();

        int reponse = QMessageBox::question(this, "Confirmation", "Etes-vous sûr de vouloir supprimer cette instance ?", "Oui", "Non");
        if(reponse == 0)
        {
            InstanceManager::instance()->removeGame(idGame);
        }
    }
}
