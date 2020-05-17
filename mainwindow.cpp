#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    config();

    /* UI elements */

    // QListWidgets
    src_list_widget = findChild<QListWidget*>("srcList");
    dst_list_widget = findChild<QListWidget*>("dstList");
    op_list_widget = findChild<QListWidget*>("copyList");

    // Image viewer area setup
    img_label = new QLabel();
    img_label->setScaledContents(true);
    img_scroll_area = findChild<QScrollArea*>("imgScrollArea");
    img_scroll_area->setWidget(img_label);

    // File name QLineEdit setup
    name_line_edit = findChild<QLineEdit*>("renameLineEdit");
    QRegExpValidator name_validator(QRegExp("^[^\\?%*:|\"<>]*$"), this);
    name_line_edit->setValidator(&name_validator);

    file_dialog = nullptr;
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::config()
{
/* Causes segfaults for some reason */
//    // Setting up shortcuts
//    QList<QKeySequence> next_img;
//    next_img << QKeySequence(Qt::Key_Right)
//             << QKeySequence(Qt::Key_L);
//    findChild<QAction*>("actionNextImg")->setShortcuts(next_img);
//    // Causes segfaults for some reason


//    QList<QKeySequence> prev_img;
//    prev_img << QKeySequence(tr("Left"))
//             << QKeySequence(tr("H"));
//    findChild<QAction*>("actionPrevImg")->setShortcuts(prev_img);
}

ImgOpData MainWindow::imgOpDefaults(QString img)
{
    ImgOpData defaults;
    defaults.copy_dsts = QStringList();
    defaults.rot = ROT0;
    defaults.crop = false;
    defaults.crop_x = 0;
    defaults.crop_y = 0;
    defaults.crop_size = QSize();
    return defaults;
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
    QImageReader img_reader(file);
    QImage img;
    return img_reader.read(&img);
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
    src_list_widget->addItem(new QListWidgetItem(
                                 QIcon(src), "", src_list_widget));
}
void MainWindow::addDst(QString dst)
{
    for(int i=0; i < dst_dirs.size(); i++)
        if(dst_dirs.at(i).compare(dst) == 0)
            return;
    dst_dirs << dst;
    new QListWidgetItem(dst, dst_list_widget, QListWidgetItem::Type);
}
void MainWindow::rmSrc(int src)
{
    src_files.removeAt(src);
    QListWidgetItem* src_item = src_list_widget->takeItem(src);
    delete src_item;
}
void MainWindow::rmSrc(QString src)
{
    for(int i=0; i < src_files.size(); i++)
        if(src_files.at(i).compare(src) == 0)
            rmSrc(i);
}
void MainWindow::rmDst(int dst)
{
    dst_dirs.removeAt(dst);
    QListWidgetItem* dst_item = dst_list_widget->takeItem(dst);
    delete dst_item;
}
void MainWindow::rmDst(QString dst)
{
    for(int i=0; i< dst_dirs.size(); i++)
        if(src_files.at(i).compare(dst) == 0)
            rmDst(i);
}

/* Image viewer functions */
void MainWindow::initImageSize()
{
    scale = .99;
    img_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    img_label->resize(
                img_label->pixmap()->size().scaled(
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
    if(img_label->hasScaledContents())
    {
        scale *= factor;
        img_label->resize(factor * img_label->size());
    }
}
void MainWindow::nextImg()
{
    src_list_widget->setCurrentRow(src_list_widget->currentRow() + 1);
}
void MainWindow::prevImg()
{
    src_list_widget->setCurrentRow(src_list_widget->currentRow() - 1);
}

/* Queueing file operations */
void MainWindow::queueCopy(QString src, QString dst)
{
    if(name_line_edit->text() != "")
    {
        dst += '/' + name_line_edit->text();
        QString ext = QFileInfo(src).completeSuffix();
        if(dst.lastIndexOf(ext) != dst.size() - ext.size() ||
                dst.lastIndexOf('.') != dst.size() - ext.size() - 1)
            dst += '.' + ext;
    }
    else
        dst += '/' + QFileInfo(src).fileName();

    if(img_op_map.count(src))
        img_op_map[src].copy_dsts << dst;
    else
    {
        ImgOpData new_op_data = imgOpDefaults(src);
        new_op_data.img = src;
        new_op_data.copy_dsts << dst;
        img_op_map[src] = new_op_data;
    }
    op_list_widget->addItem(src + " -> " + dst);
}

/* Doing file operations */
void MainWindow::runOps()
{
    QMap<QString, ImgOpData>::iterator iter;
    for(iter = img_op_map.begin(); iter != img_op_map.end(); ++iter)
    {
        QString copySrc = iter.key();
        ImgOpData opdata = iter.value();
        if(opdata.rot != ROT0 || opdata.crop == true)
        {
            QTemporaryFile tmp;
            tmp.setAutoRemove(true);
            tmp.open();
            QString tmp_name = tmp.fileName();
            tmp.remove();
            QFile(copySrc).copy(tmp_name);
            copySrc = tmp_name;

            // TODO: run image rotation on temp file
            // TODO: run crop on temp file
        }
        QFile src_file(copySrc);
        for(int i=0; i < opdata.copy_dsts.size(); i++)
        {
            src_file.copy(opdata.copy_dsts.at(i));
        }
    }
    op_list_widget->clear();
    img_op_map.clear();
}

/* Slots */
/* Widget slots */
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
    rmSrc(src_list_widget->currentRow());
}
void MainWindow::on_cropButton_clicked()
{
}
void MainWindow::on_prevButton_clicked()
{
    prevImg();
}
void MainWindow::on_nextButton_clicked()
{
    nextImg();
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
    rmDst(dst_list_widget->currentRow());
}
void MainWindow::on_pushButton_clicked()
{
}
void MainWindow::on_copyButton_clicked()
{
    queueCopy(current_src, current_dst);
}
void MainWindow::on_applyButton_clicked()
{
    runOps();
}
void MainWindow::srcFilesSelected(QStringList selected)
{
    QStringList new_files = expandFileList(selected);
    cout << "--------------------\n"
         << "Adding source files:\n";
    for(int i=0; i < new_files.size(); i++)
    {
        addSrc(new_files.at(i));
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
void MainWindow::on_actionZoomIn_triggered()
{
    zoomIn();
}
void MainWindow::on_actionZoomOut_triggered()
{
    zoomOut();
}
void MainWindow::on_dstList_itemDoubleClicked(QListWidgetItem* item)
{
    queueCopy(current_src, current_dst);
}
void MainWindow::on_srcList_currentItemChanged(
        QListWidgetItem* current,
        QListWidgetItem* previous)
{
    current_src = src_files.at(src_list_widget->currentRow());
    img_label->setPixmap(QPixmap(current_src));
    initImageSize();
}
void MainWindow::on_dstList_currentItemChanged(
        QListWidgetItem* current,
        QListWidgetItem* previous)
{
    current_dst =
            dst_dirs.at(dst_list_widget->currentRow());
}

/* QAction slots */
void MainWindow::on_actionNextImg_triggered()
{
    nextImg();
}
void MainWindow::on_actionPrevImg_triggered()
{
    prevImg();
}
