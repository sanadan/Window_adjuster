///	@file	TPath.hpp
///	@brief	パス文字列クラス
//
//	2007/01/22	田中　悟	新規作成

#pragma once

#include <shlobj.h>
#include <Psapi.h>
#pragma comment( lib, "Psapi.lib" )
#include "TString.hpp"

// namespace宣言
namespace Javelin
{
	const DWORD Path_buffer_length = 1024 ;	///< 読み出せる最大文字列

	///	@brief	Win32関数切り換えのベースのテンプレート
	///	@param	_E	要素の型
	template< class _E > class Basic_path_Win32
	{
	} ;

	///	@brief	UNICODE系Win32関数テンプレート
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

	///	@brief	ANSI系Win32関数テンプレート
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
		// typedef const_iterator _It ;	// 親クラスで宣言済
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

		///	@brief	システムのフォルダのパスを取得する
		///	@param	iFolder	フォルダの種類（CSIDL）\n
		///					= CSIDL_ADMINTOOLS           :\n
		///					= CSIDL_COMMON_ADMINTOOLS    :\n
		///					= CSIDL_APPDATA              : <ユーザー>\\Application Data\n
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
		///	@param	dwFlags	ユーザー毎にパスが変わる場合のユーザー名指定\n
		///					= SHGFP_TYPE_CURRENT : カレントユーザー名\n
		///					= SHGFP_TYPE_DEFAULT : Default User
		///	@param	hToken	アクセストークン\n
		///					= NULL : デフォルト
		///	@retval	ERROR_SUCCESS	正常終了
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

		///	@brief	パス接続
		Basic_path& Combine( const _E* path )
		{
			int top = 0 ;
			Basic_path::const_reverse_iterator crit = rbegin() ;
			if ( *crit == Basic_path_Win32< _E >::Get_separator() )
			{
				if ( path[ 0 ] == Basic_path_Win32< _E >::Get_separator() )
				{	// 元の末尾も追加の先頭もセパレータ
					top++ ;	// 追加の先頭を一つ進める
				}
			}
			else if ( path[ 0 ] != Basic_path_Win32< _E >::Get_separator() )
			{	// 元の末尾も追加の先頭もセパレータでない
				*this += Basic_path_Win32< _E >::Get_separator() ;
			}

			*this += &path[ top ] ;

			return *this ;
		}
	} ;

	///	@brief	UNICODE用Basic_path_string
	typedef Basic_path< WCHAR > WPath ;

	///	@brief	ANSI用Basic_path_string
	typedef Basic_path< CHAR > Path ;

	///	@brief	TCHAR用Basic_path_string
#if defined ( _UNICODE )
	typedef WPath TPath ;
#else
	typedef Path TPath ;
#endif
}	// namespace Javelin

// [EOF]
