///	@file	Dialog.cpp
///	@brief	ダイアログクラス
///	@note	依存：Win32 API／Javelin::Window
//
//	2007/04/15	真田	新規作成

#include "stdafx.h"
#include "Dialog.hpp"

namespace Javelin
{
	/// @brief	コンストラクタ
	///	@param	resource_ID	リソースID
	Dialog::Dialog( WORD resource_ID )
	{
		Resource_ID = resource_ID ;
	}

	/// @brief	ダイアログボックス表示
	///	@return	End()で設定された値
	INT_PTR Dialog::Do_modal()
	{
		return ::DialogBoxParam( Get_instance(), MAKEINTRESOURCE( Resource_ID ), Get_parent(), Static_dialog_proc, reinterpret_cast< LPARAM >( this ) ) ;
	}

	///	@brief	スタティックダイアログプロシージャ
	///	@note	パラメータと戻り値はDialogProc()に準拠
	INT_PTR CALLBACK Dialog::Static_dialog_proc( HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam )
	{
		Dialog *dialog_ptr = NULL ;
		if ( message == WM_INITDIALOG )
		{	// ダイアログ初期化
			// オブジェクト情報設定
			::SetWindowLongPtr( window_handle, DWLP_USER, static_cast< LONG_PTR >( lparam ) ) ;
			dialog_ptr = reinterpret_cast< Dialog * >( lparam ) ;
			dialog_ptr->Set_handle( window_handle ) ;
		}
		else
		{
			dialog_ptr = reinterpret_cast< Dialog * >( ::GetWindowLongPtr( window_handle, DWLP_USER ) ) ;
		}

		if ( dialog_ptr == NULL )
		{	// オブジェクトポインタ未設定
			return FALSE ;	// 処理しなかった
		}
		return dialog_ptr->Dialog_proc( message, wparam, lparam ) ;
	}

	///	@brief	ダイアログプロシージャ
	///	@note	パラメータと戻り値はDialogProc()に準拠
	INT_PTR Dialog::Dialog_proc( UINT message, WPARAM wparam, LPARAM )
	{
		switch ( message )
		{
			case WM_CLOSE :
				Post_message( WM_COMMAND, IDCANCEL ) ;
				Set_attribute( DWLP_MSGRESULT, 0 ) ;
				return TRUE ;	// 処理した
			
			case WM_COMMAND :
				switch ( LOWORD( wparam ) )
				{
					case IDOK :
						End( IDOK ) ;
						Set_attribute( DWLP_MSGRESULT, 0 ) ;
						return TRUE ;

					case IDCANCEL :
						End( IDCANCEL ) ;
						Set_attribute( DWLP_MSGRESULT, 0 ) ;
						return TRUE ;	// 処理した
				}
				break ;
		}

		return FALSE ;	// 処理しなかった
	}

	///	@brief	ダイアログ終了設定
	///	@result	終了コード
	HRESULT Dialog::End( INT_PTR result )
	{
		if ( ::EndDialog( Get_handle(), result ) ) return ERROR_SUCCESS ;
		return ::GetLastError() ;
	}
}

// [EOF]
