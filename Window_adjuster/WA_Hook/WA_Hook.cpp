///	@fn		WA_Hook.cpp
///	@brief	フックDLLエントリ
///
//	2007/04/19	真田	新規作成

#include "stdafx.h"
#include "WA_Hook.h"
#include "Adjust_dialog.hpp"
#include "Misc.hpp"
#include "Auto_reset.hpp"
#include "Debug_tool.hpp"
#include "TString.hpp"

using namespace Javelin;

// #pragma warning ( disable : 4127 )	// 定数条件式の警告抑止

enum Adjust_type
{
	AUTO,
	MOVE_TOP,
	MOVE_BOTTOM,
	MOVE_LEFT,
	MOVE_RIGHT,
	MAXIMIZE_HEIGHT,
	MAXIMIZE_WIDTH,
} ;


// グローバルフック用変数
#pragma data_seg ( ".share" )
HHOOK Hook_handle = NULL ;
HWND Main_hwnd = NULL;
UINT RWM_ADJUST = WM_NULL;
UINT RWM_SET_PARAM = WM_NULL;
UINT RWM_SET_PARAM2 = WM_NULL;
#pragma data_seg ()
#pragma comment( linker, "/SECTION:.share,RWS" )

HMODULE Module_handle = NULL ;
BOOL Show_dialog = FALSE ;

// ローカル関数宣言
LRESULT Show_adjust_dialog( MOUSEHOOKSTRUCT& mhs, WPARAM wparam, LPARAM lparam ) ;
LRESULT Adjust( MOUSEHOOKSTRUCT& mhs, WINDOWPLACEMENT& window_placement, Adjust_type type, UINT hit_code = NULL ) ;

///	@brief	DLLエントリ
BOOL APIENTRY DllMain( HMODULE module_handle, DWORD reason_for_call, LPVOID )
{
	switch ( reason_for_call )
	{
		case DLL_PROCESS_ATTACH :
//			::Log_init(_T("c:\\temp\\WA_Hook.log"));	// debug
			LOG(_T("----- DLL_PROCESS_ATTACH -----"));
			Module_handle = module_handle;
			break ;

		case DLL_THREAD_ATTACH :
		case DLL_THREAD_DETACH :
			break;
		case DLL_PROCESS_DETACH :
			LOG(_T("----- DLL_PROCESS_DETACH -----"));
			break;
	}
    return TRUE ;
}

typedef BOOL (WINAPI *LPFN_ISWOW64MESSAGE) (void);

LPFN_ISWOW64MESSAGE fnIsWow64Message;

///	@brief	64bitメッセージかどうかの判定
BOOL IsWow64Msg()
{
    // IsWow64Message is not available on all supported versions of Windows
    // Use LoadLibrary to ensure that the DLL containing the function is loaded
    // and GetProcAddress to get a pointer to the function if available.

    fnIsWow64Message = (LPFN_ISWOW64MESSAGE) GetProcAddress(
        LoadLibrary(TEXT("user32")), "IsWow64Message");
  
    if (NULL != fnIsWow64Message)
    {        
        return (fnIsWow64Message());
    }
    else return FALSE;
}

///	@brief	マウスフックプロシージャ
static LRESULT CALLBACK Mouse_proc( int code, WPARAM wparam, LPARAM lparam )
{
	LRESULT result = 0 ;

	FOR_ONCE()	// エラー処理用
	{
		if (code < 0) break;
		if (code != HC_ACTION) break;
		if (lparam == NULL) break;

		const LPMOUSEHOOKSTRUCT mhs_ptr = (LPMOUSEHOOKSTRUCT)lparam;
		WINDOWPLACEMENT window_placement;
		window_placement.length = sizeof(window_placement);
		if (!::GetWindowPlacement(mhs_ptr->hwnd, &window_placement))
		{	// ウィンドウ位置情報取得失敗
			break;
		}
		if (window_placement.showCmd == SW_SHOWMAXIMIZED)
		{	// 最大化されている
			break;
		}

		switch (wparam)
		{
		case WM_NCLBUTTONDBLCLK:	// 左ダブルクリック
			switch (mhs_ptr->wHitTestCode)
			{
			case HTTOP:
			case HTBOTTOM:
				result = Adjust(*mhs_ptr, window_placement, MAXIMIZE_HEIGHT, mhs_ptr->wHitTestCode);
				break;

			case HTLEFT:
			case HTRIGHT:
				result = Adjust(*mhs_ptr, window_placement, MAXIMIZE_WIDTH, mhs_ptr->wHitTestCode);
				break;
			}
			break;

		case WM_NCRBUTTONUP:	// 右クリック
			LOG(_T("WM_NCRBUTTONUP"));
			{
				TString msg;
				msg.Format_message(_T("wHitTestCode = %1!d!\n"), mhs_ptr->wHitTestCode);
				LOG(msg);
			}
			if (mhs_ptr->wHitTestCode == HTCLOSE)
			{	// クローズボタン上
				result = Show_adjust_dialog(*mhs_ptr, wparam, lparam);
				break;
			}
#if 0	// うまくフックできない
			if (mhs_ptr->wHitTestCode == HTCAPTION && ::GetAsyncKeyState(VK_CONTROL) >= 0)
			{	// キャプション上でCtrlキーが押されている
				result = Show_adjust_dialog(*mhs_ptr, wparam, lparam);
				break;
			}
#endif
			if (mhs_ptr->wHitTestCode != HTMAXBUTTON)
			{	// 最大化ボタン上ではない
				break;
			}
			result = Adjust(*mhs_ptr, window_placement, AUTO);
			break;

		case WM_NCRBUTTONDBLCLK:	// 右ダブルクリック
			LOG(_T("WM_NCRBUTTONDBLCLK"));
			switch (mhs_ptr->wHitTestCode)
			{
			case HTTOP:
			case HTTOPLEFT:
			case HTTOPRIGHT:
				result = Adjust(*mhs_ptr, window_placement, MOVE_TOP, mhs_ptr->wHitTestCode);
				break;

			case HTBOTTOM:
			case HTBOTTOMLEFT:
			case HTBOTTOMRIGHT:
				result = Adjust(*mhs_ptr, window_placement, MOVE_BOTTOM, mhs_ptr->wHitTestCode);
				break;
			}

			if (!::GetWindowPlacement(mhs_ptr->hwnd, &window_placement))
			{	// ウィンドウ位置情報取得失敗
				break;
			}

			switch (mhs_ptr->wHitTestCode)
			{
			case HTLEFT:
			case HTTOPLEFT:
			case HTBOTTOMLEFT:
				result = Adjust(*mhs_ptr, window_placement, MOVE_LEFT, mhs_ptr->wHitTestCode);
				break;

			case HTRIGHT:
			case HTTOPRIGHT:
			case HTBOTTOMRIGHT:
				result = Adjust(*mhs_ptr, window_placement, MOVE_RIGHT, mhs_ptr->wHitTestCode);
				break;
			}
			break;

		case WM_NCMBUTTONUP:	// 中央クリック
			LOG(_T("WM_NCMBUTTONUP"));
			{
				SHORT ctrl_state = ::GetAsyncKeyState(VK_CONTROL);
				TString msg;
				msg.Format_message(_T("ctrl_state = %1!x!"), ctrl_state);
				LOG(msg);
				if (ctrl_state >= 0)
				{	// Ctrlキーが押されていない
					break;
				}
				result = Show_adjust_dialog(*mhs_ptr, wparam, lparam);
				break; 
			}
		}
	}

	LRESULT result2 = ::CallNextHookEx( Hook_handle, code, wparam, lparam) ;
	if ( result == 0 )
	{	// フック処理をしなかった
		// 他のフックの結果を返す
		return result2 ;
	}
	return result ;
}

///	@brief	調整ダイアログ表示
///	@param	mhs	マウスフック情報
///	@retval	0	処理しなかった
///	@retval	1	処理した
LRESULT Show_adjust_dialog( MOUSEHOOKSTRUCT& mhs, WPARAM wparam, LPARAM lparam )
{
	::SendMessage(Main_hwnd, RWM_SET_PARAM, (WPARAM)mhs.hwnd, mhs.wHitTestCode);
	::SendMessage(Main_hwnd, RWM_SET_PARAM2, mhs.pt.x, mhs.pt.y);
	::PostMessage(Main_hwnd, RWM_ADJUST, wparam, lparam);

#if 0
	if (Show_dialog) return 0;	// 既に表示している

	Javelin::Auto_reset< BOOL > auto_reset( Show_dialog, TRUE, FALSE ) ;

	Adjust_dialog adjust_dialog ;
	adjust_dialog.Set_instance( Module_handle ) ;
	adjust_dialog.Set_parent( mhs.hwnd ) ;
	adjust_dialog.Set_position( mhs.pt ) ;
	adjust_dialog.Do_modal() ;
#endif

	return 1 ;
}

///	@brief	ウィンドウ調整
///	@param	mhs					マウスフック情報
///	@param	window_placement	ウィンドウ位置情報
///	@param	type				調整種別\n
///								== AUTO :				自動\n
///								== MOVE_TOP :			上へ移動\n
///								== MOVE_BOTTOM :		下へ移動\n
///								== MOVE_LEFT :			左へ移動\n
///								== MOVE_RIGHT :			右へ移動\n
///								== MAXIMIZE_HEIGHT :	縦最大化\n
///								== MAXIMIZE_WIDTH :		横最大化
///	@param	hit_code			マウス位置情報\n
///								== NULL :		なし\n
///								== HTTOP :		上端\n
///								== HTBOTTOM :	下端\n
///								== HTLEFT :		左端\n
///								== HTRIGHT :	右端
///	@retval	0	処理しなかった
///	@retval	1	処理した
LRESULT Adjust( MOUSEHOOKSTRUCT& mhs, WINDOWPLACEMENT& window_placement, Adjust_type type, UINT hit_code )
{
LOG( _T( "Adjust()" ) ) ;
	const RECT& target = window_placement.rcNormalPosition ;

	// モニタ情報取得
	const HMONITOR monitor = ::MonitorFromWindow( mhs.hwnd, MONITOR_DEFAULTTONEAREST ) ;
	MONITORINFO monitor_info ;
	monitor_info.cbSize = sizeof ( monitor_info ) ;
	if ( !::GetMonitorInfo( monitor, &monitor_info ) )
	{
		// @@@@@ 致命的処理
		return 0 ;
	}
	const RECT& desktop = monitor_info.rcWork ;

	// デフォルト値計算
	int x = target.left ;
	int y = target.top ;
	int cx = target.right - target.left ;
	int cy = target.bottom - target.top ;
	int desktop_width = desktop.right - desktop.left ;
	int desktop_height = desktop.bottom - desktop.top ;

	switch ( type )
	{
		case AUTO :
			if ( cx == desktop_width / 2 && cy == desktop_height / 2 )
			{	// 1/4
				// 縦最大化
				cy = desktop_height ;
			}
			else
			{
				// 1/4にする
				cx = desktop_width / 2 ;
				cy = desktop_height / 2 ;
			}
			break ;

		case MOVE_TOP :
			y = desktop.top ;
			break ;

		case MOVE_BOTTOM :
			y = desktop.bottom - cy ;
			break ;
			
		case MOVE_LEFT :
			x = desktop.left ;
			break ;

		case MOVE_RIGHT :
			x = desktop.right - cx ;
			break ;

		case MAXIMIZE_HEIGHT :
			// 縦最大化
			// cy = ( cy == desktop_height ) ? desktop_height / 2 : desktop_height ;
			if ( cy != desktop_height )
			{
				cy = desktop_height ;
				break ;
			}
			cy = desktop_height / 2 ;
			if ( hit_code == HTTOP ) y = desktop.bottom - cy ;
			break ;

		case MAXIMIZE_WIDTH :
			// 横最大化
			// cx = ( cx == desktop_width ) ? desktop_width / 2 : desktop_width ;
			if ( cx != desktop_width )
			{
				cx = desktop_width ;
				break ;
			}
			cx = desktop_width / 2 ;
			if ( hit_code == HTLEFT ) x = desktop.right - cx ;
			break ;
	}

	// 補正
	if ( cx > desktop_width ) cx = desktop_width ;
	if ( cy > desktop_height ) cy = desktop_height ;
	if ( target.left + cx > desktop.right ) x = desktop.right - cx ;
	if ( target.top + cy > desktop.bottom ) y = desktop.bottom - cy ;

	if ( hit_code != NULL )
	{
		POINT point ;
		::GetCursorPos( &point ) ;
		switch ( hit_code )
		{
			case HTLEFT :
			case HTTOPLEFT :
			case HTBOTTOMLEFT :
				point.x = x ;
				break ;
			case HTRIGHT :
			case HTTOPRIGHT :
			case HTBOTTOMRIGHT :
				point.x = x + cx - 1 ;
				break ;
		}
		switch ( hit_code )
		{
			case HTTOP :
			case HTTOPLEFT :
			case HTTOPRIGHT :
				point.y = y ;
				break ;
			case HTBOTTOM :
			case HTBOTTOMLEFT :
			case HTBOTTOMRIGHT :
				point.y = y + cy - 1 ;
				break ;
		}
		::SetCursorPos( point.x, point.y ) ;
	}

	// ウィンドウ位置設定
	::SetWindowPos( mhs.hwnd, NULL, x, y, cx, cy, SWP_NOZORDER ) ;

	return 1 ;
}

///	@brief	フック有効化
///	@return	winerror.h準拠
WA_HOOK_API HRESULT WA_Enable_hook( HWND hwnd )
{
LOG(_T("WA_Enable_hook()"));
	if ( Hook_handle != NULL ) return ERROR_SUCCESS ;

	Hook_handle = ::SetWindowsHookEx( WH_MOUSE, ( HOOKPROC )Mouse_proc, Module_handle, 0 ) ;
	if ( Hook_handle == NULL ) return ::GetLastError() ;

TString msg;
msg.Format_message(_T("Hook_handle = %1!x!"), Hook_handle);
LOG(msg);

	Main_hwnd = hwnd ;
msg.Format_message(_T("Main_hwnd = %1!x!"), Main_hwnd);
LOG(msg);

	// ウィンドウメッセージ登録
	RWM_ADJUST = ::RegisterWindowMessage(RWM_ADJUST_MESSAGE);
	RWM_SET_PARAM = ::RegisterWindowMessage(RWM_SET_PARAM_MESSAGE);
	RWM_SET_PARAM2 = ::RegisterWindowMessage(RWM_SET_PARAM2_MESSAGE);

#if 0
	// OSバージョンチェック
	OSVERSIONINFO os_version_info ;
	os_version_info.dwOSVersionInfoSize = sizeof ( os_version_info ) ;
	os_version_info.dwMajorVersion = 6 ;
	os_version_info.dwMinorVersion = 0 ;
	DWORDLONG condition_mask = 0 ;
	const int op = VER_LESS_EQUAL ;
	VER_SET_CONDITION( condition_mask, VER_MAJORVERSION, op ) ;
	VER_SET_CONDITION( condition_mask, VER_MINORVERSION, op ) ;
	if ( ::VerifyVersionInfo( &os_version_info, VER_MAJORVERSION, condition_mask ) )
	{	// Vista以前
		// 
	}
#endif

	return ERROR_SUCCESS ;
}

///	@brief	フック無効化
///	@return	winerror.h
WA_HOOK_API HRESULT WA_Disable_hook()
{
LOG(_T("WA_Disable_hook()"));
TString msg;
msg.Format_message(_T("%1!x!"), Hook_handle);
LOG(msg);

	if (Hook_handle == NULL) return ERROR_SUCCESS;

	BOOL ret = ::UnhookWindowsHookEx( Hook_handle ) ;
	Hook_handle = NULL ;

	return ret ? ERROR_SUCCESS : ::GetLastError();
}

// [EOF]
