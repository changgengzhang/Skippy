/********************************************************************************
** Form generated from reading UI file 'Skippy.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SKIPPY_H
#define UI_SKIPPY_H

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
#include "myglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_SkippyUi
{
public:
    QWidget *centralWidget;
    MyGLWidget *myGLWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SkippyUi)
    {
        if (SkippyUi->objectName().isEmpty())
            SkippyUi->setObjectName(QStringLiteral("SkippyUi"));
        SkippyUi->resize(853, 877);
        centralWidget = new QWidget(SkippyUi);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        myGLWidget = new MyGLWidget(centralWidget);
        myGLWidget->setObjectName(QStringLiteral("myGLWidget"));
        myGLWidget->setGeometry(QRect(10, 10, 800, 800));
        SkippyUi->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SkippyUi);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 853, 23));
        SkippyUi->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SkippyUi);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SkippyUi->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SkippyUi);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SkippyUi->setStatusBar(statusBar);

        retranslateUi(SkippyUi);

        QMetaObject::connectSlotsByName(SkippyUi);
    } // setupUi

    void retranslateUi(QMainWindow *SkippyUi)
    {
        SkippyUi->setWindowTitle(QApplication::translate("SkippyUi", "Skippy", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SkippyUi: public Ui_SkippyUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SKIPPY_H
