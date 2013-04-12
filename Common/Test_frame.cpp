///
///	@file	Test_frame.cpp
///	@brief	簡易テストフレームワーク
///
//	2005/12/23	真田	新規作成
//

#include "stdafx.h"
#include "Test_frame.hpp"

// namespace宣言
namespace Javelin
{
	// グローバル変数
	Test_frame *Current_test_frame_ptr = NULL ;
	DWORD Total_error_num = 0 ;
	DWORD Indent_num = 0 ;

	//
	Test_frame::Test_frame()
	{
		Test_num = 0 ;
		Test_error_num = 0 ;
	}

	///
	///	@fn		void Test_frame::Output( LPCTSTR message )
	///	@brief	コンソールとアウトプットにメッセージ出力
	///	@param	message	メッセージ
	///
	void Test_frame::Output( LPCTSTR message )
	{
		// メッセージ出力
		::OutputDebugString( message ) ;
#if _MSC_VER >= 1400
		::_tprintf_s( message ) ;
#else
		::_tprintf( message ) ;
#endif
	}

	///
	///	@fn		void Test_frame::Error( LPCTSTR file_path, DWORD line_num, LPCTSTR type_name, LPCTSTR test_content )
	///	@brief	エラータグジャンプできる形式でエラーメッセージ表示
	///	@param	file_path		エラーの発生したファイルのパス
	///	@param	line_num		エラー行番号
	///	@param	type_name		エラー種別
	///	@param	test_content	テスト内容
	///
	void Test_frame::Error( LPCTSTR file_path, DWORD line_num, LPCTSTR type_name, LPCTSTR test_content )
	{
		Indent() ;

		// メッセージ出力
		TCHAR message[ MAX_PATH ] ;
		::wsprintf( message, _T( "%s(%d) : %s( %s ) is failed.\n" ), file_path, line_num, type_name, test_content ) ;
		Output( message ) ;

		// エラーカウント
		Test_error_num++ ;
		Total_error_num++ ;
	}

	///
	///	@fn		void Test_frame::Start( LPCTSTR name )
	///	@brief	テスト開始処理
	///	@param	name	テスト名
	///
	void Test_frame::Start( LPCTSTR name )
	{
		// 初期設定
		Test_error_num = 0 ;
		::lstrcpyn( Test_name, name, sizeof ( Test_name ) / sizeof ( Test_name[ 0 ] ) ) ;
		Test_num = 0 ;

		// 開始メッセージ出力
		Indent() ;
		Output( Test_name ) ;
		Output( _T( " start.\n" ) ) ;

		Indent_num++ ;
	}

	///
	///	@fn		int Test_frame::End( Test_frame test_frame_ptr )
	///	@brief	テスト終了処理
	///
	int Test_frame::End( Test_frame *test_frame_ptr )
	{
		if ( Indent_num > 0 ) Indent_num-- ;

		// 終了メッセージ出力
		Indent() ;
		TCHAR message[ MAX_PATH ] ;
		Output( Test_name ) ;
		::wsprintf( message, _T( " end. Test : %d, Error : %d\n" ), Test_num, Test_error_num ) ;
		Output( message ) ;

		if ( test_frame_ptr != NULL )
		{
			test_frame_ptr->Test_num += Test_num ;
			test_frame_ptr->Test_error_num += Test_error_num ;
		}

		return Test_error_num ;
	}

	///
	///	@fn		void Test_frame::Dump( LPCTSTR title, const void *data_ptr, DWORD size )
	///	@brief	バイナリダンプ
	///	@param	title	ダンプの表題
	///	@param	data_ptr	ダンプデータポインタ
	///	@param	size	ダンプデータサイズ（バイト）
	///
	void Test_frame::Dump( LPCTSTR title, const void *data_ptr, DWORD size )
	{
		// ダンプ表題／アドレス／サイズ出力
		Indent() ;
		TCHAR message[ MAX_PATH ] ;
		::wsprintf( message, _T( "%s = 0x%x / size = 0x%x\n" ), title, data_ptr, size ) ;
		Output( message ) ;

		if ( data_ptr == NULL )
		{	// NULLポインタだった
			// 何もしない
			return ;
		}

		// ダンプ出力
		const BYTE *dump_ptr = ( const BYTE * )data_ptr ;
		DWORD pos = 0 ;
		TCHAR szBuf[ 8 ] ;
		while ( pos < size )
		{
			// 相対アドレスラベル設定
			::wsprintf( message, _T( "%04x :" ), pos ) ;
			for ( int i = 0 ; i < 16 ; i++ )
			{
				if ( pos >= size )
				{	// 終端に達した
					// ループを抜ける
					break ;
				}

				// ダンプデータ追加
				::wsprintf( szBuf, _T( " %02x" ), dump_ptr[ pos ] ) ;
				::lstrcat( message, szBuf ) ;

				pos++ ;
			}

			// ダンプデータ出力
			Indent() ;
			Output( message ) ;
			Output( _T( "\n" ) ) ;
		}
	}

	///
	///	@fn		void TestCount()
	///	@brief	テストカウント
	///
	void Test_frame::Count()
	{
		Test_num++ ;
	}

	///
	DWORD Test_frame::Get_total_error_num()
	{
		return Total_error_num ;
	}

	///
	DWORD Test_frame::Get_test_error_num()
	{
		return Test_error_num ;
	}

	///
	void Test_frame::Indent()
	{
		DWORD indent_num = min( Indent_num, MAX_PATH - 1 ) ;
		TCHAR indent_str[ MAX_PATH ] ;
		for ( DWORD num = 0 ; num < indent_num ; num++ )
		{
			indent_str[ num ] = _T( ' ' ) ;
		}
		indent_str[ indent_num ] = _T( '\0' ) ;
		Output( indent_str ) ;
	}
}	// namespace Javelin

// [[[[[ End of this module ]]]]]
