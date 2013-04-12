///	@file	Process.cpp
///	@brief	�v���Z�X�N���X
//
//	2011/12/09	�^�c	�V�K�쐬

#include "stdafx.h"
#include "Process.hpp"

namespace Javelin
{
	///	@brief	�R���X�g���N�^
	Process::Process()
	{
		Handle = NULL ;
	}

	///	@brief	�f�X�g���N�^
	Process::~Process()
	{
		Close() ;
	}

	///	@breif	�E�B���h�E�ɑΉ�����v���Z�X���J��
	///	@param	window_handle	�E�B���h�E�n���h��
	///	@return	winerror.h����
	///	@note	���v���Z�X�̃E�B���h�E�R���g���[���̏����擾����Ƃ��ȂǂɎg���B\n
	///			�v���Z�X���J���Ă���Ԃ�Get_handle()���L���ɂȂ�B
	HRESULT Process::Open( HWND window_handle, DWORD access_flag, BOOL inherit )
	{
		// �E�B���h�E�̃v���Z�XID�擾
		DWORD process_id ;
		::GetWindowThreadProcessId( window_handle, &process_id ) ;

		// �E�B���h�E�̃v���Z�X�n���h���擾
		Handle = ::OpenProcess( access_flag, inherit, process_id ) ;
		if ( Handle == NULL ) return ::GetLastError() ;

		return ERROR_SUCCESS ;
	}

	///	@brief	�v���Z�X�����
	///	@return	winerror.h����
	HRESULT Process::Close()
	{
		if ( Handle == NULL ) return ERROR_SUCCESS ;
		
		BOOL success = ::CloseHandle( Handle ) ;
		Handle = NULL ;
		
		if ( !success ) return ::GetLastError() ;

		return ERROR_SUCCESS ;
	}

	///	@brief	�v���Z�X�n���h�����擾����
	///	@return	�v���Z�X�n���h��\n
	///			== NULL : �v���Z�X���J���Ă��Ȃ�
	HANDLE Process::Get_handle() const
	{
		return Handle ;
	}
} ;	// namespace Javelin

// [EOF]
