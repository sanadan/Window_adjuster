///	@file	Application.cpp
///	@brief	�A�v���P�[�V�����N���X
//
//	2006/01/22	�^�c	�V�K�쐬

#include "stdafx.h"
#include "Application.hpp"
#include "assert.h"

namespace Javelin
{
	Application *App_ptr = NULL ;

	///	@brief	�R���X�g���N�^
	Application::Application()
	{
		assert( App_ptr == NULL ) ;

		App_ptr = this ;
		Instance = NULL ;
		Show_command = 0 ;
	}

	///	@brief	������
	///	@return	winerror.h����
	HRESULT Application::Initialize()
	{
		return ERROR_SUCCESS ;
	}

	///	@brief	���d�N���`�F�b�N
	///	@retval	TRUE	���ɋN�����Ă���
	///	@retval	FALSE	�ŏ��̋N��
	BOOL Application::Is_already_exists( LPCTSTR name )
	{
		HRESULT ret = Exist_mutex.Create( name ) ;
		assert( ret == ERROR_SUCCESS || ret == ERROR_ALREADY_EXISTS ) ;
		return ret != ERROR_SUCCESS ;
	}

	///	@brief	�C���X�^���X�n���h���擾
	///	@return	�C���X�^���X�n���h��
	HINSTANCE Application::Get_instance() const
	{
		return Instance ;
	}

	///	@brief	�N�����̕\���ݒ�擾
	///	@return	�\���ݒ�
	int Application::Get_show_command() const
	{
		return Show_command ;
	}

	///	@brief	�A�N�Z�����[�^�̃��[�h
	///	@return	winerror.h����
	HRESULT Application::Load_accelerator( WORD resource )
	{
		Accel.Set_instance( Instance ) ;
		return Accel.Load( resource ) ;
	}

	///	@brief	���b�Z�[�W���[�v
	///	@return	winerror.h����
	HRESULT Application::Run()
	{
		// ���C�� ���b�Z�[�W ���[�v:
		MSG msg;
		while ( ::GetMessage( &msg, NULL, 0, 0 ) > 0 ) 
		{
			if ( Accel.Translate( msg ) )
			{
				continue ;
			}

			::TranslateMessage( &msg ) ;
			::DispatchMessage( &msg ) ;
		}

		return static_cast< HRESULT >( msg.wParam ) ;
	}
}	// namespace Javelin

///	@brief	�A�v���P�[�V�����G���g��
int WINAPI WinMain( HINSTANCE instance, HINSTANCE, LPSTR, int show_command )
{
	assert( Javelin::App_ptr != NULL ) ;

	Javelin::App_ptr->Instance = instance ;
	Javelin::App_ptr->Show_command = show_command ;
	HRESULT result = Javelin::App_ptr->Initialize() ;
	if ( result != ERROR_SUCCESS ) return result ;

	return Javelin::App_ptr->Run() ;
}

// [EOF]