///	@file	Main.cpp
///	@brief	���C���N���X
//
//	2007/05/08	�^�c	�V�K�쐬

#include "stdafx.h"
#include "Main.hpp"
#include "Main_window.hpp"
#include "Debug_tool.hpp"
using namespace Javelin ;

Main App ;	///< ���C���A�v���P�[�V�����I�u�W�F�N�g
LPCTSTR Main::Title = _T( "Window adjuster" ) ;	///< �A�v���P�[�V������
LPCTSTR Main::Internal_name = _T( "Javelin_Software\\Window_adjuster" ) ;	///< �������^Application Data�z���̃t�H���_���Ɏg�p
LPCTSTR Main::Internal_id = _T( "Javelin software - Window adjuster" ) ;	///< ����ID�^��d�N���̃`�F�b�N�Ɏg�p

Main_window Main_window ;	///< ���C���E�B���h�E�N���X
LPCTSTR Main_window::Save_file_name_format = _T( "%1!d!-%2!d!.ini" ) ;	///< ���C�A�E�g���ۑ���t�@�C�����t�H�[�}�b�g
LPCTSTR Main_window::Save_file_name_format2 = _T( "%1!d!-%2!d!_icon.ini" ) ;	///< ���C�A�E�g���ۑ���t�@�C�����t�H�[�}�b�g2

///	@brief	������
///	@return	winerror.h����
HRESULT Main::Initialize()
{
	// �e�N���X�̏����������Ăяo��
	HRESULT result = Application::Initialize() ;
	if ( result != ERROR_SUCCESS )
	{	// �ʏ�͂��蓾�Ȃ��G���[
		FATAL_MESSAGE() ;
		return result ;
	}

	// ��d�N���̃`�F�b�N
	if ( Is_already_exists( Get_internal_id() ) )
	{	// ���ɋN�����Ă���
		Error_message_box( _T( "���ɋN�����Ă��܂�" ) ) ;	// @@@@@
		return ERROR_ALREADY_EXISTS ;
	}

	// �A�N�Z�����[�^�̃��[�h
	Load_accelerator( IDM_MAIN ) ;

	// ���C���E�B���h�E�쐬
	Main_window.Load_icon( IDI_MAIN ) ;
	Main_window.Set_menu( IDM_MAIN ) ;
	Main_window.Set_text( Get_title() ) ;
	result = Main_window.Create( IDS_MAIN_WINDOW_CLASS ) ;
	if ( result != ERROR_SUCCESS )
	{	// �ʏ�͂��蓾�Ȃ��G���[
		FATAL_MESSAGE() ;
		return result ;
	}

	// �A�v���P�[�V�����t�H���_�ݒ�
	result = Application_data_folder.Get_folder( CSIDL_APPDATA ) ;
	if ( result != ERROR_SUCCESS )
	{	// �ʏ�͂��蓾�Ȃ��G���[
		FATAL_MESSAGE() ;
		return result ;
	}
	Application_data_folder.Combine( Get_internal_name() ) ;

	return ERROR_SUCCESS ;
}

///	@brief	�G���[���b�Z�[�W�{�b�N�X�\��
///	@retval	IDOK	OK�{�^���������ꂽ
int Main::Error_message_box( LPCTSTR message )
{
	return ::MessageBox( NULL, message, Get_title(), MB_OK | MB_ICONERROR ) ;
}

///
LPCTSTR Main::Get_title()
{
	return Main::Title ;
}

///
LPCTSTR Main::Get_internal_name()
{
	return Main::Internal_name ;
}

///
LPCTSTR Main::Get_internal_id()
{
	return Internal_id ;
}

///	@brief	�A�v���P�[�V�����f�[�^�t�H���_�p�X�擾
LPCTSTR Main::Get_application_data_folder()
{
	return App.Application_data_folder ;
}

// [EOF]
