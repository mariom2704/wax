	
		#include "LizenzGui.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <Lizenz/Key.h>

#include <Lizenz/Decoder.h>
#include <Lizenz/Encoder.h>
#include <Lizenz/DateUtil.h>
#include <Lizenz/Harddisk.h>
#include <Lizenz/Validator.h>

using namespace License;

#define CBINDEX_MXCALC 0
#define CBINDEX_WAX 1

#define WAX 1
#define MxDesigner 2


#define WAX_MxDesigner 3

LizenzGui::LizenzGui(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowTitle(QApplication::translate("LizenzGuiClass", "LizenzGui 2.1", 0));
	init();
}

void LizenzGui::init()
{
	ui.pushButton_write_license->setEnabled(false);

	QIcon ike = QIcon(QString::fromUtf8(":/LizenzGui/Resources/argotec.png"));
	ui.comboBox_programm->addItem(ike, "MX DesignManager");
	QIcon ike2 = QIcon(QString::fromUtf8(":/LizenzGui/Resources/wax.png"));
	ui.comboBox_programm->addItem(ike2, "Wax");

	Registry myReg;

	myReg.open();
	LicenceMark = myReg.RunLicence();
	LicenceMark = 1;
	bExit = false;
	if(LicenceMark == -1)
	{
		QMessageBox::information(this, "Lizenz", "Sie haben keine Berechtigung\ndieses Programm zu benutzen");
		ui.pushButton_write_license->setEnabled(false);
		bExit = true;
		return;
	}

	bool bAlle = true;
	if( LicenceMark & WAX )
	{
		ui.comboBox_programm->setCurrentIndex(CBINDEX_WAX);
		on_program_changed("Wax");
		ui.comboBox_programm->setEnabled(false);
		bAlle = false;
	}

	if( LicenceMark & MxDesigner)
	{
		ui.comboBox_programm->setCurrentIndex(CBINDEX_MXCALC);
		on_program_changed("MX DesignManager");
		ui.comboBox_programm->setEnabled(false);
		bAlle = false;
	}

	if( bAlle )
	{
		ui.comboBox_programm->setEnabled(true);
	}
	
	connect_actions();
}

void LizenzGui::connect_actions(bool doconnect)
{
	if (doconnect)
	{
		connect(ui.pushButton_read_license, SIGNAL(clicked()), this, SLOT(read_file()));
		connect(ui.pushButton_write_license, SIGNAL(clicked()), this, SLOT(write_license_data()));
		
		connect(ui.comboBox_programm, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_program_changed(const QString&))); 
		connect(ui.comboBox_programm, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(create_license_data())); 

		connect(ui.checkBox_novec, SIGNAL(stateChanged(int)), this, SLOT(create_license_data())); 
		connect(ui.checkBox_hfc, SIGNAL(stateChanged(int)), this, SLOT(create_license_data())); 
		connect(ui.checkBox_argon, SIGNAL(stateChanged(int)), this, SLOT(create_license_data())); 
		connect(ui.checkBox_co2, SIGNAL(stateChanged(int)), this, SLOT(create_license_data())); 
		connect(ui.checkBox_ig55, SIGNAL(stateChanged(int)), this, SLOT(create_license_data())); 
		connect(ui.checkBox_ig541, SIGNAL(stateChanged(int)), this, SLOT(create_license_data())); 
		connect(ui.checkBox_stickstoff, SIGNAL(stateChanged(int)), this, SLOT(create_license_data())); 
		
		connect(ui.checkBox_autocad, SIGNAL(stateChanged(int)), this, SLOT(create_license_data())); 

		connect(ui.calendarWidget_begin, SIGNAL(selectionChanged()), this, SLOT(create_license_data())); 
		connect(ui.calendarWidget_end, SIGNAL(selectionChanged()), this, SLOT(create_license_data())); 

		connect(ui.lineEdit_hwid, SIGNAL(textChanged(const QString)), this, SLOT(create_license_data())); 
	}
	else
	{
		disconnect(ui.pushButton_read_license, SIGNAL(clicked()), this, SLOT(read_file()));
		disconnect(ui.pushButton_write_license, SIGNAL(clicked()), this, SLOT(write_license_data()));
		
		disconnect(ui.comboBox_programm, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_program_changed(const QString&))); 
		disconnect(ui.comboBox_programm, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(create_license_data())); 

		disconnect(ui.checkBox_novec, SIGNAL(stateChanged(int)), this, SLOT(create_license_data())); 
		disconnect(ui.checkBox_hfc, SIGNAL(stateChanged(int)), this, SLOT(create_license_data())); 
		disconnect(ui.checkBox_argon, SIGNAL(stateChanged(int)), this, SLOT(create_license_data())); 
		disconnect(ui.checkBox_co2, SIGNAL(stateChanged(int)), this, SLOT(create_license_data())); 
		disconnect(ui.checkBox_ig55, SIGNAL(stateChanged(int)), this, SLOT(create_license_data())); 
		disconnect(ui.checkBox_ig541, SIGNAL(stateChanged(int)), this, SLOT(create_license_data())); 
		disconnect(ui.checkBox_autocad, SIGNAL(stateChanged(int)), this, SLOT(create_license_data())); 

		disconnect(ui.calendarWidget_begin, SIGNAL(selectionChanged()), this, SLOT(create_license_data())); 
		disconnect(ui.calendarWidget_end, SIGNAL(selectionChanged()), this, SLOT(create_license_data())); 

		disconnect(ui.lineEdit_hwid, SIGNAL(textChanged(const QString)), this, SLOT(create_license_data())); 
	}
}

void LizenzGui::read_file()
{
	QString strTemp;
	if( LicenceMark == '1')
	{
		strTemp =  "Lizenzdateien (*.waxlic)";
	}

	if( LicenceMark == '2')
	{
		strTemp =  "Lizenzdateien (*.mxlic)";
	}
	QString fn = QFileDialog::getOpenFileName( this, tr("Lizenzfile laden"), 
		"", strTemp);

	if (!fn.length())
		return;

	filename = fn;

	QFile f(fn);
	f.open(QIODevice::ReadOnly);
	QTextStream txtstr(&f);

	QString line;
	content.clear();
	do 
	{
		line = txtstr.readLine();
		content.push_back(line); 
	}
	while (line != QString::Null());
	ui.pushButton_write_license->setEnabled(true);
	this->setWindowTitle( "LizenzGui - " + filename );
	ui.lineEdit_hwid->setReadOnly(true);

	// Wenn im File ein Lizenzkey eingetragen ist, soll dieser auch erstmal angezeigt werden
	if (!create_key_from_keyid())
		init_key_from_filedata();

	show_key_data(); // Lizenzdaten anzeigen
	
}

void LizenzGui::write_license_data()
{
	write_file();
}

void LizenzGui::read_key_data()
{
}

/**
Versucht die Keydaten aus der Schlüssel ID in der Datei zu erzeugen. Der Schlüssel enthält alle
wesentlichen Daten, alle anderen Daten im File werden ignoriert. 
Können die Keydaten nicht erzeugt werden, weil z.B. die Schlüssel ID falsch ist oder fehlt, 
wird der Inhalt der Datei gescannt und die vorhandenen Werte werden gesetzt. 
Dies sollte der Normalfall sein, wenn eine neue Lizenz angefordert wird. Dann sollte in der 
Lizenzdatei nur die HWID stehen.
*/
bool LizenzGui::create_key_from_keyid()
{
	const QString lictag("License:");

	key.set_key("");
	for (QList<QString>::iterator it = content.begin(); it != content.end(); it++)
	{
		if (!it->contains(lictag))
			continue;
		
		key.set_key(it->mid(lictag.length()).trimmed().toStdString());
		break;
	}
	if (!key.get_key().length())
		return false;

	if (!Decoder(key).run())
		return false;

	if (Validator(key).get_state() != Validator::NotValid)
		return true;
	
	return false;
}

void LizenzGui::init_key_from_filedata()
{
	const QString lictag("License:");
	key.set_key("");
	key.set_date_of_begin(DateUtil().get_current());
	key.set_date_of_expiration(DateUtil().get_current());
	for (QList<QString>::iterator it = content.begin(); it != content.end(); it++)
	{
		if (it->contains("HWID:"))
		{
			key.set_hwtag(it->mid(5).trimmed().toStdString());		
			continue;
		}
		
		if (it->contains("valid from:"))
		{
			QString dt = it->mid(11).trimmed();
			key.set_date_of_begin(DateUtil().from_iso(dt.toStdString()));
			continue;
		}

		if (it->contains("valid until:"))
		{
			QString dt = it->mid(12).trimmed();
			key.set_date_of_expiration(DateUtil().from_iso(dt.toStdString()));
			continue;
		}

		if (it->contains("Module ID:"))
		{
			QString mod = it->mid(10).trimmed();
			key.set_moduletag(mod.toInt());
			continue;
		}

		if (it->contains("Program ID:"))
		{
			QString prog = it->mid(11).trimmed();
			key.set_programtag(prog.toInt());
			continue;
		}
	}

}


void LizenzGui::show_key_data()
{
	connect_actions(false);
	
	ui.lineEdit_hwid->setText(key.get_hwtag().c_str());
	ui.lineEdit_key->setText(key.get_key().c_str());

	if (key.get_programtag() == Key::Wax)
	{
		ui.comboBox_programm->setCurrentIndex(CBINDEX_WAX);
		on_program_changed("Wax");
	}

	else if (key.get_programtag() == Key::MxDesignManager)
	{
		ui.comboBox_programm->setCurrentIndex(CBINDEX_MXCALC);
		on_program_changed("MxCalc");
	}

	int mod = key.get_moduletag();

	ui.checkBox_novec->setChecked(mod & Key::MX1230);
	ui.checkBox_hfc->setChecked(mod & Key::MX200);
	ui.checkBox_argon->setChecked(mod & Key::Argon);
	ui.checkBox_co2->setChecked(mod & Key::Co2);
	ui.checkBox_ig55->setChecked(mod & Key::IG55);
	ui.checkBox_ig541->setChecked(mod & Key::IG541);
	ui.checkBox_autocad->setChecked(mod & Key::AutocadExtension);


	std::string isodate = DateUtil(key.get_date_of_begin()).format_as_iso();
	QDate dt = QDate::fromString(isodate.c_str(), "yyyy-MM-dd");
	ui.calendarWidget_begin->setSelectedDate(dt);

	isodate = DateUtil(key.get_date_of_expiration()).format_as_iso();
	dt = QDate::fromString(isodate.c_str(), "yyyy-MM-dd");
	ui.calendarWidget_end->setSelectedDate(dt);

	connect_actions();
}


void LizenzGui::create_license_data()
{
	ui.pushButton_write_license->setEnabled(false);
	if (ui.lineEdit_hwid->text().length() != 8)
		return;

	const QString isoformat("yyyy-MM-dd");
	int mod = 0;

	key.set_hwtag(ui.lineEdit_hwid->text().toStdString());
	
	if (ui.comboBox_programm->currentIndex() == CBINDEX_WAX)
		key.set_programtag(Key::Wax);
	else if (ui.comboBox_programm->currentIndex() == CBINDEX_MXCALC)
	{
		key.set_programtag(Key::MxDesignManager);
		if (ui.checkBox_novec->isChecked()) 
			mod |= Key::MX1230;
		if (ui.checkBox_hfc->isChecked())
			mod |= Key::MX200;
		if (ui.checkBox_argon->isChecked())
			mod |= Key::Argon;
		if (ui.checkBox_co2->isChecked())
			mod |= Key::Co2;
		if (ui.checkBox_ig55->isChecked())
			mod |= Key::IG55;
		if (ui.checkBox_ig541->isChecked())
			mod |= Key::IG541;
		if (ui.checkBox_stickstoff->isChecked())
			mod |= Key::Stickstoff;
		
		if (ui.checkBox_autocad->isChecked())
			mod |= Key::AutocadExtension;
	}

	key.set_moduletag(mod);

	std::string isodate = ui.calendarWidget_begin->selectedDate().toString(isoformat).toStdString();
	key.set_date_of_begin(DateUtil().from_iso(isodate));

	isodate = ui.calendarWidget_end->selectedDate().toString(isoformat).toStdString();
	key.set_date_of_expiration(DateUtil().from_iso(isodate));

	Encoder(key).run();

	if (Validator(key).get_state() == Validator::NotValid)
	{
		QMessageBox::information(this, "Lizenz", "Lizenzdaten ungültig");
		ui.pushButton_write_license->setEnabled(false);
	}
	else
	{
		ui.pushButton_write_license->setEnabled(true);
		ui.lineEdit_key->setText(key.get_key().c_str());
	}



}


void LizenzGui::write_file()
{
	QString strTemp;
	if( LicenceMark == '1')
	{
		strTemp =  "Lizenzdateien (*.mxlic)";
	}

	if( LicenceMark == '1')
	{
		strTemp =  "Lizenzdateien (*.waxlic)";
	}
	QString fn = QFileDialog::getSaveFileName( this, tr("Lizenzfile speichern"), 
		filename, strTemp);

	if (!fn.length())
		return;


	QFile f(fn);
	f.open(QIODevice::WriteOnly | QIODevice::Truncate);
	QTextStream txtstr(&f);

	QString skiptags[] = 
	{
		"Program ID:", 
		"Module ID:", 
		"valid from:",
		"valid until:",
		"License:",
		"HWID:",
		"",
	};


	for (QList<QString>::iterator it = content.begin(); it != content.end(); it++)
	{
		bool skip = false;
		for (int i = 0; skiptags[i].length(); i++)
		{
			if (it->contains(skiptags[i]))
			{
				skip = true;
				break;
			}
		}
		if (skip)
			continue;
		txtstr << *it << "\r\n";	
	}

	// Vom Programm erzeugte Zeilen
	QList<QString> license_tags;
	
	license_tags.push_back(QString("Program ID: %1").arg(key.get_programtag()) );
	license_tags.push_back(QString("Module ID: %1").arg(key.get_moduletag()) );
	license_tags.push_back(QString("valid from: %1").arg(DateUtil(key.get_date_of_begin()).format_as_iso().c_str()) );
	license_tags.push_back(QString("valid until: %1").arg(DateUtil(key.get_date_of_expiration()).format_as_iso().c_str()) );
	license_tags.push_back(QString("HWID: %1").arg(key.get_hwtag().c_str()) );
	license_tags.push_back(QString("License: %1").arg(key.get_key().c_str()) );
	
	for (QList<QString>::iterator it = license_tags.begin(); it != license_tags.end(); it++)
	{
		txtstr << *it << "\r\n";	
	}
}

void LizenzGui::on_program_changed(const QString& prog)
{
	QCheckBox* chk[] = 
	{
		ui.checkBox_novec,
		ui.checkBox_hfc,
		ui.checkBox_argon,
		ui.checkBox_co2,
		ui.checkBox_stickstoff,
		ui.checkBox_ig55,
		ui.checkBox_ig541,
		ui.checkBox_autocad,
		NULL,
	};

	if (prog == "Wax")
	{
		for (int i = 0; chk[i] != NULL; i++)
			chk[i]->setEnabled(false);
	}
	else 
	{
		for (int i = 0; chk[i] != NULL; i++)
			chk[i]->setEnabled(true);
	}
}