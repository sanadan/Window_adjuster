///	@file	Mutex.cpp
///	@brief	ミューテックスクラス
//
//	2007/04/28	真田	新規作成

#include "stdafx.h"
#include "Mutex.hpp"

namespace Javelin
{
	///	@brief	コンストラクタ
	Mutex::Mutex()
	{
		Handle = NULL ;
	}

	///	@brief	デストラクタ
	Mutex::~Mutex()
	{
		Close() ;
	}

	///	@brief	ミューテックス作成
	///	@param	name			ミューテックス名
	///	@param	initial_owner	== TRUE  : 作成後カレントスレッドで所有する\n
	///							== FALSE : 作成後カレントスレッドで所有しない
	///	@retval	ERROR_SUCCESS			Mutexを作成した
	///	@retval	ERROR_ALREADY_EXISTS	Mutexが存在した（オブジェクトは有効）
	///	@retval	その他					winerror.h準拠
	HRESULT Mutex::Create( LPCTSTR name, BOOL initial_owner )
	{
		Handle = ::CreateMutex( NULL, initial_owner, name ) ;
		return ::GetLastError() ;
	}

	///	@brief	ミューテックスを閉じる
	///	@return	winerror.h準拠
	HRESULT Mutex::Close()
	{
		if ( Handle == NULL ) return ERROR_SUCCESS ;

		HRESULT result = ::CloseHandle( Handle ) ? ERROR_SUCCESS : ::GetLastError() ;
		Handle = NULL ;
		return result ;
	}
}

// [EOF]
