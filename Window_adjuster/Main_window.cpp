///	@file	Main_window.cpp
///	@brief	メインウィンドウクラス
//
//	2007/02/21	真田	新規作成

#include "stdafx.h"
#include "Main_window.hpp"
#include "resource.h"
#include "WA_Hook\\WA_Hook.h"
#include "Main.hpp"
#include "TString.hpp"
#include "Debug_tool.hpp"
#include "Misc.hpp"
#include "Process.hpp"
#include "Dialog.hpp"
#include "Adjust_dialog.hpp"
#include "Auto_reset.hpp"

using namespace Javelin;

extern Main App;

// #pragma warning ( disable : 4127 )

///	@brief	アバウトダイアログクラス
class About_dialog : public Javelin::Dialog
{
public :
	About_dialog() : Dialog( IDD_ABOUT )
	{
	}
} ;

///	@brief	コンストラクタ
Main_window::Main_window()
{
	RWM_TASKBARCREATED = WM_NULL ;
	RWM_ADJUST = WM_NULL;
	RWM_SET_PARAM = WM_NULL;
	Initialize(Mouse_hook_struct);
	Show_dialog = FALSE;
}

///	@brief	ウィンドウプロシージャ
///	@param	message	メッセージ
///	@param	wparam	WPARAM
///	@param	lparam	LPARAM
///	@return	各メッセージに対する戻り値
LRESULT Main_window::Window_proc( UINT message, WPARAM wparam, LPARAM lparam )
{
	switch( message ) 
	{
		case WM_CREATE :
			On_create() ;
			break ;

		case WM_DESTROY :
			On_destroy() ;
			break ;

		case WM_COMMAND :
		{
			// コマンド解析
			switch( LOWORD( wparam ) ) 
			{
				case IDM_ABOUT :
				{
					About_dialog about_dialog ;
					about_dialog.Do_modal() ;
					break ;
				}
				case IDM_EXIT :
					Post_message( WM_CLOSE ) ;
					break ;

				case ID_LAYOUT_SAVE :
					On_layout_save() ;
					break ;
				case ID_LAYOUT_LOAD :
					On_layout_load() ;
					break ;
				case ID_LAYOUT_CLEAR :
					On_layout_clear() ;
					break ;
			}
			break ;
		}

		case LWM_TRAY_CALLBACK :	// トレイアイコンでの操作
			On_notify_icon( lparam ) ;
			break ;

		default :
			if ( message == RWM_TASKBARCREATED )
			{	// タスクバーが再生成された
				// トレイアイコンの再登録
				Notify_icon.Modify() ;
			}
			else if (message == RWM_SET_PARAM)
			{	// SET_PARAMメッセージ
				Mouse_hook_struct.hwnd = (HWND)wparam;
				Mouse_hook_struct.wHitTestCode = (UINT)lparam;
			}
			else if (message == RWM_SET_PARAM2)
			{	// SET_PARAM2メッセージ
				Mouse_hook_struct.pt.x = (LONG)wparam;
				Mouse_hook_struct.pt.y = (LONG)lparam;
			}
			else if (message == RWM_ADJUST)
			{
				On_adjust(wparam, lparam);
			}
			else if (message == App.Get_quit_message())
			{	// コマンドラインオプションによる終了
				Post_message(WM_CLOSE);
			}
	}

	return Window::Window_proc( message, wparam, lparam ) ;
}

//	@brief	ウィンドウ調整
//	@param	wparam	メッセージ
//	@param	lparam	未使用
void Main_window::On_adjust( WPARAM wparam, LPARAM lparam )
{
	UNREFERENCED_PARAMETER(wparam);
	UNREFERENCED_PARAMETER(lparam);

// ::OutputDebugString(_T("On_adjust()\n"));
//	Message_box(_T("On_adjust()"));

	if (Show_dialog)
	{
// ::OutputDebugString(_T("Show_dialog\n"));
		return;	// 既に表示している
	}

	Javelin::Auto_reset< BOOL > auto_reset(Show_dialog, TRUE, FALSE);

	Adjust_dialog adjust_dialog;
	adjust_dialog.Set_instance(Get_instance());
//				adjust_dialog.Set_parent(Mouse_hook_struct.hwnd);
	adjust_dialog.Set_parent(NULL);
	adjust_dialog.Set_target(Mouse_hook_struct.hwnd);
	adjust_dialog.Set_position(Mouse_hook_struct.pt);
	adjust_dialog.Do_modal();
}

///	@brief	トレイアイコンでの操作
///	@param	lparam	メッセージ
void Main_window::On_notify_icon( LPARAM lparam )
{

	switch ( lparam )
	{
		case WM_RBUTTONUP :	// 右クリック
		{
			// メニュー表示
			Set_foreground() ;
			POINT point ;
			if ( !::GetCursorPos( &point ) )
			{
				FATAL_MESSAGE() ;	// 本来はあり得ない
				break ;
			}
			HMENU menu = ::LoadMenu( Get_instance(), MAKEINTRESOURCE( IDM_NOTIFY ) ) ;
			if ( menu == NULL )
			{
				FATAL_MESSAGE() ;	// 本来はあり得ない
				break ;
			}
			FOR_ONCE()
			{
				HMENU notify_menu = ::GetSubMenu( menu, 0 ) ;
				if ( notify_menu == NULL )
				{
					FATAL_MESSAGE() ;	// 本来はあり得ない
					break ;
				}
				::TrackPopupMenu( notify_menu, 0, point.x, point.y, 0, Get_handle(), NULL ) ;
			}
			::DestroyMenu( menu ) ;
			break ;
		}

		case WM_LBUTTONDBLCLK :	// 左ダブルクリック
			On_layout_load() ;
			break ;
	}
}

///	@brief	ウィンドウ作成時の処理
void Main_window::On_create()
{
	// タスクバー再生成メッセージの登録
	RWM_TASKBARCREATED = ::RegisterWindowMessage( _T( "TaskbarCreated" ) ) ;
	if ( RWM_TASKBARCREATED == NULL )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
	}

	// ADJUSTメッセージの登録
	RWM_ADJUST = ::RegisterWindowMessage(RWM_ADJUST_MESSAGE);
	if (RWM_ADJUST == NULL)
	{
		FATAL_MESSAGE();	// 本来はあり得ない
	}

	// SET_PARAMメッセージの登録
	RWM_SET_PARAM = ::RegisterWindowMessage(RWM_SET_PARAM_MESSAGE);
	if (RWM_SET_PARAM == NULL)
	{
		FATAL_MESSAGE();	// 本来はあり得ない
	}
	
	// SET_PARAM2メッセージの登録
	RWM_SET_PARAM2 = ::RegisterWindowMessage(RWM_SET_PARAM2_MESSAGE);
	if (RWM_SET_PARAM2 == NULL)
	{
		FATAL_MESSAGE();	// 本来はあり得ない
	}

	// フック設定
	WA_Enable_hook( Get_handle() ) ;
#if 0
TString msg;
msg.Format_message(_T("%1!x!"), Get_handle());
Message_box(msg);
#endif

	// トレイアイコン情報設定
	Notify_icon.Set_window_handle( Get_handle() ) ;
	Notify_icon.Set_callback_message( LWM_TRAY_CALLBACK ) ;
	Notify_icon.Set_icon( IDI_MAIN ) ;
	Notify_icon.Set_tip( IDS_APP_TITLE ) ;

	// トレイアイコン登録
	// リトライはとりあえず600回（約10分）としている
	for ( int i = 0 ; i < 600 && Notify_icon.Modify() == ERROR_TIMEOUT ; i++ )
	{	// 登録失敗
		// 1秒待つ
		::Sleep( 1000 ) ;	
	}
}

///	@brief	ウィンドウ破棄時の処理
void Main_window::On_destroy()
{
	// トレイアイコン削除
	Notify_icon.Delete() ;

	// フック解除
	HRESULT result = WA_Disable_hook() ;
	if (result != NOERROR)
	{
		TString msg;
		//msg.Format_message(_T("%1!d!"), result);
		msg.Format_message(result);
		Message_box(msg);
	}

	// アプリケーション終了メッセージをポスト
	::PostQuitMessage( 0 ) ;
}

///	@brief	デスクトップサイズによるデータファイル名取得（ウィンドウ用）
void Main_window::Get_file_name( TString& file_name )
{
	// デスクトップサイズ取得
	int virtual_screen_cx = ::GetSystemMetrics( SM_CXVIRTUALSCREEN ) ;
	int virtual_screen_cy = ::GetSystemMetrics( SM_CYVIRTUALSCREEN ) ;

	file_name.Format_message( Save_file_name_format, virtual_screen_cx, virtual_screen_cy ) ;
}

///	@brief	デスクトップサイズによるデータファイル名取得2
void Main_window::Get_file_name2( TString& file_name )
{
	// デスクトップサイズ取得
	int virtual_screen_cx = ::GetSystemMetrics( SM_CXVIRTUALSCREEN ) ;
	int virtual_screen_cy = ::GetSystemMetrics( SM_CYVIRTUALSCREEN ) ;

	file_name.Format_message( Save_file_name_format2, virtual_screen_cx, virtual_screen_cy ) ;
}

///	@brief	ウィンドウ状態の保存
void Main_window::On_layout_save()
{
	// アプリケーションデータフォルダ作成
	TPath data_folder = Main::Get_application_data_folder() ;
	HRESULT result = ::SHCreateDirectory( NULL, data_folder ) ;	// 複数階層のフォルダを一気に作ることができる
	if ( result != ERROR_SUCCESS && result != ERROR_ALREADY_EXISTS )
	{	// 既に存在する以外のエラー
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}

	// データファイルパス生成
	TString file_name ;
	Get_file_name( file_name ) ;
	TPath data_path = data_folder ;
	data_path.Combine( file_name ) ;

	// ウィンドウ列挙
//	int num = 0 ;
	for ( HWND handle = ::GetWindow( Get_handle(), GW_HWNDFIRST ) ; handle != NULL ; handle = ::GetWindow( handle, GW_HWNDNEXT ) )
	{
		::SetLastError( ERROR_SUCCESS ) ;	// GetParent()はエラーの場合しか設定しないようなので、クリアしておく
		if ( ::GetParent( handle ) != NULL ) continue ;	// 親ウィンドウあり（アプリケーションのトップウィンドウでない）のはスキップ
		if ( ::GetLastError() != ERROR_SUCCESS ) continue ;	// エラーだったらスキップ

		WINDOWINFO info ;
		info.cbSize = sizeof ( info ) ;
		if ( ::GetWindowInfo( handle, &info ) == 0 )
		{	// ウィンドウ情報取得失敗
			LOG_FATAL() ;	// 本来はあり得ない
			continue ;
		}
		if ( !( info.dwStyle & WS_VISIBLE ) ) continue ;	// 非表示ウィンドウはスキップ
		if ( info.dwStyle & WS_MINIMIZE ) continue ;	// 最小化ウィンドウはスキップ
		if ( info.dwStyle & WS_MAXIMIZE ) continue ;	// 最大化ウィンドウはスキップ

		TCHAR className[ MAX_PATH ] ;
		if ( ::GetClassName( handle, className, ARRAYSIZE( className ) ) == 0 )
		{	// クラス名取得失敗
			LOG_FATAL() ;	// 本来はあり得ない
			continue ;
		}
		if ( ::_tcscmp( className, _T( "Shell_TrayWnd" ) ) == 0 ) continue ;	// タスクトレイはスキップ
		if ( ::_tcscmp( className, _T( "Progman" ) ) == 0 ) continue ;	// プログラムマネージャはスキップ
		if ( ::_tcscmp( className, _T( "#32770" ) ) == 0 ) continue ;	// ダイアログは隠しのものがあったりするので、対象外にする

		RECT rect ;
		if ( ::GetWindowRect( handle, &rect ) == 0 )
		{	// ウィンドウ位置取得失敗
			LOG_FATAL() ;	// 本来はあり得ない
			continue ;
		}
		if ( rect.bottom - rect.top == 0 && rect.right - rect.left == 0 ) continue ;	// 大きさが0になっているものはスキップ

		// ここまで残ったウィンドウが対象
		// ウィンドウ位置保存
		TString left ;
		left.From_int( rect.left ) ;
		::WritePrivateProfileString( className, _T( "left" ), left, data_path ) ;
		TString top ;
		top.From_int( rect.top ) ;
		::WritePrivateProfileString( className, _T( "top" ), top, data_path ) ;
		TString right ;
		right.From_int( rect.right ) ;
		::WritePrivateProfileString( className, _T( "right" ), right, data_path ) ;
		TString bottom ;
		bottom.From_int( rect.bottom ) ;
		::WritePrivateProfileString( className, _T( "bottom" ), bottom, data_path ) ;

#if 0	// debug
TString num_str ;
num_str.FormatMessage( _T( "%1!d!:" ), num ) ;
num++ ;
::OutputDebugString( num_str ) ;
TCHAR title[ MAX_PATH ] ;
::GetWindowText( handle, title, ARRAYSIZE( title ) ) ;
::OutputDebugString( title ) ;
::OutputDebugString( _T( "(" ) ) ;
::GetClassName( handle, title, ARRAYSIZE( title ) ) ;
::OutputDebugString( title ) ;
::OutputDebugString( _T( ")" ) ) ;
TString rect_str ;
rect_str.FormatMessageW( _T( "%1!d!,%2!d!-%3!d!,%4!d!" ), rect.left, rect.top, rect.right, rect.bottom ) ;
::OutputDebugString( rect_str ) ;
::OutputDebugString( _T( "\n" ) ) ;
#endif
	}

	// データファイルパス2生成
	TString file_name2 ;
	Get_file_name2( file_name2 ) ;
	TPath data_path2 = data_folder ;
	data_path2.Combine( file_name2 ) ;

	// デスクトップリストコントロール取得
	HWND desktop = ::FindWindow( _T( "ProgMan" ), NULL ) ;
	desktop = ::GetWindow( desktop, GW_CHILD ) ;
	desktop = ::GetWindow( desktop, GW_CHILD ) ;
	if ( desktop == NULL )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}

	// デスクトップリストコントロールのプロセスを開く
	Process process ;
	result = process.Open( desktop, PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE ) ;
	if ( result != ERROR_SUCCESS )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}

	// デスクトップリストコントロールのメモリ空間でメモリ割り当て
	ProcessMemory< POINT > point( process ) ;
	if ( point.Allocate( PAGE_READWRITE ) != ERROR_SUCCESS )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}
	ProcessMemory< LV_ITEM > lv_item( process ) ;
	if ( lv_item.Allocate( PAGE_READWRITE ) != ERROR_SUCCESS )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}
	ProcessMemory< TCHAR[ MAX_PATH ] > text( process ) ;
	if ( text.Allocate( PAGE_READWRITE ) != ERROR_SUCCESS )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}

	// デスクトップリストコントロールの情報取得のための構造体設定
	int i = 0 ;
	if ( !::WriteProcessMemory( process.Get_handle(), &( lv_item.Get_ptr()->iSubItem ), &i, sizeof ( i ), NULL ) )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}
	i = MAX_PATH ;
	if ( !::WriteProcessMemory( process.Get_handle(), &( lv_item.Get_ptr()->cchTextMax ), &i, sizeof ( i ), NULL ) )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}
	LPTSTR text_ptr2 = reinterpret_cast< LPTSTR >( text.Get_ptr() ) ;
	if ( !::WriteProcessMemory( process.Get_handle(), &( lv_item.Get_ptr()->pszText ), &text_ptr2, sizeof ( text_ptr2 ), NULL ) )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}

	// デスクトップアイコン位置保存
	for ( int i = 0 ; i < ListView_GetItemCount( desktop ) ; i++ )
	{
		POINT icon_pos ;
		ListView_GetItemPosition( desktop, i, point.Get_ptr() ) ;
		::ReadProcessMemory( process.Get_handle(), point.Get_ptr(), &icon_pos, sizeof ( icon_pos ), NULL ) ;

		TCHAR icon_text[ MAX_PATH ] ;
		::SendMessage( desktop, LVM_GETITEMTEXT, i, reinterpret_cast< LPARAM >( lv_item.Get_ptr() ) ) ;
		::ReadProcessMemory( process.Get_handle(), text.Get_ptr(), icon_text, sizeof( icon_text ), NULL ) ;

		TString x ;
		x.From_int( icon_pos.x ) ;
		::WritePrivateProfileString( icon_text, _T( "x" ), x, data_path2 ) ;
		TString y ;
		y.From_int( icon_pos.y ) ;
		::WritePrivateProfileString( icon_text, _T( "y" ), y, data_path2 ) ;

#if 0	// debug
TString pos_str ;
pos_str.FormatMessage( _T( "%1 : %2!d!, %3!d!\n" ), icon_text, icon_pos.x, icon_pos.y ) ;
::OutputDebugString( pos_str ) ;
#endif
	}

	Message_box( _T( "保存しました" ), NULL, MB_OK | MB_ICONINFORMATION ) ;	// @@@@@
}

///	@brief	ウィンドウ状態の読み込み
void Main_window::On_layout_load()
{
	// データファイルパス生成
	TPath data_folder = Main::Get_application_data_folder() ;
	TString file_name ;
	Get_file_name( file_name ) ;
	TPath data_path = data_folder ;
	data_path.Combine( file_name ) ;

	// ウィンドウ位置列挙
//	int num = 0 ;
	for ( HWND handle = ::GetWindow( Get_handle(), GW_HWNDFIRST ) ; handle != NULL ; handle = ::GetWindow( handle, GW_HWNDNEXT ) )
	{
		::SetLastError( ERROR_SUCCESS ) ;	// GetParent()はエラーの場合しか設定しないようなので、クリアしておく
		if ( ::GetParent( handle ) != NULL ) continue ;	// 親ウィンドウあり（アプリケーションのトップウィンドウでない）のはスキップ
		if ( ::GetLastError() != ERROR_SUCCESS ) continue ;	// エラーだったらスキップ

		WINDOWINFO info ;
		info.cbSize = sizeof ( info ) ;
		if ( ::GetWindowInfo( handle, &info ) == 0 )
		{	// ウィンドウ情報取得失敗
			LOG_FATAL() ;	// 本来はあり得ない
			continue ;
		}
		if ( !( info.dwStyle & WS_VISIBLE ) ) continue ;	// 非表示ウィンドウはスキップ
		if ( info.dwStyle & WS_MINIMIZE ) continue ;	// 最小化ウィンドウはスキップ
		if ( info.dwStyle & WS_MAXIMIZE ) continue ;	// 最大化ウィンドウはスキップ

		TCHAR className[ MAX_PATH ] ;
		if ( ::GetClassName( handle, className, ARRAYSIZE( className ) ) == 0 )
		{	// クラス名取得失敗
			LOG_FATAL() ;	// 本来はあり得ない
			continue ;
		}

		// ここまで残ったウィンドウが対象
		// GetPrivateProfileString()で取得しているのは、GetPrivateProfileInt()だと対応する情報がない場合を判別するのが難しいため
		const DWORD buf_size = 16 ;	// 16文字
		TCHAR left_str[ buf_size ] ;
		if ( ::GetPrivateProfileString( className, _T( "left" ), _T( "" ), left_str, buf_size, data_path ) == 0 ) continue ;
		int left = ::_ttoi( left_str ) ;
		TCHAR top_str[ buf_size ] ;
		if ( ::GetPrivateProfileString( className, _T( "top" ), _T( "" ), top_str, buf_size, data_path ) == 0 ) continue ; 
		int top = ::_ttoi( top_str ) ;
		TCHAR right_str[ buf_size ] ;
		if ( ::GetPrivateProfileString( className, _T( "right" ), _T( "" ), right_str, buf_size, data_path ) == 0 ) continue ; 
		int right = ::_ttoi( right_str ) ;
		TCHAR bottom_str[ buf_size ] ;
		if ( ::GetPrivateProfileString( className, _T( "bottom" ), _T( "" ), bottom_str, buf_size, data_path ) == 0 ) continue ; 
		int bottom = ::_ttoi( bottom_str ) ;

		// ウィンドウ位置設定
		::SetWindowPos( handle, NULL, left, top, right - left, bottom - top, SWP_NOACTIVATE | SWP_NOZORDER ) ;

#if 0	// debug
TString num_str ;
num_str.FormatMessage( _T( "%1!d!:" ), num ) ;
num++ ;
::OutputDebugString( num_str ) ;
TCHAR title[ MAX_PATH ] ;
::GetWindowText( handle, title, ARRAYSIZE( title ) ) ;
::OutputDebugString( title ) ;
::OutputDebugString( _T( "(" ) ) ;
::GetClassName( handle, title, ARRAYSIZE( title ) ) ;
::OutputDebugString( title ) ;
::OutputDebugString( _T( ")" ) ) ;
TString rect_str ;
rect_str.FormatMessageW( _T( "%1!d!,%2!d!-%3!d!,%4!d!" ), rect.left, rect.top, rect.right, rect.bottom ) ;
::OutputDebugString( rect_str ) ;
::OutputDebugString( _T( "\n" ) ) ;
#endif
	}

	// データファイルパス2生成
	TString file_name2 ;
	Get_file_name2( file_name2 ) ;
	TPath data_path2 = data_folder ;
	data_path2.Combine( file_name2 ) ;

	// デスクトップリストコントロール取得
	HWND desktop = ::FindWindow( _T( "ProgMan" ), NULL ) ;
	desktop = ::GetWindow( desktop, GW_CHILD ) ;
	desktop = ::GetWindow( desktop, GW_CHILD ) ;
	if ( desktop == NULL )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}

	// デスクトップリストコントロールのプロセスを開く
	Process process ;
	HRESULT result = process.Open( desktop, PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE ) ;
	if ( result != ERROR_SUCCESS )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}

	// デスクトップリストコントロールのメモリ空間でメモリ割り当て
	ProcessMemory< POINT > point( process ) ;
	if ( point.Allocate( PAGE_READWRITE ) != ERROR_SUCCESS )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}
	ProcessMemory< LV_ITEM > lv_item( process ) ;
	if ( lv_item.Allocate( PAGE_READWRITE ) != ERROR_SUCCESS )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}
	ProcessMemory< TCHAR[ MAX_PATH ] > text( process ) ;
	if ( text.Allocate( PAGE_READWRITE ) != ERROR_SUCCESS )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}

	// デスクトップリストコントロールの情報取得のための構造体設定
	int i = 0 ;
	if ( !::WriteProcessMemory( process.Get_handle(), &( lv_item.Get_ptr()->iSubItem ), &i, sizeof ( i ), NULL ) )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}
	i = MAX_PATH ;
	if ( !::WriteProcessMemory( process.Get_handle(), &( lv_item.Get_ptr()->cchTextMax ), &i, sizeof ( i ), NULL ) )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}
	LPTSTR text_ptr2 = reinterpret_cast< LPTSTR >( text.Get_ptr() ) ;
	if ( !::WriteProcessMemory( process.Get_handle(), &( lv_item.Get_ptr()->pszText ), &text_ptr2, sizeof ( text_ptr2 ), NULL ) )
	{
		FATAL_MESSAGE() ;	// 本来はあり得ない
		return ;
	}

	// デスクトップアイコン位置保存
	for ( int i = 0 ; i < ListView_GetItemCount( desktop ) ; i++ )
	{
		POINT icon_pos ;
		ListView_GetItemPosition( desktop, i, point.Get_ptr() ) ;
		::ReadProcessMemory( process.Get_handle(), point.Get_ptr(), &icon_pos, sizeof ( icon_pos ), NULL ) ;

		TCHAR icon_text[ MAX_PATH ] ;
		::SendMessage( desktop, LVM_GETITEMTEXT, i, reinterpret_cast< LPARAM >( lv_item.Get_ptr() ) ) ;
		::ReadProcessMemory( process.Get_handle(), text.Get_ptr(), icon_text, sizeof( icon_text ), NULL ) ;

		const DWORD buf_size = 16 ;	// 16文字
		TCHAR x_str[ buf_size ] ;
		if ( ::GetPrivateProfileString( icon_text, _T( "x" ), _T( "" ), x_str, buf_size, data_path2 ) == 0 ) continue ;
		int x = ::_ttoi( x_str ) ;
		TCHAR y_str[ buf_size ] ;
		if ( ::GetPrivateProfileString( icon_text, _T( "y" ), _T( "" ), y_str, buf_size, data_path2 ) == 0 ) continue ; 
		int y = ::_ttoi( y_str ) ;

		// アイコン位置設定
		ListView_SetItemPosition( desktop, i, x, y ) ;

#if 0	// debug
TString pos_str ;
pos_str.FormatMessage( _T( "%1 : %2!d!, %3!d!\n" ), icon_text, icon_pos.x, icon_pos.y ) ;
::OutputDebugString( pos_str ) ;
#endif
	}
}

///	@brief	ウィンドウ状態の消去
void Main_window::On_layout_clear()
{
	// データファイルパス生成
	TPath data_folder = Main::Get_application_data_folder() ;
	TString file_name ;
	Get_file_name( file_name ) ;
	TPath data_path = data_folder ;
	data_path.Combine( file_name ) ;

	// データファイル削除
	if ( ::DeleteFile( data_path ) == 0 )
	{	// エラー
		if ( ::GetLastError() != ERROR_FILE_NOT_FOUND )
		{
			FATAL_MESSAGE() ;	// 本来はあり得ない
		}
	}

	// データファイルパス2生成
	TString file_name2 ;
	Get_file_name2( file_name2 ) ;
	TPath data_path2 = data_folder ;
	data_path2.Combine( file_name2 ) ;

	// データファイル削除
	if ( ::DeleteFile( data_path2 ) == 0 )
	{	// エラー
		if ( ::GetLastError() != ERROR_FILE_NOT_FOUND )
		{
			FATAL_MESSAGE() ;	// 本来はあり得ない
		}
	}

	Message_box( _T( "消去しました" ), NULL, MB_OK | MB_ICONINFORMATION ) ;	// @@@@@
}

// [EOF]
