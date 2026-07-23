#ifndef WAXSTAT_H
#define WAXSTAT_H

#include <QtGui/QMainWindow>
#include "ui_waxstat.h"

/**
Startdialog für die Auswertung
*/
class WaxStat : public QMainWindow
{
    Q_OBJECT

public:
    WaxStat(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~WaxStat();

private:
    Ui::WaxStatClass ui;

private slots:
	void on_pushButtonChosePath_clicked();
	void on_pushButton_Start_clicked();
};

#endif // WAXSTAT_H
