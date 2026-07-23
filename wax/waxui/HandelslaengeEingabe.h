class HandelslaengeData;

class WAXUIDLL HandelslaengeEingabe : public WaxPartForm     
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(HandelslaengeEingabe)
public:
	HandelslaengeEingabe( );
	virtual ~HandelslaengeEingabe();

	//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen wird.
	virtual bool check_plausis();

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	//Indicates the display is updated after text changes
	void onUpdate();

	void onChangeDn();

	Handelslaenge* getHandelslaenge();
	
	
private:
	int rohrDN;
	
	int rohrLaenge;
	
	int rohrQuali;
	
	int rohrKFarbe;
	
	int rohrAnfangEnde;
	
	//Rohrdatenobjekt, siehe <c Rohrdaten>
	HandelslaengeData* rohr;
};
