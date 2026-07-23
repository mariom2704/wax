#pragma once


class SapSearch : public CDialog  
{
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedCheckIppe();
	
public:
	const int Get_SDAuftragsNr() const;

	const int Get_SDPosNr() const;

	SapSearch();

	CString GetIppeMatNr()
	{
		return IppeMatNr;
	};


	bool GetIsChoosenIppe()
	{
		return IsChoosenIppe;
	};

	SapSearch(CWnd* parent);

private:

	int SDAuftragsNr, SDPosNr;

	bool IsChoosenIppe;
	CString IppeMatNr;
	

public:
	virtual void OnOK();

	virtual void DoDataExchange(CDataExchange* pDX);

	virtual BOOL OnInitDialog();
	
	
};
