///	@file	Window.hpp
///	@brief	�E�B���h�E�N���X
///	@note	�ˑ��FWin32 API�^Javelin::TString
//
//	2006/06/20	�^�c	�V�K�쐬

#include "stdafx.h"
#include "Window.hpp"

namespace Javelin
{
	const DWORD Window::Text_length = 1024 ;

	///	@brief	�R���X�g���N�^
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

	///	@brief	�C���X�^���X�ݒ�
	///	@param	instance	�C���X�^���X�n���h��
	void Window::Set_instance( HINSTANCE instance )
	{
		Wcex.hInstance = instance ;
		Class_name.Set_instance( instance ) ;
	}

	///	@brief	�C���X�^���X�擾
	///	@return	�C���X�^���X�n���h��
	HINSTANCE Window::Get_instance()
	{
		return Wcex.hInstance ;
	}

	///	@brief	�X�^�e�B�b�N�E�B���h�E�v���V�[�W��
	///	@param	window_handle	�E�B���h�E�n���h��
	///	@param	message			���b�Z�[�W
	///	@param	wparam			WPARAM
	///	@param	lparam			LPARAM
	///	@return	�e���b�Z�[�W�ɑ΂���߂�l
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

	///	@brief	�E�B���h�E�v���V�[�W��
	///	@param	message	���b�Z�[�W
	///	@param	wparam	WPARAM
	///	@param	lparam	LPARAM
	///	@return	�e���b�Z�[�W�ɑ΂���߂�l
	LRESULT Window::Window_proc( UINT message, WPARAM wparam, LPARAM lparam )
	{
		return ::DefWindowProc( Get_handle(), message, wparam, lparam ) ;
	}

	///	@brief	�A�C�R�������\�[�X���烍�[�h
	///	@param	icon_ID	�A�C�R��ID
	///	@return	winerror.h����
	HRESULT Window::Load_icon( DWORD icon_ID )
	{
		Wcex.hIcon = ::LoadIcon( Get_instance(), MAKEINTRESOURCE( icon_ID ) ) ;
		if ( Wcex.hIcon == NULL ) return ::GetLastError() ;

		return ERROR_SUCCESS ;
	}

	///	@brief	���j���[��ݒ�
	///	@param	menu_ID	���\�[�X�̃��j���[ID
	void Window::Set_menu( DWORD menu_ID )
	{
		Wcex.lpszMenuName = MAKEINTRESOURCE( menu_ID ) ;
	}

	///	@brief	�e�L�X�g�ݒ�
	///	@param	text	������
	///	@return	winerror.h����
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

	///	@brief	�e�L�X�g�ݒ�
	///	@param	string_ID	�����񃊃\�[�XID
	///	@return	winerror.h����
	HRESULT Window::Set_text( WORD string_ID )
	{
		TString text ;
		text.Set_instance( Get_instance() ) ;
		text.Load_string( string_ID ) ;
		return Set_text( text.c_str() ) ;
	}

	///	@brief	�e�E�B���h�E�ݒ�
	///	@param	parent	�e�E�B���h�E�n���h��
	///	@return	winerror.h����
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

	///	@brief	�e�E�B���h�E�n���h���擾
	///	@return	�e�E�B���h�E�n���h��
	HWND Window::Get_parent()
	{
		HWND window = Get_handle() ;
		if ( window == NULL ) 
		{
			return Parent ;
		}

		return ::GetParent( window ) ;
	}

	///	@brief	�E�B���h�E�쐬
	///	@param	class_ID	�E�B���h�E�N���X������ID
	///	@return	winerror.h����
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

	///	@brief	�E�B���h�E�N���X�o�^
	///	@param	class_name	�N���X��
	///	@return	winerror.h����
	HRESULT Window::Register_class( LPCTSTR class_name )
	{
		Wcex.lpszClassName = class_name ;

		if ( ::RegisterClassEx( &Wcex ) == 0 ) return ::GetLastError() ;
		return ERROR_SUCCESS ;
	}

	///	@brief	�E�B���h�E�n���h���擾
	///	@return	�E�B���h�E�n���h��
	HWND Window::Get_handle()
	{
		if ( this == NULL ) return NULL ;
		return Window_handle ;
	}

	///	@brief	�E�B���h�E�n���h���ݒ�
	///	@param	window_handle	�E�B���h�E�n���h��
	void Window::Set_handle( HWND window_handle )
	{
		Window_handle = window_handle ;
	}

	///	@brief	�E�B���h�E�����ݒ�
	///	@param	index	�����C���f�b�N�X�iGWL_USERDATA�Ȃǁj
	///	@param	value	�l
	///	@return	winerror.h����
	HRESULT Window::Set_attribute( int index, LONG_PTR value )
	{
		::SetLastError( 0 ) ;
		::SetWindowLongPtr( Get_handle(), index, value ) ;
		return ::GetLastError() ;
	}

	///	@brief	�E�B���h�E�����擾
	///	@param	index	�����C���f�b�N�X�iGWL_USERDATA�Ȃǁj
	///	@return	�l
	LONG_PTR Window::Get_attribute( int index )
	{
		return ::GetWindowLongPtr( Get_handle(), index ) ;
	}

	///	@brief	�E�B���h�E�e�L�X�g�擾
	LPCTSTR Window::Get_text()
	{
		::GetWindowText( Get_handle(), Text.Get_buffer( Text_length ), Text_length ) ;
		Text.Release_buffer() ;
		return Text ;
	}

	///	@brief	���b�Z�[�W���M
	///	@param	message	���b�Z�[�W
	///	@param	wparam	WPARAM
	/// @param	lparam	LPARAM
	///	@return	���b�Z�[�W�ɑ΂���߂�l
	LRESULT Window::Send_message( UINT message, WPARAM wparam, LPARAM lparam )
	{
		return ::SendMessage( Get_handle(), message, wparam, lparam ) ;
	}

	///	@brief	���b�Z�[�W�|�X�g
	///	@param	message	���b�Z�[�W
	///	@param	wparam	WPARAM
	///	@param	lparam	LPARAM
	///	@return	winerror.h����
	HRESULT Window::Post_message( UINT message, WPARAM wparam, LPARAM lparam )
	{
		if ( !::PostMessage( Get_handle(), message, wparam, lparam ) )
			return ::GetLastError() ;
		return ERROR_SUCCESS ;
	}

	///	@brief	���b�Z�[�W�{�b�N�X�\��
	///	@param	message	���b�Z�[�W
	///	@param	caption	���b�Z�[�W�{�b�N�X�^�C�g��\n
	///					== NULL : MessageBox()�W���̃E�B���h�E�^�C�g��
	///	@param	type	���b�Z�[�W�^�C�v�iMessageBox()�����j
	///	@return	�I���R�[�h\n
	///			== 0 : �G���[�i�ڍׂ�GetLastError()�Ŏ擾�j
	int Window::Message_box( LPCTSTR message, LPCTSTR caption, UINT type )
	{
		if ( caption == NULL ) caption = Get_text() ;
		return ::MessageBox( Get_handle(), message, caption, type ) ;
	}

	///	@brief	�E�B���h�E�\��
	///	@param	command	�R�}���h�iSW_SHOW�Ȃǁj
	///	@retval	TRUE	�ȑO�̏�Ԃ��\�����ł�����
	///	@retval	FALSE	�ȑO�̏�Ԃ���\���ł�����
	BOOL Window::Show( int command )
	{
		return ::ShowWindow( Get_handle(), command ) ;
	}

	///	@brief	�E�B���h�E�̍X�V
	///	@return	winerror.h����
	HRESULT Window::Update()
	{
		if ( !::UpdateWindow( Get_handle() ) ) return ::GetLastError() ;
		return ERROR_SUCCESS ;
	}

	///	@breif	�E�B���h�E��O�ʂɐݒ肷��
	///	@return	winerror.h����
	HRESULT Window::Set_foreground()
	{
		if ( !::SetForegroundWindow( Get_handle() ) ) return ::GetLastError() ;
		return ERROR_SUCCESS ;
	}
}

// [EOF]
