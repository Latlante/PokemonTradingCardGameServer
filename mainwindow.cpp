#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include "instancemanager.h"
#include "gateway.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_gateway(new Gateway())
{
    ui->setupUi(this);
    new InstanceManager();

    m_gateway->startServers();

    ui->listView_Users->setModel(&m_modelUsers);
    connect(m_gateway, &Gateway::newUserConnected, &m_modelUsers, &ModelListUsers::addNewUser);
    connect(m_gateway, &Gateway::userDisconnected, &m_modelUsers, &ModelListUsers::removeAnUser);

    ui->tableView_Games->setModel(&m_modelGames);
    connect(m_gateway, &Gateway::newInstanceConnected, &m_modelGames, &ModelTableGames::addNewGame);
    connect(m_gateway, &Gateway::instanceAuthentified, &m_modelGames, &ModelTableGames::gameAuthentified);
    connect(m_gateway, &Gateway::instanceDisconnected, &m_modelGames, &ModelTableGames::removeAGame);

    connect(InstanceManager::instance(), &InstanceManager::newGameCreated, &m_modelGames, &ModelTableGames::addNewGame);
    connect(InstanceManager::instance(), &InstanceManager::gameRemoved, &m_modelGames, &ModelTableGames::removeAGame);
    connect(ui->tableView_Games, &QTableView::doubleClicked, this, &MainWindow::onDClicked_TableView_Games);
}

MainWindow::~MainWindow()
{
    InstanceManager::deleteInstance();
    delete m_gateway;

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
