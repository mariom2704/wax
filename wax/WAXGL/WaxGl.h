#pragma once


/**
Wax Grafikschnittstelle 
Diese Klasse bildet das Frontend zum ActiveX Control VoloView bzw. 
AutodeskViewer und dient als generische
Klasse bei evtl. anderen zu implementierenden Grafikviewern.
*/
class WAXGLDLL WaxGl 
{
public:
	/// Konstruktor
	WaxGl();
	
	/// Destruktor
	virtual ~WaxGl();

	
protected:

	

	int m_ilastx;
	int m_ilasty;

private:

	/// Erzeugt die Checksumme aus der DWF Datei und liefert als boolschen 
	/// Wert zurück, ob ein Neuzeichnen nötig ist.
	bool createChecksum(const CString& filename);



	double left;

	double right;

	double top;

	double bottom;

	CString streamname;


	

};
