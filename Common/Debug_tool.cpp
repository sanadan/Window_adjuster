///	@file	Debug_tool.cpp
///	@breif	�f�o�b�O�c�[��
//
//	2007/04/15	�^�c	�V�K�쐬

#include "stdafx.h"
#include "Debug_tool.hpp"

// ���̐錾
const DWORD Javelin::Debug_log::Date_buffer_length = 32 ;	///< ���t�p�o�b�t�@�T�C�Y
const DWORD Javelin::Debug_log::Time_buffer_length = 32 ;	///< ���ԗp�o�b�t�@�T�C�Y
Javelin::Debug_log Javelin::Debug_log ;

///	@breif	���O������
void Log_init( LPCTSTR file_path )
{
	Javelin::Debug_log.Set_file_path( file_path ) ;
}

///	@brief	���O��������
void Log_write( LPCTSTR message )
{
	Javelin::Debug_log.Write( message ) ;
}

namespace Javelin
{
	///	@brief	�R���X�g���N�^
	Debug_log::Debug_log()
	{
		File_path[ 0 ] = _T( '\0' ) ;
	}

	///	@brief	�t�@�C���p�X�ݒ�
	///	@param	file_path	�t�@�C���p�X
	///	@note	�N���X�̓Ɨ������l������TPath�͎g�p���Ă��Ȃ�
	void Debug_log::Set_file_path( LPCTSTR file_path )
	{
		::lstrcpyn( File_path, file_path, ARRAYSIZE( File_path ) ) ;
	}

	///	@brief	���b�Z�[�W��������
	///	@param	message	���b�Z�[�W
	void Debug_log::Write( LPCTSTR message )
	{
		// ���ݓ������擾
		TCHAR date[ Date_buffer_length ] ;
		date[ 0 ] = _T( '\0' ) ;
		::GetDateFormat( NULL, DATE_SHORTDATE, NULL, NULL, date, ARRAYSIZE( date ) ) ;
		TCHAR time[ Time_buffer_length ] ;
		time[ 0 ] = _T( '\0' ) ;
		::GetTimeFormat( NULL, TIME_FORCE24HOURFORMAT, NULL, NULL, time, ARRAYSIZE( time ) ) ;

		Debug_log_file log_file( File_path ) ;

		log_file.Write( date ) ;
		log_file.Write( _T( " " ) ) ;
		log_file.Write( time ) ;
		log_file.Write( _T( "," ) ) ;
		log_file.Write( message ) ;
		log_file.Write( _T( "\r\n" ) ) ;
	}

// -----

	///	@brief	�R���X�g���N�^
	///	@param	file_path	�t�@�C���p�X
	Debug_log_file::Debug_log_file( LPCTSTR file_path )
	{
		File_handle = ::CreateFile( file_path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) ;
		if ( File_handle != INVALID_HANDLE_VALUE )
		{	// �t�@�C���I�[�v������
#if defined ( UNICODE )
			if ( ::GetLastError() == 0 )
			{	// �V�K�쐬��
				// BOM������
				const TCHAR bom[] = { 0xfeff, 0x0000 } ;
				Write( bom ) ;
			}
#endif
			::SetFilePointer( File_handle, 0, NULL, FILE_END ) ;
		}
	}

	///	@brief	�f�X�g���N�^
	Debug_log_file::~Debug_log_file()
	{
		if ( File_handle != INVALID_HANDLE_VALUE )
		{
			::CloseHandle( File_handle ) ;
			File_handle = INVALID_HANDLE_VALUE ;
		}
	}

	///	@brief	��������
	///	@param	message	���b�Z�[�W
	///	@retval	TRUE	�������ݐ���
	///	@retval	FALSE	�������ݎ��s
	BOOL Debug_log_file::Write( LPCTSTR message )
	{
		if ( File_handle == INVALID_HANDLE_VALUE ) return FALSE ;

		DWORD written ;
		return ::WriteFile( File_handle, message, ::lstrlen( message ) * sizeof ( TCHAR ), &written, NULL ) ;
	}
}

// [EOF]
