///	@file	Buffer.cpp
///	@brief	�������o�b�t�@�N���X
///	@note	�ˑ��FWin32 API
//
//	2006/01/09	�^�c	�V�K�쐬

// �C���N���[�h
#include "stdafx.h"
#include "Buffer.hpp"

// namespace�錾
namespace Javelin
{
	///	@brief	�R���X�g���N�^
	Buffer::Buffer()
	{
		Initialize_member() ;
	}

	///	@brief	�o�C�i���f�[�^����̃R���X�g���N�^
	///	@param	data_ptr	�f�[�^�|�C���^
	///	@size	size		�f�[�^�T�C�Y
	Buffer::Buffer( const void *data_ptr, SIZE_T size )
	{
		Initialize_member() ;

		Copy( data_ptr, size ) ;
	}

	///	@brief	�R�s�[�R���X�g���N�^
	///	@param	buffer	�R�s�[��
	Buffer::Buffer( const Buffer& buffer )
	{
		Initialize_member() ;

		*this = buffer ;
	}

	///	@brief	�����o�ϐ�������
	void Buffer::Initialize_member()
	{
		Heap_handle = NULL ;
		Ptr = NULL ;
		Size = 0 ;
	}

	///	@brief	�f�X�g���N�^
	Buffer::~Buffer()
	{
		Free() ;
	}

	///	@brief	�T�C�Y�擾
	///	@return	�o�b�t�@�T�C�Y�i�o�C�g�j
	SIZE_T Buffer::Get_size() const
	{
		return Size ;
	}

	///	@brief	�|�C���^�擾
	///	@return	�|�C���^\n
	///			== NULL : �o�b�t�@���m�ۂ���Ă��Ȃ�
	PVOID Buffer::Get_ptr() const
	{
		return Ptr ;
	}

	///	@brief	�������m��
	///	@param	size	�T�C�Y�i�o�C�g�j
	///	@retval	ERROR_SUCCESS			����I��
	///	@retval	ERROR_NOT_ENOUGH_MEMORY	�������s��
	///	@retval	ERROR_INVALID_HANDLE	�v���Z�X�̃q�[�v�n���h�����擾�ł��Ȃ������i�����G���[�j
	///	@retval	ERROR_OUTOFMEMORY		�m�ې����������T�C�Y�擾�Ɏ��s�i�����G���[�j
	DWORD Buffer::Set_size( SIZE_T size )
	{
		Free() ;

		Heap_handle = ::GetProcessHeap() ;
		if ( Heap_handle == NULL )
		{	// �n���h�����擾�ł��Ȃ������i�ʏ�͂��蓾�Ȃ��͂��j
			return ERROR_INVALID_HANDLE ;
		}

		Ptr = ::HeapAlloc( Heap_handle, 0, size ) ;
		if ( Ptr == NULL )
		{	// �������s��
			return ERROR_NOT_ENOUGH_MEMORY ;
		}

		Size = size ;

		return ERROR_SUCCESS ;
	}

	///	@brief	���������m�ۂ��Ă�����J��
	///	@retval	ERROR_SUCCESS	����I��
	///	@retval	���̑�			WinError.h�ɏ���
	DWORD Buffer::Free()
	{
		if ( Ptr != NULL )
		{	// ���������m�ۂ��Ă���
			BOOL ret_flg = ::HeapFree( Heap_handle, 0, Ptr ) ;

			Initialize_member() ;

			if ( !ret_flg )
			{	// �G���[���������Ă���
				return ::GetLastError() ;
			}
		}

		return ERROR_SUCCESS ;
	}

	///	@brief	�������Ċm��
	///	@param	size	�T�C�Y�i�o�C�g�j
	///	@return	winerror.h����
	///	@note	�G���[�ł��f�[�^�͕ێ������
	DWORD Buffer::Resize( SIZE_T size )
	{
		if ( Ptr == NULL )
		{	// �������m�ۂ��Ă��Ȃ�
			return Set_size( size ) ;
		}

		PVOID new_ptr = ::HeapReAlloc( Heap_handle, 0, Ptr, size ) ;
		if ( new_ptr == NULL )
		{	// �Ċm�ێ��s
			// ���̃n���h���ƃ|�C���^�͗L���Ȃ̂ŁA�G���[��Ԃ�����
			return ::GetLastError() ;
		}

		Ptr = new_ptr ;
		Size = size ;

		return ERROR_SUCCESS ;
	}

	///	@brief	�f�[�^�R�s�[
	///	@param	data_ptr	�f�[�^�|�C���^
	///	@param	size		�f�[�^�T�C�Y�i�o�C�g�j
	///	@return	winerror.h����
	HRESULT Buffer::Copy( const void *data_ptr, SIZE_T size )
	{
		HRESULT result = Set_size( size ) ;
		if ( result != ERROR_SUCCESS ) return result ;

		::CopyMemory( Get_ptr(), data_ptr, size ) ;
		return ERROR_SUCCESS ;
	}

	///	@brief	�R�s�[
	///	@param	buffer	�R�s�[��
	///	@return	�R�s�[��̎Q��
	Buffer& Buffer::operator =( const Buffer& buffer )
	{
		Copy( buffer.Get_ptr(), buffer.Get_size() ) ;
		return *this ;
	}
}	// namespace Javelin

// [EOF]
