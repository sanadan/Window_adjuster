///	@file	Process.hpp
///	@breif	�v���Z�X�N���X
//
//	2011/12/16	�^�c	�V�K�쐬

#pragma once

namespace Javelin
{
	///	@brief	�v���Z�X�N���X
	class Process
	{
	public :
		Process() ;
		~Process() ;

		HRESULT Open( HWND window_handle, DWORD access_flag = PROCESS_VM_READ, BOOL inherit = NULL ) ;
		HRESULT Close() ;

		HANDLE Get_handle() const ;

	private :
		HANDLE Handle ;
	} ;

	///	@brief	�v���Z�X�������[����e���v���[�g�N���X
	template< class _E > class ProcessMemory
	{
	public :
		///	@brief	�R���X�g���N�^
		ProcessMemory( const Process& process )
		{
			Ptr = NULL ;
			Process_handle = process.Get_handle() ;
		}

		///	@brief	�f�X�g���N�^
		~ProcessMemory()
		{
			Free() ;
		}

		///	@brief	�v���Z�X�������[���蓖��
		///	@retval	ERROR_SUCCESS	����I��
		///	@retval	ERROR_ALREADY_EXISTS	���Ɋ��蓖�ĂĂ���
		HRESULT Allocate( DWORD protect = PAGE_READONLY, DWORD type = MEM_COMMIT )
		{
			if ( Ptr != NULL ) return ERROR_ALREADY_EXISTS ;

			Ptr = ::VirtualAllocEx( Process_handle, NULL, sizeof ( _E ), type, protect ) ;
			if ( Ptr == NULL ) return ::GetLastError() ;
			return ERROR_SUCCESS ;
		}

		///	@brief	�v���Z�X�������[�J��
		///	@return	winerror.h����
		HRESULT ProcessMemory::Free()
		{
			if ( Ptr == NULL ) return ERROR_SUCCESS ;

			BOOL success = ::VirtualFreeEx( Process_handle, Ptr, 0, MEM_RELEASE ) ;
			Ptr = NULL ;
			Process_handle = NULL ;
			if ( !success ) return ::GetLastError() ;
			return ERROR_SUCCESS ;
		}

		///	@brief	�|�C���^�擾
		///	@return	�|�C���^
		_E* Get_ptr()
		{
			return static_cast< _E* >( Ptr ) ;
		}

	private :
		LPVOID Ptr ;
		HANDLE Process_handle ;
	} ;
}	// namespace Javelin

// [EOF]
