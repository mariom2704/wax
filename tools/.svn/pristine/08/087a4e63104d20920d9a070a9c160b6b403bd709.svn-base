#include "updateassistqt.h"

#include <qmessagebox>
#include <qlayout>

#include <string>
using namespace std;

updateassistqt::updateassistqt(QWidget *parent, Qt::WFlags flags)
    : QDialog(parent, flags)
{
	ui.setupUi(this);
	ui.progressBar->setValue(0);
	assi.set_model(ui.tableWidget);
	ui.tableWidget->setColumnWidth(0, 420);
	ui.tableWidget->setColumnWidth(1, 100);
	ui.tableWidget->setColumnWidth(2, 150);
	ui.tableWidget->setColumnWidth(3, 120);
	setWindowIcon((QIcon(QString::fromUtf8(":/updateassistqt/Resources/updateassist.png"))));
	setWindowTitle("Updateassist Version 2.0");
}

updateassistqt::~updateassistqt()
{
}


void updateassistqt::on_pushButtonOk_clicked()
{
	close();
}

void updateassistqt::on_pushButtonCheck_clicked()
{
	while (ui.tableWidget->rowCount())
		ui.tableWidget->removeRow(0);  
	ui.tableWidget->repaint();


	vector<string> lst;

	if (!assi.read_ini("standardnl", lst))
			return;
	ui.tableWidget->setSortingEnabled(false);
	ui.progressBar->setRange(0, lst.size());
	int range = 0;
	ui.progressBar->setValue(range);
	vector<string>::iterator it;
	for (it = lst.begin(); it != lst.end(); it++)
	{
		ui.tableWidget->repaint();
		assi.process_stdnl(*it);
		ui.progressBar->setValue(++range);
	}
	ui.tableWidget->setSortingEnabled(true);

/*
	if (!assi.read_ini("dbver", lst))
			return;
	
	if (((CButton *)GetDlgItem(IDC_DBVER))->GetCheck())
	{
		ui.progressBar->setRange(0, lst.size());
		range = 0;
		ui.progressBar->setValue(range);
		assi.add_output_string("Datenbankeinträge");
		for (it = lst.begin(); it != lst.end(); it++)
		{
			assi.checkLastUpdate(*it);
			ui.progressBar->setValue(++range);
			this->repaint();
		}
	}
	*/
	if (!assi.read_ini("exever", lst))
			return;
	ui.tableWidget->setSortingEnabled(false);
	if (ui.exever->isChecked())
	{
		ui.progressBar->setRange(0, lst.size());
		range = 0;
		ui.progressBar->setValue(range);
		for (it = lst.begin(); it != lst.end(); it++)
		{
			assi.check_version_info(*it);
			ui.progressBar->setValue(++range);
			this->repaint();
		}
	}
	ui.tableWidget->setSortingEnabled(true);
	
}


void updateassistqt::on_tableWidget_cellDoubleClicked(int row,int col)
{
	if (col != 0)
		return;

	QTableWidgetItem* item = ui.tableWidget->item(row, col);
	if (!item)
		return;
	QString itemtext = item->text();

	if (QMessageBox::question(this, "Update", (QString)"Datei '" + itemtext + (QString)"' wirklich updaten?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
		return;

	string msg = assi.do_update(itemtext.toStdString());
	if (msg.length())
		QMessageBox::question(this, "Update konnte nicht vorgenommen werden", msg.c_str());
}

void updateassistqt::remove_or_add_action(int add, assistant::actions action)
{
	if (add)
		assi.add_action(action);
	else
		assi.remove_action(action);

}

void updateassistqt::on_SpanxExe_stateChanged(int num)
{
	remove_or_add_action(num, assistant::SPANX_EXE);
}

void updateassistqt::on_SpleditExe_stateChanged(int state)
{
	remove_or_add_action(state, assistant::SPLEDIT_EXE);
}

void updateassistqt::on_SpanxStamm_stateChanged(int state)
{
	remove_or_add_action(state, assistant::STAMMDB_SPANX);
}

void updateassistqt::on_SpanxMdb_stateChanged(int state)
{
	remove_or_add_action(state, assistant::SPANXDB);
}

void updateassistqt::on_checkBoxNurWaxExe_stateChanged(int state)
{
	remove_or_add_action(state, assistant::WAX_EXE_ONLY);
}

void updateassistqt::on_WaxStamm_stateChanged(int state)
{
	remove_or_add_action(state, assistant::STAMMDB_WAX);
}

void updateassistqt::on_WaxUserMdb_stateChanged(int state)
{
	remove_or_add_action(state, assistant::WAXDB);
}

void updateassistqt::on_checkBoxWaxExeUndDlls_stateChanged(int state)
{
	remove_or_add_action(state, assistant::WAX_EXE);
}

void updateassistqt::on_checkBoxMXDM_stateChanged(int state)
{
	remove_or_add_action(state, assistant::MXDM);
}