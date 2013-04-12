///	@file	Window.hpp
///	@brief	ウィンドウクラス
///	@note	依存：Win32 API／Javelin::TString
//
//	2006/06/20	真田	新規作成

#include "stdafx.h"
#include "Window.hpp"

namespace Javelin
{
	const DWORD Window::Text_length = 1024 ;

	///	@brief	コンストラクタ
	Window::Window()
	{
		::ZeroMemory( &Wcex, sizeof ( Wcex ) ) ;
		Wcex.cbSize = sizeof ( Wcex ) ;
		Wcex.style = CS_HREDRAW | CS_VREDRAW ;
		Wcex.lpfnWndProc = Static_window_proc ;
		Wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 ) ;

		Set_instance( ::GetModuleHandle( NULL ) ) ;

		Window_handle = NULL ;
		Parent = NULL ;
	}

	///	@brief	インスタンス設定
	///	@param	instance	インスタンスハンドル
	void Window::Set_instance( HINSTANCE instance )
	{
		Wcex.hInstance = instance ;
		Class_name.Set_instance( instance ) ;
	}

	///	@brief	インスタンス取得
	///	@return	インスタンスハンドル
	HINSTANCE Window::Get_instance()
	{
		return Wcex.hInstance ;
	}

	///	@brief	スタティックウィンドウプロシージャ
	///	@param	window_handle	ウィンドウハンドル
	///	@param	message			メッセージ
	///	@param	wparam			WPARAM
	///	@param	lparam			LPARAM
	///	@return	各メッセージに対する戻り値
	LRESULT CALLBACK Window::Static_window_proc( HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam )
	{
		Window *window_ptr = NULL ;
		if ( message == WM_CREATE )
		{
			CREATESTRUCT *create_ptr = reinterpret_cast< CREATESTRUCT * >( lparam ) ;
			::SetWindowLongPtr( window_handle, GWLP_USERDATA, reinterpret_cast< LONG_PTR >( create_ptr->lpCreateParams ) ) ;
			window_ptr = reinterpret_cast< Window * >( create_ptr->lpCreateParams ) ;
			window_ptr->Set_handle( window_handle ) ;
		}
		else
		{
			window_ptr = reinterpret_cast< Window * >( ::GetWindowLongPtr( window_handle, GWLP_USERDATA ) ) ;
		}
		if ( window_ptr != NULL ) return window_ptr->Window_proc( message, wparam, lparam ) ;

		return ::DefWindowProc( window_handle, message, wparam, lparam ) ;
	}

	///	@brief	ウィンドウプロシージャ
	///	@param	message	メッセージ
	///	@param	wparam	WPARAM
	///	@param	lparam	LPARAM
	///	@return	各メッセージに対する戻り値
	LRESULT Window::Window_proc( UINT message, WPARAM wparam, LPARAM lparam )
	{
		return ::DefWindowProc( Get_handle(), message, wparam, lparam ) ;
	}

	///	@brief	アイコンをリソースからロード
	///	@param	icon_ID	アイコンID
	///	@return	winerror.h準拠
	HRESULT Window::Load_icon( DWORD icon_ID )
	{
		Wcex.hIcon = ::LoadIcon( Get_instance(), MAKEINTRESOURCE( icon_ID ) ) ;
		if ( Wcex.hIcon == NULL ) return ::GetLastError() ;

		return ERROR_SUCCESS ;
	}

	///	@brief	メニューを設定
	///	@param	menu_ID	リソースのメニューID
	void Window::Set_menu( DWORD menu_ID )
	{
		Wcex.lpszMenuName = MAKEINTRESOURCE( menu_ID ) ;
	}

	///	@brief	テキスト設定
	///	@param	text	文字列
	///	@return	winerror.h準拠
	HRESULT Window::Set_text( LPCTSTR text )
	{
		HWND window = Get_handle() ;
		if ( window == NULL ) 
		{
			Text = text ;
			return ERROR_SUCCESS ;
		}

		if ( ::SetWindowText( window, text ) ) return ERROR_SUCCESS ;
		return ::GetLastError() ;
	}

	///	@brief	テキスト設定
	///	@param	string_ID	文字列リソースID
	///	@return	winerror.h準拠
	HRESULT Window::Set_text( WORD string_ID )
	{
		TString text ;
		text.Set_instance( Get_instance() ) ;
		text.Load_string( string_ID ) ;
		return Set_text( text.c_str() ) ;
	}

	///	@brief	親ウィンドウ設定
	///	@param	parent	親ウィンドウハンドル
	///	@return	winerror.h準拠
	HRESULT Window::Set_parent( HWND parent )
	{
		HWND window = Get_handle() ;
		if ( window == NULL ) 
		{
			Parent = parent ;
			return ERROR_SUCCESS ;
		}

		if ( ::SetParent( window, parent ) != NULL ) return ERROR_SUCCESS ;
		return ::GetLastError() ;
	}

	///	@brief	親ウィンドウハンドル取得
	///	@return	親ウィンドウハンドル
	HWND Window::Get_parent()
	{
		HWND window = Get_handle() ;
		if ( window == NULL ) 
		{
			return Parent ;
		}

		return ::GetParent( window ) ;
	}

	///	@brief	ウィンドウ作成
	///	@param	class_ID	ウィンドウクラス文字列ID
	///	@return	winerror.h準拠
	HRESULT Window::Create( DWORD class_ID )
	{
		if ( Wcex.hCursor == NULL )
			Wcex.hCursor = ::LoadCursor( NULL, IDC_ARROW ) ;

		Class_name.Load_string( class_ID ) ;
		Register_class( Class_name.c_str() ) ;
		Window_handle = ::CreateWindowEx( WS_EX_APPWINDOW, Class_name.c_str(), Text.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, Get_parent(), NULL, Get_instance(), this ) ;
		if ( Window_handle == NULL ) return ::GetLastError() ;

		return ERROR_SUCCESS ;
	}

	///	@brief	ウィンドウクラス登録
	///	@param	class_name	クラス名
	///	@return	winerror.h準拠
	HRESULT Window::Register_class( LPCTSTR class_name )
	{
		Wcex.lpszClassName = class_name ;

		if ( ::RegisterClassEx( &Wcex ) == 0 ) return ::GetLastError() ;
		return ERROR_SUCCESS ;
	}

	///	@brief	ウィンドウハンドル取得
	///	@return	ウィンドウハンドル
	HWND Window::Get_handle()
	{
		if ( this == NULL ) return NULL ;
		return Window_handle ;
	}

	///	@brief	ウィンドウハンドル設定
	///	@param	window_handle	ウィンドウハンドル
	void Window::Set_handle( HWND window_handle )
	{
		Window_handle = window_handle ;
	}

	///	@brief	ウィンドウ属性設定
	///	@param	index	属性インデックス（GWL_USERDATAなど）
	///	@param	value	値
	///	@return	winerror.h準拠
	HRESULT Window::Set_attribute( int index, LONG_PTR value )
	{
		::SetLastError( 0 ) ;
		::SetWindowLongPtr( Get_handle(), index, value ) ;
		return ::GetLastError() ;
	}

	///	@brief	ウィンドウ属性取得
	///	@param	index	属性インデックス（GWL_USERDATAなど）
	///	@return	値
	LONG_PTR Window::Get_attribute( int index )
	{
		return ::GetWindowLongPtr( Get_handle(), index ) ;
	}

	///	@brief	ウィンドウテキスト取得
	LPCTSTR Window::Get_text()
	{
		::GetWindowText( Get_handle(), Text.Get_buffer( Text_length ), Text_length ) ;
		Text.Release_buffer() ;
		return Text ;
	}

	///	@brief	メッセージ送信
	///	@param	message	メッセージ
	///	@param	wparam	WPARAM
	/// @param	lparam	LPARAM
	///	@return	メッセージに対する戻り値
	LRESULT Window::Send_message( UINT message, WPARAM wparam, LPARAM lparam )
	{
		return ::SendMessage( Get_handle(), message, wparam, lparam ) ;
	}

	///	@brief	メッセージポスト
	///	@param	message	メッセージ
	///	@param	wparam	WPARAM
	///	@param	lparam	LPARAM
	///	@return	winerror.h準拠
	HRESULT Window::Post_message( UINT message, WPARAM wparam, LPARAM lparam )
	{
		if ( !::PostMessage( Get_handle(), message, wparam, lparam ) )
			return ::GetLastError() ;
		return ERROR_SUCCESS ;
	}

	///	@brief	メッセージボックス表示
	///	@param	message	メッセージ
	///	@param	caption	メッセージボックスタイトル\n
	///					== NULL : MessageBox()標準のウィンドウタイトル
	///	@param	type	メッセージタイプ（MessageBox()準拠）
	///	@return	終了コード\n
	///			== 0 : エラー（詳細はGetLastError()で取得）
	int Window::Message_box( LPCTSTR message, LPCTSTR caption, UINT type )
	{
		if ( caption == NULL ) caption = Get_text() ;
		return ::MessageBox( Get_handle(), message, caption, type ) ;
	}

	///	@brief	ウィンドウ表示
	///	@param	command	コマンド（SW_SHOWなど）
	///	@retval	TRUE	以前の状態が表示中であった
	///	@retval	FALSE	以前の状態が非表示であった
	BOOL Window::Show( int command )
	{
		return ::ShowWindow( Get_handle(), command ) ;
	}

	///	@brief	ウィンドウの更新
	///	@return	winerror.h準拠
	HRESULT Window::Update()
	{
		if ( !::UpdateWindow( Get_handle() ) ) return ::GetLastError() ;
		return ERROR_SUCCESS ;
	}

	///	@breif	ウィンドウを前面に設定する
	///	@return	winerror.h準拠
	HRESULT Window::Set_foreground()
	{
		if ( !::SetForegroundWindow( Get_handle() ) ) return ::GetLastError() ;
		return ERROR_SUCCESS ;
	}
}

// [EOF]
