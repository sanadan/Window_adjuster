///	@file	Application.cpp
///	@brief	アプリケーションクラス
//
//	2006/01/22	真田	新規作成

#include "stdafx.h"
#include "Application.hpp"
#include "assert.h"

namespace Javelin
{
	Application *App_ptr = NULL ;

	///	@brief	コンストラクタ
	Application::Application()
	{
		assert( App_ptr == NULL ) ;

		App_ptr = this ;
		Instance = NULL ;
		Show_command = 0 ;
	}

	///	@brief	初期化
	///	@return	winerror.h準拠
	HRESULT Application::Initialize()
	{
		return ERROR_SUCCESS ;
	}

	///	@brief	多重起動チェック
	///	@retval	TRUE	既に起動している
	///	@retval	FALSE	最初の起動
	BOOL Application::Is_already_exists( LPCTSTR name )
	{
		HRESULT ret = Exist_mutex.Create( name ) ;
		assert( ret == ERROR_SUCCESS || ret == ERROR_ALREADY_EXISTS ) ;
		return ret != ERROR_SUCCESS ;
	}

	///	@brief	インスタンスハンドル取得
	///	@return	インスタンスハンドル
	HINSTANCE Application::Get_instance() const
	{
		return Instance ;
	}

	///	@brief	起動時の表示設定取得
	///	@return	表示設定
	int Application::Get_show_command() const
	{
		return Show_command ;
	}

	///	@brief	アクセラレータのロード
	///	@return	winerror.h準拠
	HRESULT Application::Load_accelerator( WORD resource )
	{
		Accel.Set_instance( Instance ) ;
		return Accel.Load( resource ) ;
	}

	///	@brief	メッセージループ
	///	@return	winerror.h準拠
	HRESULT Application::Run()
	{
		// メイン メッセージ ループ:
		MSG msg;
		while ( ::GetMessage( &msg, NULL, 0, 0 ) > 0 ) 
		{
			if ( Accel.Translate( msg ) )
			{
				continue ;
			}

			::TranslateMessage( &msg ) ;
			::DispatchMessage( &msg ) ;
		}

		return static_cast< HRESULT >( msg.wParam ) ;
	}
}	// namespace Javelin

///	@brief	アプリケーションエントリ
int WINAPI WinMain( HINSTANCE instance, HINSTANCE, LPSTR, int show_command )
{
	assert( Javelin::App_ptr != NULL ) ;

	Javelin::App_ptr->Instance = instance ;
	Javelin::App_ptr->Show_command = show_command ;
	HRESULT result = Javelin::App_ptr->Initialize() ;
	if ( result != ERROR_SUCCESS ) return result ;

	return Javelin::App_ptr->Run() ;
}

// [EOF]
