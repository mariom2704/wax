#ifndef __MONITOR_
#define __MONITOR_

class WAXINT Monitor 
{
public:
	virtual ~Monitor();

	Monitor();

	const CWordArray& get_aktionen() const;

	void set_currentAction(int value);

	void makeAction(int id);

	void initAction();

	CString getName(int id);

private:
	CWordArray aktionen;

	int currentAction;

public:

	void showAnlage(int anlage);
};

#endif 
