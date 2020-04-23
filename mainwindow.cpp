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
void MainWindow::on_addDirButton_clicked()
{
    openFileDialog(QFileDialog::Directory);
    connect(file_dialog, &QFileDialog::filesSelected,
            this, &MainWindow::srcFilesSelected);
    file_dialog->show();
}
void MainWindow::on_addFilesButton_clicked()
{
    openFileDialog(QFileDialog::ExistingFiles);
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
        std::cout << src_files.at(i).toLocal8Bit().data() << "\n";
//        addSrc(src_files.at(i));
//        if(isImage(src_files.at(i)))
//            QListWidgetItem src_file_item(QIcon(src_files.at(i)),
//                                          src_files.at(i),
//                                          src_list,
//                                          QListWidgetItem::Type);
    }

}
void MainWindow::dstFilesSelected(QStringList selected)
{
    for(int i=0; i < selected.size(); i++)
    {
        QString currentFile = selected.at(i);
        if(QFileInfo(currentFile).isDir())
            addDst(currentFile);
    }
}
QStringList MainWindow::expandDir(QString dir)
{
    QStringList new_file_list;
    QDirIterator it(dir, QStringList() << "scaling_cur_freq",
                    QDir::NoFilter, QDirIterator::Subdirectories);
    while(it.hasNext())
    {
        QString next = it.next();
        if(QFileInfo(next).isDir() == false && isSupportedFile(next))
            new_file_list << next;
    }
    return new_file_list;
}
QStringList MainWindow::expandFileList(QStringList file_list)
{
    QStringList new_file_list;
    for(int i=0; i < file_list.size(); i++)
    {
        QString file_name = file_list.at(i);
        QFileInfo file_info(file_name);
        if(file_info.isDir())
            new_file_list << expandDir(file_name);
        else if(isSupportedFile(file_name))
             new_file_list << file_name;
    }
    return new_file_list;
}
bool MainWindow::isSupportedFile(QString file)
{
    QImageReader image_reader(file);
    if(image_reader.canRead())
        return true;
    return false;
}
void MainWindow::openFileDialog(QFileDialog::FileMode mode)
{
    file_dialog_open = true;
    file_dialog = new QFileDialog();
    file_dialog->setFileMode(mode);
}
void MainWindow::addSrc(QString src)
{
}
void MainWindow::addDst(QString dst)
{
}
