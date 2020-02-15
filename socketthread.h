#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QProgressBar>


class SocketThread : public QThread
{
    Q_OBJECT
public:
    SocketThread(QString name, QString ip, QString port);

    void set_str(QString name, QString ip, QString port);

    void end_socket();

private:
    QTcpSocket * _tcpSocket = nullptr;

    QString fileName;
    QString _ip;
    QString _port;

    int result_status = 0;
protected:
    void run();


signals:
    void send_bar_num(int num);//进度条值
    void send_result_num(int num);//传输结果

    void send_now_packet(int num, int recall);//当前包号以及重传次数
};

#endif // SOCKETTHREAD_H
