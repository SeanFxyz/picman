#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QFileDialog>
#include <QListWidget>

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

    bool file_dialog_open;
    QFileDialog* file_dialog;
    QListWidget* src_list = findChild<QListWidget*>("srcList");

private slots:
    void on_addSrcButton_clicked();
    void on_rmSrcButton_clicked();
    void on_cropButton_clicked();
    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_addDstButton_clicked();
    void on_rmDstButton_clicked();
    void on_pushButton_clicked();

    void srcFilesSelected(QStringList selected);
    void dstFilesSelected(QStringList selected);

    QStringList expandFileList(QStringList file_list);
    bool isImage(QString file);
};

#endif // MAINWINDOW_H
