///	@file	Dialog.hpp
///	@brief	�_�C�A���O�N���X
///	@note	�ˑ��FWin32 API�^Javelin::Window
//
//	2007/04/15	�^�c	�V�K�쐬

#pragma once

#include "Window.hpp"

namespace Javelin
{
	///	@brief	�_�C�A���O�N���X
	class Dialog : public Window
	{
	public :
		Dialog( WORD resource_ID ) ;

		// �_�C�A���O�\��
		INT_PTR Do_modal() ;

		// �_�C�A���O�\����
		HRESULT End( INT_PTR result ) ;
		HWND Get_item( int control_id )
		{
			return ::GetDlgItem( Get_handle(), control_id ) ;
		}

	protected :
		static INT_PTR CALLBACK Static_dialog_proc( HWND window, UINT message, WPARAM wparam, LPARAM lparam ) ;
		virtual INT_PTR Dialog_proc( UINT message, WPARAM wparam, LPARAM lparam ) ;

	private :
		int Resource_ID ;	///< �_�C�A���O���\�[�XID

		// �R�s�[�֎~����
		Dialog( const Dialog& ) ;
		Dialog& operator =( const Dialog& ) ;
	} ;
}

// [EOF]
