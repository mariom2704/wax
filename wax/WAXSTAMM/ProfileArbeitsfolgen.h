#include "StammDat.h"

class WAXSTAMMDLL ProfileArbeitsfolgen : public StammDat
{
public:
	ProfileArbeitsfolgen();

	virtual void load(const std::string& db);

	class WAXSTAMMDLL data 
	: public CObject
	{
	public:
		const int get_code() const;

		const CString& get_typ() const;

	private:
		int code;

		CString typ;

	friend class ProfileArbeitsfolgen;
	};
};
