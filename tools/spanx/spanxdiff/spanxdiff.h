#ifndef __spanxdiff
#define __spanxdiff

#include <qtgui\QtGui>

#include "stl.h"

extern QApplication *mainapp;

class spanxdiff : public QDialog
{
	Q_OBJECT
public:
	/// Konstruktor
	spanxdiff(QWidget *parent = 0);
	virtual ~spanxdiff();

	/// Oberfläche erstellen
	void init();

private slots:
	/// Pfad zur ersten Spanx Datenbank auswählen
	void select_db1();

	/// Pfad zur zweiten Spanx Datenbank auswählen
	void select_db2();

	/// Langtext der beiden ausgewählten Datenbanken vergleichen
	void auswerten();

	/// Funktion bzw. Dialog abbrechen
	void abbrechen();


private:
	void select_db(QLineEdit *entry);
	QString get_dbpath(const QString &file) const;
	bool txt_equal(const string& code, const string& txta);

	void init_layout(QString text, QHBoxLayout *&layout, QLabel *&label, QLineEdit *&db, QPushButton *&seldb);
	QLabel *label1;
	QLineEdit *db1;
	QPushButton *seldb1;
	
	QLabel *label2;
	QLineEdit *db2;
	QPushButton *seldb2;

	QHBoxLayout *upper;
	QHBoxLayout *lower;

	QVBoxLayout *mainlayout;

	QHBoxLayout *buttons;
	QPushButton *auswertung;
	QPushButton *abbruch;
};





#endif