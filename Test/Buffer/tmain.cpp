///		

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include "Buffer_test.hpp"
#include "Test_frame.hpp" 

int _tmain( int, TCHAR **, TCHAR ** )
{
	JT_TEST_CHECK( Javelin::Buffer_test::Test ) ;

	return JT_FINISH() ;
}

// [[[[[ End of this module ]]]]]
