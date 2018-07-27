#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Models/modellistusers.h"
#include "Models/modeltablegames.h"

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

private:
    Ui::MainWindow *ui;
    TcpServerClients* m_serverClient;

    ModelListUsers m_modelUsers;
    ModelTableGames m_modelGames;
};

#endif // MAINWINDOW_H
