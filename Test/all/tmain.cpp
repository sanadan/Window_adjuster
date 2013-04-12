///		

#include "stdafx.h"
#include <windows.h>
#include "Test_frame.hpp"
#include "../Buffer/Buffer_test.hpp"
#include "../TString/TString_test.hpp"

DWORD all() ;

int _tmain( int, TCHAR **, TCHAR ** )
{
	JT_TEST_CHECK( all ) ;

	return JT_FINISH() ;
}

DWORD all()
{
	JT_TEST_CHECK( Javelin::Buffer_test::Test ) ;
	JT_TEST_CHECK( Javelin::TString_test::Test ) ;

	return JT_FINISH() ;
}

// [[[[[ End of this module ]]]]]
