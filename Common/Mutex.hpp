///	@file	Mutex.hpp
///	@brief	�~���[�e�b�N�X�N���X
//
//	2007/04/28	�^�c	�V�K�쐬

#pragma once

namespace Javelin
{
	///	@brief	�~���[�e�b�N�X�N���X
	class Mutex
	{
	public :
		Mutex() ;
		~Mutex() ;

		HRESULT Create( LPCTSTR name, BOOL initial_owner = FALSE ) ;
		HRESULT Close() ;

	private :
		HANDLE Handle ;

		// �R�s�[�֎~����
		Mutex( const Mutex& ) ;
		Mutex& operator =( const Mutex& ) ;
	} ;
}

// [EOF]
