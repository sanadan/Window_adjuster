///	@file	Process.cpp
///	@brief	プロセスクラス
//
//	2011/12/09	真田	新規作成

#include "stdafx.h"
#include "Process.hpp"

namespace Javelin
{
	///	@brief	コンストラクタ
	Process::Process()
	{
		Handle = NULL ;
	}

	///	@brief	デストラクタ
	Process::~Process()
	{
		Close() ;
	}

	///	@breif	ウィンドウに対応するプロセスを開く
	///	@param	window_handle	ウィンドウハンドル
	///	@return	winerror.h準拠
	///	@note	他プロセスのウィンドウコントロールの情報を取得するときなどに使う。\n
	///			プロセスを開いている間はGet_handle()が有効になる。
	HRESULT Process::Open( HWND window_handle, DWORD access_flag, BOOL inherit )
	{
		// ウィンドウのプロセスID取得
		DWORD process_id ;
		::GetWindowThreadProcessId( window_handle, &process_id ) ;

		// ウィンドウのプロセスハンドル取得
		Handle = ::OpenProcess( access_flag, inherit, process_id ) ;
		if ( Handle == NULL ) return ::GetLastError() ;

		return ERROR_SUCCESS ;
	}

	///	@brief	プロセスを閉じる
	///	@return	winerror.h準拠
	HRESULT Process::Close()
	{
		if ( Handle == NULL ) return ERROR_SUCCESS ;
		
		BOOL success = ::CloseHandle( Handle ) ;
		Handle = NULL ;
		
		if ( !success ) return ::GetLastError() ;

		return ERROR_SUCCESS ;
	}

	///	@brief	プロセスハンドルを取得する
	///	@return	プロセスハンドル\n
	///			== NULL : プロセスを開いていない
	HANDLE Process::Get_handle() const
	{
		return Handle ;
	}
} ;	// namespace Javelin

// [EOF]
