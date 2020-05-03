#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QFileDialog>
#include <QListWidget>
#include <QImageReader>
#include <QImage>
#include <QDirIterator>
#include <QLabel>
#include <QPushButton>

#include "ui_mainwindow.h"

enum rotDeg{rot90, rot180, rot270};
struct ImgOpData
{
    QString img;

    QString new_name;

    QStringList copy_dirs;

    rotDeg rot;

    int crop_x, crop_y;
    QSize crop_size;
};

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

    /* UI elements */
    QListWidget* src_list_widget;
    QListWidget* dst_list_widget;
    QLabel img_label;
    QScrollArea* img_scroll_area;
    QFileDialog* file_dialog;

    double scale = 0.99;

    /* File-related data fields */
    QStringList src_files;
    QStringList dst_dirs;
    QString current_src;
    QString current_dst;
    QList<ImgOpData> img_op_data;

    /* Private methods */
    void config();

    void addSrc(QString src);
    void addDst(QString dst);
    void rmSrc(int src);
    void rmSrc(QString src);
    void rmDst(int row);
    void rmDst(QString dst);
    void openFileDialog(QFileDialog::FileMode mode);
    QStringList expandFileList(QStringList file_list);
    bool isSupportedFile(QString file);

    void initImageSize();
    void nextImg();
    void prevImg();
    void scaleImage(double factor);
    void zoomIn();
    void zoomOut();

    int findSrcIndex(QString src);
    void rename(QString src);
    void queueCopy(QString src, QString dst);

private slots:
    void on_addDirButton_clicked();
    void on_rmSrcButton_clicked();
    void on_addFilesButton_clicked();
    void on_cropButton_clicked();
    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_addDstButton_clicked();
    void on_rmDstButton_clicked();
    void on_pushButton_clicked();
    void on_copyButton_clicked();
    void on_srcList_currentItemChanged(
            QListWidgetItem* current,
            QListWidgetItem* previous);
    void on_dstList_currentItemChanged(
            QListWidgetItem* current,
            QListWidgetItem* previous);
    void on_dstList_itemDoubleClicked(QListWidgetItem* item);

    void srcFilesSelected(QStringList selected);
    void dstFilesSelected(QStringList selected);

    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void on_actionNextImg_triggered();
    void on_actionPrevImg_triggered();
};

#endif // MAINWINDOW_H
