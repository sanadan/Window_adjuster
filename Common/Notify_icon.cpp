///	@file	Notify_icon.cpp
///	@brief	タスクトレイアイコンクラス
//
//	2007/04/16	真田	新規作成

#include "stdafx.h"
#include "Notify_icon.hpp"
#include "TString.hpp"
#include "Misc.hpp"

namespace Javelin
{
	// アイコンサイズ
	const int Notify_icon::X_size = 16 ;
	const int Notify_icon::Y_size = 16 ;

	///	@brief	コンストラクタ
	Notify_icon::Notify_icon()
	{
		::ZeroMemory( &Notify_icon_data, sizeof ( Notify_icon_data ) ) ;
		Notify_icon_data.cbSize = sizeof ( Notify_icon_data ) ;
		Notify_icon_data.uVersion = NOTIFYICON_VERSION ;

		Set_instance( ::GetModuleHandle( NULL ) ) ;
	}

	///	@brief	デストラクタ
	Notify_icon::~Notify_icon()
	{
		if ( Notify_icon_data.hIcon != NULL )
		{
			::DestroyIcon( Notify_icon_data.hIcon ) ;
		}
	}

	///	@brief	インスタンス設定
	///	@param	instance	インスタンスハンドル
	void Notify_icon::Set_instance( HINSTANCE instance )
	{
		Instance = instance ;
	}

	///	@brief	インスタンス取得
	///	@return	インスタンスハンドル
	HINSTANCE Notify_icon::Get_instance()
	{
		return Instance ;
	}

	///	@brief	メッセージ通知先ウィンドウハンドル設定
	///	@param	window	ウィンドウハンドル
	void Notify_icon::Set_window_handle( HWND window )
	{
		Notify_icon_data.hWnd = window ;
	}

	///	@brief	タスクトレイイベント通知メッセージ設定
	///	@param	message	メッセージ
	void Notify_icon::Set_callback_message( UINT message )
	{
		Notify_icon_data.uCallbackMessage = message ;
		Notify_icon_data.uFlags |= NIF_MESSAGE ;
	}

	///	@brief	アイコン設定
	///	@param	resource_ID	リソースID
	void Notify_icon::Set_icon( WORD resource_ID )
	{
		if ( Notify_icon_data.hIcon != NULL )
		{
			::DestroyIcon( Notify_icon_data.hIcon ) ;
		}

		Notify_icon_data.hIcon = ( HICON )::LoadImage( Get_instance(), MAKEINTRESOURCE( resource_ID ), IMAGE_ICON, X_size, Y_size, LR_DEFAULTCOLOR ) ;
		Notify_icon_data.uFlags |= NIF_ICON ;
	}

	///	@brief	チップ文字列設定
	///	@param	tip	チップ文字列
	void Notify_icon::Set_tip( LPCTSTR tip )
	{
		::lstrcpyn( Notify_icon_data.szTip, tip, ARRAYSIZE( Notify_icon_data.szTip ) ) ;
		Notify_icon_data.uFlags |= NIF_TIP ;
	}

	///	@brief	チップ文字列設定
	///	@param	resource_ID	リソースID
	///	@return	winerror.h準拠
	HRESULT Notify_icon::Set_tip( WORD resource_ID )
	{
		TString tip ;
		HRESULT result = tip.Load_string( resource_ID ) ;
		if ( result != ERROR_SUCCESS ) return result ;

		Set_tip( tip.c_str() ) ;

		return ERROR_SUCCESS ;
	}

	///	@brief	アイコン追加
	///	@return	winerror.h準拠
	HRESULT Notify_icon::Add()
	{
		if ( !::Shell_NotifyIcon( NIM_ADD, &Notify_icon_data ) ) return ::GetLastError() ;

		if ( !::Shell_NotifyIcon( NIM_SETVERSION, &Notify_icon_data ) ) return ::GetLastError() ;

		return ERROR_SUCCESS ;
	}

	///	@brief	アイコン変更
	///	@return	winerror.h準拠
	HRESULT Notify_icon::Modify()
	{
		if ( ::Shell_NotifyIcon( NIM_MODIFY, &Notify_icon_data ) ) return ERROR_SUCCESS ;

		// まだ登録されていなかったので追加
		return Add() ;
	}

	///	@brief	アイコン削除
	///	@return	winerror.h準拠
	HRESULT Notify_icon::Delete()
	{
		if ( !::Shell_NotifyIcon( NIM_DELETE, &Notify_icon_data ) ) return ::GetLastError() ;

		return ERROR_SUCCESS ;
	}
}

// [EOF]
