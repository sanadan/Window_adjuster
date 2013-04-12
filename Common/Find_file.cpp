//	2006/05/19	Jecy	�V�K�쐬

#include "stdafx.h"
#include "Find_file.hpp"

// namespace�錾
namespace Javelin
{
	///
	Find_file::Find_file()
	{
		Find_handle = INVALID_HANDLE_VALUE ;
		::ZeroMemory( &Find_data, sizeof ( Find_data ) ) ;
	}

	///
	Find_file::~Find_file()
	{
		if ( Find_handle != INVALID_HANDLE_VALUE )
		{
			::FindClose( Find_handle ) ;
		}
	}

	///
	void Find_file::Set_file_name( LPCTSTR file_name )
	{
		File_name = file_name ;
	}

	///	@retval	ERROR_SUCCESS		�t�@�C������������
	///	@retval	ERROR_NO_MORE_FILES	�t�@�C�������I��
	DWORD Find_file::Find()
	{
		if ( Find_handle == INVALID_HANDLE_VALUE )
		{	// ����
			Find_handle = ::FindFirstFile( File_name.c_str(), &Find_data ) ;
			if ( Find_handle == INVALID_HANDLE_VALUE )
			{
				DWORD error = ::GetLastError() ;
				if ( error == ERROR_FILE_NOT_FOUND )
				{	// �t�@�C���Ȃ�
					// �t�@�C�������I���́A���ERROR_NO_MORE_FILES�Ƃ���
					return ERROR_NO_MORE_FILES ;
				}
				return error ;
			}
			return ERROR_SUCCESS ;
		}

		// 2��ڈȍ~
		if ( !::FindNextFile( Find_handle, &Find_data ) )
		{
			return ::GetLastError() ;
		}

		return ERROR_SUCCESS ;
	}

	///
	LPCTSTR Find_file::Get_file_name()
	{
		return Find_data.cFileName ;
	}

	///
	DWORD Find_file::Get_attributes()
	{
		return Find_data.dwFileAttributes ;
	}

	///
	BOOL Find_file::Is_folder()
	{
		return Get_attributes() & FILE_ATTRIBUTE_DIRECTORY ;
	}
}	// namespace Javelin

// [[[[[ End of this module ]]]]]
