/********************************************************************************
** Form generated from reading UI file 'ConvStudio.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONVSTUDIO_H
#define UI_CONVSTUDIO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CQConvStudioClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CQConvStudioClass)
    {
        if (CQConvStudioClass->objectName().isEmpty())
            CQConvStudioClass->setObjectName(QStringLiteral("CQConvStudioClass"));
        CQConvStudioClass->resize(600, 400);
        menuBar = new QMenuBar(CQConvStudioClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        CQConvStudioClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CQConvStudioClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        CQConvStudioClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(CQConvStudioClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        CQConvStudioClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(CQConvStudioClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CQConvStudioClass->setStatusBar(statusBar);

        retranslateUi(CQConvStudioClass);

        QMetaObject::connectSlotsByName(CQConvStudioClass);
    } // setupUi

    void retranslateUi(QMainWindow *CQConvStudioClass)
    {
        CQConvStudioClass->setWindowTitle(QApplication::translate("CQConvStudioClass", "ConvSTUDIO", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CQConvStudioClass: public Ui_CQConvStudioClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONVSTUDIO_H
