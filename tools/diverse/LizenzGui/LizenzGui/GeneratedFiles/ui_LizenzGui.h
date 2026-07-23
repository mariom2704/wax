/********************************************************************************
** Form generated from reading UI file 'LizenzGui.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIZENZGUI_H
#define UI_LIZENZGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LizenzGuiClass
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label_begin;
    QLabel *label_end;
    QHBoxLayout *hboxLayout1;
    QCalendarWidget *calendarWidget_begin;
    QCalendarWidget *calendarWidget_end;
    QVBoxLayout *vboxLayout;
    QLabel *label_program;
    QComboBox *comboBox_programm;
    QGridLayout *gridLayout1;
    QCheckBox *checkBox_novec;
    QCheckBox *checkBox_hfc;
    QCheckBox *checkBox_co2;
    QCheckBox *checkBox_ig55;
    QCheckBox *checkBox_ig541;
    QCheckBox *checkBox_autocad;
    QLabel *label;
    QLabel *label_module;
    QLabel *label_2;
    QLabel *label_3;
    QCheckBox *checkBox_stickstoff;
    QCheckBox *checkBox_argon;
    QVBoxLayout *vboxLayout1;
    QLabel *label_hwid;
    QLineEdit *lineEdit_hwid;
    QLabel *label_key;
    QLineEdit *lineEdit_key;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacerItem;
    QPushButton *pushButton_read_license;
    QPushButton *pushButton_write_license;

    void setupUi(QDialog *LizenzGuiClass)
    {
        if (LizenzGuiClass->objectName().isEmpty())
            LizenzGuiClass->setObjectName(QStringLiteral("LizenzGuiClass"));
        LizenzGuiClass->resize(552, 482);
        QIcon icon;
        icon.addFile(QStringLiteral(":/LizenzGui/Resources/argotec.png"), QSize(), QIcon::Normal, QIcon::Off);
        LizenzGuiClass->setWindowIcon(icon);
        gridLayout = new QGridLayout(LizenzGuiClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        label_begin = new QLabel(LizenzGuiClass);
        label_begin->setObjectName(QStringLiteral("label_begin"));

        hboxLayout->addWidget(label_begin);

        label_end = new QLabel(LizenzGuiClass);
        label_end->setObjectName(QStringLiteral("label_end"));

        hboxLayout->addWidget(label_end);


        gridLayout->addLayout(hboxLayout, 0, 0, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QStringLiteral("hboxLayout1"));
        calendarWidget_begin = new QCalendarWidget(LizenzGuiClass);
        calendarWidget_begin->setObjectName(QStringLiteral("calendarWidget_begin"));

        hboxLayout1->addWidget(calendarWidget_begin);

        calendarWidget_end = new QCalendarWidget(LizenzGuiClass);
        calendarWidget_end->setObjectName(QStringLiteral("calendarWidget_end"));

        hboxLayout1->addWidget(calendarWidget_end);


        gridLayout->addLayout(hboxLayout1, 1, 0, 1, 1);

        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QStringLiteral("vboxLayout"));
        label_program = new QLabel(LizenzGuiClass);
        label_program->setObjectName(QStringLiteral("label_program"));

        vboxLayout->addWidget(label_program);

        comboBox_programm = new QComboBox(LizenzGuiClass);
        comboBox_programm->setObjectName(QStringLiteral("comboBox_programm"));

        vboxLayout->addWidget(comboBox_programm);


        gridLayout->addLayout(vboxLayout, 2, 0, 1, 1);

        gridLayout1 = new QGridLayout();
        gridLayout1->setSpacing(6);
        gridLayout1->setObjectName(QStringLiteral("gridLayout1"));
        checkBox_novec = new QCheckBox(LizenzGuiClass);
        checkBox_novec->setObjectName(QStringLiteral("checkBox_novec"));

        gridLayout1->addWidget(checkBox_novec, 1, 1, 1, 1);

        checkBox_hfc = new QCheckBox(LizenzGuiClass);
        checkBox_hfc->setObjectName(QStringLiteral("checkBox_hfc"));

        gridLayout1->addWidget(checkBox_hfc, 1, 2, 1, 1);

        checkBox_co2 = new QCheckBox(LizenzGuiClass);
        checkBox_co2->setObjectName(QStringLiteral("checkBox_co2"));

        gridLayout1->addWidget(checkBox_co2, 2, 5, 1, 1);

        checkBox_ig55 = new QCheckBox(LizenzGuiClass);
        checkBox_ig55->setObjectName(QStringLiteral("checkBox_ig55"));

        gridLayout1->addWidget(checkBox_ig55, 2, 6, 1, 1);

        checkBox_ig541 = new QCheckBox(LizenzGuiClass);
        checkBox_ig541->setObjectName(QStringLiteral("checkBox_ig541"));

        gridLayout1->addWidget(checkBox_ig541, 2, 7, 1, 1);

        checkBox_autocad = new QCheckBox(LizenzGuiClass);
        checkBox_autocad->setObjectName(QStringLiteral("checkBox_autocad"));

        gridLayout1->addWidget(checkBox_autocad, 3, 1, 1, 1);

        label = new QLabel(LizenzGuiClass);
        label->setObjectName(QStringLiteral("label"));

        gridLayout1->addWidget(label, 1, 0, 1, 1);

        label_module = new QLabel(LizenzGuiClass);
        label_module->setObjectName(QStringLiteral("label_module"));

        gridLayout1->addWidget(label_module, 0, 0, 1, 1);

        label_2 = new QLabel(LizenzGuiClass);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFocusPolicy(Qt::NoFocus);

        gridLayout1->addWidget(label_2, 2, 0, 1, 1);

        label_3 = new QLabel(LizenzGuiClass);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFocusPolicy(Qt::NoFocus);

        gridLayout1->addWidget(label_3, 3, 0, 1, 1);

        checkBox_stickstoff = new QCheckBox(LizenzGuiClass);
        checkBox_stickstoff->setObjectName(QStringLiteral("checkBox_stickstoff"));

        gridLayout1->addWidget(checkBox_stickstoff, 2, 2, 1, 1);

        checkBox_argon = new QCheckBox(LizenzGuiClass);
        checkBox_argon->setObjectName(QStringLiteral("checkBox_argon"));

        gridLayout1->addWidget(checkBox_argon, 2, 1, 1, 1);


        gridLayout->addLayout(gridLayout1, 3, 0, 1, 1);

        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(6);
        vboxLayout1->setObjectName(QStringLiteral("vboxLayout1"));
        label_hwid = new QLabel(LizenzGuiClass);
        label_hwid->setObjectName(QStringLiteral("label_hwid"));

        vboxLayout1->addWidget(label_hwid);

        lineEdit_hwid = new QLineEdit(LizenzGuiClass);
        lineEdit_hwid->setObjectName(QStringLiteral("lineEdit_hwid"));
        lineEdit_hwid->setMaxLength(8);
        lineEdit_hwid->setReadOnly(false);

        vboxLayout1->addWidget(lineEdit_hwid);


        gridLayout->addLayout(vboxLayout1, 4, 0, 1, 1);

        label_key = new QLabel(LizenzGuiClass);
        label_key->setObjectName(QStringLiteral("label_key"));

        gridLayout->addWidget(label_key, 5, 0, 1, 1);

        lineEdit_key = new QLineEdit(LizenzGuiClass);
        lineEdit_key->setObjectName(QStringLiteral("lineEdit_key"));
        lineEdit_key->setMaxLength(300);
        lineEdit_key->setFrame(true);
        lineEdit_key->setReadOnly(true);

        gridLayout->addWidget(lineEdit_key, 6, 0, 1, 1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(13);
        hboxLayout2->setObjectName(QStringLiteral("hboxLayout2"));
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem);

        pushButton_read_license = new QPushButton(LizenzGuiClass);
        pushButton_read_license->setObjectName(QStringLiteral("pushButton_read_license"));

        hboxLayout2->addWidget(pushButton_read_license);

        pushButton_write_license = new QPushButton(LizenzGuiClass);
        pushButton_write_license->setObjectName(QStringLiteral("pushButton_write_license"));

        hboxLayout2->addWidget(pushButton_write_license);


        gridLayout->addLayout(hboxLayout2, 7, 0, 1, 1);


        retranslateUi(LizenzGuiClass);

        QMetaObject::connectSlotsByName(LizenzGuiClass);
    } // setupUi

    void retranslateUi(QDialog *LizenzGuiClass)
    {
        LizenzGuiClass->setWindowTitle(QApplication::translate("LizenzGuiClass", "LizenzGui", 0));
        label_begin->setText(QApplication::translate("LizenzGuiClass", "<b>Lizenzbeginn", 0));
        label_end->setText(QApplication::translate("LizenzGuiClass", "<b>Lizenzende", 0));
        label_program->setText(QApplication::translate("LizenzGuiClass", "<b>Programm", 0));
        checkBox_novec->setText(QApplication::translate("LizenzGuiClass", "MX1230/Novec", 0));
        checkBox_hfc->setText(QApplication::translate("LizenzGuiClass", "MX200/HFC", 0));
        checkBox_co2->setText(QApplication::translate("LizenzGuiClass", "Co2", 0));
        checkBox_ig55->setText(QApplication::translate("LizenzGuiClass", "IG55", 0));
        checkBox_ig541->setText(QApplication::translate("LizenzGuiClass", "IG541", 0));
        checkBox_autocad->setText(QApplication::translate("LizenzGuiClass", "Autocad Erweiterung", 0));
        label->setText(QApplication::translate("LizenzGuiClass", "<b>Chemie", 0));
        label_module->setText(QApplication::translate("LizenzGuiClass", "<b>Module", 0));
        label_2->setText(QApplication::translate("LizenzGuiClass", "<b>Inertgase", 0));
        label_3->setText(QApplication::translate("LizenzGuiClass", "<b>Diverses", 0));
        checkBox_stickstoff->setText(QApplication::translate("LizenzGuiClass", "IG100/Stickstoff", 0));
        checkBox_argon->setText(QApplication::translate("LizenzGuiClass", "IG01/Argon", 0));
        label_hwid->setText(QApplication::translate("LizenzGuiClass", "<b>Hardware ID", 0));
        label_key->setText(QApplication::translate("LizenzGuiClass", "<b>Lizenzschl\303\274ssel", 0));
        pushButton_read_license->setText(QApplication::translate("LizenzGuiClass", "Lizenzdatei laden...", 0));
        pushButton_write_license->setText(QApplication::translate("LizenzGuiClass", "Lizenzdatei schreiben", 0));
    } // retranslateUi

};

namespace Ui {
    class LizenzGuiClass: public Ui_LizenzGuiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIZENZGUI_H
