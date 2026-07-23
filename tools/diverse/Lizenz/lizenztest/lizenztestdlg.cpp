#include "stdafx.h"
#include "lizenztest.h"
#include <licensegenerator.h>
#include "lizenztestdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(LizenztestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_KEYGEN, on_keygen)
	ON_BN_CLICKED(IDC_KEYRESOLVE, on_keyresolve)
END_MESSAGE_MAP()



LizenztestDlg::LizenztestDlg(CWnd* pParent)	: CDialog(IDD_LIZENZTEST_DIALOG, pParent)
	, mAblaufdatum(COleDateTime::GetCurrentTime()), mBegin(COleDateTime::GetCurrentTime())
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void LizenztestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_MonthCalCtrl(pDX, IDC_DATUM_ABLAUF, mAblaufdatum);
	DDX_MonthCalCtrl(pDX, IDC_DATUM_BEGIN, mBegin);
}




BOOL LizenztestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	// TODO: Hier zusätzliche Initialisierung einfügen
	
	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void LizenztestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR LizenztestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void LizenztestDlg::on_keygen()
{
	UpdateData(true);
	LicenseGenerator gen;

	CString hd;
	GetDlgItem(IDC_HDKENNUNG)->GetWindowText(hd);
	if (hd.GetLength() != 8)
	{
		AfxMessageBox("Die Hardwarekennung muss eine Länge von 8 Zeichen haben", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	CString testchar = hd.SpanIncluding("0123456789abcdefABCDEF");
	if (testchar.GetLength() != 8)
	{
		AfxMessageBox("Die Hardwarekennung darf nur folgende Zeichen enthalten:\n\"0123456789abcdefABCDEF\"", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	gen.initkey((LPCSTR)hd, (unsigned int)mBegin.m_dt, (unsigned int)mAblaufdatum.m_dt, LicenseGenerator::WAX, LicenseGenerator::NORMALKEY);
	GetDlgItem(IDC_KEY)->SetWindowText(gen.get_key().c_str());
}

void LizenztestDlg::on_keyresolve()
{
	CString key;
	GetDlgItem(IDC_KEY)->GetWindowText(key);
	
	LicenseGenerator gen;
	gen.set_key((LPCSTR)key);
	gen.decode_key();
	mBegin.m_dt = gen.get_begin();
	mAblaufdatum.m_dt = gen.get_expiration();
	UpdateData(false);
	GetDlgItem(IDC_HDKENNUNG)->SetWindowText(gen.get_hdkennung().c_str());
	AfxMessageBox(gen.get_crcok() ? "Checksumme ok" : "Fehlerhafte Checksumme, ungültiger Key!!", MB_OK | MB_ICONEXCLAMATION);
}