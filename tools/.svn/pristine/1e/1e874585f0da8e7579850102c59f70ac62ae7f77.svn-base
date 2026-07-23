#include "kaufhof.h"



Kaufhof::Kaufhof(QWidget *parent) : QDialog(parent)
{
	label1 = NULL;
	label2 = NULL;
	db1 = NULL;
	db2 = NULL;
	upper = NULL;
	lower = NULL;
	mainlayout = NULL;
	seldb1 = NULL;
}

Kaufhof::~Kaufhof()
{
	QSettings setting("Spanx", "Kaufhof" );
	setting.setValue("/main/db1", db1->text()); 
		
	QSettings set;
	delete label1;
	delete label2;
	delete db1;
	delete db2;
	delete upper;
	delete lower;
	delete mainlayout;
	delete seldb1;
}

void Kaufhof::init()
{
	init_layout("&Datenbank", upper, label1, db1, seldb1);
	
	QSettings setting("Spanx", "Kaufhof" );
	QString xdb1 = setting.value("/main/db1").toString();
	
	db1->setText(xdb1);
		
	mainlayout = new QVBoxLayout;
	
	mainlayout->addLayout(upper);
	mainlayout->setSpacing(3);
	mainlayout->setMargin(10);

	auswertung = new QPushButton(tr("&Vorkalkulation erzeugen..."));
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
	
	connect(auswertung, SIGNAL(clicked()), this, SLOT(auswerten()));
	connect(abbruch, SIGNAL(clicked()), this, SLOT(abbrechen()));
	show();
}

void Kaufhof::init_layout(QString text, QHBoxLayout *&layout, QLabel *&label, QLineEdit *&db, QPushButton *&seldb)
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

void Kaufhof::select_db1()
{
	select_db(db1);
}


void Kaufhof::select_db(QLineEdit *entry)
{
	QString path = get_dbpath(entry->text());
	path.replace('/', '\\');
	if (path.length())
		entry->setText(path);
}

QString Kaufhof::get_dbpath(const QString &file) const
{
	QFileDialog f(0, "Datenbankauswahl",  ".", "*.mdb");
	f.selectFile(file);
	return f.getOpenFileName( );
}

void Kaufhof::auswerten()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
	QString con;
	QTextStream(&con) << "DRIVER=Microsoft Access-Treiber (*.mdb);DSN='';DBQ=" << db1->text();

	db.setDatabaseName(con);

	if (!db.open()) 
	{
        QMessageBox::critical(0, tr("Öffnen der Datenbank gescheitert!"),
            tr("Es konnte keine Verbindung zur ausgewählten Datenbank erstellt werden.\n"
					"Bitte die Pfadangaben überprüfen.\n"), QMessageBox::Cancel,
                     QMessageBox::NoButton);
        return;
    }

	QSqlQuery xquery(db);
    xquery.prepare("Select Count(*) from PreislistenArtikel WHERE Liste = 'K_KFRV'");
    xquery.exec();
	xquery.first();
	int count = xquery.value(0).toInt();

	QSqlQuery query("Select code, gruppe, liste, Preis, Montagezeit, WM, WL from PreislistenArtikel WHERE Liste = 'K_KFRV'", db);
	query.next();
	
	QProgressDialog progress( "Kaufhof...", "&Abbrechen", 0, count, this );
	progress.setMinimumDuration(100);
	progress.setMinimumWidth(500); 

	string lastgroup, lastlist;
	int entries = 0;
	for ( unsigned int i = 0; i < count && query.next(); i++ ) 
	{
		QString code = query.value(0).toString();
		QString gruppe = query.value(1).toString();
		QString liste = query.value(2).toString();
		double preis = query.value(3).toDouble();
		double zeit = query.value(4).toDouble();
		double wm = query.value(5).toDouble();
		double wl = query.value(6).toDouble();

		double vk = preis + preis*wm/100.0 + zeit*28.0 + zeit*28.0*wl/100.0;
		double vkneu = vk * 1.03;

		update(db, vkneu, code);
		/*
		QString sql;
		QString p;
		p.setNum(vk);
		sql = (QString)"Update PreislistenArtikel set Bemerkung ='" + p + (QString)"' WHERE code = '" + code + (QString)"'";
		QSqlQuery update(db);
		update.exec(sql);
		*/
		progress.setValue(i);
		QString labeltxt;
		QTextStream(&labeltxt) << liste << "/" << gruppe << "/" << code;
		progress.setLabelText(labeltxt);
		mainapp->processEvents();
	    if ( progress.wasCanceled() )
			break;
	}
	db.close();
}

void Kaufhof::update(QSqlDatabase& db, double vk, QString code)
{
	QString sql1, sql2, vkvk;
	vkvk.setNum(vk);
	
	sql1 = (QString)"Insert into verkaufspreise (code) values ('" + code + (QString)"')";
    sql2 = (QString)"Update verkaufspreise set preis = " + vkvk + (QString)" WHERE code = '" + code + (QString)"'";
	QSqlQuery update(db);
	update.exec(sql1);
	update.exec(sql2);
}

void Kaufhof::abbrechen()
{
	QMessageBox box("Abbruch", "Programm wirklich abbrechen?", QMessageBox::Information, 
		QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
	if ( box.exec() == QMessageBox::Ok )
	{
		emit done(1);
	}

}