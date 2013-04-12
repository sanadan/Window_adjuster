///
///	@file	Comb_sort_test.hpp
///	@brief	コムソート　テスト
///	@note	依存：Win32 API／Javelin::Comb_sort／Javelin::Test_frame
///
//	2007/05/21	真田	新規作成
//

#pragma once

//	インクルード
#include "Comb_sort.hpp"
#include "Test_frame.hpp"
#include <list>

namespace Javelin
{
	namespace Comb_sort_test
	{
		///
		///	@fn		void Ascending_test()
		///	@brief	昇順テスト
		///
		DWORD Ascending_test()
		{
			// テストデータ作成
			std::list< char > test1 ;
			for ( int i = 0 ; i < 26 ; i++ )
			{
				test1.push_back( static_cast< char >( 'a' + i ) ) ;
				test1.push_back( static_cast< char >( 'A' + i ) ) ;
			}
			std::list< char > test2 ;
			for ( int i = 0 ; i < 26 ; i++ ) test2.push_back( static_cast< char >( 'A' + i ) ) ;
			for ( int i = 0 ; i < 26 ; i++ ) test2.push_back( static_cast< char >( 'a' + i ) ) ;

			// 昇順ソート
			Comb_sort( test1.begin(), test1.end() ) ;

			// 確認
			std::list< char >::const_iterator it1 = test1.begin() ;
			std::list< char >::const_iterator it2 = test2.begin() ;
			for ( ; it1 != test1.end() ; it1++, it2++ )
			{
				JT_REQUIRE( *it1 == *it2 ) ;
			}

			return JT_RETURN() ;
		}

		///
		///	@fn		void Descending_test()
		///	@brief	降順テスト
		///
		DWORD Descending_test()
		{
			// テストデータ作成
			std::list< char > test1 ;
			for ( int i = 0 ; i < 26 ; i++ )
			{
				test1.push_back( static_cast< char >( 'a' + i ) ) ;
				test1.push_back( static_cast< char >( 'A' + i ) ) ;
			}
			std::list< char > test2 ;
			for ( int i = 0 ; i < 26 ; i++ ) test2.push_back( static_cast< char >( 'z' - i ) ) ;
			for ( int i = 0 ; i < 26 ; i++ ) test2.push_back( static_cast< char >( 'Z' - i ) ) ;

			// 降順ソート
			Comb_sort( test1.begin(), test1.end(), std::greater< char >() ) ;

			// 確認
			std::list< char >::const_iterator it1 = test1.begin() ;
			std::list< char >::const_iterator it2 = test2.begin() ;
			for ( ; it1 != test1.end() ; it1++, it2++ )
			{
				JT_REQUIRE( *it1 == *it2 ) ;
			}

			return JT_RETURN() ;
		}

		///
		///	@fn		DWORD Test()
		///	@brief	テスト
		///
		DWORD Test()
		{
			JT_TEST_REQUIRE( Ascending_test ) ;
			JT_TEST_REQUIRE( Descending_test ) ;

			return JT_RETURN() ;
		}
	}
}

// [[[[[ End of this module ]]]]]
