#ifndef __Kaufhof
#define __Kaufhof

#include <qtgui\QtGui>
#include <QtSql\QtSql>
#include <QtCore\QtCore>

#include "stl.h"

extern QApplication *mainapp;

class Kaufhof : public QDialog
{
	Q_OBJECT
public:
	/// Konstruktor
	Kaufhof(QWidget *parent = 0);
	virtual ~Kaufhof();

	/// Oberfläche erstellen
	void init();

private slots:
	/// Pfad zur ersten Spanx Datenbank auswählen
	void select_db1();

	void auswerten();

	/// Funktion bzw. Dialog abbrechen
	void abbrechen();


private:
	void update(QSqlDatabase& db, double vk, QString code);

	void select_db(QLineEdit *entry);
	QString get_dbpath(const QString &file) const;
	bool txt_equal(const string& code, const string& txta);

	void init_layout(QString text, QHBoxLayout *&layout, QLabel *&label, QLineEdit *&db, QPushButton *&seldb);
	QLabel *label1;
	QLineEdit *db1;
	QPushButton *seldb1;
	
	QLabel *label2;
	QLineEdit *db2;
	
	QHBoxLayout *upper;
	QHBoxLayout *lower;

	QVBoxLayout *mainlayout;

	QHBoxLayout *buttons;
	QPushButton *auswertung;
	QPushButton *abbruch;
};





#endif