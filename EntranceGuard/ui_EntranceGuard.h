/********************************************************************************
** Form generated from reading UI file 'EntranceGuard.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENTRANCEGUARD_H
#define UI_ENTRANCEGUARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EntranceGuardClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLabel *m_lblOptInfo;
    QListView *m_listDevInfo;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_4;
    QTabWidget *tabWidget;
    QWidget *m_tabUserManage;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_4;
    QVBoxLayout *verticalLayout_7;
    QLabel *label;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_8;
    QVBoxLayout *verticalLayout_10;
    QTableView *m_tbUserInfo;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QPushButton *m_btnAddUser;
    QLabel *label_3;
    QPushButton *m_btnDelUser;
    QLabel *label_4;
    QPushButton *m_btnModifUser;
    QLabel *label_5;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_8;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_6;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_7;
    QVBoxLayout *verticalLayout_9;
    QPushButton *m_btnDevAblity;
    QPushButton *m_btnSelect;
    QPushButton *m_btnSend;
    QPushButton *m_btnDelAllUser;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EntranceGuardClass)
    {
        if (EntranceGuardClass->objectName().isEmpty())
            EntranceGuardClass->setObjectName(QString::fromUtf8("EntranceGuardClass"));
        EntranceGuardClass->resize(914, 651);
        EntranceGuardClass->setContextMenuPolicy(Qt::NoContextMenu);
        EntranceGuardClass->setAutoFillBackground(true);
        centralWidget = new QWidget(EntranceGuardClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        m_lblOptInfo = new QLabel(groupBox);
        m_lblOptInfo->setObjectName(QString::fromUtf8("m_lblOptInfo"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        m_lblOptInfo->setFont(font);
        m_lblOptInfo->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(m_lblOptInfo, 0, 0, 1, 1);


        verticalLayout_3->addWidget(groupBox);

        m_listDevInfo = new QListView(centralWidget);
        m_listDevInfo->setObjectName(QString::fromUtf8("m_listDevInfo"));

        verticalLayout_3->addWidget(m_listDevInfo);

        verticalLayout_3->setStretch(0, 1);
        verticalLayout_3->setStretch(1, 5);

        verticalLayout_2->addLayout(verticalLayout_3);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        m_tabUserManage = new QWidget();
        m_tabUserManage->setObjectName(QString::fromUtf8("m_tabUserManage"));
        gridLayout_3 = new QGridLayout(m_tabUserManage);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox_2 = new QGroupBox(m_tabUserManage);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_4 = new QGridLayout(groupBox_2);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(label);

        groupBox_6 = new QGroupBox(groupBox_2);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        gridLayout_8 = new QGridLayout(groupBox_6);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        m_tbUserInfo = new QTableView(groupBox_6);
        m_tbUserInfo->setObjectName(QString::fromUtf8("m_tbUserInfo"));

        verticalLayout_10->addWidget(m_tbUserInfo);


        gridLayout_8->addLayout(verticalLayout_10, 0, 0, 1, 1);


        verticalLayout_7->addWidget(groupBox_6);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        m_btnAddUser = new QPushButton(groupBox_2);
        m_btnAddUser->setObjectName(QString::fromUtf8("m_btnAddUser"));

        horizontalLayout_3->addWidget(m_btnAddUser);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        m_btnDelUser = new QPushButton(groupBox_2);
        m_btnDelUser->setObjectName(QString::fromUtf8("m_btnDelUser"));

        horizontalLayout_3->addWidget(m_btnDelUser);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_3->addWidget(label_4);

        m_btnModifUser = new QPushButton(groupBox_2);
        m_btnModifUser->setObjectName(QString::fromUtf8("m_btnModifUser"));

        horizontalLayout_3->addWidget(m_btnModifUser);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_3->addWidget(label_5);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 2);
        horizontalLayout_3->setStretch(2, 1);
        horizontalLayout_3->setStretch(3, 2);
        horizontalLayout_3->setStretch(4, 1);
        horizontalLayout_3->setStretch(5, 2);
        horizontalLayout_3->setStretch(6, 1);

        verticalLayout_7->addLayout(horizontalLayout_3);

        verticalLayout_7->setStretch(0, 1);
        verticalLayout_7->setStretch(1, 7);
        verticalLayout_7->setStretch(2, 1);

        gridLayout_4->addLayout(verticalLayout_7, 0, 0, 1, 1);


        horizontalLayout_2->addWidget(groupBox_2);


        verticalLayout_5->addLayout(horizontalLayout_2);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        groupBox_3 = new QGroupBox(m_tabUserManage);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_5 = new QGridLayout(groupBox_3);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        groupBox_5 = new QGroupBox(groupBox_3);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));

        verticalLayout_8->addWidget(groupBox_5);


        horizontalLayout_4->addLayout(verticalLayout_8);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setSpacing(6);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        groupBox_4 = new QGroupBox(groupBox_3);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_7 = new QGridLayout(groupBox_4);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        m_btnDevAblity = new QPushButton(groupBox_4);
        m_btnDevAblity->setObjectName(QString::fromUtf8("m_btnDevAblity"));

        verticalLayout_9->addWidget(m_btnDevAblity);

        m_btnSelect = new QPushButton(groupBox_4);
        m_btnSelect->setObjectName(QString::fromUtf8("m_btnSelect"));

        verticalLayout_9->addWidget(m_btnSelect);

        m_btnSend = new QPushButton(groupBox_4);
        m_btnSend->setObjectName(QString::fromUtf8("m_btnSend"));

        verticalLayout_9->addWidget(m_btnSend);

        m_btnDelAllUser = new QPushButton(groupBox_4);
        m_btnDelAllUser->setObjectName(QString::fromUtf8("m_btnDelAllUser"));

        verticalLayout_9->addWidget(m_btnDelAllUser);


        gridLayout_7->addLayout(verticalLayout_9, 0, 0, 1, 1);


        gridLayout_6->addWidget(groupBox_4, 0, 0, 1, 1);


        horizontalLayout_4->addLayout(gridLayout_6);

        horizontalLayout_4->setStretch(0, 4);
        horizontalLayout_4->setStretch(1, 1);

        gridLayout_5->addLayout(horizontalLayout_4, 0, 0, 1, 1);


        verticalLayout_6->addWidget(groupBox_3);


        verticalLayout_5->addLayout(verticalLayout_6);

        verticalLayout_5->setStretch(0, 5);
        verticalLayout_5->setStretch(1, 2);

        gridLayout_3->addLayout(verticalLayout_5, 0, 0, 1, 1);

        tabWidget->addTab(m_tabUserManage, QString());

        verticalLayout_4->addWidget(tabWidget);


        verticalLayout->addLayout(verticalLayout_4);


        horizontalLayout->addLayout(verticalLayout);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 4);

        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        EntranceGuardClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(EntranceGuardClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        EntranceGuardClass->setStatusBar(statusBar);

        retranslateUi(EntranceGuardClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(EntranceGuardClass);
    } // setupUi

    void retranslateUi(QMainWindow *EntranceGuardClass)
    {
        EntranceGuardClass->setWindowTitle(QApplication::translate("EntranceGuardClass", "EntranceGuard", nullptr));
        groupBox->setTitle(QApplication::translate("EntranceGuardClass", "\345\217\263\351\224\256\347\202\271\346\210\221", nullptr));
        m_lblOptInfo->setText(QApplication::translate("EntranceGuardClass", "\345\217\263\351\224\256\347\202\271\346\210\221\350\277\233\350\241\214\347\231\273\345\275\225", nullptr));
        groupBox_2->setTitle(QString());
        label->setText(QApplication::translate("EntranceGuardClass", "\351\227\250\347\246\201\347\224\250\346\210\267\344\277\241\346\201\257", nullptr));
        groupBox_6->setTitle(QString());
        label_2->setText(QString());
        m_btnAddUser->setText(QApplication::translate("EntranceGuardClass", "\346\267\273\345\212\240\347\224\250\346\210\267", nullptr));
        label_3->setText(QString());
        m_btnDelUser->setText(QApplication::translate("EntranceGuardClass", "\345\210\240\351\231\244\347\224\250\346\210\267", nullptr));
        label_4->setText(QString());
        m_btnModifUser->setText(QApplication::translate("EntranceGuardClass", "\344\277\256\346\224\271\347\224\250\346\210\267", nullptr));
        label_5->setText(QString());
        groupBox_3->setTitle(QString());
        groupBox_5->setTitle(QString());
        groupBox_4->setTitle(QString());
        m_btnDevAblity->setText(QApplication::translate("EntranceGuardClass", "\350\256\276\345\244\207\350\203\275\345\212\233\347\272\247", nullptr));
        m_btnSelect->setText(QApplication::translate("EntranceGuardClass", "\346\237\245\350\257\242", nullptr));
        m_btnSend->setText(QApplication::translate("EntranceGuardClass", "\344\270\213\345\217\221", nullptr));
        m_btnDelAllUser->setText(QApplication::translate("EntranceGuardClass", "\346\270\205\347\251\272", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(m_tabUserManage), QApplication::translate("EntranceGuardClass", "\347\224\250\346\210\267\347\256\241\347\220\206", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EntranceGuardClass: public Ui_EntranceGuardClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENTRANCEGUARD_H
