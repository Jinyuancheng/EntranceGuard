/********************************************************************************
** Form generated from reading UI file 'fmMenJinLogin.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FMMENJINLOGIN_H
#define UI_FMMENJINLOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *m_editNodeName;
    QLabel *label_2;
    QLineEdit *m_editIp;
    QLabel *label_3;
    QLineEdit *m_editPort;
    QLabel *label_4;
    QLineEdit *m_editUser;
    QLabel *label_5;
    QLineEdit *m_editPass;
    QPushButton *m_btnLogin;
    QPushButton *m_btnQuit;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(560, 361);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(160, 80, 250, 178));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        m_editNodeName = new QLineEdit(layoutWidget);
        m_editNodeName->setObjectName(QString::fromUtf8("m_editNodeName"));

        gridLayout->addWidget(m_editNodeName, 0, 1, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        m_editIp = new QLineEdit(layoutWidget);
        m_editIp->setObjectName(QString::fromUtf8("m_editIp"));

        gridLayout->addWidget(m_editIp, 1, 1, 1, 1);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        m_editPort = new QLineEdit(layoutWidget);
        m_editPort->setObjectName(QString::fromUtf8("m_editPort"));

        gridLayout->addWidget(m_editPort, 2, 1, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        m_editUser = new QLineEdit(layoutWidget);
        m_editUser->setObjectName(QString::fromUtf8("m_editUser"));

        gridLayout->addWidget(m_editUser, 3, 1, 1, 1);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        m_editPass = new QLineEdit(layoutWidget);
        m_editPass->setObjectName(QString::fromUtf8("m_editPass"));
        m_editPass->setEchoMode(QLineEdit::PasswordEchoOnEdit);

        gridLayout->addWidget(m_editPass, 4, 1, 1, 1);

        m_btnLogin = new QPushButton(layoutWidget);
        m_btnLogin->setObjectName(QString::fromUtf8("m_btnLogin"));
        m_btnLogin->setFont(font);

        gridLayout->addWidget(m_btnLogin, 5, 0, 1, 1);

        m_btnQuit = new QPushButton(layoutWidget);
        m_btnQuit->setObjectName(QString::fromUtf8("m_btnQuit"));
        m_btnQuit->setFont(font);

        gridLayout->addWidget(m_btnQuit, 5, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QApplication::translate("MainWindow", "\350\212\202\347\202\271\345\220\215\347\247\260", nullptr));
        m_editNodeName->setText(QApplication::translate("MainWindow", "\346\265\213\350\257\225\351\227\250\347\246\201\344\270\273\346\234\272", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "\344\270\273\346\234\272ip", nullptr));
        m_editIp->setText(QApplication::translate("MainWindow", "192.168.50.55", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "\344\270\273\346\234\272\347\253\257\345\217\243", nullptr));
        m_editPort->setText(QApplication::translate("MainWindow", "8000", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "\347\224\250\346\210\267\345\220\215", nullptr));
        m_editUser->setText(QApplication::translate("MainWindow", "admin", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "\345\257\206\347\240\201", nullptr));
        m_editPass->setText(QApplication::translate("MainWindow", "qwer1234", nullptr));
        m_btnLogin->setText(QApplication::translate("MainWindow", "\347\231\273\345\275\225", nullptr));
        m_btnQuit->setText(QApplication::translate("MainWindow", "\350\277\224\345\233\236", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FMMENJINLOGIN_H
