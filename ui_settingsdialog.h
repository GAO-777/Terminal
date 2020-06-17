/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *HL_RB;
    QRadioButton *Serial_RB;
    QRadioButton *SSH_RB;
    QHBoxLayout *HL_Parameters;
    QHBoxLayout *HL_Apply;
    QSpacerItem *horizontalSpacer;
    QPushButton *applyButton;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->resize(209, 269);
        verticalLayout = new QVBoxLayout(SettingsDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        HL_RB = new QHBoxLayout();
        HL_RB->setObjectName(QString::fromUtf8("HL_RB"));
        Serial_RB = new QRadioButton(SettingsDialog);
        Serial_RB->setObjectName(QString::fromUtf8("Serial_RB"));
        Serial_RB->setCheckable(true);
        Serial_RB->setChecked(false);

        HL_RB->addWidget(Serial_RB);

        SSH_RB = new QRadioButton(SettingsDialog);
        SSH_RB->setObjectName(QString::fromUtf8("SSH_RB"));

        HL_RB->addWidget(SSH_RB);


        verticalLayout->addLayout(HL_RB);

        HL_Parameters = new QHBoxLayout();
        HL_Parameters->setObjectName(QString::fromUtf8("HL_Parameters"));

        verticalLayout->addLayout(HL_Parameters);

        HL_Apply = new QHBoxLayout();
        HL_Apply->setObjectName(QString::fromUtf8("HL_Apply"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        HL_Apply->addItem(horizontalSpacer);

        applyButton = new QPushButton(SettingsDialog);
        applyButton->setObjectName(QString::fromUtf8("applyButton"));

        HL_Apply->addWidget(applyButton);


        verticalLayout->addLayout(HL_Apply);


        retranslateUi(SettingsDialog);

        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QCoreApplication::translate("SettingsDialog", " \320\241onnection settings", nullptr));
        Serial_RB->setText(QCoreApplication::translate("SettingsDialog", "Serial", nullptr));
        SSH_RB->setText(QCoreApplication::translate("SettingsDialog", "SSH", nullptr));
        applyButton->setText(QCoreApplication::translate("SettingsDialog", "Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
