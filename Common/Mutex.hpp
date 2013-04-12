///	@file	Mutex.hpp
///	@brief	ミューテックスクラス
//
//	2007/04/28	真田	新規作成

#pragma once

namespace Javelin
{
	///	@brief	ミューテックスクラス
	class Mutex
	{
	public :
		Mutex() ;
		~Mutex() ;

		HRESULT Create( LPCTSTR name, BOOL initial_owner = FALSE ) ;
		HRESULT Close() ;

	private :
		HANDLE Handle ;

		// コピー禁止処理
		Mutex( const Mutex& ) ;
		Mutex& operator =( const Mutex& ) ;
	} ;
}

// [EOF]
