///
///	@file	TPath_string_test.hpp
///	@brief	文字列クラス　テスト
///	@note	依存：Win32 API／Javelin::TPath_string／Javelin::TString／Javelin::TestFrame
///
//	2006/01/09	田中　悟	新規作成
//

//	インクルード
#include "stdafx.h"
#include "TPath_string.hpp"
#include "Test_frame.hpp"
#include "strsafe.h"

namespace Javelin
{
	namespace TPath_string_test
	{
		///
		///	@fn		void Constructor_test()
		///	@brief	コンストラクタテスト
		///
		DWORD Constructor_test()
		{
			// 空であることを確認
			Javelin::TPath_string TPath_string1 ;
			JT_CHECK( TPath_string1 == _T( "" ) ) ;

			// コピーコンストラクタ
			Javelin::TPath_string TPath_string2( _T( "Test" ) ) ;
			JT_CHECK( TPath_string2 == _T( "Test" ) ) ;

			return JT_RETURN() ;
		}

		// 結合

		// フォルダパス取得

		// ファイル名取得


		///
		///	@fn		DWORD Test()
		///	@brief	テストメイン
		///
		DWORD Test()
		{
			JT_TEST_REQUIRE( Constructor_test ) ;
//			JT_TEST_CHECK( Operator_test ) ;
//			JT_TEST_CHECK( Buffer_test ) ;
//			JT_TEST_CHECK( LoadString_test ) ;
//			JT_TEST_CHECK( FormatMessage_test ) ;

			return JT_FINISH() ;
		}
	} // namespace TPath_string_test
} // namespace Javelin

// [[[[[ End of this module ]]]]]
