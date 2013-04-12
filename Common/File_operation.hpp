///
///	@file		File_operation.hpp
///	@brief		�t�@�C������N���X
///	@note		
///
//	2007/01/22	�c���@��	�V�K�쐬
//

#pragma once

#include <shellapi.h>
#pragma comment( lib, "shell32.lib" )

// namespace�錾
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

			return ERROR_INVALID_FUNCTION ;	// @@@@@ �Ƃ肠����
		}

		HRESULT Delete()
		{
			SHFileopstruct.wFunc = FO_DELETE ;
			if ( ::SHFileOperation( &SHFileopstruct ) ) == 0 ) return ERROR_SUCCESS ;

			return ERROR_INVALID_FUNCTION ;	// @@@@@ �Ƃ肠����
		}

	private :
		::SHFILEOPSTRUCT SHFileopstruct ;
	} ;
}	// namespace Javelin

// [[[[[ End of this header ]]]]]
