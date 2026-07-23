#ifndef UPDATEASSISTQT_H
#define UPDATEASSISTQT_H

#include <QDialog>
#include "ui_updateassistqt.h"
#include "assistant.h"

class updateassistqt : public QDialog
{
    Q_OBJECT

public:
    updateassistqt(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~updateassistqt();

private:
	assistant assi;
    Ui::updateassistqtClass ui;
	void remove_or_add_action(int add, assistant::actions action);

private slots:
	void on_checkBoxWaxExeUndDlls_stateChanged(int state);
	void on_WaxUserMdb_stateChanged(int state);
	void on_WaxStamm_stateChanged(int state);
	void on_checkBoxNurWaxExe_stateChanged(int state);
	void on_SpanxMdb_stateChanged(int state);
	void on_SpanxStamm_stateChanged(int state);
	void on_SpleditExe_stateChanged(int state);
	void on_tableWidget_cellDoubleClicked(int row, int col);
	void on_SpanxExe_stateChanged(int num);
	void on_checkBoxMXDM_stateChanged(int state);
	void on_pushButtonCheck_clicked();
	void on_pushButtonOk_clicked();
};

#endif // UPDATEASSISTQT_H
