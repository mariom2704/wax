#include "stdafx.h"
#include "lizenz.rc.h"
#include <algorithm>
#include <sstream>



#include "../rohrkonst/mxdate.h"


		#include "Lizenzeingabe.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDC_SHOWERROR (WM_USER+1234)

BEGIN_MESSAGE_MAP(Lizenzeingabe, CDialog)
	ON_BN_CLICKED(ID_READ_LICENCE_FILE, on_readLicenceFile)
	ON_BN_CLICKED(ID_CREATE_LICENCE_FILE, OnBnClickedCreateLiceneceFile)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


Lizenzeingabe::Lizenzeingabe(CWnd* pParent,  const CWinApp *app)	: CDialog(IDD_LICENCE, pParent), mApp(app)
{
	m_pParent = pParent;
	WaxRegKey.Empty();
	WaxRegKey  = load_key();
	bRegkeyOk = false;
	if(!WaxRegKey.IsEmpty())
	{
		FileContent.push_back(CString( "License: " + WaxRegKey));
		if(create_key_from_keyid())
		{
			bRegkeyOk = true;
		}
	}
}


void Lizenzeingabe::readfile()
{

	CFileDialog fo(TRUE, NULL, NULL, 
			OFN_EXPLORER|OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR , 
			"Wax Licence File (*.waxlic)|*.waxlic||", this);

	TCHAR path[_MAX_PATH];
	GetModuleFileName(0, path, _MAX_PATH);
	CString strPath = path;
	int iWhere = strPath.ReverseFind('\\');
	CString pfadDefault = strPath.Mid(0, iWhere);

	
	fo.m_ofn.lpstrInitialDir = pfadDefault;
	fo.m_ofn.lpfnHook=NULL;
	fo.m_ofn.Flags &= ~OFN_ENABLEHOOK;
	fo.m_ofn.hwndOwner = this->m_hWnd;

	
	if(GetOpenFileName(&fo.m_ofn))
	{

		ifstream is(fo.GetPathName());
		FileContent.clear();
		while (!is.eof())
		{
			char line[255];
			is.getline(line, 255);
			FileContent.push_back(CString(line));
		}
		create_key_from_keyid();
	}
	else
	{
		 StateLic = License::Validator::NotValid;
	}
}



bool Lizenzeingabe::create_key_from_keyid()
{
	WaxRegKey.Empty();
	const CString lictag("License:");

	key.set_key("");
	for (vector<CString>::iterator it = FileContent.begin(); it != FileContent.end(); it++)
	{
		if (it->Find(lictag) == -1)
			continue;
		
		string strTemp  = it->Mid(lictag.GetLength()).Trim();
		key.set_key(strTemp);
		break;
	}
	
	if (!key.get_key().length())
		return false;

	if (!License::Decoder(key).run())
		return false;

	StateLic = License::Validator(key).get_state();
	if(StateLic != License::Validator::NotValid)
	{
		unsigned int acttime = License::DateUtil().get_current();
		if (acttime < key.get_date_of_begin() || acttime > key.get_date_of_expiration())
		{
			StateLic = License::Validator::TimedOut;
			return false;
		}	
	}
	WaxRegKey = "Ok";

	time_t  exptime = key.get_date_of_expiration()*100;
	time_t  acttime = License::DateUtil().get_current()*100;

	COleDateTimeSpan tSpan = COleDateTime(exptime) - COleDateTime(acttime);

	LONG daysleft =  tSpan.GetTotalDays();
	if( 30 > tSpan.GetDays())
	{
		CString msg;
		msg.Format(getStammdaten().getText("IDS_TIME_LEFT").c_str(), daysleft);
		AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
	}
	return true;
}


Lizenzeingabe::~Lizenzeingabe()
{}



bool Lizenzeingabe::is_valid() const
{
	
	return (StateLic == License::Validator::Valid && !WaxRegKey.IsEmpty()) || bRegkeyOk  ? true : false;
}



void Lizenzeingabe::remove_license()
{
	/**
	Entfernt die Lizenz aus der Registry
	*/
	(const_cast<CWinApp*>(mApp))->WriteProfileString("licence", "key", "");
}

void Lizenzeingabe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	if (!pDX->m_bSaveAndValidate)
	{
		CString tmp1(key.get_hwtag().c_str()),
			tmp2(key.get_key().c_str());
		DDX_Text(pDX, IDC_HWKENNUNG, tmp1);
		DDX_Text(pDX, IDC_LIZENZ, tmp2);
		;//validate();
	}

	if (is_valid())
	{	
		SetDlgItemText(IDC_GUELTIGVON, License::DateUtil(key.get_date_of_begin()).format_as_iso().c_str());
		SetDlgItemText(IDC_GUELTIGBIS, License::DateUtil(key.get_date_of_expiration()).format_as_iso().c_str());
	}
	else
	{
		SetDlgItemText(IDC_GUELTIGVON, "???");
		SetDlgItemText(IDC_GUELTIGBIS, "???");
		CString strTemp;
	strTemp = getStammdaten().getText("IDC_STATIC_FAULT_TEXT").c_str();
	GetDlgItem(IDC_STATIC_FAULT_TEXT)->SetWindowText(strTemp);
	}
}



BOOL Lizenzeingabe::OnInitDialog()
{	

	if (!CDialog::OnInitDialog())
		return false;


	this->SetWindowTextA(getStammdaten().getText("IDD_LICENCE").c_str());


	setWindowsText(IDC_STATIC_LIC_EINGABE, "IDC_STATIC_LIC_EINGABE");
	setWindowsTextWithColon(IDC_STATIC_SYSTEMKENNUNG, "IDC_STATIC_SYSTEMKENNUNG");
	setWindowsTextWithColon(IDC_STATIC_LICKEY, "IDC_STATIC_LICKEY");
	setWindowsText(IDC_STATIC_LIC_INFO, "IDC_STATIC_LIC_INFO");
	setWindowsTextWithColon(IDC_STATIC_LIC_VON, "IDC_STATIC_LIC_VON");
	setWindowsTextWithColon(IDC_STATIC_LIC_BIS, "IDC_STATIC_LIC_BIS");
	

	return true;
}



void Lizenzeingabe::setWindowsText(int id, const string& txtId)
{
	GetDlgItem(id)->SetWindowText(getStammdaten().getText(txtId).c_str());
}

void Lizenzeingabe::setWindowsTextWithColon(int id, const string& txtId)
{
	GetDlgItem(id)->SetWindowText(getStammdaten().getTextWithColon(txtId).c_str());
}



const CString Lizenzeingabe::load_key() const
{
	/**
	Lädt den Lizenzkey aus der Registry. Bei Wax ist das HKEY_LOCAL_MACHINE
	Software/Minimax/wax/license
	*/
	char key[255];
	::GetPrivateProfileString("wax", "license", "", key, 255, "wax.ini");
	string tmpkey = key;
	if (tmpkey.length() != 36)
	{
		HKEY hKey;
		LONG lret;
		DWORD reservedZero = 0;
		CString SubKeyName = L"SOFTWARE\\Minimax\\Wax+";
		CString ValueFound;
			lret = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					  SubKeyName,
					  reservedZero,
					  KEY_READ,
					  &hKey);

		if (lret == ERROR_SUCCESS)
		{
			
			
			DWORD datelen = 255;
			DWORD type = REG_SZ;
			::RegQueryValueEx(hKey, "license", 0, &type, (BYTE*)key, &datelen);
			::RegCloseKey(hKey);
		}	
	}

	return key;
}


void Lizenzeingabe::on_readLicenceFile()
{
	readfile();
	if(is_valid())
	{
		HKEY hKey;
        DWORD dwDisp = 0;
        LPDWORD lpdwDisp = &dwDisp;
        CString l_strExampleKey = "SOFTWARE\\Minimax\\Wax+";

		LONG iSuccess = RegOpenKeyEx(HKEY_LOCAL_MACHINE, l_strExampleKey,0, KEY_WRITE, &hKey);
 
		if (iSuccess != ERROR_SUCCESS)
		{
			 RegCreateKeyEx( HKEY_LOCAL_MACHINE, l_strExampleKey, 0L,NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,lpdwDisp);
       	}
		l_strExampleKey = key.get_key().c_str();
		RegSetValueEx(hKey,"license",0,REG_SZ,(unsigned char*)l_strExampleKey.GetBuffer(0),l_strExampleKey.GetLength());
		RegCloseKey(hKey);
		SendMessage(WM_COMMAND, IDOK);
	
	}

}


void Lizenzeingabe::OnBnClickedCreateLiceneceFile()
{	
	key.set_programtag(License::Key::Wax);
	key.set_hwtag(get_hwkennung());


	TCHAR szFilters[]= _T("Wax Licence File (*.waxlic)|*.waxlic||");

    // Create an Open dialog; the default file name extension is ".my".
    CFileDialog fo(FALSE, _T("waxlic"), _T("*.waxlic"),
      OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
  

    TCHAR path[_MAX_PATH];
	GetModuleFileName(0, path, _MAX_PATH);
	CString strPath = path;
	int iWhere = strPath.ReverseFind('\\');
	CString pfadDefault = strPath.Mid(0, iWhere);

	
	fo.m_ofn.lpstrInitialDir = pfadDefault;
	fo.m_ofn.lpfnHook=NULL;
	fo.m_ofn.Flags &= ~OFN_ENABLEHOOK;
	fo.m_ofn.hwndOwner = this->m_hWnd;

	
	
	CString pathName;
	if(GetSaveFileName(&fo.m_ofn))
	{
	 pathName = fo.GetPathName();
	}
	if (!pathName.GetLength())
		return;

	std::ofstream ofs (pathName, std::ofstream::out);

	// Vom Programm erzeugte Zeilen
	vector<CString> license_tags;
	
	CString temp = "Program ID: ";
	temp.Format("Program ID: %i", key.get_programtag());
	license_tags.push_back(temp);
	license_tags.push_back(CString("HWID: ")+ CString(key.get_hwtag().c_str()) );
	
	
	for (vector<CString>::iterator it = license_tags.begin(); it != license_tags.end(); it++)
	{
		ofs << *it << "\r\n";	
	}
	ofs.close();
	CString msg;
	msg = CString(getStammdaten().getText("IDS_WAX_LIC_FILE").c_str()) + CString ("\r\n") + pathName ;
	AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);	

}


const string Lizenzeingabe::get_hwkennung() const 
{
	/**
	Liest die Kennung der Partition c:\ aus, diese dient zur Erzeugung eines Lizenzschlüssels bzw. 
	zur Prüfung, ob der richtige Lizenzschlüssel eingegeben wurde
	*/
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

	char key[1024];
	sprintf(key, "%x", VolumeSerialNumber);
	string ret = key;
	while (ret.length() < 8)
		ret += "0";
	if (ret.length() > 8)
		ret = ret.substr(0, 8);

	return ret;
}


HBRUSH Lizenzeingabe::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if( pWnd->GetDlgCtrlID() == ID_FAULT_TEXT )
	{
		pDC->SetTextColor(RGB(255,0,0));
	}
	return hbr;
}
