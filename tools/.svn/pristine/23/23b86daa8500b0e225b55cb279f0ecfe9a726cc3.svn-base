#include "stdafx.h"
#include "lizenz.rc.h"
#include "Lizenzeingabe.h"
#include <algorithm>
#include <sstream>

BEGIN_MESSAGE_MAP(Lizenzeingabe, CDialog)
	ON_BN_CLICKED(IDC_CHECKKEY, on_checkkey)
END_MESSAGE_MAP()


Lizenzeingabe::Lizenzeingabe(CWnd* pParent, CWinApp *app)	: CDialog(IDD_LICENCE, pParent)
{
	mApp = app;
	mHwkennung = get_hwkennung();
	mKey = load_key();
	validate();
}

Lizenzeingabe::~Lizenzeingabe()
{
}

bool Lizenzeingabe::is_valid() const
{
	return mValid;
}

void Lizenzeingabe::set_valid(bool val)
{
	mValid = val;
}
void Lizenzeingabe::validate()
{ 
	lic.set_hdkennung(mHwkennung);
	lic.set_key(mKey);
	lic.decode_key();
	set_valid(lic.get_crcok());
}


void Lizenzeingabe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	CString tmp1(mHwkennung.c_str()), tmp2(mKey.c_str());
	DDX_Text(pDX, IDC_HWKENNUNG, tmp1);
	DDX_Text(pDX, IDC_LIZENZ, tmp2);
	mHwkennung = (LPCSTR)tmp1;
	mKey = (LPCSTR)tmp2;
	validate();

	if (is_valid())
	{
		COleDateTime tm;
		tm.m_dt = lic.get_begin();
		SetDlgItemText(IDC_GUELTIGVON, tm.Format(VAR_DATEVALUEONLY));
		tm.m_dt = lic.get_expiration();
		SetDlgItemText(IDC_GUELTIGBIS, tm.Format(VAR_DATEVALUEONLY));
		((CButton*)GetDlgItem(IDC_DEMO))->SetCheck(lic.get_testkey());
		set_valid(true);
	}
	else
	{
		if (mKey.length())
			AfxMessageBox("Ungültige Lizenz");
		SetDlgItemText(IDC_GUELTIGVON, "???");
		SetDlgItemText(IDC_GUELTIGBIS, "???");
		((CButton*)GetDlgItem(IDC_DEMO))->SetCheck(false);
		set_valid(false);
	}
}

BOOL Lizenzeingabe::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;
	return true;
}


const string Lizenzeingabe::get_hwkennung() const 
{
	char VolumeNameBuffer[512];
	DWORD VolumeNameSize = 512;
	DWORD VolumeSerialNumber = 512;
	DWORD MaximumComponentLength;
	DWORD FileSystemFlags;
	char FileSystemNameBuffer[512];

	string laufwerk("C:\\");

	GetVolumeInformation( laufwerk.c_str(),        // address of root directory of the 
                                 // file system
		VolumeNameBuffer,     // address of name of the volume
		VolumeNameSize,         // length of lpVolumeNameBuffer
		&VolumeSerialNumber,  // address of volume serial number
		&MaximumComponentLength,// address of system's maximum filename length
		&FileSystemFlags,     // address of file system flags
		FileSystemNameBuffer, // address of name of file system
		512);

	char key[10];
	sprintf(key, "%x", VolumeSerialNumber);
	return key;
}

const string Lizenzeingabe::load_key() const
{
	return  (LPCSTR)mApp->GetProfileString("licence", "key");
}

void Lizenzeingabe::on_checkkey()
{
	UpdateData();
	if (is_valid())
		mApp->WriteProfileString("licence", "key", mKey.c_str());
}
