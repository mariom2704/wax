#pragma once

#include <QDialog>
#include <QList>
#include "ui_LizenzGui.h"
#include <lizenz/key.h>



class LizenzGui : public QDialog
{
	Q_OBJECT

public:
	LizenzGui(QWidget *parent = 0);

	bool get_ExitState()
	{
		return bExit; 
	}
	

private slots:

	void read_file();
	
	/// Schreibt die Lizenzdatei
	void write_license_data();

	/// Erstellt aus den Eingaben den Key
	void create_license_data();

	/// Setzt die möglichen Module für das jeweilige Programm
	void on_program_changed(const QString& prog);

private:

	/// Liest die Eingabefelder aus und setzt die Daten in Key
	void read_key_data();

	/// Setzt die Eingabefelder dem Key entsprechend
	void show_key_data();

	
	int LicenceMark;

	bool bExit;

	/// Sucht im Dateiinhalt nach einem Lizenzschlüssel und versucht ihn zu decodieren
	bool create_key_from_keyid();

	/// Sucht im Dateiinhalt nach Werten für HWID, valid from etc. 
	void init_key_from_filedata();

	/// Verdrahten der Eingabefelder
	void connect_actions(bool doconnect = true);

	void init();


	void write_file();

	Ui::LizenzGuiClass ui;

	/// Inhalt des Lizenzfiles
	QList<QString> content;

	License::Key key;

	QString filename;

};


