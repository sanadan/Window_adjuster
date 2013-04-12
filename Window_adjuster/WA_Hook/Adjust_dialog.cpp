///	@file	Adjust_dialog.cpp
///	@note	調整ダイアログクラス
//
//	2007/04/15	真田	新規作成

#include "stdafx.h"
#include "Adjust_dialog.hpp"
#include "resource.h"
#include <tchar.h>
#include "Debug_tool.hpp"
#include "Misc.hpp"

///	@brief	コンストラクタ
Adjust_dialog::Adjust_dialog() : Dialog( IDD_ADJUST )
{
	Last_window_placement.length = sizeof ( Last_window_placement ) ;
	Point.x = 0 ;
	Point.y = 0 ;
	Icon03 = NULL ;
	Icon06 = NULL ;
}

///	@brief	表示位置設定
///	@param	point	表示位置
void Adjust_dialog::Set_position( POINT& point )
{
	Point = point ;
}

///	@brief	ダイアログプロシージャ
///	@param	message	メッセージ
///	@param	wparam	WPARAM
///	@param	lparam	LPARAM
///	@retval	TRUE	メッセージを処理した
///	@retval	FALSE	メッセージを処理しなかった
INT_PTR Adjust_dialog::Dialog_proc( UINT message, WPARAM wparam, LPARAM lparam )
{
	switch ( message )
	{
		case WM_INITDIALOG :
			Dialog::Dialog_proc( message, wparam, lparam ) ;
			On_initdialog() ;
			return FALSE ;

		case WM_COMMAND :
			On_command( wparam ) ;
			break ;
	}

	return Dialog::Dialog_proc( message, wparam, lparam ) ;
}

///	@brief	ボタンにアイコンを設定
///	@param	control_id	コントロールID
///	@param	icon_id		アイコンリソースID
///	@note	リソースでボタンにBS_ICONの属性をつけておかないと、アイコンが表示されない
void Adjust_dialog::Set_icon( int control_id, int icon_id )
{
	HICON icon_handle = ( HICON )::LoadImage( Get_instance(), MAKEINTRESOURCE( icon_id ), IMAGE_ICON, 16, 16, LR_SHARED ) ;
	::SendMessage( Get_item( control_id ), BM_SETIMAGE, IMAGE_ICON, ( LPARAM )icon_handle ) ;
}

///	@brief	WM_INITDIALOGに対する処理
void Adjust_dialog::On_initdialog()
{
	const struct ICON_LIST
	{
		int control_id ;
		int icon_id ;
	} icon_list[] =
	{
		{ IDC_MOVE1, IDI_A01 },
		{ IDC_MOVE2, IDI_A02 },
		{ IDC_MOVE3, IDI_A03 },
		{ IDC_MOVE4, IDI_A04 },
		{ IDC_MOVE5, IDI_A05 },
		{ IDC_MOVE6, IDI_A06 },
		{ IDC_MOVE7, IDI_A07 },
		{ IDC_MOVE8, IDI_A08 },
		{ IDC_MOVE9, IDI_A09 },
		{ IDC_SIZE1, IDI_A01 },
		{ IDC_SIZE2, IDI_A02 },
		{ IDC_SIZE3, IDI_A03 },
		{ IDC_SIZE4, IDI_A04 },
		{ IDC_SIZE5, IDI_A05 },
		{ IDC_SIZE6, IDI_A06 },
		{ IDC_SIZE7, IDI_A07 },
		{ IDC_SIZE8, IDI_A08 },
		{ IDC_SIZE9, IDI_A09 },
		{ IDC_SIZE10, IDI_A08 },
		{ IDC_SIZE11, IDI_A06 },
	} ;
	for ( int i = 0 ; i < ARRAYSIZE( icon_list ) ; i++ )
	{
		Set_icon( icon_list[ i ].control_id, icon_list[ i ].icon_id ) ;
	}

	// 親ウィンドウの位置情報取得
	if ( !::GetWindowPlacement( Get_parent(), &Last_window_placement ) )
	{
		// @@@@@ 致命的処理
		FATAL_MESSAGE() ;
		End( IDCANCEL ) ;
	}

	// ダイアログの位置情報取得
	WINDOWPLACEMENT window_placement ;
	window_placement.length = sizeof ( window_placement ) ;
	if ( !::GetWindowPlacement( Get_handle(), &window_placement ) )
	{
		// @@@@@ 致命的処理
		FATAL_MESSAGE() ;
		End( IDCANCEL ) ;
	}
	const RECT& target = window_placement.rcNormalPosition ;

	// モニタ情報取得
	HMONITOR monitor = ::MonitorFromWindow( Get_handle(), MONITOR_DEFAULTTONEAREST ) ;
	MONITORINFO monitor_info ;
	monitor_info.cbSize = sizeof ( monitor_info ) ;
	if ( !::GetMonitorInfo( monitor, &monitor_info ) )
	{
		// @@@@@ 致命的処理
		FATAL_MESSAGE() ;
		End( IDCANCEL ) ;
	}
	const RECT& desktop = monitor_info.rcWork ;

	int x = Point.x ;
	int y = Point.y ;
	int cx = target.right - target.left ;
	int cy = target.bottom - target.top ;

	if ( x + cx > desktop.right ) x = desktop.right - cx ;
	if ( y + cy > desktop.bottom ) y = desktop.bottom - cy ;

	SetWindowPos( Get_handle(), NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER ) ;
}

///	@brief	WM_COMMANDに対する処理
///	@param	wparam	WM_COMMANDのWPARAM
void Adjust_dialog::On_command( WPARAM wparam )
{
	switch ( LOWORD( wparam ) )
	{
		case IDCANCEL :
			On_cancel() ;
			return ;
	}

	On_adjust_button( wparam ) ;
}

///	@brief	調整ボタン処理
///	@param	wParam	WM_COMMANDのWPARAM
void Adjust_dialog::On_adjust_button( WPARAM wparam )
{
	// 対象ウィンドウ情報取得
	WINDOWPLACEMENT window_placement ;
	window_placement.length = sizeof ( window_placement ) ;
	if ( !::GetWindowPlacement( Get_parent(), &window_placement ) )
	{
		// @@@@@ 致命的処理
		return ;
	}
	const RECT& target = window_placement.rcNormalPosition ;

	// モニタ情報取得
	HMONITOR monitor = ::MonitorFromWindow( Get_parent(), MONITOR_DEFAULTTONEAREST ) ;
	MONITORINFO monitor_info ;
	monitor_info.cbSize = sizeof ( monitor_info ) ;
	if ( !::GetMonitorInfo( monitor, &monitor_info ) )
	{
		// @@@@@ 致命的処理
		return ;
	}
	const RECT& desktop = monitor_info.rcWork ;

	// デフォルト値計算
	int x = target.left ;
	int y = target.top ;
	int cx = target.right - target.left ;
	int cy = target.bottom - target.top ;
	int desktop_width = desktop.right - desktop.left ;
	int desktop_height = desktop.bottom - desktop.top ;
	int dx = ( desktop_width ) / 8 ;
	int dy = ( desktop_height ) / 8 ;

	switch ( LOWORD( wparam ) )
	{
		case IDC_MOVE1 :	// 左上に移動
			x = desktop.left ;
			y = desktop.top ;
			break ;
		case IDC_MOVE2 :	// 上に移動
			y = desktop.top ;
			break ;
		case IDC_MOVE3 :	// 右上に移動
			x = desktop.right - ( target.right - target.left ) ;
			y = desktop.top ;
			break ;
		case IDC_MOVE4 :	// 左に移動
			x = desktop.left ;
			break ;
		case IDC_MOVE5 :	// 中央に移動
			x = desktop.left + ( desktop_width - ( target.right - target.left ) ) / 2 ;
			y = desktop.top + ( desktop_height - ( target.bottom - target.top ) ) / 2 ;
			break ;
		case IDC_MOVE6 :	// 右に移動
			x = desktop.right - ( target.right - target.left ) ;
			break ;
		case IDC_MOVE7 :	// 左下に移動
			x = desktop.left ;
			y = desktop.bottom - ( target.bottom - target.top ) ;
			break ;
		case IDC_MOVE8 :	// 下に移動
			y = desktop.bottom - ( target.bottom - target.top ) ;
			break ;
		case IDC_MOVE9 :	// 右下に移動
			x = desktop.right - ( target.right - target.left ) ;
			y = desktop.bottom - ( target.bottom - target.top ) ;
			break ;

		case IDC_SIZE1 :	// 縦横縮小
			cx -= dx ;
			cy -= dy ;
			break ;
		case IDC_SIZE2 :	// 縦縮小
			cy -= dy ;
			break ;
		case IDC_SIZE3 :	// 縦縮小／横拡大
			cx += dx ;
			cy -= dy ;
			break ;
		case IDC_SIZE4 :	// 横縮小
			cx -= dx ;
			break ;
		case IDC_SIZE5 :	// 縦横をデスクトップの1/2に
			cx = desktop_width / 2 ;
			cy = desktop_height / 2 ;
			break ;
		case IDC_SIZE6 :	// 横拡大
			cx += dx ;
			break ;
		case IDC_SIZE7 :	// 縦拡大／横縮小
			cx -= dx ;
			cy += dy ;
			break ;
		case IDC_SIZE8 :	// 縦拡大
			cy += dy ;
			break ;
		case IDC_SIZE9 :	// 縦横拡大
			cx += dx ;
			cy += dy ;
			break ;
		case IDC_SIZE10 :	// 縦最大化
			y = desktop.top ;
			cy = desktop_height ;
			break ;
		case IDC_SIZE11 :	// 横最大化
			x = desktop.left ;
			cx = desktop_width ;
			break ;

		default :	// その他
			return ;	// 何もしない
	}

	// 補正
	if ( cx > desktop_width ) cx = desktop_width ;
	if ( cy > desktop_height ) cy = desktop_height ;
	if ( target.left + cx > desktop.right ) x = desktop.right - cx ;
	if ( target.top + cy > desktop.bottom ) y = desktop.bottom - cy ;

	// ウィンドウ位置設定
	::SetWindowPos( Get_parent(), NULL, x, y, cx, cy, SWP_NOZORDER ) ;
}

///	@brief	キャンセルボタン処理
void Adjust_dialog::On_cancel()
{
	// ウィンドウを元の位置に戻す
	::SetWindowPlacement( Get_parent(), &Last_window_placement ) ;
}

// [EOF]
