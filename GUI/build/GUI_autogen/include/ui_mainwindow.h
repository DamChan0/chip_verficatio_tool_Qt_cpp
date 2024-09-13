/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QFrame *FunctionTab;
    QVBoxLayout *verticalLayout;
    QComboBox *functionComboBox;
    QScrollArea *TestLists;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_4;
    QPushButton *generateButton;
    QFrame *RegisterTab;
    QFormLayout *formLayout;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEdit_3;
    QSpacerItem *horizontalSpacer;
    QCheckBox *checkBox;
    QSpinBox *spinBox;
    QPushButton *saveRegisterButton;
    QFrame *Resulttab;
    QVBoxLayout *verticalLayout_6;
    QScrollArea *scrollArea_4;
    QWidget *scrollAreaWidgetContents_4;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tableWidget_3;
    QPushButton *deserializeButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1182, 829);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        FunctionTab = new QFrame(centralwidget);
        FunctionTab->setObjectName(QString::fromUtf8("FunctionTab"));
        FunctionTab->setFrameShape(QFrame::StyledPanel);
        FunctionTab->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(FunctionTab);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        functionComboBox = new QComboBox(FunctionTab);
        functionComboBox->addItem(QString());
        functionComboBox->addItem(QString());
        functionComboBox->addItem(QString());
        functionComboBox->addItem(QString());
        functionComboBox->setObjectName(QString::fromUtf8("functionComboBox"));

        verticalLayout->addWidget(functionComboBox);

        TestLists = new QScrollArea(FunctionTab);
        TestLists->setObjectName(QString::fromUtf8("TestLists"));
        TestLists->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 376, 703));
        verticalLayout_4 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(1, 1, 1, 1);
        TestLists->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(TestLists);

        generateButton = new QPushButton(FunctionTab);
        generateButton->setObjectName(QString::fromUtf8("generateButton"));

        verticalLayout->addWidget(generateButton);


        horizontalLayout->addWidget(FunctionTab);

        RegisterTab = new QFrame(centralwidget);
        RegisterTab->setObjectName(QString::fromUtf8("RegisterTab"));
        RegisterTab->setCursor(QCursor(Qt::ArrowCursor));
        RegisterTab->setFrameShape(QFrame::StyledPanel);
        RegisterTab->setFrameShadow(QFrame::Raised);
        formLayout = new QFormLayout(RegisterTab);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(2, 2, 2, 2);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(2, 0, 2, -1);
        horizontalSpacer_2 = new QSpacerItem(5, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 1, 2, 1);

        lineEdit_3 = new QLineEdit(RegisterTab);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        gridLayout->addWidget(lineEdit_3, 0, 2, 2, 1);

        horizontalSpacer = new QSpacerItem(4, 1, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 3, 2, 2);

        checkBox = new QCheckBox(RegisterTab);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout->addWidget(checkBox, 1, 4, 1, 1);

        spinBox = new QSpinBox(RegisterTab);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));

        gridLayout->addWidget(spinBox, 0, 0, 2, 1);


        formLayout->setLayout(1, QFormLayout::SpanningRole, gridLayout);

        saveRegisterButton = new QPushButton(RegisterTab);
        saveRegisterButton->setObjectName(QString::fromUtf8("saveRegisterButton"));

        formLayout->setWidget(2, QFormLayout::SpanningRole, saveRegisterButton);


        horizontalLayout->addWidget(RegisterTab, 0, Qt::AlignTop);

        Resulttab = new QFrame(centralwidget);
        Resulttab->setObjectName(QString::fromUtf8("Resulttab"));
        Resulttab->setFrameShape(QFrame::StyledPanel);
        Resulttab->setFrameShadow(QFrame::Raised);
        verticalLayout_6 = new QVBoxLayout(Resulttab);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        scrollArea_4 = new QScrollArea(Resulttab);
        scrollArea_4->setObjectName(QString::fromUtf8("scrollArea_4"));
        scrollArea_4->setWidgetResizable(true);
        scrollAreaWidgetContents_4 = new QWidget();
        scrollAreaWidgetContents_4->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_4"));
        scrollAreaWidgetContents_4->setGeometry(QRect(0, 0, 362, 747));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents_4);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tableWidget_3 = new QTableWidget(scrollAreaWidgetContents_4);
        tableWidget_3->setObjectName(QString::fromUtf8("tableWidget_3"));
        tableWidget_3->setEnabled(true);

        verticalLayout_2->addWidget(tableWidget_3);

        deserializeButton = new QPushButton(scrollAreaWidgetContents_4);
        deserializeButton->setObjectName(QString::fromUtf8("deserializeButton"));

        verticalLayout_2->addWidget(deserializeButton);

        scrollArea_4->setWidget(scrollAreaWidgetContents_4);

        verticalLayout_6->addWidget(scrollArea_4);


        horizontalLayout->addWidget(Resulttab);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1182, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        functionComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Function A", nullptr));
        functionComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Function B", nullptr));
        functionComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Function C", nullptr));
        functionComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Function D", nullptr));

        generateButton->setText(QCoreApplication::translate("MainWindow", "Send test Config", nullptr));
        lineEdit_3->setText(QCoreApplication::translate("MainWindow", "    SET Register", nullptr));
        checkBox->setText(QCoreApplication::translate("MainWindow", "fixedpoint", nullptr));
        saveRegisterButton->setText(QCoreApplication::translate("MainWindow", "save", nullptr));
        deserializeButton->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
