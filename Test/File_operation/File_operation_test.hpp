///
///	@file	File_operation_test.hpp
///	@brief	ファイル操作クラス　テスト
///	@note	依存：Win32 API
///
//	2007/01/22	田中　悟	新規作成
//

//	インクルード
#include "stdafx.h"
#include "File_operation.hpp"
#include "Test_frame.hpp"
#include "TString.hpp"

namespace Javelin
{
	namespace File_operation_test
	{
		///
		///	@fn		void Constructor_test()
		///	@brief	コンストラクタテスト
		///
		DWORD Constructor_test()
		{
			Javelin::File_operation fo ;

			return JT_RETURN() ;
		}

		///	@brief	コピーテスト
		DWORD Copy_test()
		{
			Javelin::File_operation fo ;

//			Javelin::TString str ;
//			str << 2 ;
//			::OutputDebugString( str ) ;
//			JT_CHECK( fo.Copy() == ERROR_SUCCESS ) ;
			
			return JT_RETURN() ;
		}

		/// @brief	削除テスト

		/// @brief	移動テスト

		///	@brief	名前変更テスト

		///	@brief	後始末
		///
		///	@fn		DWORD Test()
		///	@brief	テストメイン
		///
		DWORD Test()
		{
			JT_TEST_REQUIRE( Constructor_test ) ;
			JT_TEST_CHECK( Copy_test ) ;
//			JT_TEST_CHECK( Buffer_test ) ;
//			JT_TEST_CHECK( LoadString_test ) ;
//			JT_TEST_CHECK( FormatMessage_test ) ;

			return JT_FINISH() ;
		}
	} // namespace File_operation_test
} // namespace Javelin

// [[[[[ End of this module ]]]]]
