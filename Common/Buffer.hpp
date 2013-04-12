///	@file	Buffer.hpp
///	@brief	�o�b�t�@�N���X
///	@note	�ˑ��FWin32 API
//
//	2006/01/09	�^�c	�V�K�쐬

#pragma once

// �C���N���[�h
#include <windows.h>

// namespace�錾
namespace Javelin
{
	///	@brief	�o�b�t�@�N���X
	class Buffer
	{
	public :
		Buffer() ;
		Buffer( const void *data_ptr, SIZE_T size ) ;
		Buffer( const Buffer& buffer ) ;
		~Buffer() ;

		SIZE_T Get_size() const ;
		DWORD Set_size( SIZE_T size ) ;
		DWORD Resize( SIZE_T size ) ;
		DWORD Free() ;

		PVOID Get_ptr() const ;

		HRESULT Copy( const void *data_ptr, SIZE_T size ) ;
		Buffer& operator =( const Buffer& buffer ) ;

	protected :
		void Initialize_member() ;

	private :
		HANDLE Heap_handle ;	///< �q�[�v�n���h��
		PVOID Ptr ;				///< �o�b�t�@�|�C���^
		SIZE_T Size ;			///< �o�b�t�@�T�C�Y

	} ;
}	// namespace Javelin

// [EOF]
