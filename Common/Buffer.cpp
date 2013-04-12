///	@file	Buffer.cpp
///	@brief	メモリバッファクラス
///	@note	依存：Win32 API
//
//	2006/01/09	真田	新規作成

// インクルード
#include "stdafx.h"
#include "Buffer.hpp"

// namespace宣言
namespace Javelin
{
	///	@brief	コンストラクタ
	Buffer::Buffer()
	{
		Initialize_member() ;
	}

	///	@brief	バイナリデータからのコンストラクタ
	///	@param	data_ptr	データポインタ
	///	@size	size		データサイズ
	Buffer::Buffer( const void *data_ptr, SIZE_T size )
	{
		Initialize_member() ;

		Copy( data_ptr, size ) ;
	}

	///	@brief	コピーコンストラクタ
	///	@param	buffer	コピー元
	Buffer::Buffer( const Buffer& buffer )
	{
		Initialize_member() ;

		*this = buffer ;
	}

	///	@brief	メンバ変数初期化
	void Buffer::Initialize_member()
	{
		Heap_handle = NULL ;
		Ptr = NULL ;
		Size = 0 ;
	}

	///	@brief	デストラクタ
	Buffer::~Buffer()
	{
		Free() ;
	}

	///	@brief	サイズ取得
	///	@return	バッファサイズ（バイト）
	SIZE_T Buffer::Get_size() const
	{
		return Size ;
	}

	///	@brief	ポインタ取得
	///	@return	ポインタ\n
	///			== NULL : バッファが確保されていない
	PVOID Buffer::Get_ptr() const
	{
		return Ptr ;
	}

	///	@brief	メモリ確保
	///	@param	size	サイズ（バイト）
	///	@retval	ERROR_SUCCESS			正常終了
	///	@retval	ERROR_NOT_ENOUGH_MEMORY	メモリ不足
	///	@retval	ERROR_INVALID_HANDLE	プロセスのヒープハンドルが取得できなかった（内部エラー）
	///	@retval	ERROR_OUTOFMEMORY		確保成功したがサイズ取得に失敗（内部エラー）
	DWORD Buffer::Set_size( SIZE_T size )
	{
		Free() ;

		Heap_handle = ::GetProcessHeap() ;
		if ( Heap_handle == NULL )
		{	// ハンドルが取得できなかった（通常はあり得ないはず）
			return ERROR_INVALID_HANDLE ;
		}

		Ptr = ::HeapAlloc( Heap_handle, 0, size ) ;
		if ( Ptr == NULL )
		{	// メモリ不足
			return ERROR_NOT_ENOUGH_MEMORY ;
		}

		Size = size ;

		return ERROR_SUCCESS ;
	}

	///	@brief	メモリを確保していたら開放
	///	@retval	ERROR_SUCCESS	正常終了
	///	@retval	その他			WinError.hに準拠
	DWORD Buffer::Free()
	{
		if ( Ptr != NULL )
		{	// メモリを確保している
			BOOL ret_flg = ::HeapFree( Heap_handle, 0, Ptr ) ;

			Initialize_member() ;

			if ( !ret_flg )
			{	// エラーが発生していた
				return ::GetLastError() ;
			}
		}

		return ERROR_SUCCESS ;
	}

	///	@brief	メモリ再確保
	///	@param	size	サイズ（バイト）
	///	@return	winerror.h準拠
	///	@note	エラーでもデータは保持される
	DWORD Buffer::Resize( SIZE_T size )
	{
		if ( Ptr == NULL )
		{	// メモリ確保していない
			return Set_size( size ) ;
		}

		PVOID new_ptr = ::HeapReAlloc( Heap_handle, 0, Ptr, size ) ;
		if ( new_ptr == NULL )
		{	// 再確保失敗
			// 元のハンドルとポインタは有効なので、エラーを返すだけ
			return ::GetLastError() ;
		}

		Ptr = new_ptr ;
		Size = size ;

		return ERROR_SUCCESS ;
	}

	///	@brief	データコピー
	///	@param	data_ptr	データポインタ
	///	@param	size		データサイズ（バイト）
	///	@return	winerror.h準拠
	HRESULT Buffer::Copy( const void *data_ptr, SIZE_T size )
	{
		HRESULT result = Set_size( size ) ;
		if ( result != ERROR_SUCCESS ) return result ;

		::CopyMemory( Get_ptr(), data_ptr, size ) ;
		return ERROR_SUCCESS ;
	}

	///	@brief	コピー
	///	@param	buffer	コピー元
	///	@return	コピー先の参照
	Buffer& Buffer::operator =( const Buffer& buffer )
	{
		Copy( buffer.Get_ptr(), buffer.Get_size() ) ;
		return *this ;
	}
}	// namespace Javelin

// [EOF]
