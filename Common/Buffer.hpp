///	@file	Buffer.hpp
///	@brief	バッファクラス
///	@note	依存：Win32 API
//
//	2006/01/09	真田	新規作成

#pragma once

// インクルード
#include <windows.h>

// namespace宣言
namespace Javelin
{
	///	@brief	バッファクラス
	class Buffer
	{
	public :
		Buffer() ;
		Buffer( const void *data_ptr, SIZE_T size ) ;
		Buffer( const Buffer& buffer ) ;
		~Buffer() ;

		SIZE_T Get_size() const ;
		DWORD Set_size( SIZE_T size ) ;
		DWORD Resize( SIZE_T size ) ;
		DWORD Free() ;

		PVOID Get_ptr() const ;

		HRESULT Copy( const void *data_ptr, SIZE_T size ) ;
		Buffer& operator =( const Buffer& buffer ) ;

	protected :
		void Initialize_member() ;

	private :
		HANDLE Heap_handle ;	///< ヒープハンドル
		PVOID Ptr ;				///< バッファポインタ
		SIZE_T Size ;			///< バッファサイズ

	} ;
}	// namespace Javelin

// [EOF]
