#include "test_cadmodell_typrohr.h"

class Testsuite_rohrkonst : public test_suite
{
public:
	Testsuite_rohrkonst::Testsuite_rohrkonst() : test_suite("Rohrkonst Testsuite")
	{
		// Typrohr: Längenermittlung von Rohren in Cad Positionen testen
		boost::shared_ptr<test_cadmodell_typrohr> instance(new test_cadmodell_typrohr());
		test_case* do_test_cadmodell_typrohr_len = BOOST_CLASS_TEST_CASE(&test_cadmodell_typrohr::test_len, instance );
		add(do_test_cadmodell_typrohr_len);
	}
};