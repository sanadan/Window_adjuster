///	@file	Main.cpp
///	@brief	メインクラス
//
//	2007/05/08	真田	新規作成

///	@mainpage
///	　このプログラムはウィンドウの位置を画面の端に合わせたり、大きさを画面の
///	縦横半分／縦最大化／横最大化などに調整するツールです。\n
///	　また、ウィンドウクラス単位のウィンドウ位置とアイコン名単位のデスクトッ
///	プアイコン位置を、画面サイズ毎に保存／復元することができます。

#include "stdafx.h"
#include "Main.hpp"
#include "Main_window.hpp"
#include "Debug_tool.hpp"
using namespace Javelin ;

Main App ;	///< メインアプリケーションオブジェクト
LPCTSTR Main::Title = _T( "Window adjuster" ) ;	///< アプリケーション名
LPCTSTR Main::Internal_name = _T( "Javelin_Software\\Window_adjuster" ) ;	///< 内部名／Application Data配下のフォルダ名に使用
LPCTSTR Main::Internal_id = _T( "Javelin software - Window adjuster" ) ;	///< 内部ID／二重起動のチェックに使用
UINT Main::RWM_QUIT = WM_NULL;	///< コマンドラインオプションによる終了メッセージ

Main_window Main_window ;	///< メインウィンドウクラス
LPCTSTR Main_window::Save_file_name_format = _T( "%1!d!-%2!d!.ini" ) ;	///< レイアウト情報保存先ファイル名フォーマット
LPCTSTR Main_window::Save_file_name_format2 = _T( "%1!d!-%2!d!_icon.ini" ) ;	///< レイアウト情報保存先ファイル名フォーマット2

///	@brief	初期化
///	@return	winerror.h準拠
HRESULT Main::Initialize()
{
	// 親クラスの初期化処理呼び出し
	HRESULT result = Application::Initialize() ;
	if ( result != ERROR_SUCCESS )
	{	// 通常はあり得ないエラー
		FATAL_MESSAGE() ;
		return result ;
	}

	// パラメータチェック
	for (int i = 0; i < Get_argv(); i++)
	{
		if (::_tcscmp(Get_argc()[i], L"/q") == 0)
		{	// 終了
			::PostMessage(HWND_BROADCAST, Get_quit_message(), 0, 0);
			return ERROR_ALREADY_EXISTS;
		}
	}

	// 二重起動のチェック
	if ( Is_already_exists( Get_internal_id() ) )
	{	// 既に起動している
		Error_message_box(_T("既に起動しています"));	// @@@@@
		return ERROR_ALREADY_EXISTS ;
	}

	// アクセラレータのロード
	Load_accelerator( IDM_MAIN ) ;

	// メインウィンドウ作成
	Main_window.Load_icon( IDI_MAIN ) ;
	Main_window.Set_menu( IDM_MAIN ) ;
	Main_window.Set_text( Get_title() ) ;
	result = Main_window.Create( IDS_MAIN_WINDOW_CLASS ) ;
	if ( result != ERROR_SUCCESS )
	{	// 通常はあり得ないエラー
		FATAL_MESSAGE() ;
		return result ;
	}

	// アプリケーションフォルダ設定
	result = Application_data_folder.Get_folder( CSIDL_APPDATA ) ;
	if ( result != ERROR_SUCCESS )
	{	// 通常はあり得ないエラー
		FATAL_MESSAGE() ;
		return result ;
	}
	Application_data_folder.Combine( Get_internal_name() ) ;

	return ERROR_SUCCESS ;
}

///	@brief	エラーメッセージボックス表示
///	@retval	IDOK	OKボタンが押された
int Main::Error_message_box( LPCTSTR message )
{
	return ::MessageBox( NULL, message, Get_title(), MB_OK | MB_ICONERROR ) ;
}

///
LPCTSTR Main::Get_title()
{
	return Main::Title ;
}

///
LPCTSTR Main::Get_internal_name()
{
	return Main::Internal_name ;
}

///
LPCTSTR Main::Get_internal_id()
{
	return Internal_id ;
}

///	@brief	アプリケーションデータフォルダパス取得
LPCTSTR Main::Get_application_data_folder()
{
	return App.Application_data_folder ;
}

///	@brief	アプリケーション終了メッセージ
UINT Main::Get_quit_message()
{
	if (RWM_QUIT == WM_NULL)
	{	// アプリケーション終了メッセージの登録
		RWM_QUIT = ::RegisterWindowMessage(_T("Javelin/Window_adjuster/RWM_QUIT"));
		if (RWM_QUIT == NULL)
		{
			FATAL_MESSAGE();	// 本来はあり得ない
		}
	}

	return RWM_QUIT;
}

// [EOF]
