///
///	@file	Buffer_test.hpp
///	@brief	バッファクラス　テスト
///	@note	依存：Win32 API／Javelin::Buffer／Javelin::Test_frame
///
//	2006/01/09	真田	新規作成
//

#pragma once

//	インクルード
#include "Buffer.hpp"
#include "Test_frame.hpp"
#include <strsafe.h>

namespace Javelin
{
	namespace Buffer_test
	{
		///
		///	@fn		void Constructor_test()
		///	@brief	コンストラクタテスト
		///
		DWORD Constructor_test()
		{
			// 空であることを確認
			Javelin::Buffer buffer1 ;
			JT_REQUIRE( buffer1.Get_size() == 0 ) ;
			JT_REQUIRE( buffer1.Get_ptr() == NULL ) ;

			// バイナリデータからのコンストラクト
			const CHAR test_str[] = "Test String" ;
			Javelin::Buffer buffer2( test_str, sizeof ( test_str ) ) ;
			JT_REQUIRE( buffer2.Get_size() == sizeof ( test_str ) ) ;
			JT_REQUIRE( ::strcmp( ( LPSTR )buffer2.Get_ptr(), test_str ) == 0 ) ;

			// コピーコンストラクタ
			Javelin::Buffer buffer3( buffer2 ) ;
			JT_REQUIRE( buffer3.Get_size() == buffer2.Get_size() ) ;
			JT_REQUIRE( ::strcmp( ( LPSTR )buffer3.Get_ptr(), ( LPSTR )buffer2.Get_ptr() ) == 0 ) ;
			JT_REQUIRE( buffer3.Get_ptr() != buffer2.Get_ptr() ) ;	// 実体は違うことを確認

			return JT_RETURN() ;
		}

		///
		///	@fn		void Alloc_test()
		///	@brief	メモリ確保テスト
		///
		DWORD Alloc_test()
		{
			// 0バイト確保
			Javelin::Buffer buffer1 ;
			JT_REQUIRE( buffer1.Set_size( 0 ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == 0 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != NULL ) ;

			// 開放
			JT_CHECK( buffer1.Free() == ERROR_SUCCESS ) ;
			JT_CHECK( buffer1.Get_size() == 0 ) ;
			JT_CHECK( buffer1.Get_ptr() == NULL ) ;

			// 1バイト確保
			JT_REQUIRE( buffer1.Set_size( 1 ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == 1 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != NULL ) ;

			// 開放
			JT_CHECK( buffer1.Free() == ERROR_SUCCESS ) ;
			JT_CHECK( buffer1.Get_size() == 0 ) ;
			JT_CHECK( buffer1.Get_ptr() == NULL ) ;

			// 1Kバイト確保
			JT_REQUIRE( buffer1.Set_size( 1024 ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == 1024 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != NULL ) ;

			// 開放
			JT_CHECK( buffer1.Free() == ERROR_SUCCESS ) ;
			JT_CHECK( buffer1.Get_size() == 0 ) ;
			JT_CHECK( buffer1.Get_ptr() == NULL ) ;

			// 1Mバイト確保
			JT_REQUIRE( buffer1.Set_size( 1024 * 1024 ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == 1024 * 1024 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != NULL ) ;

			// 開放
			JT_CHECK( buffer1.Free() == ERROR_SUCCESS ) ;
			JT_CHECK( buffer1.Get_size() == 0 ) ;
			JT_CHECK( buffer1.Get_ptr() == NULL ) ;

			return JT_RETURN() ;
		}

		///
		///	@fn		void Resize_test()
		///	@brief	メモリ再確保テスト
		///
		DWORD Resize_test()
		{
			// 0バイト確保
			Javelin::Buffer buffer1 ;
			JT_REQUIRE( buffer1.Set_size( 0 ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == 0 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != NULL ) ;

			// 1バイト再確保
			JT_REQUIRE( buffer1.Resize( 1 ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == 1 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != NULL ) ;

			// テスト用データ設定
			PBYTE data_ptr = ( PBYTE )buffer1.Get_ptr() ;
			*data_ptr = 0x55 ;

			// 256バイト再確保
			JT_REQUIRE( buffer1.Resize( 256 ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == 256 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != NULL ) ;
			data_ptr = ( PBYTE )buffer1.Get_ptr() ;
			JT_REQUIRE( *data_ptr == 0x55 ) ;

			// テスト用データ設定
			const CHAR test_str[] = "Test String" ;
			LPSTR buffer_ptr = ( LPSTR )buffer1.Get_ptr() ;
			::StringCbCopyA( buffer_ptr, buffer1.Get_size(), test_str ) ;

			// 128バイト再確保
			JT_REQUIRE( buffer1.Resize( 128 ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == 128 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != NULL ) ;
			buffer_ptr = ( LPSTR )buffer1.Get_ptr() ;
			JT_REQUIRE( ::strcmp( buffer_ptr, test_str ) == 0 ) ;

			return JT_RETURN() ;
		}

		///
		///	@fn		DWORD Copy_test()
		///	@brief	コピーテスト
		///
		DWORD Copy_test()
		{
			// ポインタからコピー
			const CHAR test_str[] = "Test String" ;
			Javelin::Buffer buffer1 ;
			JT_REQUIRE( buffer1.Copy( test_str, sizeof ( test_str ) ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == sizeof ( test_str ) ) ;
			JT_REQUIRE( ::strcmp( ( LPCSTR )buffer1.Get_ptr(), test_str ) == 0 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != test_str ) ;

			// 代入によるコピー
			Javelin::Buffer buffer2 ;
			buffer2 = buffer1 ;
			JT_REQUIRE( buffer2.Get_size() == buffer1.Get_size() ) ;
			JT_REQUIRE( ::strcmp( ( LPCSTR )buffer2.Get_ptr(), ( LPCSTR )buffer1.Get_ptr() ) == 0 ) ;
			JT_REQUIRE( buffer2.Get_ptr() != buffer1.Get_ptr() ) ;

			return JT_RETURN() ;
		}

		///
		///	@fn		DWORD Test()
		///	@brief	テスト
		///
		DWORD Test()
		{
			JT_TEST_REQUIRE( Constructor_test ) ;
			JT_TEST_REQUIRE( Alloc_test ) ;
			JT_TEST_REQUIRE( Resize_test ) ;
			JT_TEST_REQUIRE( Copy_test ) ;

			return JT_RETURN() ;
		}
	}
}

// [[[[[ End of this module ]]]]]
