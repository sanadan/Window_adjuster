//	2006/05/19	Jecy	êVãKçÏê¨

#pragma once

#include <Windows.h>
#include "TString.hpp"

// namespaceêÈåæ
namespace Javelin
{
	class Find_file
	{
	public :
		Find_file() ;
		~Find_file() ;

		void Set_file_name( LPCTSTR file_name ) ;

		DWORD Find() ;

		LPCTSTR Get_file_name() ;
		DWORD Get_attributes() ;

		BOOL Find_file::Is_folder() ;

	private :
		HANDLE Find_handle ;
		WIN32_FIND_DATA Find_data ;
		Javelin::TString File_name ;
	} ;
}	// namespace Javelin

// [[[[[ End of this header ]]]]]
