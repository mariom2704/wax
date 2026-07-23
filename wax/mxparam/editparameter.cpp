#include "stdafx.h"

#include "resource.h"
#include "parameter.h"
#include "editparameter.h"

#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(EditParameter, CDialog)
END_MESSAGE_MAP()

EditParameter::EditParameter(CWnd* parent, Parameter *para) : CDialog(IDD_EDIT_PARAMETER, parent)
{
	par = para;
	ASSERT(par);
}

EditParameter::~EditParameter()
{
}


BOOL EditParameter::OnInitDialog()
{
	if (!CDialog::OnInitDialog() || !par)
		return (FALSE);

	GetDlgItem(IDC_TXT_WERT)->SetWindowText(TextDataList::getInstance()->getTextWithColon("IDS_WERT").c_str());
	GetDlgItem(IDC_STATIC_GUELTIGKEIT)->SetWindowText(TextDataList::getInstance()->getText("IDC_STATIC_GUELTIGKEIT").c_str());
	GetDlgItem(IDC_LOCATION)->SetWindowText(TextDataList::getInstance()->getText("IDC_LOCATION").c_str());
	GetDlgItem(IDC_MACHINE)->SetWindowText(TextDataList::getInstance()->getText("IDC_MACHINE").c_str());
	GetDlgItem(IDC_USER)->SetWindowText(TextDataList::getInstance()->getText("IDC_USER").c_str());


	GetDlgItem(IDC_VAL)->SetWindowText(par->get_val_str().c_str());

	idc2namesp.insert(make_pair<UINT, int>(IDC_MACHINE, Parameter::MACHINE));
	idc2namesp.insert(make_pair<UINT, int>(IDC_USER, Parameter::USER));
	idc2namesp.insert(make_pair<UINT, int>(IDC_LOCATION, Parameter::LOCATION));

	map<UINT, int>::iterator it;

	for (it = idc2namesp.begin(); it != idc2namesp.end(); it++)
	{
		if (!(par->valid_namespaces() & it->second))
			GetDlgItem(it->first)->EnableWindow(false);
		if (par->get_valid_namespace() == it->second)
			((CButton*)GetDlgItem(it->first))->SetCheck(true);
	}
	GetDlgItem(IDC_BEMERKUNG)->SetWindowText(par->getBemerkung().c_str());
	SetWindowText(par->get_entry().c_str());

	return true;
}

void EditParameter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VAL, val);
}

void EditParameter::OnOK()
{
	CDialog::OnOK();	
	par->set_val((LPCSTR)val);
	map<UINT, int>::iterator it;

	for (it = idc2namesp.begin(); it != idc2namesp.end(); it++)
	{
		if (((CButton*)GetDlgItem(it->first))->GetCheck())
		{
			XASSERT(par->set_valid_namespace((Parameter::VALID_NAMESPACE)it->second));
				
		}
	}
}

