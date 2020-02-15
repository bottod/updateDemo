#include "socketthread.h"
#include "crc.h"
#include <QFileInfo>
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QHostAddress>

char receive_pack[14] = {0x00};//需要重设包号， crc值

SocketThread::SocketThread(QString name, QString ip, QString port)
{
    fileName = name;
    _ip = ip;
    _port = port;

    //固定部分
    receive_pack[0] = 0xFF;
    receive_pack[1] = 0xFF;
    receive_pack[2] = 0x00;//ver
    receive_pack[3] = 0x10;//seq
    //data
    receive_pack[4] = 0x00;
    receive_pack[5] = 0x00;
    receive_pack[6] = 0x00;//ver
    receive_pack[7] = 0x04;//seq
    receive_pack[8] = 0x01;
    //9,10位包号
    //11为状态
    //12,13为crc
}

void SocketThread::set_str(QString name, QString ip, QString port)
{
    fileName = name;
    _ip = ip;
    _port = port;
}

void SocketThread::end_socket()
{
    if(_tcpSocket != nullptr)
    {
        _tcpSocket->disconnectFromHost();
        delete _tcpSocket;
    }
    exit(0);
}



void SocketThread::run()
{
    _tcpSocket = new QTcpSocket();
    _tcpSocket->connectToHost(QHostAddress(_ip), _port.toInt());

    _tcpSocket->waitForConnected(10);
    if(_tcpSocket->state() != QAbstractSocket::ConnectedState)
    {
        result_status = -3;
        emit send_result_num(result_status);
        _tcpSocket->deleteLater();
        return;
    }


    QFileInfo fileinfo =  QFileInfo(fileName);
    int filesize = fileinfo.size();         //文件字节数

    //最大一包总长1037
    char send_pack[1037] = {0};

    int packet_num = 0;//包号
    int all_packet_num = filesize / 1024 + 1;

    //空文件
    if(filesize == 0)
    {
        result_status = -1;
        _tcpSocket->disconnectFromHost();
        emit send_result_num(result_status);
        _tcpSocket->deleteLater();
        return;
    }

    //准备包头一样的部分
    send_pack[0] = 0xFF;
    send_pack[1] = 0xFF;
    send_pack[2] = 0x00;//ver
    send_pack[3] = 0x10;//seq

    //发整数1024字节包
    for(packet_num = 0; packet_num < filesize/1024; packet_num++)
    {
        //LEN 4个字节 这部分Data全为1027字节
        send_pack[4] = 0x00;
        send_pack[5] = 0x00;
        send_pack[6] = 0x04;
        send_pack[7] = 0x03;
        //固定值
        send_pack[8] = 0x01;
        //包号最大值65535
        send_pack[9] = packet_num / 256;
        send_pack[10] = packet_num % 256;
        //数据部分
        QFile bin_file(fileName);
        bin_file.open(QIODevice::ReadWrite);
        bin_file.seek(1024 * packet_num);
        bin_file.read(send_pack + 11, 1024);
        //crc
        uint16_t crc = table_crc16_x25(0xFFFF,reinterpret_cast<unsigned char*>(send_pack + 2), 1033);
        send_pack[1035] = crc / 256;
        send_pack[1036] = crc % 256;

        //以上包准备完成

        int loop_times = 0;     //每一包已经循环传输的次数， 一开始都为0，如果为3则失败
        //发包
        _tcpSocket->write(send_pack, 1037);
        //等待回应，重传大于等于10次则失败
        for(loop_times = 0; loop_times < 10; loop_times++)
        {
            //阻塞等待1秒
            int delay_time = 2000;
            if(packet_num == 0)
                delay_time = 5000;

            //发出信号
            send_now_packet(packet_num, loop_times);

            if(_tcpSocket->waitForReadyRead(delay_time) == false)
            {
                //没收到回应就继续发
                _tcpSocket->write(send_pack, 1037);
            }
            else
            {
                //收到回应正确则跳出就跳出
                QByteArray A = _tcpSocket->readAll();
                qDebug()<<A;
                receive_pack[9] = packet_num / 256;;
                receive_pack[10] = packet_num % 256;
                receive_pack[11] = 0x00;
                //crc
                uint16_t crc = table_crc16_x25(0xFFFF,reinterpret_cast<unsigned char*>(receive_pack + 2), 10);
                receive_pack[12] = crc / 256;
                receive_pack[13] = crc % 256;
                qDebug()<<QByteArray(receive_pack, 14);
                int result = 0;
                //比较是否相同
                for(int i = 0; i < A.size(); i++)
                {
                    if(A.at(i) != QByteArray(receive_pack, 14).at(i))
                    {
                        result = 1;
                        break;
                    }
                }
                if(!result)
                {
                    qDebug()<<"the same";
                    break;
                }
                //回应不对则重传
                _tcpSocket->write(send_pack, 1037);
            }
        }
        if(loop_times >= 10)
        {
            _tcpSocket->disconnectFromHost();
            result_status = -2;
            emit send_result_num(result_status);
            _tcpSocket->deleteLater();
            return;
        }

       emit send_bar_num(100 * (packet_num + 1) / all_packet_num);

    }
    //也就是最后一包----------------------------------------------------------
    //剩余部分正好为0
    if(filesize % 1024 == 0)
    {
        //LEN 4个字节 这部分Data构造3字节0x00
        send_pack[4] = 0x00;
        send_pack[5] = 0x00;
        send_pack[6] = 0x00;
        send_pack[7] = 0x03;
        //固定值
        send_pack[8] = 0x01;
        //包号最大值65535
        send_pack[9] = packet_num / 256;
        send_pack[10] = packet_num % 256;
        //数据部分
        //空
        //crc
        uint16_t crc = table_crc16_x25(0xFFFF,reinterpret_cast<unsigned char*>(send_pack + 2), 9);
        send_pack[11] = crc / 256;
        send_pack[12] = crc % 256;

        //以上包准备完成

        int loop_times = 0;     //每一包已经循环传输的次数， 一开始都为0，如果为3则失败
        //发包
        _tcpSocket->write(send_pack, 13);
        //等待回应，重传大于等于10次则失败
        for(loop_times = 0; loop_times < 10; loop_times++)
        {
            //发出信号
            send_now_packet(packet_num, loop_times);

            //阻塞等待1秒
            if(_tcpSocket->waitForReadyRead(2000) == false)
            {
                //没收到回应就继续发
                _tcpSocket->write(send_pack, 13);
            }
            else
            {
                //收到回应正确则跳出就跳出
                QByteArray A = _tcpSocket->readAll();
                qDebug()<<A;
                receive_pack[9] = packet_num / 256;;
                receive_pack[10] = packet_num % 256;
                receive_pack[11] = 0x00;
                //crc
                uint16_t crc = table_crc16_x25(0xFFFF,reinterpret_cast<unsigned char*>(receive_pack + 2), 10);
                receive_pack[12] = crc / 256;
                receive_pack[13] = crc % 256;
                qDebug()<<QByteArray(receive_pack, 14);
                int result = 0;
                //比较是否相同
                for(int i = 0; i < A.size(); i++)
                {
                    if(A.at(i) != QByteArray(receive_pack, 14).at(i))
                    {
                        result = 1;
                        break;
                    }
                }
                if(!result)
                {
                    qDebug()<<"the same";
                    break;
                }
                //回应不对则重传
                _tcpSocket->write(send_pack, 1037);
            }
        }
        if(loop_times >= 10)
        {
            result_status = -2;
            _tcpSocket->disconnectFromHost();
            emit send_result_num(result_status);
            _tcpSocket->deleteLater();
            return;
        }

      emit send_bar_num(100 * (packet_num + 1) / all_packet_num);
    }
    else
    {
        //LEN 4个字节 这部分Data构造3字节0x00
        send_pack[4] = 0x00;
        send_pack[5] = 0x00;
        send_pack[6] = (3 + filesize % 1024) / 256;
        send_pack[7] = (3 + filesize % 1024) % 256;
        //固定值
        send_pack[8] = 0x01;
        //包号最大值65535
        send_pack[9] = packet_num / 256;
        send_pack[10] = packet_num % 256;
        //数据部分
        QFile bin_file(fileName);
        bin_file.open(QIODevice::ReadWrite);
        bin_file.seek(1024 * packet_num);
        bin_file.read(send_pack + 11, 1024);//返回值为实际剩下的data长度
        //crc
        uint16_t crc = table_crc16_x25(0xFFFF,reinterpret_cast<unsigned char*>(send_pack + 2), 9 + filesize % 1024);
        send_pack[11 + filesize % 1024] = crc / 256;
        send_pack[12 + filesize % 1024] = crc % 256;

        //以上包准备完成

        int loop_times = 0;     //每一包已经循环传输的次数， 一开始都为0，如果为3则失败
        //发包
        _tcpSocket->write(send_pack, 13 + filesize % 1024);
        //等待回应，重传大于等于10次则失败
        for(loop_times = 0; loop_times < 10; loop_times++)
        {
            //发出信号
            send_now_packet(packet_num, loop_times);

            //阻塞等待1秒
            if(_tcpSocket->waitForReadyRead(2000) == false)
            {
                //没收到回应就继续发
                _tcpSocket->write(send_pack, 13 + filesize % 1024);
            }
            else
            {
                //收到回应正确则跳出就跳出
                QByteArray A = _tcpSocket->readAll();
                qDebug()<<A;
                receive_pack[9] = packet_num / 256;
                receive_pack[10] = packet_num % 256;
                receive_pack[11] = 0x00;
                //crc
                uint16_t crc = table_crc16_x25(0xFFFF,reinterpret_cast<unsigned char*>(receive_pack + 2), 10);
                receive_pack[12] = crc / 256;
                receive_pack[13] = crc % 256;
                qDebug()<<QByteArray(receive_pack, 14);
                int result = 0;
                //比较是否相同
                for(int i = 0; i < A.size(); i++)
                {
                    if(A.at(i) != QByteArray(receive_pack, 14).at(i))
                    {
                        result = 1;
                        break;
                    }
                }
                if(!result)
                {
                    qDebug()<<"the same";
                    break;
                }
                //回应不对则重传
                _tcpSocket->write(send_pack, 1037);
            }
        }
        if(loop_times >= 10)
        {
            result_status = -2;
            _tcpSocket->disconnectFromHost();
            emit send_result_num(result_status);
            _tcpSocket->deleteLater();
            return;
        }

        emit send_bar_num(100 * (packet_num + 1) / all_packet_num);
    }

    result_status = 0;
    _tcpSocket->disconnectFromHost();
    emit send_result_num(result_status);
    _tcpSocket->deleteLater();
    return;
}
