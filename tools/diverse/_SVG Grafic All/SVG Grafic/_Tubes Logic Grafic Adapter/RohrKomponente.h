#pragma once


class WAXGLDLL CGraficRohrKomponente;
class WAXGLDLL CGraficRohrKomponenteTeilRohr;
class WAXGLDLL CGraficRohrKomponenteBogen;
class WAXGLDLL CGraficTubeController;
class WAXGLDLL CRohrKomponente;
class WAXGLDLL CGraficController;
class WAXGLDLL CCRohrOneAssembeldKomponenteVector;
class WAXGLDLL CRohrBemassungsKomponentenVectorVector;
class WAXGLDLL CRohrBemassungsAnbauChildKomponentenVectorVector;

typedef std::vector<CRohrKomponente*> CRohrOneAssembeldKomponenteVector;
typedef std::vector<CCRohrOneAssembeldKomponenteVector*> CRohrKomponenteVectorVector;



class WAXGLDLL CRohrKomponente
{
private:
	
	 bool m_FirstKomponent;
	
	 	
public:
	enum CLASS_NAME
	{
		RohrKomponente,
		RohrKomponenteTeilRohr,
		RohrKomponenteBogen,
		RohrKomponenteNut,
		RohrKomponenteGlatt,
	};

	CRohrKomponente(void);
	~CRohrKomponente(void);


	double GetWinkel()
	{
		return winkel;
	};

	int GetDn()
	{
		return m_iDN;
	};

	void SetDn(int iDN)
	{
		m_iDN = iDN;
	};

	int GetRadius()
	{
		return m_iDN/2;
	};

	void SetFirstKomponent()
	{
		m_FirstKomponent = true;
	}

	bool GetFirstKomponent()
	{
		return m_FirstKomponent;
	}

	void SetRadius(int iLocalRedius)
	{
		m_iDN = iLocalRedius * 2;
		return;
	};

	void SetWinkel(double lWinkel)
	{
		winkel = lWinkel;// + 0.00000001;
	};

	int GetLaenge()
	{
		return iLaenge;
	};

	void SetLaenge(int iLocalLaenge)
	{
		iLaenge = iLocalLaenge;
	};

	void SetRelativerAbstandzumNaechsten(int RelativerAbstand)
	{
		m_RelativerAbstand = RelativerAbstand;
	}
	
	int GetRelativerAbstandzumNaechsten()
	{
		return m_RelativerAbstand;
	}

	void SetName(CString strName)
	{
		m_strName = strName;
	}

	CLASS_NAME GetKlassenName()
	{
		return m_eClassName;
	}

	virtual CGraficRohrKomponente* GetGraficRohrKomponente( CGraficController* StrangRohrVector);
	

private:
	int m_RelativerAbstand;
	int m_iDN;
	int iLaenge;
	int NutIdEnde;
	int NutIdAnfang;
	double winkel;
	CRohrKomponente* m_prevStrangRohr;
	CRohrKomponente* m_pNextStrangRohr;
	CString m_strName;
	
protected:
	CGraficRohrKomponente* m_pGraficRohrKomponente;
	CLASS_NAME m_eClassName;
};




class WAXGLDLL CRohrKomponenteTeilRohr : public CRohrKomponente
{
   public:
		CRohrKomponenteTeilRohr()
		{
			m_eClassName   = RohrKomponenteTeilRohr;
		};
		virtual CGraficRohrKomponente*  GetGraficRohrKomponente(CGraficController* pRohController);
};




class WAXGLDLL CRohrKomponenteBogen : public CRohrKomponente
{
	public:
		CRohrKomponenteBogen()
		{
				m_eClassName   =  RohrKomponenteBogen;
		};
		virtual CGraficRohrKomponente* GetGraficRohrKomponente(CGraficController* StrangRohrVector);
};




class WAXGLDLL CRohrKomponenteNut : public CRohrKomponente
{
	public:
		CRohrKomponenteNut()
		{
				m_eClassName = RohrKomponenteNut;
		};
		
		virtual CGraficRohrKomponente* GetGraficRohrKomponente(CGraficController* StrangRohrVector);
};


class WAXGLDLL CRohrKomponenteGlatt : public CRohrKomponente
{
public:
	CRohrKomponenteGlatt()
	{
		m_eClassName = RohrKomponenteGlatt;
	};
	virtual CGraficRohrKomponente* GetGraficRohrKomponente(CGraficController* StrangRohrVector);
};