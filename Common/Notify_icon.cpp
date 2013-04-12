///	@file	Notify_icon.cpp
///	@brief	�^�X�N�g���C�A�C�R���N���X
//
//	2007/04/16	�^�c	�V�K�쐬

#include "stdafx.h"
#include "Notify_icon.hpp"
#include "TString.hpp"
#include "Misc.hpp"

namespace Javelin
{
	// �A�C�R���T�C�Y
	const int Notify_icon::X_size = 16 ;
	const int Notify_icon::Y_size = 16 ;

	///	@brief	�R���X�g���N�^
	Notify_icon::Notify_icon()
	{
		::ZeroMemory( &Notify_icon_data, sizeof ( Notify_icon_data ) ) ;
		Notify_icon_data.cbSize = sizeof ( Notify_icon_data ) ;
		Notify_icon_data.uVersion = NOTIFYICON_VERSION ;

		Set_instance( ::GetModuleHandle( NULL ) ) ;
	}

	///	@brief	�f�X�g���N�^
	Notify_icon::~Notify_icon()
	{
		if ( Notify_icon_data.hIcon != NULL )
		{
			::DestroyIcon( Notify_icon_data.hIcon ) ;
		}
	}

	///	@brief	�C���X�^���X�ݒ�
	///	@param	instance	�C���X�^���X�n���h��
	void Notify_icon::Set_instance( HINSTANCE instance )
	{
		Instance = instance ;
	}

	///	@brief	�C���X�^���X�擾
	///	@return	�C���X�^���X�n���h��
	HINSTANCE Notify_icon::Get_instance()
	{
		return Instance ;
	}

	///	@brief	���b�Z�[�W�ʒm��E�B���h�E�n���h���ݒ�
	///	@param	window	�E�B���h�E�n���h��
	void Notify_icon::Set_window_handle( HWND window )
	{
		Notify_icon_data.hWnd = window ;
	}

	///	@brief	�^�X�N�g���C�C�x���g�ʒm���b�Z�[�W�ݒ�
	///	@param	message	���b�Z�[�W
	void Notify_icon::Set_callback_message( UINT message )
	{
		Notify_icon_data.uCallbackMessage = message ;
		Notify_icon_data.uFlags |= NIF_MESSAGE ;
	}

	///	@brief	�A�C�R���ݒ�
	///	@param	resource_ID	���\�[�XID
	void Notify_icon::Set_icon( WORD resource_ID )
	{
		if ( Notify_icon_data.hIcon != NULL )
		{
			::DestroyIcon( Notify_icon_data.hIcon ) ;
		}

		Notify_icon_data.hIcon = ( HICON )::LoadImage( Get_instance(), MAKEINTRESOURCE( resource_ID ), IMAGE_ICON, X_size, Y_size, LR_DEFAULTCOLOR ) ;
		Notify_icon_data.uFlags |= NIF_ICON ;
	}

	///	@brief	�`�b�v������ݒ�
	///	@param	tip	�`�b�v������
	void Notify_icon::Set_tip( LPCTSTR tip )
	{
		::lstrcpyn( Notify_icon_data.szTip, tip, ARRAYSIZE( Notify_icon_data.szTip ) ) ;
		Notify_icon_data.uFlags |= NIF_TIP ;
	}

	///	@brief	�`�b�v������ݒ�
	///	@param	resource_ID	���\�[�XID
	///	@return	winerror.h����
	HRESULT Notify_icon::Set_tip( WORD resource_ID )
	{
		TString tip ;
		HRESULT result = tip.Load_string( resource_ID ) ;
		if ( result != ERROR_SUCCESS ) return result ;

		Set_tip( tip.c_str() ) ;

		return ERROR_SUCCESS ;
	}

	///	@brief	�A�C�R���ǉ�
	///	@return	winerror.h����
	HRESULT Notify_icon::Add()
	{
		if ( !::Shell_NotifyIcon( NIM_ADD, &Notify_icon_data ) ) return ::GetLastError() ;

		if ( !::Shell_NotifyIcon( NIM_SETVERSION, &Notify_icon_data ) ) return ::GetLastError() ;

		return ERROR_SUCCESS ;
	}

	///	@brief	�A�C�R���ύX
	///	@return	winerror.h����
	HRESULT Notify_icon::Modify()
	{
		if ( ::Shell_NotifyIcon( NIM_MODIFY, &Notify_icon_data ) ) return ERROR_SUCCESS ;

		// �܂��o�^����Ă��Ȃ������̂Œǉ�
		return Add() ;
	}

	///	@brief	�A�C�R���폜
	///	@return	winerror.h����
	HRESULT Notify_icon::Delete()
	{
		if ( !::Shell_NotifyIcon( NIM_DELETE, &Notify_icon_data ) ) return ::GetLastError() ;

		return ERROR_SUCCESS ;
	}
}

// [EOF]
