#include "spanxdiff.h"

#include <QtSql\QtSql>
#include <QtCore\QtCore>

#include "rtfparser.h"


class artikel
{
public:
	artikel(const string &liste, const string &gruppe, const string &code, const string &langtext);
	artikel(const artikel& other);

	string liste;
	string gruppe;
	string code;
	string langtext;
};

artikel::artikel(const string &liste, const string &gruppe, const string &code, const string &langtext)
{
	this->liste = liste;
	this->gruppe = gruppe;
	this->code = code;
	this->langtext = langtext;
}

artikel::artikel(const artikel &other)
{
	liste = other.liste;
	gruppe = other.gruppe;
	code = other.code;
	langtext = other.langtext;
}

vector<artikel> artikellst;

spanxdiff::spanxdiff(QWidget *parent) : QDialog(parent)
{
	label1 = NULL;
	label2 = NULL;
	db1 = NULL;
	db2 = NULL;
	upper = NULL;
	lower = NULL;
	mainlayout = NULL;
	seldb1 = NULL;
	seldb2 = NULL;
}

spanxdiff::~spanxdiff()
{
	QSettings setting("Spanx", "Spanxdiff" );
	setting.setValue("/main/db1", db1->text()); 
	setting.setValue("/main/db2", db2->text()); 
	
	QSettings set;
	delete label1;
	delete label2;
	delete db1;
	delete db2;
	delete upper;
	delete lower;
	delete mainlayout;
	delete seldb1;
	delete seldb2;
}

void spanxdiff::init()
{
	init_layout("Datenbank &1", upper, label1, db1, seldb1);
	init_layout("Datenbank &2", lower, label2, db2, seldb2);

	QSettings setting("Spanx", "Spanxdiff" );
	QString xdb1 = setting.value("/main/db1").toString();
	QString xdb2 = setting.value("/main/db2").toString();

	db1->setText(xdb1);
	db2->setText(xdb2);

	
	mainlayout = new QVBoxLayout;
	
	mainlayout->addLayout(upper);
	mainlayout->addLayout(lower);
	mainlayout->setSpacing(3);
	mainlayout->setMargin(10);

	auswertung = new QPushButton(tr("&Vergleichen..."));
	auswertung->setDefault(true);

	abbruch = new QPushButton(tr("&Abbrechen"));
	buttons = new QHBoxLayout;
	

	buttons->addWidget(auswertung);
	buttons->addWidget(abbruch);
	buttons->setSpacing(12);
	buttons->setMargin(10);

	
	mainlayout->addLayout(buttons);
	setLayout(mainlayout);
	
	connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );
	connect(seldb1, SIGNAL(clicked()), this, SLOT(select_db1()));
	connect(seldb2, SIGNAL(clicked()), this, SLOT(select_db2()));

	connect(auswertung, SIGNAL(clicked()), this, SLOT(auswerten()));
	connect(abbruch, SIGNAL(clicked()), this, SLOT(abbrechen()));
	show();
}

void spanxdiff::init_layout(QString text, QHBoxLayout *&layout, QLabel *&label, QLineEdit *&db, QPushButton *&seldb)
{
	label = new QLabel(text, this );
	db = new QLineEdit(this);
	label->setBuddy(db1);
	db->setMinimumWidth(350);
	seldb = new QPushButton(this);
	seldb->setFixedSize(40, 20);
	seldb->setText("...");

	layout = new QHBoxLayout;
	layout->addWidget(label);
	layout->addWidget(db);
	layout->addWidget(seldb);
	layout->setMargin(10);
	layout->setSpacing(6);
}

void spanxdiff::select_db1()
{
	select_db(db1);
}

void spanxdiff::select_db2()
{
	select_db(db2);
}

void spanxdiff::select_db(QLineEdit *entry)
{
	QString path = get_dbpath(entry->text());
	path.replace('/', '\\');
	if (path.length())
		entry->setText(path);
}

QString spanxdiff::get_dbpath(const QString &file) const
{
	QFileDialog f(0, "Datenbankauswahl",  ".", "*.mdb");
	f.selectFile(file);
	return f.getOpenFileName( );
}

void spanxdiff::auswerten()
{
	QFile splfile("change.spl");
	splfile.open( QIODevice::ReadWrite | QIODevice::Truncate);


	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
	QString con;
	QTextStream(&con) << "DRIVER=Microsoft Access-Treiber (*.mdb);DSN='';DBQ=" << db1->text();

	db.setDatabaseName(con);

	if (!db.open()) 
	{
        QMessageBox::critical(0, tr("Öffnen der Datenbank 1 gescheitert!"),
            tr("Es konnte keine Verbindung zur ausgewählten Datenbank erstellt werden.\n"
					"Bitte die Pfadangaben überprüfen.\n"), QMessageBox::Cancel,
                     QMessageBox::NoButton);
        return;
    }

	QSqlQuery xquery(db);
    xquery.prepare("Select Count(*) from PreislistenArtikel ");
    xquery.exec();
	xquery.first();
	int count = xquery.value(0).toInt();

	QSqlQuery query("Select code, gruppe, liste, langtext from PreislistenArtikel", db);
	query.next();

	
	QProgressDialog progress( "Langtexte vergleichen...", "&Abbrechen", 0, count, this );
	progress.setMinimumDuration(100);
	progress.setMinimumWidth(500); 

	string lastgroup, lastlist;
	int entries = 0;
	for ( unsigned int i = 0; i < count && query.next(); i++ ) 
	{
		QString code = query.value(0).toString();
		QString gruppe = query.value(1).toString();
		QString liste = query.value(2).toString();
		QString langtext = query.value(3).toString();

		RtfParser rtf;
		rtf.rtf = langtext.toStdString();
		rtf.parse();
		progress.setValue(i);
		QString labeltxt;
		QTextStream(&labeltxt) << liste << "/" << gruppe << "/" << code;
		progress.setLabelText(labeltxt);
		if (!txt_equal(code.toStdString().c_str(), rtf.txt.c_str()))
		{
			if (lastlist != liste.toStdString())
			{
				string output;
				output = (string)"^^" + liste.toStdString() + (string)"\r\n";
				splfile.write(output.c_str(), output.length());
			}
			if (lastgroup != gruppe.toStdString())
			{
				string output;
				output = (string)"^" + gruppe.toStdString() + (string)"\r\n";
				splfile.write(output.c_str(), output.length());
			}
			string output;
			output = code.toStdString() + (string)"\r\n";
			splfile.write(output.c_str(), output.length());
			lastgroup = gruppe.toStdString();
			lastlist = liste.toStdString();
			entries++;
		}
		
		mainapp->processEvents();
	    if ( progress.wasCanceled() )
			break;
	}
	splfile.close();
	if (entries)
	{
		QString msg;
		QTextStream(&msg) << tr("Es wurden ") << entries << " Einträge in " << splfile.fileName() << " vorgenommen";	
		QMessageBox::information(this, "SPL Datei", msg, QMessageBox::Ok, QMessageBox::NoButton);
	}
	db.close();
}

bool spanxdiff::txt_equal(const string& checkcode, const string& txta)
{
/** 
	Sucht in der zweiten Datenbank nach dem Code und Liefert true, wenn Texte gleich sind
*/
	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "zweitedb");
	QString con;
	QTextStream(&con) << "DRIVER=Microsoft Access-Treiber (*.mdb);DSN='';DBQ=" << db2->text();

	db.setDatabaseName(con);

	if (!db.open()) 
	{
        QMessageBox::critical(0, tr("Öffnen der Datenbank 2 gescheitert!"),
            tr("Es konnte keine Verbindung zur ausgewählten Datenbank erstellt werden.\n"
					"Bitte die Pfadangaben überprüfen.\n"), QMessageBox::Cancel,
                     QMessageBox::NoButton);
        throw "Fehler";
    }

	QString select;
	QTextStream(&select) << "Select langtext from PreislistenArtikel where code = '" << checkcode.c_str() << "'";
	QSqlQuery xquery(db);
    xquery.prepare(select);
    xquery.exec();
	xquery.first();
	string langtext = xquery.value(0).toString().toStdString();
	db.close();
	RtfParser rtf;
	rtf.rtf = langtext;
	rtf.parse();
	return txta == rtf.txt.c_str();
}

void spanxdiff::abbrechen()
{
	QMessageBox box("Abbruch", "Programm wirklich abbrechen?", QMessageBox::Information, 
		QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
	if ( box.exec() == QMessageBox::Ok )
	{
		emit done(1);
	}

}