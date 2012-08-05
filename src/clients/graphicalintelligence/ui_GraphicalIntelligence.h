/********************************************************************************
** Form generated from reading UI file 'GraphicalIntelligence.ui'
**
** Created: Tue 20. Sep 13:57:53 2011
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHICALINTELLIGENCE_H
#define UI_GRAPHICALINTELLIGENCE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "stageview.h"

QT_BEGIN_NAMESPACE

class Ui_GraphicalIntelligenceClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout_2;
    QLabel *lblJuiz;
    QLineEdit *txtJuiz;
    QLabel *lblTempoJogo;
    QLineEdit *txtTempoJogo;
    QVBoxLayout *verticalLayout;
    QLabel *lblPlacar;
    QHBoxLayout *horizontalLayout;
    QLineEdit *txtPlacarAmarelo;
    QLabel *lblX;
    QLineEdit *txtPlacarAzul;
    QGridLayout *gridLayout;
    QLabel *lblNrTimeouts;
    QLineEdit *txtNrTimeoutsAzul;
    QLabel *lblTempoTimeouts;
    QLineEdit *txtTempoTimeoutsAzul;
    QLineEdit *txtNrTimeoutsAmarelo;
    QLineEdit *txtTempoTimeoutsAmarelo;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *chkUpdate;
    QCheckBox *chkForceInt;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QLineEdit *txtStageWidth;
    QLineEdit *txtStageLenght;
    StageView *stageView;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GraphicalIntelligenceClass)
    {
        if (GraphicalIntelligenceClass->objectName().isEmpty())
            GraphicalIntelligenceClass->setObjectName(QString::fromUtf8("GraphicalIntelligenceClass"));
        GraphicalIntelligenceClass->resize(1056, 429);
        centralWidget = new QWidget(GraphicalIntelligenceClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        lblJuiz = new QLabel(centralWidget);
        lblJuiz->setObjectName(QString::fromUtf8("lblJuiz"));

        gridLayout_2->addWidget(lblJuiz, 0, 0, 1, 1);

        txtJuiz = new QLineEdit(centralWidget);
        txtJuiz->setObjectName(QString::fromUtf8("txtJuiz"));

        gridLayout_2->addWidget(txtJuiz, 0, 1, 1, 1);

        lblTempoJogo = new QLabel(centralWidget);
        lblTempoJogo->setObjectName(QString::fromUtf8("lblTempoJogo"));

        gridLayout_2->addWidget(lblTempoJogo, 1, 0, 1, 1);

        txtTempoJogo = new QLineEdit(centralWidget);
        txtTempoJogo->setObjectName(QString::fromUtf8("txtTempoJogo"));

        gridLayout_2->addWidget(txtTempoJogo, 1, 1, 1, 1);


        horizontalLayout_2->addLayout(gridLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lblPlacar = new QLabel(centralWidget);
        lblPlacar->setObjectName(QString::fromUtf8("lblPlacar"));
        lblPlacar->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);

        verticalLayout->addWidget(lblPlacar);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        txtPlacarAmarelo = new QLineEdit(centralWidget);
        txtPlacarAmarelo->setObjectName(QString::fromUtf8("txtPlacarAmarelo"));

        horizontalLayout->addWidget(txtPlacarAmarelo);

        lblX = new QLabel(centralWidget);
        lblX->setObjectName(QString::fromUtf8("lblX"));

        horizontalLayout->addWidget(lblX);

        txtPlacarAzul = new QLineEdit(centralWidget);
        txtPlacarAzul->setObjectName(QString::fromUtf8("txtPlacarAzul"));

        horizontalLayout->addWidget(txtPlacarAzul);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lblNrTimeouts = new QLabel(centralWidget);
        lblNrTimeouts->setObjectName(QString::fromUtf8("lblNrTimeouts"));

        gridLayout->addWidget(lblNrTimeouts, 0, 0, 1, 1);

        txtNrTimeoutsAzul = new QLineEdit(centralWidget);
        txtNrTimeoutsAzul->setObjectName(QString::fromUtf8("txtNrTimeoutsAzul"));

        gridLayout->addWidget(txtNrTimeoutsAzul, 0, 1, 1, 1);

        lblTempoTimeouts = new QLabel(centralWidget);
        lblTempoTimeouts->setObjectName(QString::fromUtf8("lblTempoTimeouts"));

        gridLayout->addWidget(lblTempoTimeouts, 1, 0, 1, 1);

        txtTempoTimeoutsAzul = new QLineEdit(centralWidget);
        txtTempoTimeoutsAzul->setObjectName(QString::fromUtf8("txtTempoTimeoutsAzul"));

        gridLayout->addWidget(txtTempoTimeoutsAzul, 1, 1, 1, 1);

        txtNrTimeoutsAmarelo = new QLineEdit(centralWidget);
        txtNrTimeoutsAmarelo->setObjectName(QString::fromUtf8("txtNrTimeoutsAmarelo"));

        gridLayout->addWidget(txtNrTimeoutsAmarelo, 0, 2, 1, 1);

        txtTempoTimeoutsAmarelo = new QLineEdit(centralWidget);
        txtTempoTimeoutsAmarelo->setObjectName(QString::fromUtf8("txtTempoTimeoutsAmarelo"));

        gridLayout->addWidget(txtTempoTimeoutsAmarelo, 1, 2, 1, 1);


        horizontalLayout_2->addLayout(gridLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        chkUpdate = new QCheckBox(centralWidget);
        chkUpdate->setObjectName(QString::fromUtf8("chkUpdate"));
        chkUpdate->setChecked(true);

        horizontalLayout_3->addWidget(chkUpdate);

        chkForceInt = new QCheckBox(centralWidget);
        chkForceInt->setObjectName(QString::fromUtf8("chkForceInt"));

        horizontalLayout_3->addWidget(chkForceInt);


        horizontalLayout_5->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        txtStageWidth = new QLineEdit(centralWidget);
        txtStageWidth->setObjectName(QString::fromUtf8("txtStageWidth"));

        horizontalLayout_4->addWidget(txtStageWidth);

        txtStageLenght = new QLineEdit(centralWidget);
        txtStageLenght->setObjectName(QString::fromUtf8("txtStageLenght"));

        horizontalLayout_4->addWidget(txtStageLenght);


        horizontalLayout_5->addLayout(horizontalLayout_4);


        verticalLayout_2->addLayout(horizontalLayout_5);

        stageView = new StageView(centralWidget);
        stageView->setObjectName(QString::fromUtf8("stageView"));

        verticalLayout_2->addWidget(stageView);

        GraphicalIntelligenceClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(GraphicalIntelligenceClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1056, 26));
        GraphicalIntelligenceClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GraphicalIntelligenceClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        GraphicalIntelligenceClass->addToolBar(Qt::RightToolBarArea, mainToolBar);
        statusBar = new QStatusBar(GraphicalIntelligenceClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        GraphicalIntelligenceClass->setStatusBar(statusBar);

        retranslateUi(GraphicalIntelligenceClass);

        QMetaObject::connectSlotsByName(GraphicalIntelligenceClass);
    } // setupUi

    void retranslateUi(QMainWindow *GraphicalIntelligenceClass)
    {
        GraphicalIntelligenceClass->setWindowTitle(QApplication::translate("GraphicalIntelligenceClass", "GraphicalIntelligence", 0, QApplication::UnicodeUTF8));
        lblJuiz->setText(QApplication::translate("GraphicalIntelligenceClass", "Juiz:", 0, QApplication::UnicodeUTF8));
        lblTempoJogo->setText(QApplication::translate("GraphicalIntelligenceClass", "Tempo restante:", 0, QApplication::UnicodeUTF8));
        lblPlacar->setText(QApplication::translate("GraphicalIntelligenceClass", "Placar", 0, QApplication::UnicodeUTF8));
        lblX->setText(QApplication::translate("GraphicalIntelligenceClass", "x", 0, QApplication::UnicodeUTF8));
        lblNrTimeouts->setText(QApplication::translate("GraphicalIntelligenceClass", "# Timeouts:", 0, QApplication::UnicodeUTF8));
        lblTempoTimeouts->setText(QApplication::translate("GraphicalIntelligenceClass", "Tempo Timeouts:", 0, QApplication::UnicodeUTF8));
        chkUpdate->setText(QApplication::translate("GraphicalIntelligenceClass", "Update", 0, QApplication::UnicodeUTF8));
        chkForceInt->setText(QApplication::translate("GraphicalIntelligenceClass", "For\303\247ar intelig\303\252ncia", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("GraphicalIntelligenceClass", "Stage size", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GraphicalIntelligenceClass: public Ui_GraphicalIntelligenceClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHICALINTELLIGENCE_H
