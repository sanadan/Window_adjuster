///	@brief	ウィンドウクラス
///	@note	依存：Win32 API／Javelin::TString
//
//	2006/06/20	真田	新規作成

#pragma once

#include "TString.hpp"

// 2007/05/10
// /Wp64の時、このSetWindowLongPtr/GetWindowLongPtrの定義の問題で警告が出るので、定義し直す
#if !defined( _WIN64 )
	#if defined( SetWindowLongPtr )
		#undef SetWindowLongPtr
	#endif
	static __inline LONG_PTR SetWindowLongPtr( HWND window, int index, LONG_PTR value )
	{
		return ::SetWindowLong( window, index, static_cast< LONG >( value ) ) ;
	}

	#if defined( GetWindowLongPtr )
		#undef GetWindowLongPtr
	#endif
	static __inline LONG_PTR GetWindowLongPtr( HWND window, int index )
	{
		return static_cast< LONG_PTR >( ::GetWindowLong( window, index ) ) ;
	}
#endif	// _WIN64

namespace Javelin
{
	class Window
	{
	public :
		// Create()前の設定関数
		Window() ;
		void Set_instance( HINSTANCE instance ) ;
		HINSTANCE Get_instance() ;
		HRESULT Load_icon( DWORD icon_ID ) ;
		void Set_menu( DWORD menu_ID ) ;

		// Create()後も使える関数
		HRESULT Set_text( LPCTSTR text ) ;
		HRESULT Set_text( WORD string_ID ) ;
		HRESULT Set_parent( HWND parent ) ;
		HWND Get_parent() ;
		
		HRESULT Create( DWORD class_ID ) ;

		// Create()後に使える関数
		HWND Get_handle() ;
		HRESULT Set_attribute( int index, LONG_PTR value ) ;
		LONG_PTR Get_attribute( int index ) ;
		LPCTSTR Get_text() ;

		LRESULT Send_message( UINT message, WPARAM wparam = 0, LPARAM lparam = 0 ) ;
		HRESULT Post_message( UINT message, WPARAM wparam = 0, LPARAM lparam = 0 ) ;

		int Message_box( LPCTSTR message, LPCTSTR caption = NULL, UINT type = MB_OK ) ;

		BOOL Show( int command ) ;
		HRESULT Update() ;
		HRESULT Set_foreground() ;

	protected :
		HRESULT Register_class( LPCTSTR class_name ) ;
		static LRESULT CALLBACK Static_window_proc( HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam ) ;
		virtual LRESULT Window_proc( UINT message, WPARAM wparam, LPARAM lparam ) ;
		void Set_handle( HWND window_handle ) ;

	private :
		WNDCLASSEX Wcex ;
		TString Class_name ;
		HWND Window_handle ;
		TString Text ;
		HWND Parent ;

		static const DWORD Text_length ;

		// コピー禁止処理
		Window( const Window& ) ;
		Window& operator =( const Window& ) ;
	} ;
}

// [EOF]
