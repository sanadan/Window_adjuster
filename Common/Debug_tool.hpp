///	@file	Debug_tool.h
///	@brief	�f�o�b�O�c�[��
//
//	2007/04/15	�^�c	�V�K�쐬

#pragma once

// �}�N��
#define	LOG_TO_STR2( x )	#x
#define LOG_TO_STR( x )	LOG_TO_STR2( x )
#define	LOG_FATAL_STR()	_T( __FILE__ ) _T( "(" ) _T( LOG_TO_STR( __LINE__ ) ) _T( ") : fatal error" )
#define	LOG_FATAL()	Log_write( _T( "\"\r\n" ) LOG_FATAL_STR() _T( "\"" ) )
#define	LOG( x )	Log_write( x )
#define	FATAL_MESSAGE()	::MessageBox( NULL, LOG_FATAL_STR(), NULL, MB_OK | MB_ICONERROR ) ;

void Log_init( LPCTSTR file_path ) ;
void Log_write( LPCTSTR message ) ;

namespace Javelin
{
	class Debug_log
	{
	public :
		Debug_log() ;

		void Set_file_path( LPCTSTR file_path ) ;

		void Write( LPCTSTR message ) ;

	private :
		TCHAR File_path[ MAX_PATH ] ;

		// �R�s�[�֎~
		Debug_log( const Debug_log& debug_log ) ;
		Debug_log& operator =( const Debug_log& debug_log ) ;

		static const DWORD Date_buffer_length ;
		static const DWORD Time_buffer_length ;
	} ;

	class Debug_log_file
	{
	public :
		Debug_log_file( LPCTSTR file_path ) ;
		~Debug_log_file() ;

		BOOL Write( LPCTSTR message ) ;

	private :
		HANDLE File_handle ;

		// �R�s�[�֎~
		Debug_log_file( const Debug_log_file& debug_log_file ) ;
		Debug_log_file& operator =( const Debug_log& debug_log_file ) ;
	} ;
}

// [EOF]
