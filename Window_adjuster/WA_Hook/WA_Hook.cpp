///
///	@fn		WA_Hook.cpp
///	@brief	�t�b�NDLL�G���g��
///
//	2007/04/19	�^�c	�V�K�쐬
//

#include "stdafx.h"
#include "WA_Hook.h"
#include "Adjust_dialog.hpp"
#include "Misc.hpp"
#include "Auto_reset.hpp"
#include "Debug_tool.hpp"
// #include "Point.hpp"

#pragma warning ( disable : 4127 )	// �萔�������̌x���}�~

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


// �O���[�o���t�b�N�p�ϐ�
#pragma data_seg ( ".share" )
HHOOK Hook_handle = NULL ;
#pragma data_seg ()
#pragma comment( linker, "/SECTION:.share,RWS" )

HMODULE Module_handle = NULL ;
BOOL Show_dialog = FALSE ;

// ���[�J���֐��錾
LRESULT Show_adjust_dialog( MOUSEHOOKSTRUCT& mhs ) ;
// BOOL Is_caption( UINT hit_test_code ) ;
LRESULT Adjust( MOUSEHOOKSTRUCT& mhs, WINDOWPLACEMENT& window_placement, Adjust_type type, UINT hit_code = NULL ) ;

///
///	@fn		BOOL APIENTRY DllMain( HMODULE module_handle, DWORD reason_for_call, LPVOID )
///	@brief	DLL�G���g��
///
BOOL APIENTRY DllMain( HMODULE module_handle, DWORD reason_for_call, LPVOID )
{
	switch ( reason_for_call )
	{
		case DLL_PROCESS_ATTACH :
			Module_handle = module_handle ;
//			::Log_init( _T( "c:\\temp\\WA_Hook.log" ) ) ;	// debug
			break ;

		case DLL_THREAD_ATTACH :
		case DLL_THREAD_DETACH :
		case DLL_PROCESS_DETACH :
			break ;
	}
    return TRUE ;
}

typedef BOOL (WINAPI *LPFN_ISWOW64MESSAGE) (void);

LPFN_ISWOW64MESSAGE fnIsWow64Message;

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

///
///	@fn		static LRESULT CALLBACK Mouse_proc( int code, WPARAM wparam, LPARAM lparam )
///	@brief	�}�E�X�t�b�N�v���V�[�W��
///
static LRESULT CALLBACK Mouse_proc( int code, WPARAM wparam, LPARAM lparam )
{
	LRESULT result = 0 ;

	do	// �G���[�����p
	{
		if ( code < 0 ) break ;
		if ( code != HC_ACTION ) break ;
		if ( lparam == NULL ) break ;

		const LPMOUSEHOOKSTRUCT mhs_ptr = ( LPMOUSEHOOKSTRUCT )lparam ;
		WINDOWPLACEMENT window_placement ;
		window_placement.length = sizeof ( window_placement ) ;
		if ( !::GetWindowPlacement( mhs_ptr->hwnd, &window_placement ) )
		{	// �E�B���h�E�ʒu���擾���s
			break ;
		}
		if ( window_placement.showCmd == SW_SHOWMAXIMIZED )
		{	// �ő剻����Ă���
			break ;
		}

		switch ( wparam )
		{
			case WM_NCLBUTTONDBLCLK :	// ���_�u���N���b�N
#if 0
				switch ( mhs_ptr->wHitTestCode )
				{
					case HTTOP :
					case HTTOPLEFT :
					case HTTOPRIGHT :
						result = Adjust( *mhs_ptr, window_placement, MOVE_TOP ) ;
						break ;

					case HTBOTTOM :
					case HTBOTTOMLEFT :
					case HTBOTTOMRIGHT :
						result = Adjust( *mhs_ptr, window_placement, MOVE_BOTTOM ) ;
						break ;
				}

				if ( !::GetWindowPlacement( mhs_ptr->hwnd, &window_placement ) )
				{	// �E�B���h�E�ʒu���擾���s
					break ;
				}

				switch ( mhs_ptr->wHitTestCode )
				{
					case HTLEFT :
					case HTTOPLEFT :
					case HTBOTTOMLEFT :
						result = Adjust( *mhs_ptr, window_placement, MOVE_LEFT ) ;
						break ;

					case HTRIGHT :
					case HTTOPRIGHT :
					case HTBOTTOMRIGHT :
						result = Adjust( *mhs_ptr, window_placement, MOVE_RIGHT ) ;
						break ;
				}
				break ;
#endif
				switch ( mhs_ptr->wHitTestCode )
				{
					case HTTOP :
					case HTBOTTOM :
						result = Adjust( *mhs_ptr, window_placement, MAXIMIZE_HEIGHT, mhs_ptr->wHitTestCode ) ;
						break ;

					case HTLEFT :
					case HTRIGHT :
						result = Adjust( *mhs_ptr, window_placement, MAXIMIZE_WIDTH, mhs_ptr->wHitTestCode ) ;
						break ;
				}
				break ;

			case WM_NCRBUTTONUP :	// �E�N���b�N
LOG( _T( "WM_NCRBUTTONUP" ) ) ;
				if ( mhs_ptr->wHitTestCode == HTCLOSE )
				{
					result = Show_adjust_dialog( *mhs_ptr ) ;
					break ;
				}
				if ( mhs_ptr->wHitTestCode != HTMAXBUTTON )
				{	// �ő剻�{�^����ł͂Ȃ�
					break ;
				}
				result = Adjust( *mhs_ptr, window_placement, AUTO ) ;
				break ;

			case WM_NCRBUTTONDBLCLK :	// �E�_�u���N���b�N
LOG( _T( "WM_NCRBUTTONDBLCLK" ) ) ;
#if 0
				switch ( mhs_ptr->wHitTestCode )
				{
					case HTTOP :
					case HTBOTTOM :
						result = Adjust( *mhs_ptr, window_placement, MAXIMIZE_HEIGHT, mhs_ptr->wHitTestCode ) ;
						break ;

					case HTLEFT :
					case HTRIGHT :
						result = Adjust( *mhs_ptr, window_placement, MAXIMIZE_WIDTH, mhs_ptr->wHitTestCode ) ;
						break ;
				}
				break ;
#endif
				switch ( mhs_ptr->wHitTestCode )
				{
					case HTTOP :
					case HTTOPLEFT :
					case HTTOPRIGHT :
						result = Adjust( *mhs_ptr, window_placement, MOVE_TOP, mhs_ptr->wHitTestCode ) ;
						break ;

					case HTBOTTOM :
					case HTBOTTOMLEFT :
					case HTBOTTOMRIGHT :
						result = Adjust( *mhs_ptr, window_placement, MOVE_BOTTOM, mhs_ptr->wHitTestCode ) ;
						break ;
				}

				if ( !::GetWindowPlacement( mhs_ptr->hwnd, &window_placement ) )
				{	// �E�B���h�E�ʒu���擾���s
					break ;
				}

				switch ( mhs_ptr->wHitTestCode )
				{
					case HTLEFT :
					case HTTOPLEFT :
					case HTBOTTOMLEFT :
						result = Adjust( *mhs_ptr, window_placement, MOVE_LEFT, mhs_ptr->wHitTestCode ) ;
						break ;

					case HTRIGHT :
					case HTTOPRIGHT :
					case HTBOTTOMRIGHT :
						result = Adjust( *mhs_ptr, window_placement, MOVE_RIGHT, mhs_ptr->wHitTestCode ) ;
						break ;
				}
				break ;

			case WM_NCMBUTTONUP :	// �����N���b�N
				if ( ::GetAsyncKeyState( VK_CONTROL ) >= 0 )
				{	// Ctrl�L�[��������Ă��Ȃ�
					break ;
				}
				result = Show_adjust_dialog( *mhs_ptr ) ;
				break ;
		}
	} while ( 0 ) ;	// �G���[�����p

	LRESULT result2 = ::CallNextHookEx( Hook_handle, code, wparam, lparam) ;
	if ( result == 0 )
	{	// �t�b�N���������Ȃ�����
		// ���̃t�b�N�̌��ʂ�Ԃ�
		return result2 ;
	}
	return result ;
}

///
///	@fn		LRESULT Show_adjust_dialog( MOUSEHOOKSTRUCT& mhs )
///	@brief	�����_�C�A���O�\��
///	@param	mhs	�}�E�X�t�b�N���
///	@retval	0	�������Ȃ�����
///	@retval	1	��������
///
LRESULT Show_adjust_dialog( MOUSEHOOKSTRUCT& mhs )
{
	if ( Show_dialog ) return 0	;	// ���ɕ\�����Ă���
#if defined( _WIN64 )
//	if ( !IsWow64() ) return 0 ;
#else
//	if ( IsWow64() ) return 0 ;
#endif

	Javelin::Auto_reset< BOOL > auto_reset( Show_dialog, TRUE, FALSE ) ;

	Adjust_dialog adjust_dialog ;
	adjust_dialog.Set_instance( Module_handle ) ;
	adjust_dialog.Set_parent( mhs.hwnd ) ;
	adjust_dialog.Set_position( mhs.pt ) ;
	adjust_dialog.Do_modal() ;

	return 1 ;
}

///
///	@fn		LRESULT Adjust( MOUSEHOOKSTRUCT& mhs, WINDOWPLACEMENT& window_placement, Adjust_type type, UINT hit_code )
///	@brief	�E�B���h�E����
///	@param	mhs					�}�E�X�t�b�N���
///	@param	window_placement	�E�B���h�E�ʒu���
///	@param	type				�������\n
///								== AUTO :				����\n
///								== MOVE_TOP :			��ֈړ�\n
///								== MOVE_BOTTOM :		���ֈړ�\n
///								== MOVE_LEFT :			���ֈړ�\n
///								== MOVE_RIGHT :			�E�ֈړ�\n
///								== MAXIMIZE_HEIGHT :	�c�ő剻\n
///								== MAXIMIZE_WIDTH :		���ő剻
///	@param	hit_code			�}�E�X�ʒu���\n
///								== NULL :		�Ȃ�\n
///								== HTTOP :		��[\n
///								== HTBOTTOM :	���[\n
///								== HTLEFT :		���[\n
///								== HTRIGHT :	�E�[
///	@retval	0	�������Ȃ�����
///	@retval	1	��������
///
LRESULT Adjust( MOUSEHOOKSTRUCT& mhs, WINDOWPLACEMENT& window_placement, Adjust_type type, UINT hit_code )
{
LOG( _T( "Adjust()" ) ) ;
	const RECT& target = window_placement.rcNormalPosition ;

	// ���j�^���擾
	const HMONITOR monitor = ::MonitorFromWindow( mhs.hwnd, MONITOR_DEFAULTTONEAREST ) ;
	MONITORINFO monitor_info ;
	monitor_info.cbSize = sizeof ( monitor_info ) ;
	if ( !::GetMonitorInfo( monitor, &monitor_info ) )
	{
		// @@@@@ �v���I����
		return 0 ;
	}
	const RECT& desktop = monitor_info.rcWork ;

	// �f�t�H���g�l�v�Z
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
				// �c�ő剻
				cy = desktop_height ;
			}
			else
			{
				// 1/4�ɂ���
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
			// �c�ő剻
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
			// ���ő剻
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

	// �␳
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

	// �E�B���h�E�ʒu�ݒ�
	::SetWindowPos( mhs.hwnd, NULL, x, y, cx, cy, SWP_NOZORDER ) ;

	return 1 ;
}

///
///	@fn		WA_HOOK_API HRESULT WA_Enable_hook()
///	@brief	�t�b�N�L����
///	@return	winerror.h����
///
WA_HOOK_API HRESULT WA_Enable_hook()
{
	if ( Hook_handle != NULL ) return ERROR_SUCCESS ;

	Hook_handle = ::SetWindowsHookEx( WH_MOUSE, ( HOOKPROC )Mouse_proc, Module_handle, 0 ) ;
	if ( Hook_handle == NULL ) return ::GetLastError() ;

#if 0
	// OS�o�[�W�����`�F�b�N
	OSVERSIONINFO os_version_info ;
	os_version_info.dwOSVersionInfoSize = sizeof ( os_version_info ) ;
	os_version_info.dwMajorVersion = 6 ;
	os_version_info.dwMinorVersion = 0 ;
	DWORDLONG condition_mask = 0 ;
	const int op = VER_LESS_EQUAL ;
	VER_SET_CONDITION( condition_mask, VER_MAJORVERSION, op ) ;
	VER_SET_CONDITION( condition_mask, VER_MINORVERSION, op ) ;
	if ( ::VerifyVersionInfo( &os_version_info, VER_MAJORVERSION, condition_mask ) )
	{	// Vista�ȑO
		// 
	}
#endif

	return ERROR_SUCCESS ;
}

///
///	@fn		WA_HOOK_API HRESULT WA_Disable_hook()
///	@brief	�t�b�N������
///	@return	winerror.h
///
WA_HOOK_API HRESULT WA_Disable_hook()
{
	if ( Hook_handle == NULL ) return ERROR_SUCCESS ;

	BOOL ret = ::UnhookWindowsHookEx( Hook_handle ) ;
	Hook_handle = NULL ;

	return ret ? ERROR_SUCCESS : ::GetLastError() ;
}

// [[[[[ End of this module ]]]]]