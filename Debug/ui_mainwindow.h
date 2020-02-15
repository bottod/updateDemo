/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *ipInput;
    QLineEdit *portInput;
    QPushButton *connectBtn;
    QPushButton *disconnectBtn;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLineEdit *fileInput;
    QPushButton *chooseBtn;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *uploadBtn;
    QSpacerItem *horizontalSpacer_2;
    QProgressBar *uploadProgress;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(303, 352);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        ipInput = new QLineEdit(centralWidget);
        ipInput->setObjectName(QStringLiteral("ipInput"));

        gridLayout->addWidget(ipInput, 0, 1, 1, 1);

        portInput = new QLineEdit(centralWidget);
        portInput->setObjectName(QStringLiteral("portInput"));

        gridLayout->addWidget(portInput, 2, 1, 1, 1);

        connectBtn = new QPushButton(centralWidget);
        connectBtn->setObjectName(QStringLiteral("connectBtn"));

        gridLayout->addWidget(connectBtn, 0, 3, 1, 1);

        disconnectBtn = new QPushButton(centralWidget);
        disconnectBtn->setObjectName(QStringLiteral("disconnectBtn"));

        gridLayout->addWidget(disconnectBtn, 2, 3, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout->addWidget(label_3);

        fileInput = new QLineEdit(centralWidget);
        fileInput->setObjectName(QStringLiteral("fileInput"));

        horizontalLayout->addWidget(fileInput);

        chooseBtn = new QPushButton(centralWidget);
        chooseBtn->setObjectName(QStringLiteral("chooseBtn"));

        horizontalLayout->addWidget(chooseBtn);


        verticalLayout->addLayout(horizontalLayout);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 0, 1, 1);

        uploadBtn = new QPushButton(centralWidget);
        uploadBtn->setObjectName(QStringLiteral("uploadBtn"));

        gridLayout_2->addWidget(uploadBtn, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 2, 1, 1);


        verticalLayout->addLayout(gridLayout_2);

        uploadProgress = new QProgressBar(centralWidget);
        uploadProgress->setObjectName(QStringLiteral("uploadProgress"));
        uploadProgress->setValue(0);

        verticalLayout->addWidget(uploadProgress);


        horizontalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 303, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207IP:", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "\347\253\257\345\217\243\345\217\267:", nullptr));
        connectBtn->setText(QApplication::translate("MainWindow", "\350\277\236\346\216\245", nullptr));
        disconnectBtn->setText(QApplication::translate("MainWindow", "\346\226\255\345\274\200\350\277\236\346\216\245", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\266\351\200\211\346\213\251:", nullptr));
        chooseBtn->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251", nullptr));
        uploadBtn->setText(QApplication::translate("MainWindow", "\345\215\207\347\272\247", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
