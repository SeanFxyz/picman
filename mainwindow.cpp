#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

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


void MainWindow::on_addSrcButton_clicked()
{
    std::cout << "addSrcButton_clicked\n";
}
void MainWindow::on_rmSrcButton_clicked()
{
}
void MainWindow::on_cropButton_clicked()
{
}
void MainWindow::on_prevButton_clicked()
{
}
void MainWindow::on_nextButton_clicked()
{
}
void MainWindow::on_addDstButton_clicked()
{
}
void MainWindow::on_rmDstButton_clicked()
{
}
void MainWindow::on_pushButton_clicked()
{
}
