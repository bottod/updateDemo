#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegExp>
#include <QRegExpValidator>
#include <QFileDialog>
#include <QTcpSocket>
#include <QLabel>
#include <QHostAddress>
#include <QFileInfo>
#include <QMessageBox>
#include <QTimer>
#include <QCloseEvent>
#include <QSettings>
#include "socketthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void mainUI_set();      //界面在UI文件里，该函数做一些绑定设置等

    void openFileEvent();   //打开文件事件


    void uploadEvent();     //上传升级事件



    QString fileName;       //需要上传的文件的文件名
    QString filePath;       //文件的选择位置



    SocketThread *a;
    void setbarvalue(int num);
    void get_result_num(int num);
    void get_now_pack(int num, int recall);



    //菜单和帮助
    QMenu*          fileMenu;
    QMenu*          helpMenu;
    //动作
    QAction*        exitAction;
    QAction*        openFileAction;
    QAction*        aboutAction;
    QAction*        fileInfoAction;


protected:
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
