/********************************************************************************
** Form generated from reading UI file 'fmChangeUser.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FMCHANGEUSER_H
#define UI_FMCHANGEUSER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CFmChangeUser
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGroupBox *groupBox_4;
    QComboBox *m_cbCardType;
    QComboBox *m_cbUserType;
    QPushButton *m_btnAdd;
    QPushButton *m_btnQuit;
    QSplitter *splitter_2;
    QLabel *label_4;
    QLineEdit *m_editCardPass;
    QSplitter *splitter_3;
    QLabel *label_7;
    QLineEdit *m_editUserName;
    QSplitter *splitter_4;
    QLabel *label_8;
    QLineEdit *m_editUserNum;
    QPushButton *m_btnPicPath;
    QLineEdit *m_linePath;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(583, 239);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox_4 = new QGroupBox(groupBox);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(0, 0, 563, 199));
        m_cbCardType = new QComboBox(groupBox_4);
        m_cbCardType->addItem(QString());
        m_cbCardType->addItem(QString());
        m_cbCardType->addItem(QString());
        m_cbCardType->addItem(QString());
        m_cbCardType->addItem(QString());
        m_cbCardType->addItem(QString());
        m_cbCardType->addItem(QString());
        m_cbCardType->addItem(QString());
        m_cbCardType->addItem(QString());
        m_cbCardType->addItem(QString());
        m_cbCardType->addItem(QString());
        m_cbCardType->setObjectName(QString::fromUtf8("m_cbCardType"));
        m_cbCardType->setGeometry(QRect(40, 26, 91, 20));
        m_cbUserType = new QComboBox(groupBox_4);
        m_cbUserType->addItem(QString());
        m_cbUserType->addItem(QString());
        m_cbUserType->setObjectName(QString::fromUtf8("m_cbUserType"));
        m_cbUserType->setGeometry(QRect(40, 60, 91, 22));
        m_btnAdd = new QPushButton(groupBox_4);
        m_btnAdd->setObjectName(QString::fromUtf8("m_btnAdd"));
        m_btnAdd->setGeometry(QRect(89, 149, 121, 23));
        m_btnQuit = new QPushButton(groupBox_4);
        m_btnQuit->setObjectName(QString::fromUtf8("m_btnQuit"));
        m_btnQuit->setGeometry(QRect(340, 149, 111, 23));
        splitter_2 = new QSplitter(groupBox_4);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setGeometry(QRect(141, 26, 187, 20));
        splitter_2->setOrientation(Qt::Horizontal);
        label_4 = new QLabel(splitter_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        splitter_2->addWidget(label_4);
        m_editCardPass = new QLineEdit(splitter_2);
        m_editCardPass->setObjectName(QString::fromUtf8("m_editCardPass"));
        splitter_2->addWidget(m_editCardPass);
        splitter_3 = new QSplitter(groupBox_4);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setGeometry(QRect(140, 61, 187, 20));
        splitter_3->setOrientation(Qt::Horizontal);
        label_7 = new QLabel(splitter_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        splitter_3->addWidget(label_7);
        m_editUserName = new QLineEdit(splitter_3);
        m_editUserName->setObjectName(QString::fromUtf8("m_editUserName"));
        splitter_3->addWidget(m_editUserName);
        splitter_4 = new QSplitter(groupBox_4);
        splitter_4->setObjectName(QString::fromUtf8("splitter_4"));
        splitter_4->setGeometry(QRect(337, 60, 187, 20));
        splitter_4->setOrientation(Qt::Horizontal);
        label_8 = new QLabel(splitter_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        splitter_4->addWidget(label_8);
        m_editUserNum = new QLineEdit(splitter_4);
        m_editUserNum->setObjectName(QString::fromUtf8("m_editUserNum"));
        splitter_4->addWidget(m_editUserNum);
        m_btnPicPath = new QPushButton(groupBox_4);
        m_btnPicPath->setObjectName(QString::fromUtf8("m_btnPicPath"));
        m_btnPicPath->setGeometry(QRect(40, 98, 91, 23));
        m_linePath = new QLineEdit(groupBox_4);
        m_linePath->setObjectName(QString::fromUtf8("m_linePath"));
        m_linePath->setGeometry(QRect(144, 99, 381, 20));
        m_linePath->raise();
        splitter_2->raise();
        splitter_3->raise();
        splitter_4->raise();
        m_cbCardType->raise();
        m_cbUserType->raise();
        m_btnAdd->raise();
        m_btnQuit->raise();
        m_btnPicPath->raise();

        verticalLayout->addWidget(groupBox);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

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
        groupBox->setTitle(QString());
        groupBox_4->setTitle(QString());
        m_cbCardType->setItemText(0, QApplication::translate("MainWindow", "\346\231\256\351\200\232\345\215\241", nullptr));
        m_cbCardType->setItemText(1, QApplication::translate("MainWindow", "\346\256\213\347\226\276\344\272\272\345\215\241", nullptr));
        m_cbCardType->setItemText(2, QApplication::translate("MainWindow", "\351\273\221\345\220\215\345\215\225\345\215\241", nullptr));
        m_cbCardType->setItemText(3, QApplication::translate("MainWindow", "\345\267\241\346\233\264\345\215\241", nullptr));
        m_cbCardType->setItemText(4, QApplication::translate("MainWindow", "\350\203\201\350\277\253\345\215\241", nullptr));
        m_cbCardType->setItemText(5, QApplication::translate("MainWindow", "\350\266\205\347\272\247\345\215\241", nullptr));
        m_cbCardType->setItemText(6, QApplication::translate("MainWindow", "\346\235\245\345\256\276\345\215\241", nullptr));
        m_cbCardType->setItemText(7, QApplication::translate("MainWindow", "\350\247\243\351\231\244\345\215\241", nullptr));
        m_cbCardType->setItemText(8, QApplication::translate("MainWindow", "\345\221\230\345\267\245\345\215\241", nullptr));
        m_cbCardType->setItemText(9, QApplication::translate("MainWindow", "\345\272\224\346\200\245\345\215\241", nullptr));
        m_cbCardType->setItemText(10, QApplication::translate("MainWindow", "\345\272\224\346\200\245\347\256\241\347\220\206\345\215\241", nullptr));

        m_cbUserType->setItemText(0, QApplication::translate("MainWindow", "\346\231\256\351\200\232\347\224\250\346\210\267", nullptr));
        m_cbUserType->setItemText(1, QApplication::translate("MainWindow", "\347\256\241\347\220\206\345\221\230\347\224\250\346\210\267", nullptr));

        m_btnAdd->setText(QApplication::translate("MainWindow", "\344\277\256\346\224\271", nullptr));
        m_btnQuit->setText(QApplication::translate("MainWindow", "\350\277\224\345\233\236", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "\345\215\241\345\257\206\347\240\201 : ", nullptr));
        label_7->setText(QApplication::translate("MainWindow", " \345\247\223\345\220\215  : ", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "\345\267\245\345\217\267   : ", nullptr));
        m_btnPicPath->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\345\233\276\347\211\207\350\267\257\345\276\204", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CFmChangeUser : public Ui_CFmChangeUser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FMCHANGEUSER_H
