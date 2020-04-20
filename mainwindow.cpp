#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    file_dialog_open = false;
    file_dialog = nullptr;
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_addSrcButton_clicked()
{
    file_dialog_open = true;
    file_dialog = new QFileDialog();
    file_dialog->setFileMode(QFileDialog::Directory);
    connect(file_dialog, &QFileDialog::filesSelected,
            this, &MainWindow::srcFilesSelected);
    file_dialog->show();
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
void MainWindow::srcFilesSelected(QStringList selected)
{
    QStringList src_files = expandFileList(selected);
    for(int i=0; i < src_files.size(); i++)
    {
        std::cout << src_files.at(i).toLocal8Bit().data() << std::endl;
        if(isImage(src_files.at(i)))
            QListWidgetItem src_file_item(QIcon(src_files.at(i)),
                                          src_files.at(i),
                                          src_list,
                                          QListWidgetItem::Type);
    }
}
void MainWindow::dstFilesSelected(QStringList selected)
{
}

QStringList MainWindow::expandFileList(QStringList file_list)
{
    QStringList new_file_list;
    for(int i=0; i<file_list.size(); i++)
    {
        QFileInfo file_info(file_list.at(i));
        if(file_info.isDir())
            new_file_list.append(
                    expandFileList(file_info.absoluteDir().entryList()));
        else
            new_file_list.append(file_list.at(i));
    }
    return new_file_list;
}
