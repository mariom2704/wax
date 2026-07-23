// ImportSAPToWAX.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "wax.h"
#include "resource.h"
#include "mxodbc\mxdatabase.h"
#include "mxodbc\DBConnection.h"
#include "..\waxprj\JScriptDruck.h"
#include <io.h>

	#include "ImportSAPToWAX.h"






IMPLEMENT_DYNAMIC(CImportSAPToWAX, CDialog)

CImportSAPToWAX::CImportSAPToWAX(CWnd* pParent )
	: CDialog(CImportSAPToWAX::IDD, pParent), m_bImportAbbrechen(FALSE)
{
	m_bImported = FALSE;
}


CImportSAPToWAX::~CImportSAPToWAX()
{
}



void CImportSAPToWAX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SAP_TO_WAX_FILE, m_SapToWaxImportFile);
	DDX_Control(pDX, IDC_STATIC_STAMM_DB, m_PlaceForStammDB);
	DDX_Control(pDX, IDC_BUTTON_IMPORT_ABBRECHEN, m_ImportAbbrechen);
	DDX_Control(pDX, IDC_BUTTON_SAP_IMPORTIEREN, m_ImportStarten);
	DDX_Control(pDX, IDOK, m_Abort);
	DDX_Control(pDX, IDC_PROGRESS_IMPORT, m_ProgressControl);
}



BEGIN_MESSAGE_MAP(CImportSAPToWAX, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAP_IMPORTIEREN, &CImportSAPToWAX::OnBnClickedSapDatenImportieren)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_ABBRECHEN, &CImportSAPToWAX::OnBnClickedButtonImportAbbrechen)
END_MESSAGE_MAP()



BOOL CImportSAPToWAX::OnInitDialog()
{
	CDialog::OnInitDialog();
		
	m_strPlaceForStammDB = (LPCSTR)WaxSys::para_str(STAMMDB);
	m_PlaceForStammDB.SetWindowText(m_strPlaceForStammDB);

	CString strAppPath = CJScriptPrinting::GetApplicationPath().c_str();
	m_strSapToWaxImportFile = strAppPath + _T("\\sap\\wax_mat_stamm.txt");

	int iWhere = m_strSapToWaxImportFile.ReverseFind('\\');
	m_strLogFile = m_strSapToWaxImportFile.Mid(0, iWhere);
	m_strLogFile = m_strLogFile + _T("\\SAPtoWax.log");
		  
	m_SapToWaxImportFile.SetWindowText(m_strSapToWaxImportFile);
	m_iCountUpdates = 0;
	m_iCountInsert = 0;

	TRY
	{
		CFile::Remove(m_strLogFile);
	}
	CATCH(CException, e)
	{

	}
	END_CATCH
	return TRUE;  
}



void CImportSAPToWAX::OnBnClickedSapDatenImportieren()
{
	m_bImportAbbrechen = FALSE;
	m_ImportAbbrechen.EnableWindow(TRUE);
	m_Abort.EnableWindow(FALSE);
	m_ImportStarten.EnableWindow(FALSE);
	m_bImported = TRUE;
	::AfxBeginThread(CImportSAPToWAX::ImportThread, this, THREAD_BASE_PRIORITY_MIN, 0, 0);
};



UINT CImportSAPToWAX::ImportThread(LPVOID pMasterProcess)
{
	CImportSAPToWAX* pImportSAPToWAX = (CImportSAPToWAX*)pMasterProcess;
	DBConnection connect;
	CString strConnection = WaxSys::getStammdb(&connect);
	MXDatabase db;
	db.doOpen(strConnection);	

	string strLine;
    ifstream ifile;
	CFileStatus FileStatus;
	int bExist = CFile::GetStatus(pImportSAPToWAX->m_strSapToWaxImportFile, FileStatus);

	CTime tCurrent = CTime::GetCurrentTime();
	tCurrent-= CTimeSpan( 2, 0, 0, 0 );
	ifstream infile;

	if(bExist && (FileStatus.m_mtime > tCurrent || FileStatus.m_mtime < tCurrent && AfxMessageBox(CString(_T("Die Importdatei ist älter als 2 Tage.\nSoll diese dennoch importiert werden?")), MB_ICONQUESTION | MB_YESNO) == IDYES))
	{
	

			ifile.open (pImportSAPToWAX->m_strSapToWaxImportFile);

			int zaehl = 0;
			while(getline(ifile,strLine)) 
			{
				zaehl++;
			}
			ifile.close();

			
			
			if(zaehl == 0)
			{
			
				AfxMessageBox(CString(_T("Die Importdatei ")) + pImportSAPToWAX->m_strSapToWaxImportFile + CString(_T(" enhält 0 Datensätze")), MB_OK);
			}
			else
			{
			
				infile.open (pImportSAPToWAX->m_strSapToWaxImportFile);

				pImportSAPToWAX->m_ProgressControl.SetRange32(0, zaehl);
				pImportSAPToWAX->m_ProgressControl.SetPos(0);
				pImportSAPToWAX->m_ProgressControl.SetStep(1);
				
				const int anzTabArtikelnummer = 0;
				const int anzTabBezeichnungDeutsch = 1;
				const int anzTabSAPPreis = 2;
				const int anzTabAnzahlEinheiten = 3;
				const int anzTabEinheit = 4;     
				const int anzTabGewicht = 6;
			//	const int anzTabStatus = 10;
				const int anzTabBezeichnungEnglisch = 13;

				TRY
				{
					CFile::Remove(pImportSAPToWAX->m_strLogFile);
				}
				CATCH(CException, e)
				{

				}
				END_CATCH

				BOOL bFindFaults = FALSE;
				CString strContent;
				while(getline(infile, strLine)) 
				{
					CString strLineforSplitt(strLine.c_str());
					strContent = strLineforSplitt;
					int iWo = 0;		 
					CStringArray RowVector;


					while((iWo = strLineforSplitt.Find(_T("\t"))) > -1)
					{	
						CString strSplitt = strLineforSplitt.Mid(0, iWo);
						strSplitt.Trim();

						if(strSplitt.GetLength() > 0)
						{
							RowVector.Add(strSplitt);
						}
						else
						{
							RowVector.Add(CString("0"));
						}
						
						strLineforSplitt = strLineforSplitt.Mid(iWo + 1, strLineforSplitt.GetLength() - iWo - 1);
					}
					
					if(strLineforSplitt.IsEmpty())
					{
						strLineforSplitt = RowVector[anzTabBezeichnungDeutsch];
					}
					RowVector.Add(strLineforSplitt);

					if(	RowVector.GetCount() < 14)
					{	
						bFindFaults = TRUE;
						DumpHtmlFile(pImportSAPToWAX->m_strLogFile, CString(_T(" Lese Fehler ")) + CString("\r\n") + strContent + CString("\r\n"), TRUE);
						continue;
					}

					RowVector[anzTabBezeichnungDeutsch].Replace('\'', ' ');
					RowVector[anzTabBezeichnungEnglisch].Replace('\'', ' ');

					RowVector[anzTabSAPPreis].Replace(',','.');
					RowVector[anzTabGewicht].Replace(',','.');

					CString strInsert;
					double dSAPPreisGsamt = (double)CStringConvert(RowVector[anzTabSAPPreis]);
					double dAnzahlEinheiten = (double)CStringConvert(RowVector[anzTabAnzahlEinheiten]);
					double dSAPPreisProVerbrauchsEinheit = dSAPPreisGsamt/dAnzahlEinheiten; // SAPPreis Pro VerbrauchsEinheit ist die Einheit der Minimax Verbrsauchsgroesse
					
					strInsert = "INSERT INTO artikelstamm (Artikelnummer, BezeichnungDeutsch, BezeichnungEnglisch, Einheit, Gewicht, SapPreis) VALUES( " 
										+ RowVector[anzTabArtikelnummer]+ CString(",'") 
										+ RowVector[anzTabBezeichnungDeutsch] + CString("','") 
										+ RowVector[anzTabBezeichnungEnglisch] + CString("','") 
										/*+ RowVector[anzTabStatus] + CString(",'")*/ 
										+ RowVector[anzTabEinheit] + CString("',") 
										+ RowVector[anzTabGewicht] + CString(",") 
										+ CStringConvert(dSAPPreisProVerbrauchsEinheit) + CString(")");
					BOOL bInsertCatched = FALSE;
					TRY
					{
						db.ExecuteSQL(strInsert);
					}

					CATCH(CDBException, e)
					{
						TCHAR szErr[1024];
						e->GetErrorMessage(szErr, 1024);
						bInsertCatched = TRUE;
						if(e->m_strStateNativeOrigin.Find(_T("State:23000")) == -1)
						{
							pImportSAPToWAX->m_iCountInsert++;
							bFindFaults = TRUE;
							DumpHtmlFile(pImportSAPToWAX->m_strLogFile, CString(_T(" Insert Fehler ")) + CString(szErr) + CString("\r\n") + strContent + CString("\r\n"), TRUE);
						}
					}
					END_CATCH

					if(bInsertCatched)
					{

					
						CString strUdate;			
						strUdate = "UPDATE artikelstamm	 SET Artikelnummer = " + RowVector[anzTabArtikelnummer]+ CString(",") +
														" BezeichnungDeutsch = '" + RowVector[anzTabBezeichnungDeutsch] + CString("',") +
														" BezeichnungEnglisch = '" + RowVector[anzTabBezeichnungEnglisch] + CString("',") +
														/*" Status = " + RowVector[anzTabStatus] + CString(",") + */
														" Einheit = '" + RowVector[anzTabEinheit] + CString("',") +
														" Gewicht = " + RowVector[anzTabGewicht] + CString(",") +
														" SapPreis = " + CStringConvert(dSAPPreisProVerbrauchsEinheit) + " WHERE Artikelnummer = " + RowVector[anzTabArtikelnummer];
						
						
						TRY
						{
							db.ExecuteSQL(strUdate);
						}
						CATCH_ALL(e)
						{
							TCHAR szErr[1024];
							e->GetErrorMessage(szErr, 1024);
							pImportSAPToWAX->m_iCountUpdates++;
							bFindFaults = TRUE;
							DumpHtmlFile(pImportSAPToWAX->m_strLogFile, CString(_T(" Update Fehler ")) + CString(szErr) + CString("\r\n") + strContent + CString("\r\n"), TRUE);
						}
						END_CATCH_ALL
						
					}
					pImportSAPToWAX->m_ProgressControl.StepIt();
					if(pImportSAPToWAX->m_bImportAbbrechen)
					{
						break;
					}
				}
				

				if(bFindFaults)
				{
					
					if(AfxMessageBox("Es sind Fehler aufgetreten während des Importes !!\nSollen diese angezeigt werden ?", MB_ICONQUESTION | MB_YESNO) == IDYES)
					{
						ShellExecute(NULL, _T("open"), _T("notepad.exe"), pImportSAPToWAX->m_strLogFile, NULL, SW_SHOW);
					}
				}

			}	
	}
	else
	{
		if(!bExist)
		{
			AfxMessageBox(CString(_T("Die Importdatei ")) + pImportSAPToWAX->m_strSapToWaxImportFile + CString(_T("\nist nicht vorhanden")), MB_OK);
		}
	}
	pImportSAPToWAX->m_bImportAbbrechen = FALSE;
	pImportSAPToWAX->m_ImportAbbrechen.EnableWindow(FALSE);
	pImportSAPToWAX->m_ImportStarten.EnableWindow(TRUE);
	pImportSAPToWAX->m_Abort.EnableWindow(TRUE);
	pImportSAPToWAX->m_ProgressControl.SetPos(0);

    infile.close(); 
	return 0;
}




void CImportSAPToWAX::OnBnClickedButtonImportAbbrechen()
{
	m_bImportAbbrechen = TRUE;
}
