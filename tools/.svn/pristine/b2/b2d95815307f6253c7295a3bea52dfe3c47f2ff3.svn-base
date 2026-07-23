#include "waxstat.h"
#include <QtGui>
#include "BefehlAuswertungAfMatKumuliert.h"

WaxStat::WaxStat(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
	ui.setupUi(this);

	ui.dateEditStart->setDate(QDate::fromString("01.01.2005","dd.MM.yyyy"));
	ui.dateEditEnde->setDate(QDate::fromString("31.12.2005","dd.MM.yyyy"));

	ui.comboBoxAuswertung->addItem("Arbeitsfolgen und Material kumuliert");
}

WaxStat::~WaxStat()
{

}

/**
Auswertung starten.
*/
void WaxStat::on_pushButton_Start_clicked()
{

	//Hier Auswerten welche Statistik gewählt wurde, automatisch Objekt holen
	/*
		wird erst realisiert wenn es mehrere Statistiken zur Auswahl gibt
	*/


	
	BefehlAuswertungAfMatKumuliert befehl;

	//Werte setzen
	befehl.setStartDate(ui.dateEditStart->date());
	befehl.setEndDate(ui.dateEditEnde->date());
	befehl.setVerzeichnis(ui.lineEditVerzeichnis->text());
	befehl.setDatei(ui.lineEditDatei->text());


	//Befehl starten, Fehlerauswertung erfolgt in dieser Funktion
	befehl.start();

	
}





/**
Verzeichnis wählen in dem die Exceldatei gespeichert werden soll
*/
void WaxStat::on_pushButtonChosePath_clicked()
{
	QString directory = QFileDialog::getExistingDirectory(this,
														  tr("Verzeichnis wählen"), 
														  ui.lineEditVerzeichnis->text());

	ui.lineEditVerzeichnis->setText(directory);
}