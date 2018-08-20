#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Models/modellistusers.h"
#include "Models/modeltablegames.h"

class Gateway;
class TcpServerClients;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDClicked_TableView_Games(const QModelIndex& index);

private:
    Ui::MainWindow *ui;
    Gateway *m_gateway;

    ModelListUsers m_modelUsers;
    ModelTableGames m_modelGames;
};

#endif // MAINWINDOW_H
