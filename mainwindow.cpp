#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tab.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Tab *m_tab = new Tab(this);
    setCentralWidget(m_tab);

}

MainWindow::~MainWindow()
{
    delete ui;
}
