///	@file	Process.hpp
///	@breif	プロセスクラス
//
//	2011/12/16	真田	新規作成

#pragma once

namespace Javelin
{
	///	@brief	プロセスクラス
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

	///	@brief	プロセスメモリー操作テンプレートクラス
	template< class _E > class ProcessMemory
	{
	public :
		///	@brief	コンストラクタ
		ProcessMemory( const Process& process )
		{
			Ptr = NULL ;
			Process_handle = process.Get_handle() ;
		}

		///	@brief	デストラクタ
		~ProcessMemory()
		{
			Free() ;
		}

		///	@brief	プロセスメモリー割り当て
		///	@retval	ERROR_SUCCESS	正常終了
		///	@retval	ERROR_ALREADY_EXISTS	既に割り当てている
		HRESULT Allocate( DWORD protect = PAGE_READONLY, DWORD type = MEM_COMMIT )
		{
			if ( Ptr != NULL ) return ERROR_ALREADY_EXISTS ;

			Ptr = ::VirtualAllocEx( Process_handle, NULL, sizeof ( _E ), type, protect ) ;
			if ( Ptr == NULL ) return ::GetLastError() ;
			return ERROR_SUCCESS ;
		}

		///	@brief	プロセスメモリー開放
		///	@return	winerror.h準拠
		HRESULT ProcessMemory::Free()
		{
			if ( Ptr == NULL ) return ERROR_SUCCESS ;

			BOOL success = ::VirtualFreeEx( Process_handle, Ptr, 0, MEM_RELEASE ) ;
			Ptr = NULL ;
			Process_handle = NULL ;
			if ( !success ) return ::GetLastError() ;
			return ERROR_SUCCESS ;
		}

		///	@brief	ポインタ取得
		///	@return	ポインタ
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
