///	@file	Debug_tool.cpp
///	@breif	デバッグツール
//
//	2007/04/15	真田	新規作成

#include "stdafx.h"
#include "Debug_tool.hpp"

// 実体宣言
const DWORD Javelin::Debug_log::Date_buffer_length = 32 ;	///< 日付用バッファサイズ
const DWORD Javelin::Debug_log::Time_buffer_length = 32 ;	///< 時間用バッファサイズ
Javelin::Debug_log Javelin::Debug_log ;

///	@breif	ログ初期化
void Log_init( LPCTSTR file_path )
{
	Javelin::Debug_log.Set_file_path( file_path ) ;
}

///	@brief	ログ書き込み
void Log_write( LPCTSTR message )
{
	Javelin::Debug_log.Write( message ) ;
}

namespace Javelin
{
	///	@brief	コンストラクタ
	Debug_log::Debug_log()
	{
		File_path[ 0 ] = _T( '\0' ) ;
	}

	///	@brief	ファイルパス設定
	///	@param	file_path	ファイルパス
	///	@note	クラスの独立性を考慮してTPathは使用していない
	void Debug_log::Set_file_path( LPCTSTR file_path )
	{
		::lstrcpyn( File_path, file_path, ARRAYSIZE( File_path ) ) ;
	}

	///	@brief	メッセージ書き込み
	///	@param	message	メッセージ
	void Debug_log::Write( LPCTSTR message )
	{
		// 現在日時を取得
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

	///	@brief	コンストラクタ
	///	@param	file_path	ファイルパス
	Debug_log_file::Debug_log_file( LPCTSTR file_path )
	{
		File_handle = ::CreateFile( file_path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) ;
		if ( File_handle != INVALID_HANDLE_VALUE )
		{	// ファイルオープン成功
#if defined ( UNICODE )
			if ( ::GetLastError() == 0 )
			{	// 新規作成時
				// BOMを書く
				const TCHAR bom[] = { 0xfeff, 0x0000 } ;
				Write( bom ) ;
			}
#endif
			::SetFilePointer( File_handle, 0, NULL, FILE_END ) ;
		}
	}

	///	@brief	デストラクタ
	Debug_log_file::~Debug_log_file()
	{
		if ( File_handle != INVALID_HANDLE_VALUE )
		{
			::CloseHandle( File_handle ) ;
			File_handle = INVALID_HANDLE_VALUE ;
		}
	}

	///	@brief	書き込み
	///	@param	message	メッセージ
	///	@retval	TRUE	書き込み成功
	///	@retval	FALSE	書き込み失敗
	BOOL Debug_log_file::Write( LPCTSTR message )
	{
		if ( File_handle == INVALID_HANDLE_VALUE ) return FALSE ;

		DWORD written ;
		return ::WriteFile( File_handle, message, ::lstrlen( message ) * sizeof ( TCHAR ), &written, NULL ) ;
	}
}

// [EOF]
