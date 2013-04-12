///	@file	Application.hpp
///	@brief	アプリケーションクラス
//
//	2006/01/22	真田	新規作成

#pragma once

#include "Accelerator.hpp"
#include "Mutex.hpp"

namespace Javelin
{
	class Application
	{
	public :
		Application() ;

		HINSTANCE Get_instance() const ;

	protected :
		virtual HRESULT Initialize() ;
		BOOL Is_already_exists( LPCTSTR name ) ;
		int Get_show_command() const ;
		HRESULT Load_accelerator( WORD resource ) ;

	private :
		HINSTANCE Instance ;
		int Show_command ;
		Accelerator Accel ;
		Mutex Exist_mutex ;

		HRESULT Run() ;

		friend int WINAPI ::WinMain( HINSTANCE instance, HINSTANCE, LPSTR, int show_command ) ;

		// コピー禁止処理
		Application( const Application& ) ;
		Application& operator =( const Application& ) ;
	} ;
}	// namespace Javelin

// [EOF]
