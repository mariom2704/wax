#pragma once

class ROHRKONSTDLL MXDate : public COleDateTime
{

public:
	
	MXDate();

	MXDate(const MXDate& other);

	MXDate(const COleDateTime& other);

	static const MXDate get_current_date();

	virtual CString Format(DWORD dwFlags = 0, LCID lcid = LANG_USER_DEFAULT ) const;

	virtual CString Format(LPCTSTR lpszFormat) const;

	virtual CString Format(UINT nFormatID) const;

	
	/// Prüft das Datum 
	bool is_valid() const;

	void copy(const MXDate& other);

	/// Datumsvergleich
	bool is_newer(const MXDate& other) const;

	/// Integerwert für Datum mit Uhrzeit 59 Std 59 Min bzw. 00 Std 00 Min
	int toNornamlizedInt(bool bis) const;

	int convDateToInt() const;
};

