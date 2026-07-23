#pragma once

#include "Rohrleitung.h"




typedef std::map< int, int >  CDnToIndex;


//typedef vector <double> VECTOR_POINTS_ELEMENTUNTE_BEFORE;

class ROHRKONSTDLL COIIVerteiler : public Rohrleitung  
{

	enum{ eTrestdleToRightOrLeftTubeEnd = 500};
	double PointElemente_Before;
	int PreElementId;
	void DetectAndMarkEqualDistanceBockAbgang();
	bool bHochgestellt;
	bool bFirstGraficAbgang;

public:
	void SetPreElementId(int PreElementId);
	int GetPreElementId();
	bool has_GegenflanschWithMuffe();
	virtual bool createVk3Liste();
	bool createOIIVerteilerVk3Liste();
	void ResetRohrElementePointElementBeforeWinkel();
	void SetRohrPointElementBeforeWinkel(double PointElementeBefore);
	double GetRohrPointElementBeforeWinkel();
	
	void SetFirstGraficAbgang(bool bFirstGraficAbgangLokal) // Damit die Bemassung immer gleich bleibt hoch tief hoch
	{
		bFirstGraficAbgang = bFirstGraficAbgangLokal;

	}

	bool GetFirstGraficAbgang()
	{
		return bFirstGraficAbgang;

	}

	void SetHochgestellt(bool blocalHochgestellt)
	{
		bHochgestellt = blocalHochgestellt;

	}

	bool GetHochgestellt()
	{
		return bHochgestellt;
	}
	void PutDichtungAndStrews(CObList& templist, int iDnvl );
	void create_templist(CObList& templist, CObList& elementList);
	void BereichsVentilStuecklist(CObList& templist);
	void EntleerungAndOtherStuecklist(CObList& templist);
	virtual void setSicherheitsventil(CObList& templist);
	void FlanschBlindflanschAndOtherStuecklist(CObList& templist);
	void create_stueckliste();
	void AnfasenFuerGewindeundDruckpruefung();
	virtual void SchweissUHeftFaktorenhinzufuegen(Arbeitsfolge *afo);
	static int iDNAbgToDNvl[7][7];
	CDnToIndex iSearchDnToIndex;

	const int get_AnzahlAnschluesse();
	const void set_AnzahlAnschluesse(int iAnzahlAnschluesse);
	CString TestDNvlTODNAbgang(int iVon, int iBis);

	int GetTrestleSpaceToFlanschLeft(int iDN);
	int GetTrestleSpaceToFlanschRight(int iDN);
	int GetTrestleSpaceToGewindeLeft(int iDN);
	int GetTrestleSpaceToGewindeRight(int iDN);
	int GetTrestleSpaceToBodenLeft(int iDN);
	int GetTrestleSpaceToBodenRight(int iDN);

	int FirstAbgangLinks();
	int LastAbgangRechts();

	void SetAbstandBockZumLinkenRohrEnde(int iAbstandBockAbsolut);
	int  GetAbstandBockZumLinkenRohrEnde();
	int  GetAbstandErsterMittlererBockZumLinkenRohrEnde();
	int  GetAbstandZweiterMittlererBockZumErstenMittlerenBock();
	int  GetAbstandBockZumRechtenRohrEnde();
	int  GetAbstandBockZumRechtenRohrEndeInput(){return m_iTrestdleToRightTubeEnd;};
	
	void SetAbstandErsterMittlererBockZumLinkenRohrEnde(int iAbstandBockRel);
	void SetAbstandZweiterMittlererBockZumErstenMittlerenBock(int iAbstandBockRel);
	void SetAbstandBockZumRechtenRohrEnde(int iAbstandBockRel );
	int  m_iTrestdleToRightTubeEnd;
	int	 m_iTrestdleToLeftNozzle; 
	int	 m_iTrestdleToLeftTubeEndFehlerAbstandZuKleinOrGross;
	int	 m_iTrestdleToRightTubeEndFehlerAbstanZuKleinOrGross;
	virtual void setQualiRohrstutzen(Element *ele );
	int GetTrestleSpaceLeftAreaNozzle(int iDN, int iAbstand = 0);
	enum {	MAXCOUNT = 7 };

protected:
	virtual Arbeitsfolge* CreateDruckprobe();
	virtual void AfoChangeVirtualAssembledElements(Element* pElement);
	virtual void fill_elementArray();
	int m_AbgangSicherheitsventilVorhanden; 
	virtual void setGewindeAnfangEnde(CObList& templist);
	void AddElementeAfos();
	int	elementDN[MAXCOUNT];

private:
	
	CString m_strFirstBVOrBockSpaceWrong;
	int AbstandBockAbsolut[4];
	
	enum { extraAbgObenDN = 15};
	enum { extraAbgObenLaenge = 50};
	
	int	elementRelAbst[MAXCOUNT];
	int	elementAbsAbst[MAXCOUNT];
	int elementTypAbgangEnde[MAXCOUNT];
	int elementTypGegenFLansch[MAXCOUNT];
	int elementLaengeGegenFLansche[MAXCOUNT];
	
	
	int m_AbgangEntleerungVorhanden; 
	int m_AbgangEntleerungRechts;
	int elementEndschalterTyp;


	
	int m_AbgangSicherheitsventilRechts;
	

	void setQualiRohrstutzen(CObList& templist);
	
	int GetTrestleSpaceRightAreaNozzle(int iDN);


	int elementlEndeLaenge;

	/// Elementlaenge von Rohrende
	int elementrEndeLaenge;
	
	/// Laenge Rohrstuecke
	int rohrLaengen[256];

	/// Laenge von Anfang/Ende/Verbindung setzen
	void setAnfaEndeVerbLaenge();

	int m_iAnzahlAnschluesse;


public:
	void AdditionalStuecklist(CObList& templist);
	const int get_elementGegenflansch(int index) const;
	const int get_ElementLaengeGegenflansch(int index) const;
	const int get_elementTypeAbgang(int index) const;
	const void set_elementTypeAbgang(int index, int value);
	const int get_EndschalterTyp() const;
	const int set_EndschalterTyp(int value);
	virtual void check_elementArray();

	virtual CString getTabname();

	virtual EnContainerRohrElemente& getElementArray();

	const int get_muffenRelAbst(int index) const;

	virtual CString checkPlausis();

	CString checkAnbauElemente();

	virtual void createAutoArbeit(bool beschichtung = true);

	COIIVerteiler();
	virtual ~COIIVerteiler();

	virtual void serialize(MxFilesystem::BinaryArchive& ar);

	void SicherventilBothSides(int SicherventilBothSides, int& TubeBeginToSecureNozzleRelLeft);
	void SicherventilLinks(int& TubeBeginToSecureNozzleRelLeft);
	void SicherventilRechts(int iLastAbsolutelementAbsAbst, int TreastleSpaceRohende);
	void SetRohrAnfangToTrestle();
	void FillInEntleerungOrMiddleBockFirstPosition(int i);
	void FillInEntleerungSecondPosition(int i);
	void FillInEntleerungLastPosition(int iMerkDNLast);
	void SetRestUndLaenge(int& TreastleSpaceRohende);
	
	void setElementdaten(	int elementDN[], int elementRelAbst[], int elementTypAbgangEnde[], 
							int elementTypGegenFLansch[], int elementLaengeGegenFLansche[], int EndschalterTyp, 
							int elementlEnde,	 int elementrEnde, 
							int EntLeerungVorhanden,     BOOL bEntleerungRechts,
							int SicherheitsventilVorhanden, BOOL bSicherheitsventilRechts);

	
	void createModell();
	

	int GetAbgangSicherheitsventilVorhanden()
	{
		return m_AbgangSicherheitsventilVorhanden;
	}

	void SetAbgangSicherheitsventilVorhanden(int AbgangSicherheitsventilVorhanden )
	{
		m_AbgangSicherheitsventilVorhanden = AbgangSicherheitsventilVorhanden;
	}

	int GetAbgangSicherheitsventilRechts()
	{
		return m_AbgangSicherheitsventilRechts;
	}
	

	void SetAbgangSicherheitsventilRechts(int AbgangSicherheitsventilRechts)
	{
		m_AbgangSicherheitsventilRechts = AbgangSicherheitsventilRechts;
	}

	int GetAbgangEntleerungVorhanden()
	{
		return m_AbgangEntleerungVorhanden;
	}

	void SetAbgangEntleerungVorhanden(int AbgangEntleerungVorhanden)
	{
		 m_AbgangEntleerungVorhanden = AbgangEntleerungVorhanden;
	}

	int GetAbgangEntleerungRechts()
	{
		return m_AbgangEntleerungRechts;
	}

	void SetAbgangEntleerungRechts(int AbgangEntleerungRechts )
	{
		m_AbgangEntleerungRechts = AbgangEntleerungRechts;
	}
	
	const int get_elementDN(int index) const;

	void set_elementDN(int index, int value);

	const int get_elementRelAbst(int index) const;

	void set_elementRelAbst(int index, int value);
	

	int GetElementTypAbgangEnde(int iLauf)
	{
		return elementTypAbgangEnde[iLauf];
	}

	void SetElementTypAbgangEnde(int iLauf, int LocalElementTypAbgangEnde )
	{
		elementTypAbgangEnde[iLauf] = LocalElementTypAbgangEnde;
	}

	int GetElementTypGegenFLansch(int iLauf)
	{
		return elementTypGegenFLansch[iLauf];
	}

	void SetElementTypGegenFLansch(int iLauf, int LocalElementTypGegenFLansch)
	{
		 elementTypGegenFLansch[iLauf] = LocalElementTypGegenFLansch;
	}

	int GetElementLaengeGegenFLansche(int iLauf)
	{
		return elementLaengeGegenFLansche[iLauf];
	}

	void SetElementLaengeGegenFLansche(int iLauf, int LocalelementLaengeGegenFLansche)
	{
		 elementLaengeGegenFLansche[iLauf] = LocalelementLaengeGegenFLansche;
	}
	

	int GetElementEndschalterTyp()
	{
		return elementEndschalterTyp;
	}

	void SetElementEndschalterTyp(int LocalElementEndschalterTyp)
	{
		 elementEndschalterTyp = LocalElementEndschalterTyp;
	}


	virtual bool isCOIIVerteiler() const { return true; }

};


