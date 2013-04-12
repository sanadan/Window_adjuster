///		

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include "File_operation_test.hpp"
#include "Test_frame.hpp" 

int _tmain( int, TCHAR **, TCHAR ** )
{
	JT_TEST_CHECK( Javelin::File_operation_test::Test ) ;

	return JT_FINISH() ;
}

// [[[[[ End of this module ]]]]]
