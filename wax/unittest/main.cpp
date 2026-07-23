#include "stdafx.h"
#include <iostream>
using namespace std;


#define BOOST_TEST_REPORT_FORMAT XML
#include <boost/test/unit_test.hpp>

using boost::unit_test_framework::test_suite;

//#include "../waxprj/unittest/testsuite_waxprj.h"
#include "../waxstamm/unittest/testsuite_waxstamm.h"
#include "../rohrkonst/unittest/testsuite_rohrkonst.h"

#include "../mxparam/mxparam_dll.h"
#include <mxodbc/mxodbc.h>
#include "unittestApp.h"

UnitTestApp unittest;

test_suite* init_unit_test_suite(int, char* [])
{
	unittest.InitInstance();
	// create the top test suite
   test_suite* top_test_suite(BOOST_TEST_SUITE("Master test suite"));

   // add test suites to the top test suite
   top_test_suite->add(new Testsuite_waxstamm());
   top_test_suite->add(new Testsuite_rohrkonst());

   return top_test_suite;
}