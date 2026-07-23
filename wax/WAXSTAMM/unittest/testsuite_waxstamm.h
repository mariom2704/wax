#include "test_rohrstutzenbrennzugabe.h"

class Testsuite_waxstamm : public test_suite
{
public:
	Testsuite_waxstamm::Testsuite_waxstamm() : test_suite("Waxstamm Testsuite")
	{
			// Test  rtfdatei::write_file
		boost::shared_ptr<test_rohrstutzenbrennzugabe> instance(new test_rohrstutzenbrennzugabe());
		test_case* do_test_rohrstutzenbrennzugabe = BOOST_CLASS_TEST_CASE(&test_rohrstutzenbrennzugabe::test_brennzugabe, instance );
		add(do_test_rohrstutzenbrennzugabe);
	}
};