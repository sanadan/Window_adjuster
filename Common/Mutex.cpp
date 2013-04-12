///	@file	Mutex.cpp
///	@brief	�~���[�e�b�N�X�N���X
//
//	2007/04/28	�^�c	�V�K�쐬

#include "stdafx.h"
#include "Mutex.hpp"

namespace Javelin
{
	///	@brief	�R���X�g���N�^
	Mutex::Mutex()
	{
		Handle = NULL ;
	}

	///	@brief	�f�X�g���N�^
	Mutex::~Mutex()
	{
		Close() ;
	}

	///	@brief	�~���[�e�b�N�X�쐬
	///	@param	name			�~���[�e�b�N�X��
	///	@param	initial_owner	== TRUE  : �쐬��J�����g�X���b�h�ŏ��L����\n
	///							== FALSE : �쐬��J�����g�X���b�h�ŏ��L���Ȃ�
	///	@retval	ERROR_SUCCESS			Mutex���쐬����
	///	@retval	ERROR_ALREADY_EXISTS	Mutex�����݂����i�I�u�W�F�N�g�͗L���j
	///	@retval	���̑�					winerror.h����
	HRESULT Mutex::Create( LPCTSTR name, BOOL initial_owner )
	{
		Handle = ::CreateMutex( NULL, initial_owner, name ) ;
		return ::GetLastError() ;
	}

	///	@brief	�~���[�e�b�N�X�����
	///	@return	winerror.h����
	HRESULT Mutex::Close()
	{
		if ( Handle == NULL ) return ERROR_SUCCESS ;

		HRESULT result = ::CloseHandle( Handle ) ? ERROR_SUCCESS : ::GetLastError() ;
		Handle = NULL ;
		return result ;
	}
}

// [EOF]
