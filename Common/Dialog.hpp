///	@file	Dialog.hpp
///	@brief	ダイアログクラス
///	@note	依存：Win32 API／Javelin::Window
//
//	2007/04/15	真田	新規作成

#pragma once

#include "Window.hpp"

namespace Javelin
{
	///	@brief	ダイアログクラス
	class Dialog : public Window
	{
	public :
		Dialog( WORD resource_ID ) ;

		// ダイアログ表示
		INT_PTR Do_modal() ;

		// ダイアログ表示後
		HRESULT End( INT_PTR result ) ;
		HWND Get_item( int control_id )
		{
			return ::GetDlgItem( Get_handle(), control_id ) ;
		}

	protected :
		static INT_PTR CALLBACK Static_dialog_proc( HWND window, UINT message, WPARAM wparam, LPARAM lparam ) ;
		virtual INT_PTR Dialog_proc( UINT message, WPARAM wparam, LPARAM lparam ) ;

	private :
		int Resource_ID ;	///< ダイアログリソースID

		// コピー禁止処理
		Dialog( const Dialog& ) ;
		Dialog& operator =( const Dialog& ) ;
	} ;
}

// [EOF]
