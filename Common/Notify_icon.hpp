///	@file	Notify_icon.hpp
///	@brief	�^�X�N�g���C�A�C�R���N���X
//
//	2007/04/16	�^�c	�V�K�쐬

#pragma once

#include <shellapi.h>

namespace Javelin
{
	///	@brief	�^�X�N�g���C�A�C�R���N���X
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

		// �R�s�[�֎~����
		Notify_icon( const Notify_icon& ) ;
		Notify_icon& operator =( const Notify_icon& ) ;
	} ;
}

// [EOF]
