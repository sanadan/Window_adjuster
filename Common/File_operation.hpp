///
///	@file		File_operation.hpp
///	@brief		ファイル操作クラス
///	@note		
///
//	2007/01/22	田中　悟	新規作成
//

#pragma once

#include <shellapi.h>
#pragma comment( lib, "shell32.lib" )

// namespace宣言
namespace Javelin
{
	class File_operation
	{
	public :
		File_operation()
		{
			::ZeroMemory( &SHFileopstruct, sizeof ( SHFileopstruct ) ) ;
		}

		HRESULT Copy()
		{
			SHFileopstruct.wFunc = FO_COPY ;
			if ( ::SHFileOperation( &SHFileopstruct ) ) == 0 ) return ERROR_SUCCESS ;

			return ERROR_INVALID_FUNCTION ;	// @@@@@ とりあえず
		}

		HRESULT Delete()
		{
			SHFileopstruct.wFunc = FO_DELETE ;
			if ( ::SHFileOperation( &SHFileopstruct ) ) == 0 ) return ERROR_SUCCESS ;

			return ERROR_INVALID_FUNCTION ;	// @@@@@ とりあえず
		}

	private :
		::SHFILEOPSTRUCT SHFileopstruct ;
	} ;
}	// namespace Javelin

// [[[[[ End of this header ]]]]]
