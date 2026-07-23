#include "stdafx.h"
#include "resource.h"

#include "StatusAktualisierung.h"
#include "..\waxprj\WpPreisanfrage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(StatusAktualisierung, CDialog)
	ON_COMMAND(IDC_DOAKTUALISIERUNG, on_aktualisierung)
END_MESSAGE_MAP()


StatusAktualisierung::StatusAktualisierung(CObArray& objects, WaxPart* selObject)
: CDialog(IDD_STATUSAKTUALISIERUNG, NULL), objectList(objects), selObj(selObject)
{
	/**
	Konstruktor, setzt die Objektliste und das zu aktualisierende Objekt
	*/	
}

BOOL StatusAktualisierung::OnInitDialog()
{
	/**
	Dialogelemente initialisieren
	Als Default wird 'Nur Auswahl' ausgewählt. 
	*/
	if (selObj)
	{
		// Nur freigegebene Werkaufträge können im Status aktualisiert werden
		// Hier wird das ausgewählte und alle Unterobjekte auf Freigabe geprüft.
		Werkauftrag *wa = selObj->get_werkauftrag();
		if (!wa->get_waid())
		{
			bool subobject_freigegeben = false;
			for (POSITION pos = selObj->get_subobjects().GetHeadPosition(); pos;)
			{
				Werkauftrag *watmp = NULL;
				WaxPart *pt = dynamic_cast<WaxPart*>(selObj->get_subobjects().GetNext(pos));
				if (pt && !pt->get_deleted())
				{
					watmp = pt->get_werkauftrag();
					XASSERT(watmp);
					if (watmp && watmp->get_waid())
					{
						subobject_freigegeben = true;
						break;
					}

				}
			}
			if (!subobject_freigegeben)
				selObj = NULL;
		}
	}
	// Ist markiertes Objekt oder Unterobjekt nicht updatefähig, wird "Nur Auswahl" Radio Button
	// abgeblendet.
	GetDlgItem(IDC_AUSWAHL)->EnableWindow(selObj ? TRUE : FALSE);
	((CButton*)GetDlgItem(selObj ? IDC_AUSWAHL : IDC_GESAMT))->SetCheck(TRUE);



	GetDlgItem(IDC_STATIC_WA_AKT)->SetWindowText(getStammdaten().getText("IDC_STATIC_WA_AKT").c_str());
	GetDlgItem(IDC_AUSWAHL)->SetWindowText(getStammdaten().getText("IDC_AUSWAHL").c_str());
	GetDlgItem(IDC_GESAMT)->SetWindowText(getStammdaten().getText("IDC_GESAMT").c_str());
	GetDlgItem(IDC_DOAKTUALISIERUNG)->SetWindowText(getStammdaten().getText("IDC_DOAKTUALISIERUNG").c_str());
	


	return CDialog::OnInitDialog();
}


void StatusAktualisierung::on_aktualisierung()
{
	/**
	Startet die Aktualisierung
	*/
	progress.SetStep(1);
	try
	{
		if (((CButton*)GetDlgItem(IDC_AUSWAHL))->GetCheck())
		{
			progress.SetRange32(0, 1);
			aktualisierung(selObj);
		}
		else
		{
			progress.SetRange32(0, get_update_count());
			for (int i = 0; i < objectList.GetSize(); i++)
			{
				aktualisierung(dynamic_cast<WaxPart*>(objectList.GetAt(i)));
			}
		}
	}
	catch (CString *msg)
	{
		AfxMessageBox(*msg, MB_OK | MB_ICONERROR);
		delete msg;
	}
	OnOK();
}


int StatusAktualisierung::aktualisierung(WaxPart* pt, bool testOnly)
{
	/**
	Aktualisierung für einen Wa inklusive Unteraufträge durchführen
	testonly wird für die Ermittlung der Anzahl der upzudatenden Objekte benutzt
	*/
	XASSERT(pt);
	if (!pt)
		return 0;
	
	int count = 0;
	for (POSITION pos = pt->get_subobjects().GetHeadPosition(); pos; )
	{
		count += aktualisierung(dynamic_cast<WaxPart*>(pt->get_subobjects().GetNext(pos)), testOnly);
	}
	
	//Alle Unterobjekte storniert, wird SA gelöscht 
	if (pt->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
	{
		WaxPart *wp = NULL;
		for (POSITION pos = pt->get_subobjects().GetHeadPosition(); pos; )
		{
			wp = dynamic_cast<WaxPart*>(pt->get_subobjects().GetNext(pos));
			if (!wp->get_deleted())
				break;
		}
		if (wp && wp->get_deleted())
			pt->set_deleted(true);
		else if (wp && !wp->get_deleted())
			pt->set_deleted(false);
	}

	// Sammelaufträge müssen die Daten der Leitkarte eines Unterobjektes bekommen. 
	if (!testOnly && pt->IsKindOf(RUNTIME_CLASS(SammelAuftrag)) && !pt->get_subobjects().IsEmpty())
	{
		WaxPart *erster_sub = dynamic_cast<WaxPart*>(pt->get_subobjects().GetHead());
		pt->get_leitkarte().clone(erster_sub->get_leitkarte());
		pt->get_werkauftrag()->get_status().copy(erster_sub->get_werkauftrag()->get_status());
	}


	//Bei einer Preisanfrage aus dem 1. Sammelauftrag das Datum GültigBis lesen
	if (!testOnly && pt->IsKindOf(RUNTIME_CLASS(WpPreisanfrage)) && !pt->get_subobjects().IsEmpty())
	{
		WaxPart *erster_sub = dynamic_cast<WaxPart*>(pt->get_subobjects().GetHead());
		pt->get_leitkarte().get_preisanfrage().set_gueltigBis(erster_sub->get_leitkarte().get_preisanfrage().get_gueltigBis());
	}




	// Wenn pt nicht freigegeben ist, braucht nur die Anzahl der aktualisierten Unterobjekte zurück
	// gegeben werden. testOnly ist egal.
	if (!pt->get_werkauftrag()->get_waid())
		return count;

	if (!testOnly)
	{
		// Wenn nicht getestet wird, muss der Status aktualisiert werden und der Fortschrittsanzeiger 
		// gesetzt werden.
		if (!pt->updateStatus())
		{
			CString *msg = new CString(getStammdaten().getText("IDS_ERROR8").c_str());
			throw msg;
		}
		progress.StepIt();
	}
	// Hier muss pt noch zu count addiert werden
	return count+1;
}



int StatusAktualisierung::get_update_count()
{
	/**
	Ermittelt die Anzahl der Werkaufträge, deren Status upgedatet wird.
	*/
	int count = 0;
	for (int i = 0; i < objectList.GetSize(); i++)
	{
		count += aktualisierung(dynamic_cast<WaxPart*>(objectList.GetAt(i)), true);
	}
	return count;
}



void StatusAktualisierung::DoDataExchange(CDataExchange* pDX)
{
	CWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UPDATESTATUS, progress);
}

