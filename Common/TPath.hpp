///	@file	TPath.hpp
///	@brief	�p�X������N���X
//
//	2007/01/22	�c���@��	�V�K�쐬

#pragma once

#include <shlobj.h>
#include <Psapi.h>
#pragma comment( lib, "Psapi.lib" )
#include "TString.hpp"

// namespace�錾
namespace Javelin
{
	const DWORD Path_buffer_length = 1024 ;	///< �ǂݏo����ő啶����

	///	@brief	Win32�֐��؂芷���̃x�[�X�̃e���v���[�g
	///	@param	_E	�v�f�̌^
	template< class _E > class Basic_path_Win32
	{
	} ;

	///	@brief	UNICODE�nWin32�֐��e���v���[�g
	template<> class Basic_path_Win32< WCHAR >
	{
	public :
		static HRESULT Get_folder( HWND hwndOwner, int nFolder, HANDLE hToken, DWORD dwFlags, LPWSTR pszPath )
		{
			return ::SHGetFolderPathW( hwndOwner, nFolder, hToken, dwFlags, pszPath ) ;
		}
		static HRESULT Get_module_base_name( HANDLE hProcess, HMODULE hModule, LPWSTR lpBaseName, DWORD nSize )
		{
			if ( ::GetModuleBaseNameW( hProcess, hModule, lpBaseName, nSize ) == 0 )
				return ::GetLastError() ;
			return ERROR_SUCCESS ;
		}
		static WCHAR Get_separator()
		{
			return L'\\' ;
		}
	} ;

	///	@brief	ANSI�nWin32�֐��e���v���[�g
	template<> class Basic_path_Win32< CHAR >
	{
	public :
		static HRESULT Get_folder( HWND hwndOwner, int nFolder, HANDLE hToken, DWORD dwFlags, LPSTR pszPath )
		{
			return ::SHGetFolderPathA( hwndOwner, nFolder, hToken, dwFlags, pszPath ) ;
		}
		static HRESULT Get_module_base_name( HANDLE hProcess, HMODULE hModule, LPSTR lpBaseName, DWORD nSize )
		{
			if ( ::GetModuleBaseNameA( hProcess, hModule, lpBaseName, nSize ) == 0 )
				return ::GetLastError() ;
			return ERROR_SUCCESS ;
		}
		static CHAR Get_separator()
		{
			return '\\' ;
		}
	} ;

	///
	template< class _E, class _Tr = char_traits< _E >, class _A = allocator< _E > >
	class Basic_path : public Basic_string< _E, _Tr, _A >
	{
	public :
		explicit Basic_path( const _A& _Al = _A() )
			: Basic_string< _E, _Tr, _A >( _Al )
		{
		}
		Basic_path( const _Myt& _X )
			: Basic_string< _E, _Tr, _A >( _X )
		{
		}
		Basic_path( const _Myt& _X, size_type _P, size_type _M, const _A& _Al = _A() )
			: Basic_string< _E, _Tr, _A >( _X, _P, _M, _Al )
		{
		}
		Basic_path( const _E *_S, size_type _N, const _A& _Al = _A() )
			: Basic_string< _E, _Tr, _A >( _S, _N, _Al )
		{
		}
		Basic_path( const _E *_S, const _A& _Al = _A() )
			: Basic_string< _E, _Tr, _A >( _S, _Al )
		{
		}
		Basic_path( size_type _N, _E _C, const _A& _Al = _A() )
			: Basic_string< _E, _Tr, _A >( _N, _C, _Al )
		{
		}
		// typedef const_iterator _It ;	// �e�N���X�Ő錾��
#if _MSC_VER < 1310
		Basic_path( _It _F, _It _L, const _A& _Al = _A() )
			: Basic_string< _E, _Tr, _A >( _F, _L, _Al )
		{
		}
#else
		template< class _It > Basic_path( _It _First, _It _Last )
			: Basic_string< _E, _Tr, _A >( _First, _Last )
		{
		}

		template< class _It > Basic_path( _It _First, _It _Last, const _A& _Al )
			: Basic_string< _E, _Tr, _A >( _First, _Last, _Al )
		{
		}
#endif

		///	@brief	�V�X�e���̃t�H���_�̃p�X���擾����
		///	@param	iFolder	�t�H���_�̎�ށiCSIDL�j\n
		///					= CSIDL_ADMINTOOLS           :\n
		///					= CSIDL_COMMON_ADMINTOOLS    :\n
		///					= CSIDL_APPDATA              : <���[�U�[>\\Application Data\n
		///					= CSIDL_COMMON_APPDATA       : All Users\\Application Data\n
		///					= CSIDL_COMMON_DOCUMENTS     :\n
		///					= CSIDL_COOKIES              :\n
		///					= CSIDL_FLAG_CREATE          :\n
		///					= CSIDL_HISTORY              :\n
		///					= CSIDL_INTERNET_CACHE       :\n
		///					= CSIDL_LOCAL_APPDATA        :\n
		///					= CSIDL_MYPICTURES           :\n
		///					= CSIDL_PERSONAL             :\n
		///					= CSIDL_PROGRAM_FILES        :\n
		///					= CSIDL_PROGRAM_FILES_COMMON :\n
		///					= CSIDL_SYSTEM               :\n
		///					= CSIDL_WINDOWS              :\n
		///	@param	dwFlags	���[�U�[���Ƀp�X���ς��ꍇ�̃��[�U�[���w��\n
		///					= SHGFP_TYPE_CURRENT : �J�����g���[�U�[��\n
		///					= SHGFP_TYPE_DEFAULT : Default User
		///	@param	hToken	�A�N�Z�X�g�[�N��\n
		///					= NULL : �f�t�H���g
		///	@retval	ERROR_SUCCESS	����I��
		HRESULT Get_folder( int iFolder, DWORD dwFlags = SHGFP_TYPE_CURRENT, HANDLE hToken = NULL )
		{
			_E szPath[ Path_buffer_length ] ;
			DWORD dwRet = Basic_path_Win32< _E >::Get_folder( NULL, iFolder, hToken, dwFlags, szPath ) ;
			if ( dwRet != ERROR_SUCCESS ) return dwRet ;

			*this = szPath ;
			return ERROR_SUCCESS ;
		}

		///
		HRESULT Get_module_base_name()
		{
			_E baseName[ Path_buffer_length ] ;
			HANDLE process = ::GetCurrentProcess() ;
			HRESULT result = Basic_path_Win32< _E >::Get_module_base_name( process, Instance, baseName, ARRAYSIZE( baseName ) ) ;
			if ( result != ERROR_SUCCESS ) return result ;

			*this = baseName ;
			return ERROR_SUCCESS ;
		}

		///	@brief	�p�X�ڑ�
		Basic_path& Combine( const _E* path )
		{
			int top = 0 ;
			Basic_path::const_reverse_iterator crit = rbegin() ;
			if ( *crit == Basic_path_Win32< _E >::Get_separator() )
			{
				if ( path[ 0 ] == Basic_path_Win32< _E >::Get_separator() )
				{	// ���̖������ǉ��̐擪���Z�p���[�^
					top++ ;	// �ǉ��̐擪����i�߂�
				}
			}
			else if ( path[ 0 ] != Basic_path_Win32< _E >::Get_separator() )
			{	// ���̖������ǉ��̐擪���Z�p���[�^�łȂ�
				*this += Basic_path_Win32< _E >::Get_separator() ;
			}

			*this += &path[ top ] ;

			return *this ;
		}
	} ;

	///	@brief	UNICODE�pBasic_path_string
	typedef Basic_path< WCHAR > WPath ;

	///	@brief	ANSI�pBasic_path_string
	typedef Basic_path< CHAR > Path ;

	///	@brief	TCHAR�pBasic_path_string
#if defined ( _UNICODE )
	typedef WPath TPath ;
#else
	typedef Path TPath ;
#endif
}	// namespace Javelin

// [EOF]
