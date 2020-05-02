#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // UI elements
    src_list_widget = findChild<QListWidget*>("srcList");
    dst_list_widget = findChild<QListWidget*>("dstList");

    img_label.setScaledContents(true);
    img_scroll_area = findChild<QScrollArea*>("imgScrollArea");
    img_scroll_area->setWidget(&img_label);

    file_dialog = nullptr;
}
MainWindow::~MainWindow()
{
    delete ui;
}

/* */
QStringList MainWindow::expandFileList(QStringList file_list)
{
    QStringList new_file_list;
    for(int i=0; i < file_list.size(); i++)
    {
        QString file_name = file_list.at(i);
        QFileInfo file_info(file_name);
        if(file_info.isDir())
        {
            QDirIterator it(file_name, QStringList(),
                            QDir::NoFilter, QDirIterator::Subdirectories);
            while(it.hasNext())
            {
                QString next = it.next();
                if(QFileInfo(next).isDir() == false && isSupportedFile(next))
                    new_file_list << next;
            }
        }
        else if(isSupportedFile(file_name))
             new_file_list << file_name;
    }
    return new_file_list;
}
bool MainWindow::isSupportedFile(QString file)
{
    QImageReader image_reader(file);
    QImage image;
    return image_reader.read(&image);
}
void MainWindow::openFileDialog(QFileDialog::FileMode mode)
{
    file_dialog = new QFileDialog();
    file_dialog->setFileMode(mode);
}
void MainWindow::addSrc(QString src)
{
    for(int i=0; i < src_files.size(); i++)
        if(src_files.at(i).compare(src) == 0)
            return;
    src_files << src;
    new QListWidgetItem(QIcon(src), "", src_list_widget,
                        QListWidgetItem::Type);
}
void MainWindow::addDst(QString dst)
{
    for(int i=0; i < dst_dirs.size(); i++)
        if(dst_dirs.at(i).compare(dst) == 0)
            return;
    dst_dirs << dst;
    new QListWidgetItem(dst, dst_list_widget, QListWidgetItem::Type);
}

/* Image viewer functions */
void MainWindow::initImageSize()
{
    scale = .99;
    img_label.setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    img_label.resize(
                img_label.pixmap()->size().scaled(
                    img_scroll_area->size() * scale, Qt::KeepAspectRatio));
}
void MainWindow::zoomIn()
{
    scaleImage(1.25);
}
void MainWindow::zoomOut()
{
    scaleImage(0.8);
}
void MainWindow::scaleImage(double factor)
{
    if(img_label.hasScaledContents())
    {
        scale *= factor;
        img_label.resize(scale * img_label.pixmap()->size());
    }
}

/* Queueing file operations */
void MainWindow::queueCopy(QString src, QString dst)
{
    CopyOp copy = {src, dst};
    queue_list_copy << copy;
}

/* Doing file operations */
//void MainWindow::applyOperations()
//{
//    //TODO: Start by applying edits to a temp file.
//}
//void MainWindow::doCopy(CopyOp copy)
//{
//}
//

/* Slots */
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
    openFileDialog(QFileDialog::Directory);
    connect(file_dialog, &QFileDialog::filesSelected,
            this, &MainWindow::dstFilesSelected);
    file_dialog->show();
}
void MainWindow::on_rmDstButton_clicked()
{
}
void MainWindow::on_pushButton_clicked()
{
}
void MainWindow::on_copyButton_clicked()
{
    queueCopy(current_src, current_src);
}
void MainWindow::srcFilesSelected(QStringList selected)
{
    QStringList src_files = expandFileList(selected);
    cout << "--------------------\n"
         << "Adding source files:\n";
    for(int i=0; i < src_files.size(); i++)
    {
        cout << src_files.at(i).toLocal8Bit().data() << endl;
        addSrc(src_files.at(i));
    }
    cout << "Done.\n"
         << "--------------------\n";

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
void MainWindow::on_dstList_itemDoubleClicked(QListWidgetItem* item)
{
    queueCopy(current_src, current_dst);
}
void MainWindow::on_srcList_currentItemChanged(
        QListWidgetItem* current,
        QListWidgetItem* previous)
{
    int i = src_list_widget->row(current);
    img_label.setPixmap(QPixmap(src_files.at(i)));
    initImageSize();
}
void MainWindow::on_dstList_currentItemChanged(
        QListWidgetItem* current,
        QListWidgetItem* previous)
{
    int i = dst_list_widget->row(current);
    current_dst = dst_dirs.at(i);
}
void MainWindow::on_actionZoomIn_triggered()
{
    zoomIn();
}
void MainWindow::on_actionZoomOut_triggered()
{
    zoomOut();
}
