///	@file	Notify_icon.hpp
///	@brief	タスクトレイアイコンクラス
//
//	2007/04/16	真田	新規作成

#pragma once

#include <shellapi.h>

namespace Javelin
{
	///	@brief	タスクトレイアイコンクラス
	class Notify_icon
	{
	public :
		Notify_icon() ;
		~Notify_icon() ;

		void Set_instance( HINSTANCE instance ) ;
		HINSTANCE Get_instance() ;
		void Set_window_handle( HWND window ) ;
		void Set_callback_message( UINT message ) ;
		void Set_icon( WORD resource_ID ) ;
		void Set_tip( LPCTSTR tip ) ;
		HRESULT Set_tip( WORD resource_ID ) ;

		HRESULT Add() ;
		HRESULT Modify() ;
		HRESULT Delete() ;

	private :
		NOTIFYICONDATA Notify_icon_data ;
		HINSTANCE Instance ;
		static const int X_size ;
		static const int Y_size ;

		// コピー禁止処理
		Notify_icon( const Notify_icon& ) ;
		Notify_icon& operator =( const Notify_icon& ) ;
	} ;
}

// [EOF]
