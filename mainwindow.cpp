#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "crc.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mainUI_set();

    //开始传输才显示进度条
    ui->uploadProgress->hide();


    //加载用户配置
    QSettings setting("SendFile", "UserSets");
    setting.beginGroup("Sock");

    QString ip = setting.value("IP",QString("127.0.0.1")).toString();
    ui->ipInput->setText(ip);
    QString port = setting.value("Port",QString("80")).toString();
    ui->portInput->setText(port);
    QString file_addr = setting.value("FileAddr",QString("")).toString();
    ui->fileInput->setText(file_addr);
    QString file_raw_addr = setting.value("FileRawAddr",QString("")).toString();
    ui->fileInput->setToolTip(file_raw_addr);
    fileName = file_raw_addr;
    QString file_path = setting.value("FilePath",QString("")).toString();
    filePath = file_path;

    setting.endGroup();




    a = new SocketThread(fileName, ui->ipInput->text(), ui->portInput->text());
    connect(a, &SocketThread::send_bar_num, this, &MainWindow::setbarvalue);
    connect(a, &SocketThread::send_result_num, this, &MainWindow::get_result_num);
    connect(a, &SocketThread::send_now_packet, this, &MainWindow::get_now_pack);

}


void MainWindow::get_now_pack(int num, int recall)
{
    statusBar()->showMessage("当前传输包号：" + QString::number(num) + "; 重传次数：" + QString::number(recall));
}


//关闭时写入用户配置
void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings setting("SendFile", "UserSets");

    setting.beginGroup("Sock");
    setting.setValue("IP",ui->ipInput->text());
    setting.setValue("Port",ui->portInput->text());
    setting.setValue("FileAddr",ui->fileInput->text());
    setting.setValue("FileRawAddr",ui->fileInput->toolTip());
    setting.setValue("FilePath",filePath);

    setting.endGroup();

    //接收事件，关闭窗口
    event->accept();
}


MainWindow::~MainWindow()
{
    delete ui;
    a->end_socket();
    a->terminate();
}


void MainWindow::mainUI_set()
{
    this->setWindowTitle("发包测试工具");
    this->setWindowIcon(QIcon(":/logo/favicon.ico"));


    //ip输入正则
    QRegExp iprx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");     //IP的正则表达式
    ui->ipInput->setValidator(new QRegExpValidator(iprx, this));

    //端口输入正则
    QRegExp ptrx("([0-9]|[1-9]\\d{1,3}|[1-5]\\d{4}|6[0-4]\\d{3}|65[0-4]\\d{2}|655[0-2]\\d|6553[0-5])");     //IP的正则表达式
    ui->portInput->setValidator(new QRegExpValidator(ptrx, this));

    //选择文件按钮
    connect(ui->chooseBtn, &QPushButton::clicked, this, &MainWindow::openFileEvent);

    ui->fileInput->setReadOnly(true);
    ui->ipInput->setPlaceholderText("请输入ip地址...");
    ui->portInput->setPlaceholderText("请输入端口号...");


    connect(ui->uploadBtn, &QPushButton::clicked, this, &MainWindow::uploadEvent);


    //菜单栏
    fileMenu = new QMenu("文件", this);
    helpMenu = new QMenu("帮助", this);
    exitAction = new QAction("退出", this);
    connect(exitAction, &QAction::triggered, [=]()
    {
        MainWindow::close();
    });
    openFileAction = new QAction("文件选择", this);
    connect(openFileAction, &QAction::triggered, [=]()
    {
        MainWindow::openFileEvent();
    });
    aboutAction = new QAction(QIcon(":/logo/favicon.ico"), "关于", this);
    connect(aboutAction, &QAction::triggered, [=]()
    {
        QMessageBox message(QMessageBox::NoIcon, "关于", "测试程序v2.0");
        message.setIconPixmap(QPixmap(":/logo/favicon.ico"));
        message.exec();
    });
    fileInfoAction = new QAction("文件信息", this);
    connect(fileInfoAction, &QAction::triggered, [=]()
    {
        if(fileName != "")
        {
            QFileInfo fileinfo =  QFileInfo(fileName);
            QString str = QString("文件大小为 %1 bytes").arg(fileinfo.size());
            statusBar()->showMessage(str, 2000);
        }
        else
        {
            statusBar()->showMessage("还未选择文件...", 2000);
        }
    });

    helpMenu->addAction(aboutAction);
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(fileInfoAction);
    fileMenu->addAction(exitAction);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(helpMenu);

    ui->mainToolBar->addAction(fileInfoAction);

}




//打开文件选择窗口选择文件
void MainWindow::openFileEvent()
{
    // 打开文件操作
    QString tmpFileName;
    tmpFileName = QFileDialog::getOpenFileName(this, "", filePath, "bin类型文件 (*.bin)");
    if ( tmpFileName != "" )
    {
        QFileInfo fileinfo;
        fileinfo = QFileInfo(tmpFileName);
        //设置单文件名
        ui->fileInput->setText(fileinfo.fileName());
        ui->fileInput->setToolTip(fileinfo.absoluteFilePath());
        fileName = tmpFileName;
        filePath = fileinfo.path();
    }
}


//上传事件
void MainWindow::uploadEvent()
{

    if(fileName == "")
    {
        statusBar()->showMessage("请先选择需要传输的文件...", 2000);
    }
    else
    {
        statusBar()->showMessage("开始传输...");

        ui->ipInput->setReadOnly(true);
        ui->portInput->setReadOnly(true);
        ui->chooseBtn->setEnabled(false);


        ui->uploadProgress->setValue(0);
        ui->uploadProgress->show();//进度条显示

        ui->uploadBtn->setDisabled(true);
        a->set_str(fileName, ui->ipInput->text(), ui->portInput->text());
        a->start();
    }

}



//设置进度条显示
void MainWindow::setbarvalue(int num)
{
    ui->uploadProgress->setValue(num);
}

//设置传输结果值
void MainWindow::get_result_num(int result)
{
    switch (result)
    {
        case -3:
            statusBar()->showMessage("无法连接到设备...", 2000);
            break;
        case -2:
            statusBar()->showMessage("传输中设备10次未回复，超时...", 2000);
            break;
        case 0:
            statusBar()->showMessage("传输完成...", 2000);
            break;
        case -1:
            statusBar()->showMessage("文件为空，取消传输...", 2000);
            break;
        default:
            break;
    }
    ui->uploadBtn->setEnabled(true);
    ui->ipInput->setReadOnly(false);
    ui->portInput->setReadOnly(false);
    ui->chooseBtn->setEnabled(true);
    ui->uploadProgress->hide();
}


