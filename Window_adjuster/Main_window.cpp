///	@file	Main_window.cpp
///	@brief	���C���E�B���h�E�N���X
//
//	2007/02/21	�^�c	�V�K�쐬

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

///	@brief	�A�o�E�g�_�C�A���O�N���X
class About_dialog : public Javelin::Dialog
{
public :
	About_dialog() : Dialog( IDD_ABOUT )
	{
	}
} ;

///	@brief	�R���X�g���N�^
Main_window::Main_window()
{
	RWM_TASKBARCREATED = WM_NULL ;
	RWM_ADJUST = WM_NULL;
	RWM_SET_PARAM = WM_NULL;
	Initialize(Mouse_hook_struct);
	Show_dialog = FALSE;
}

///	@brief	�E�B���h�E�v���V�[�W��
///	@param	message	���b�Z�[�W
///	@param	wparam	WPARAM
///	@param	lparam	LPARAM
///	@return	�e���b�Z�[�W�ɑ΂���߂�l
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
			// �R�}���h���
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

		case LWM_TRAY_CALLBACK :	// �g���C�A�C�R���ł̑���
			On_notify_icon( lparam ) ;
			break ;

		default :
			if ( message == RWM_TASKBARCREATED )
			{	// �^�X�N�o�[���Đ������ꂽ
				// �g���C�A�C�R���̍ēo�^
				Notify_icon.Modify() ;
			}
			else if (message == RWM_SET_PARAM)
			{	// SET_PARAM���b�Z�[�W
				Mouse_hook_struct.hwnd = (HWND)wparam;
				Mouse_hook_struct.wHitTestCode = (UINT)lparam;
			}
			else if (message == RWM_SET_PARAM2)
			{	// SET_PARAM2���b�Z�[�W
				Mouse_hook_struct.pt.x = (LONG)wparam;
				Mouse_hook_struct.pt.y = (LONG)lparam;
			}
			else if (message == RWM_ADJUST)
			{
				On_adjust(wparam, lparam);
			}
			else if (message == App.Get_quit_message())
			{	// �R�}���h���C���I�v�V�����ɂ��I��
				Post_message(WM_CLOSE);
			}
	}

	return Window::Window_proc( message, wparam, lparam ) ;
}

//	@brief	�E�B���h�E����
//	@param	wparam	���b�Z�[�W
//	@param	lparam	���g�p
void Main_window::On_adjust( WPARAM wparam, LPARAM lparam )
{
	UNREFERENCED_PARAMETER(wparam);
	UNREFERENCED_PARAMETER(lparam);

// ::OutputDebugString(_T("On_adjust()\n"));
//	Message_box(_T("On_adjust()"));

	if (Show_dialog)
	{
// ::OutputDebugString(_T("Show_dialog\n"));
		return;	// ���ɕ\�����Ă���
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

///	@brief	�g���C�A�C�R���ł̑���
///	@param	lparam	���b�Z�[�W
void Main_window::On_notify_icon( LPARAM lparam )
{

	switch ( lparam )
	{
		case WM_RBUTTONUP :	// �E�N���b�N
		{
			// ���j���[�\��
			Set_foreground() ;
			POINT point ;
			if ( !::GetCursorPos( &point ) )
			{
				FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
				break ;
			}
			HMENU menu = ::LoadMenu( Get_instance(), MAKEINTRESOURCE( IDM_NOTIFY ) ) ;
			if ( menu == NULL )
			{
				FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
				break ;
			}
			FOR_ONCE()
			{
				HMENU notify_menu = ::GetSubMenu( menu, 0 ) ;
				if ( notify_menu == NULL )
				{
					FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
					break ;
				}
				::TrackPopupMenu( notify_menu, 0, point.x, point.y, 0, Get_handle(), NULL ) ;
			}
			::DestroyMenu( menu ) ;
			break ;
		}

		case WM_LBUTTONDBLCLK :	// ���_�u���N���b�N
			On_layout_load() ;
			break ;
	}
}

///	@brief	�E�B���h�E�쐬���̏���
void Main_window::On_create()
{
	// �^�X�N�o�[�Đ������b�Z�[�W�̓o�^
	RWM_TASKBARCREATED = ::RegisterWindowMessage( _T( "TaskbarCreated" ) ) ;
	if ( RWM_TASKBARCREATED == NULL )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
	}

	// ADJUST���b�Z�[�W�̓o�^
	RWM_ADJUST = ::RegisterWindowMessage(RWM_ADJUST_MESSAGE);
	if (RWM_ADJUST == NULL)
	{
		FATAL_MESSAGE();	// �{���͂��蓾�Ȃ�
	}

	// SET_PARAM���b�Z�[�W�̓o�^
	RWM_SET_PARAM = ::RegisterWindowMessage(RWM_SET_PARAM_MESSAGE);
	if (RWM_SET_PARAM == NULL)
	{
		FATAL_MESSAGE();	// �{���͂��蓾�Ȃ�
	}
	
	// SET_PARAM2���b�Z�[�W�̓o�^
	RWM_SET_PARAM2 = ::RegisterWindowMessage(RWM_SET_PARAM2_MESSAGE);
	if (RWM_SET_PARAM2 == NULL)
	{
		FATAL_MESSAGE();	// �{���͂��蓾�Ȃ�
	}

	// �t�b�N�ݒ�
	WA_Enable_hook( Get_handle() ) ;
#if 0
TString msg;
msg.Format_message(_T("%1!x!"), Get_handle());
Message_box(msg);
#endif

	// �g���C�A�C�R�����ݒ�
	Notify_icon.Set_window_handle( Get_handle() ) ;
	Notify_icon.Set_callback_message( LWM_TRAY_CALLBACK ) ;
	Notify_icon.Set_icon( IDI_MAIN ) ;
	Notify_icon.Set_tip( IDS_APP_TITLE ) ;

	// �g���C�A�C�R���o�^
	// ���g���C�͂Ƃ肠����600��i��10���j�Ƃ��Ă���
	for ( int i = 0 ; i < 600 && Notify_icon.Modify() == ERROR_TIMEOUT ; i++ )
	{	// �o�^���s
		// 1�b�҂�
		::Sleep( 1000 ) ;	
	}
}

///	@brief	�E�B���h�E�j�����̏���
void Main_window::On_destroy()
{
	// �g���C�A�C�R���폜
	Notify_icon.Delete() ;

	// �t�b�N����
	HRESULT result = WA_Disable_hook() ;
	if (result != NOERROR)
	{
		TString msg;
		//msg.Format_message(_T("%1!d!"), result);
		msg.Format_message(result);
		Message_box(msg);
	}

	// �A�v���P�[�V�����I�����b�Z�[�W���|�X�g
	::PostQuitMessage( 0 ) ;
}

///	@brief	�f�X�N�g�b�v�T�C�Y�ɂ��f�[�^�t�@�C�����擾�i�E�B���h�E�p�j
void Main_window::Get_file_name( TString& file_name )
{
	// �f�X�N�g�b�v�T�C�Y�擾
	int virtual_screen_cx = ::GetSystemMetrics( SM_CXVIRTUALSCREEN ) ;
	int virtual_screen_cy = ::GetSystemMetrics( SM_CYVIRTUALSCREEN ) ;

	file_name.Format_message( Save_file_name_format, virtual_screen_cx, virtual_screen_cy ) ;
}

///	@brief	�f�X�N�g�b�v�T�C�Y�ɂ��f�[�^�t�@�C�����擾2
void Main_window::Get_file_name2( TString& file_name )
{
	// �f�X�N�g�b�v�T�C�Y�擾
	int virtual_screen_cx = ::GetSystemMetrics( SM_CXVIRTUALSCREEN ) ;
	int virtual_screen_cy = ::GetSystemMetrics( SM_CYVIRTUALSCREEN ) ;

	file_name.Format_message( Save_file_name_format2, virtual_screen_cx, virtual_screen_cy ) ;
}

///	@brief	�E�B���h�E��Ԃ̕ۑ�
void Main_window::On_layout_save()
{
	// �A�v���P�[�V�����f�[�^�t�H���_�쐬
	TPath data_folder = Main::Get_application_data_folder() ;
	HRESULT result = ::SHCreateDirectory( NULL, data_folder ) ;	// �����K�w�̃t�H���_����C�ɍ�邱�Ƃ��ł���
	if ( result != ERROR_SUCCESS && result != ERROR_ALREADY_EXISTS )
	{	// ���ɑ��݂���ȊO�̃G���[
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}

	// �f�[�^�t�@�C���p�X����
	TString file_name ;
	Get_file_name( file_name ) ;
	TPath data_path = data_folder ;
	data_path.Combine( file_name ) ;

	// �E�B���h�E��
//	int num = 0 ;
	for ( HWND handle = ::GetWindow( Get_handle(), GW_HWNDFIRST ) ; handle != NULL ; handle = ::GetWindow( handle, GW_HWNDNEXT ) )
	{
		::SetLastError( ERROR_SUCCESS ) ;	// GetParent()�̓G���[�̏ꍇ�����ݒ肵�Ȃ��悤�Ȃ̂ŁA�N���A���Ă���
		if ( ::GetParent( handle ) != NULL ) continue ;	// �e�E�B���h�E����i�A�v���P�[�V�����̃g�b�v�E�B���h�E�łȂ��j�̂̓X�L�b�v
		if ( ::GetLastError() != ERROR_SUCCESS ) continue ;	// �G���[��������X�L�b�v

		WINDOWINFO info ;
		info.cbSize = sizeof ( info ) ;
		if ( ::GetWindowInfo( handle, &info ) == 0 )
		{	// �E�B���h�E���擾���s
			LOG_FATAL() ;	// �{���͂��蓾�Ȃ�
			continue ;
		}
		if ( !( info.dwStyle & WS_VISIBLE ) ) continue ;	// ��\���E�B���h�E�̓X�L�b�v
		if ( info.dwStyle & WS_MINIMIZE ) continue ;	// �ŏ����E�B���h�E�̓X�L�b�v
		if ( info.dwStyle & WS_MAXIMIZE ) continue ;	// �ő剻�E�B���h�E�̓X�L�b�v

		TCHAR className[ MAX_PATH ] ;
		if ( ::GetClassName( handle, className, ARRAYSIZE( className ) ) == 0 )
		{	// �N���X���擾���s
			LOG_FATAL() ;	// �{���͂��蓾�Ȃ�
			continue ;
		}
		if ( ::_tcscmp( className, _T( "Shell_TrayWnd" ) ) == 0 ) continue ;	// �^�X�N�g���C�̓X�L�b�v
		if ( ::_tcscmp( className, _T( "Progman" ) ) == 0 ) continue ;	// �v���O�����}�l�[�W���̓X�L�b�v
		if ( ::_tcscmp( className, _T( "#32770" ) ) == 0 ) continue ;	// �_�C�A���O�͉B���̂��̂��������肷��̂ŁA�ΏۊO�ɂ���

		RECT rect ;
		if ( ::GetWindowRect( handle, &rect ) == 0 )
		{	// �E�B���h�E�ʒu�擾���s
			LOG_FATAL() ;	// �{���͂��蓾�Ȃ�
			continue ;
		}
		if ( rect.bottom - rect.top == 0 && rect.right - rect.left == 0 ) continue ;	// �傫����0�ɂȂ��Ă�����̂̓X�L�b�v

		// �����܂Ŏc�����E�B���h�E���Ώ�
		// �E�B���h�E�ʒu�ۑ�
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

	// �f�[�^�t�@�C���p�X2����
	TString file_name2 ;
	Get_file_name2( file_name2 ) ;
	TPath data_path2 = data_folder ;
	data_path2.Combine( file_name2 ) ;

	// �f�X�N�g�b�v���X�g�R���g���[���擾
	HWND desktop = ::FindWindow( _T( "ProgMan" ), NULL ) ;
	desktop = ::GetWindow( desktop, GW_CHILD ) ;
	desktop = ::GetWindow( desktop, GW_CHILD ) ;
	if ( desktop == NULL )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}

	// �f�X�N�g�b�v���X�g�R���g���[���̃v���Z�X���J��
	Process process ;
	result = process.Open( desktop, PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE ) ;
	if ( result != ERROR_SUCCESS )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}

	// �f�X�N�g�b�v���X�g�R���g���[���̃�������ԂŃ��������蓖��
	ProcessMemory< POINT > point( process ) ;
	if ( point.Allocate( PAGE_READWRITE ) != ERROR_SUCCESS )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}
	ProcessMemory< LV_ITEM > lv_item( process ) ;
	if ( lv_item.Allocate( PAGE_READWRITE ) != ERROR_SUCCESS )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}
	ProcessMemory< TCHAR[ MAX_PATH ] > text( process ) ;
	if ( text.Allocate( PAGE_READWRITE ) != ERROR_SUCCESS )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}

	// �f�X�N�g�b�v���X�g�R���g���[���̏��擾�̂��߂̍\���̐ݒ�
	int i = 0 ;
	if ( !::WriteProcessMemory( process.Get_handle(), &( lv_item.Get_ptr()->iSubItem ), &i, sizeof ( i ), NULL ) )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}
	i = MAX_PATH ;
	if ( !::WriteProcessMemory( process.Get_handle(), &( lv_item.Get_ptr()->cchTextMax ), &i, sizeof ( i ), NULL ) )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}
	LPTSTR text_ptr2 = reinterpret_cast< LPTSTR >( text.Get_ptr() ) ;
	if ( !::WriteProcessMemory( process.Get_handle(), &( lv_item.Get_ptr()->pszText ), &text_ptr2, sizeof ( text_ptr2 ), NULL ) )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}

	// �f�X�N�g�b�v�A�C�R���ʒu�ۑ�
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

	Message_box( _T( "�ۑ����܂���" ), NULL, MB_OK | MB_ICONINFORMATION ) ;	// @@@@@
}

///	@brief	�E�B���h�E��Ԃ̓ǂݍ���
void Main_window::On_layout_load()
{
	// �f�[�^�t�@�C���p�X����
	TPath data_folder = Main::Get_application_data_folder() ;
	TString file_name ;
	Get_file_name( file_name ) ;
	TPath data_path = data_folder ;
	data_path.Combine( file_name ) ;

	// �E�B���h�E�ʒu��
//	int num = 0 ;
	for ( HWND handle = ::GetWindow( Get_handle(), GW_HWNDFIRST ) ; handle != NULL ; handle = ::GetWindow( handle, GW_HWNDNEXT ) )
	{
		::SetLastError( ERROR_SUCCESS ) ;	// GetParent()�̓G���[�̏ꍇ�����ݒ肵�Ȃ��悤�Ȃ̂ŁA�N���A���Ă���
		if ( ::GetParent( handle ) != NULL ) continue ;	// �e�E�B���h�E����i�A�v���P�[�V�����̃g�b�v�E�B���h�E�łȂ��j�̂̓X�L�b�v
		if ( ::GetLastError() != ERROR_SUCCESS ) continue ;	// �G���[��������X�L�b�v

		WINDOWINFO info ;
		info.cbSize = sizeof ( info ) ;
		if ( ::GetWindowInfo( handle, &info ) == 0 )
		{	// �E�B���h�E���擾���s
			LOG_FATAL() ;	// �{���͂��蓾�Ȃ�
			continue ;
		}
		if ( !( info.dwStyle & WS_VISIBLE ) ) continue ;	// ��\���E�B���h�E�̓X�L�b�v
		if ( info.dwStyle & WS_MINIMIZE ) continue ;	// �ŏ����E�B���h�E�̓X�L�b�v
		if ( info.dwStyle & WS_MAXIMIZE ) continue ;	// �ő剻�E�B���h�E�̓X�L�b�v

		TCHAR className[ MAX_PATH ] ;
		if ( ::GetClassName( handle, className, ARRAYSIZE( className ) ) == 0 )
		{	// �N���X���擾���s
			LOG_FATAL() ;	// �{���͂��蓾�Ȃ�
			continue ;
		}

		// �����܂Ŏc�����E�B���h�E���Ώ�
		// GetPrivateProfileString()�Ŏ擾���Ă���̂́AGetPrivateProfileInt()���ƑΉ������񂪂Ȃ��ꍇ�𔻕ʂ���̂��������
		const DWORD buf_size = 16 ;	// 16����
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

		// �E�B���h�E�ʒu�ݒ�
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

	// �f�[�^�t�@�C���p�X2����
	TString file_name2 ;
	Get_file_name2( file_name2 ) ;
	TPath data_path2 = data_folder ;
	data_path2.Combine( file_name2 ) ;

	// �f�X�N�g�b�v���X�g�R���g���[���擾
	HWND desktop = ::FindWindow( _T( "ProgMan" ), NULL ) ;
	desktop = ::GetWindow( desktop, GW_CHILD ) ;
	desktop = ::GetWindow( desktop, GW_CHILD ) ;
	if ( desktop == NULL )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}

	// �f�X�N�g�b�v���X�g�R���g���[���̃v���Z�X���J��
	Process process ;
	HRESULT result = process.Open( desktop, PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE ) ;
	if ( result != ERROR_SUCCESS )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}

	// �f�X�N�g�b�v���X�g�R���g���[���̃�������ԂŃ��������蓖��
	ProcessMemory< POINT > point( process ) ;
	if ( point.Allocate( PAGE_READWRITE ) != ERROR_SUCCESS )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}
	ProcessMemory< LV_ITEM > lv_item( process ) ;
	if ( lv_item.Allocate( PAGE_READWRITE ) != ERROR_SUCCESS )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}
	ProcessMemory< TCHAR[ MAX_PATH ] > text( process ) ;
	if ( text.Allocate( PAGE_READWRITE ) != ERROR_SUCCESS )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}

	// �f�X�N�g�b�v���X�g�R���g���[���̏��擾�̂��߂̍\���̐ݒ�
	int i = 0 ;
	if ( !::WriteProcessMemory( process.Get_handle(), &( lv_item.Get_ptr()->iSubItem ), &i, sizeof ( i ), NULL ) )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}
	i = MAX_PATH ;
	if ( !::WriteProcessMemory( process.Get_handle(), &( lv_item.Get_ptr()->cchTextMax ), &i, sizeof ( i ), NULL ) )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}
	LPTSTR text_ptr2 = reinterpret_cast< LPTSTR >( text.Get_ptr() ) ;
	if ( !::WriteProcessMemory( process.Get_handle(), &( lv_item.Get_ptr()->pszText ), &text_ptr2, sizeof ( text_ptr2 ), NULL ) )
	{
		FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		return ;
	}

	// �f�X�N�g�b�v�A�C�R���ʒu�ۑ�
	for ( int i = 0 ; i < ListView_GetItemCount( desktop ) ; i++ )
	{
		POINT icon_pos ;
		ListView_GetItemPosition( desktop, i, point.Get_ptr() ) ;
		::ReadProcessMemory( process.Get_handle(), point.Get_ptr(), &icon_pos, sizeof ( icon_pos ), NULL ) ;

		TCHAR icon_text[ MAX_PATH ] ;
		::SendMessage( desktop, LVM_GETITEMTEXT, i, reinterpret_cast< LPARAM >( lv_item.Get_ptr() ) ) ;
		::ReadProcessMemory( process.Get_handle(), text.Get_ptr(), icon_text, sizeof( icon_text ), NULL ) ;

		const DWORD buf_size = 16 ;	// 16����
		TCHAR x_str[ buf_size ] ;
		if ( ::GetPrivateProfileString( icon_text, _T( "x" ), _T( "" ), x_str, buf_size, data_path2 ) == 0 ) continue ;
		int x = ::_ttoi( x_str ) ;
		TCHAR y_str[ buf_size ] ;
		if ( ::GetPrivateProfileString( icon_text, _T( "y" ), _T( "" ), y_str, buf_size, data_path2 ) == 0 ) continue ; 
		int y = ::_ttoi( y_str ) ;

		// �A�C�R���ʒu�ݒ�
		ListView_SetItemPosition( desktop, i, x, y ) ;

#if 0	// debug
TString pos_str ;
pos_str.FormatMessage( _T( "%1 : %2!d!, %3!d!\n" ), icon_text, icon_pos.x, icon_pos.y ) ;
::OutputDebugString( pos_str ) ;
#endif
	}
}

///	@brief	�E�B���h�E��Ԃ̏���
void Main_window::On_layout_clear()
{
	// �f�[�^�t�@�C���p�X����
	TPath data_folder = Main::Get_application_data_folder() ;
	TString file_name ;
	Get_file_name( file_name ) ;
	TPath data_path = data_folder ;
	data_path.Combine( file_name ) ;

	// �f�[�^�t�@�C���폜
	if ( ::DeleteFile( data_path ) == 0 )
	{	// �G���[
		if ( ::GetLastError() != ERROR_FILE_NOT_FOUND )
		{
			FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		}
	}

	// �f�[�^�t�@�C���p�X2����
	TString file_name2 ;
	Get_file_name2( file_name2 ) ;
	TPath data_path2 = data_folder ;
	data_path2.Combine( file_name2 ) ;

	// �f�[�^�t�@�C���폜
	if ( ::DeleteFile( data_path2 ) == 0 )
	{	// �G���[
		if ( ::GetLastError() != ERROR_FILE_NOT_FOUND )
		{
			FATAL_MESSAGE() ;	// �{���͂��蓾�Ȃ�
		}
	}

	Message_box( _T( "�������܂���" ), NULL, MB_OK | MB_ICONINFORMATION ) ;	// @@@@@
}

// [EOF]
