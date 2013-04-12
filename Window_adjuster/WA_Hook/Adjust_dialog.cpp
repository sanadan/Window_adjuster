///	@file	Adjust_dialog.cpp
///	@note	�����_�C�A���O�N���X
//
//	2007/04/15	�^�c	�V�K�쐬

#include "stdafx.h"
#include "Adjust_dialog.hpp"
#include "resource.h"
#include <tchar.h>
#include "Debug_tool.hpp"
#include "Misc.hpp"

///	@brief	�R���X�g���N�^
Adjust_dialog::Adjust_dialog() : Dialog( IDD_ADJUST )
{
	Last_window_placement.length = sizeof ( Last_window_placement ) ;
	Point.x = 0 ;
	Point.y = 0 ;
	Icon03 = NULL ;
	Icon06 = NULL ;
}

///	@brief	�\���ʒu�ݒ�
///	@param	point	�\���ʒu
void Adjust_dialog::Set_position( POINT& point )
{
	Point = point ;
}

///	@brief	�_�C�A���O�v���V�[�W��
///	@param	message	���b�Z�[�W
///	@param	wparam	WPARAM
///	@param	lparam	LPARAM
///	@retval	TRUE	���b�Z�[�W����������
///	@retval	FALSE	���b�Z�[�W���������Ȃ�����
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

///	@brief	�{�^���ɃA�C�R����ݒ�
///	@param	control_id	�R���g���[��ID
///	@param	icon_id		�A�C�R�����\�[�XID
///	@note	���\�[�X�Ń{�^����BS_ICON�̑��������Ă����Ȃ��ƁA�A�C�R�����\������Ȃ�
void Adjust_dialog::Set_icon( int control_id, int icon_id )
{
	HICON icon_handle = ( HICON )::LoadImage( Get_instance(), MAKEINTRESOURCE( icon_id ), IMAGE_ICON, 16, 16, LR_SHARED ) ;
	::SendMessage( Get_item( control_id ), BM_SETIMAGE, IMAGE_ICON, ( LPARAM )icon_handle ) ;
}

///	@brief	WM_INITDIALOG�ɑ΂��鏈��
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

	// �e�E�B���h�E�̈ʒu���擾
	if ( !::GetWindowPlacement( Get_parent(), &Last_window_placement ) )
	{
		// @@@@@ �v���I����
		FATAL_MESSAGE() ;
		End( IDCANCEL ) ;
	}

	// �_�C�A���O�̈ʒu���擾
	WINDOWPLACEMENT window_placement ;
	window_placement.length = sizeof ( window_placement ) ;
	if ( !::GetWindowPlacement( Get_handle(), &window_placement ) )
	{
		// @@@@@ �v���I����
		FATAL_MESSAGE() ;
		End( IDCANCEL ) ;
	}
	const RECT& target = window_placement.rcNormalPosition ;

	// ���j�^���擾
	HMONITOR monitor = ::MonitorFromWindow( Get_handle(), MONITOR_DEFAULTTONEAREST ) ;
	MONITORINFO monitor_info ;
	monitor_info.cbSize = sizeof ( monitor_info ) ;
	if ( !::GetMonitorInfo( monitor, &monitor_info ) )
	{
		// @@@@@ �v���I����
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

///	@brief	WM_COMMAND�ɑ΂��鏈��
///	@param	wparam	WM_COMMAND��WPARAM
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

///	@brief	�����{�^������
///	@param	wParam	WM_COMMAND��WPARAM
void Adjust_dialog::On_adjust_button( WPARAM wparam )
{
	// �ΏۃE�B���h�E���擾
	WINDOWPLACEMENT window_placement ;
	window_placement.length = sizeof ( window_placement ) ;
	if ( !::GetWindowPlacement( Get_parent(), &window_placement ) )
	{
		// @@@@@ �v���I����
		return ;
	}
	const RECT& target = window_placement.rcNormalPosition ;

	// ���j�^���擾
	HMONITOR monitor = ::MonitorFromWindow( Get_parent(), MONITOR_DEFAULTTONEAREST ) ;
	MONITORINFO monitor_info ;
	monitor_info.cbSize = sizeof ( monitor_info ) ;
	if ( !::GetMonitorInfo( monitor, &monitor_info ) )
	{
		// @@@@@ �v���I����
		return ;
	}
	const RECT& desktop = monitor_info.rcWork ;

	// �f�t�H���g�l�v�Z
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
		case IDC_MOVE1 :	// ����Ɉړ�
			x = desktop.left ;
			y = desktop.top ;
			break ;
		case IDC_MOVE2 :	// ��Ɉړ�
			y = desktop.top ;
			break ;
		case IDC_MOVE3 :	// �E��Ɉړ�
			x = desktop.right - ( target.right - target.left ) ;
			y = desktop.top ;
			break ;
		case IDC_MOVE4 :	// ���Ɉړ�
			x = desktop.left ;
			break ;
		case IDC_MOVE5 :	// �����Ɉړ�
			x = desktop.left + ( desktop_width - ( target.right - target.left ) ) / 2 ;
			y = desktop.top + ( desktop_height - ( target.bottom - target.top ) ) / 2 ;
			break ;
		case IDC_MOVE6 :	// �E�Ɉړ�
			x = desktop.right - ( target.right - target.left ) ;
			break ;
		case IDC_MOVE7 :	// �����Ɉړ�
			x = desktop.left ;
			y = desktop.bottom - ( target.bottom - target.top ) ;
			break ;
		case IDC_MOVE8 :	// ���Ɉړ�
			y = desktop.bottom - ( target.bottom - target.top ) ;
			break ;
		case IDC_MOVE9 :	// �E���Ɉړ�
			x = desktop.right - ( target.right - target.left ) ;
			y = desktop.bottom - ( target.bottom - target.top ) ;
			break ;

		case IDC_SIZE1 :	// �c���k��
			cx -= dx ;
			cy -= dy ;
			break ;
		case IDC_SIZE2 :	// �c�k��
			cy -= dy ;
			break ;
		case IDC_SIZE3 :	// �c�k���^���g��
			cx += dx ;
			cy -= dy ;
			break ;
		case IDC_SIZE4 :	// ���k��
			cx -= dx ;
			break ;
		case IDC_SIZE5 :	// �c�����f�X�N�g�b�v��1/2��
			cx = desktop_width / 2 ;
			cy = desktop_height / 2 ;
			break ;
		case IDC_SIZE6 :	// ���g��
			cx += dx ;
			break ;
		case IDC_SIZE7 :	// �c�g��^���k��
			cx -= dx ;
			cy += dy ;
			break ;
		case IDC_SIZE8 :	// �c�g��
			cy += dy ;
			break ;
		case IDC_SIZE9 :	// �c���g��
			cx += dx ;
			cy += dy ;
			break ;
		case IDC_SIZE10 :	// �c�ő剻
			y = desktop.top ;
			cy = desktop_height ;
			break ;
		case IDC_SIZE11 :	// ���ő剻
			x = desktop.left ;
			cx = desktop_width ;
			break ;

		default :	// ���̑�
			return ;	// �������Ȃ�
	}

	// �␳
	if ( cx > desktop_width ) cx = desktop_width ;
	if ( cy > desktop_height ) cy = desktop_height ;
	if ( target.left + cx > desktop.right ) x = desktop.right - cx ;
	if ( target.top + cy > desktop.bottom ) y = desktop.bottom - cy ;

	// �E�B���h�E�ʒu�ݒ�
	::SetWindowPos( Get_parent(), NULL, x, y, cx, cy, SWP_NOZORDER ) ;
}

///	@brief	�L�����Z���{�^������
void Adjust_dialog::On_cancel()
{
	// �E�B���h�E�����̈ʒu�ɖ߂�
	::SetWindowPlacement( Get_parent(), &Last_window_placement ) ;
}

// [EOF]
