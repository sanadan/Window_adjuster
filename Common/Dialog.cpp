///	@file	Dialog.cpp
///	@brief	�_�C�A���O�N���X
///	@note	�ˑ��FWin32 API�^Javelin::Window
//
//	2007/04/15	�^�c	�V�K�쐬

#include "stdafx.h"
#include "Dialog.hpp"

namespace Javelin
{
	/// @brief	�R���X�g���N�^
	///	@param	resource_ID	���\�[�XID
	Dialog::Dialog( WORD resource_ID )
	{
		Resource_ID = resource_ID ;
	}

	/// @brief	�_�C�A���O�{�b�N�X�\��
	///	@return	End()�Őݒ肳�ꂽ�l
	INT_PTR Dialog::Do_modal()
	{
		return ::DialogBoxParam( Get_instance(), MAKEINTRESOURCE( Resource_ID ), Get_parent(), Static_dialog_proc, reinterpret_cast< LPARAM >( this ) ) ;
	}

	///	@brief	�X�^�e�B�b�N�_�C�A���O�v���V�[�W��
	///	@note	�p�����[�^�Ɩ߂�l��DialogProc()�ɏ���
	INT_PTR CALLBACK Dialog::Static_dialog_proc( HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam )
	{
		Dialog *dialog_ptr = NULL ;
		if ( message == WM_INITDIALOG )
		{	// �_�C�A���O������
			// �I�u�W�F�N�g���ݒ�
			::SetWindowLongPtr( window_handle, DWLP_USER, static_cast< LONG_PTR >( lparam ) ) ;
			dialog_ptr = reinterpret_cast< Dialog * >( lparam ) ;
			dialog_ptr->Set_handle( window_handle ) ;
		}
		else
		{
			dialog_ptr = reinterpret_cast< Dialog * >( ::GetWindowLongPtr( window_handle, DWLP_USER ) ) ;
		}

		if ( dialog_ptr == NULL )
		{	// �I�u�W�F�N�g�|�C���^���ݒ�
			return FALSE ;	// �������Ȃ�����
		}
		return dialog_ptr->Dialog_proc( message, wparam, lparam ) ;
	}

	///	@brief	�_�C�A���O�v���V�[�W��
	///	@note	�p�����[�^�Ɩ߂�l��DialogProc()�ɏ���
	INT_PTR Dialog::Dialog_proc( UINT message, WPARAM wparam, LPARAM )
	{
		switch ( message )
		{
			case WM_CLOSE :
				Post_message( WM_COMMAND, IDCANCEL ) ;
				Set_attribute( DWLP_MSGRESULT, 0 ) ;
				return TRUE ;	// ��������
			
			case WM_COMMAND :
				switch ( LOWORD( wparam ) )
				{
					case IDOK :
						End( IDOK ) ;
						Set_attribute( DWLP_MSGRESULT, 0 ) ;
						return TRUE ;

					case IDCANCEL :
						End( IDCANCEL ) ;
						Set_attribute( DWLP_MSGRESULT, 0 ) ;
						return TRUE ;	// ��������
				}
				break ;
		}

		return FALSE ;	// �������Ȃ�����
	}

	///	@brief	�_�C�A���O�I���ݒ�
	///	@result	�I���R�[�h
	HRESULT Dialog::End( INT_PTR result )
	{
		if ( ::EndDialog( Get_handle(), result ) ) return ERROR_SUCCESS ;
		return ::GetLastError() ;
	}
}

// [EOF]
