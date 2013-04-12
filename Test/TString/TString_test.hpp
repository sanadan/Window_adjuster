///
///	@file	TString_test.hpp
///	@brief	������N���X�@�e�X�g
///	@note	�ˑ��FWin32 API�^Javelin::TString�^Javelin::TestFrame
///
//	2006/01/09	�c���@��	�V�K�쐬
//

//	�C���N���[�h
#include "stdafx.h"
#include "TString.hpp"
#include "Test_frame.hpp"
#include "resource.h"
#include "Messages.h"
#include "strsafe.h"

namespace Javelin
{
	namespace TString_test
	{
		///
		///	@fn		void Constructor_test()
		///	@brief	�R���X�g���N�^�e�X�g
		///
		DWORD Constructor_test()
		{
			// ��ł��邱�Ƃ��m�F
			Javelin::TString tstring1 ;
			JT_CHECK( tstring1 == _T( "" ) ) ;

			// �R�s�[�R���X�g���N�^
			Javelin::TString tstring2( _T( "Test" ) ) ;
			JT_CHECK( tstring2 == _T( "Test" ) ) ;

			return JT_RETURN() ;
		}

		///
		///	@fn		void Operator_test()
		///	@brief	�I�y���[�^�e�X�g
		///
		DWORD Operator_test()
		{
			// ���
			Javelin::TString tstring1 ;
			tstring1 = _T( "Test" ) ;
			JT_CHECK( tstring1 == _T( "Test" ) ) ;

			// �ǉ�
			tstring1 += _T( "Test2" ) ;
			JT_CHECK( tstring1 == _T( "TestTest2" ) ) ;

			return JT_RETURN() ;
		}

		///
		///	@fn		void Buffer_test()
		///	@brief	�o�b�t�@�e�X�g
		///
		DWORD Buffer_test()
		{
			//
			Javelin::TString tstring1 ;
			LPTSTR string_ptr = tstring1.GetBuffer( 5 ) ;
			JT_REQUIRE( string_ptr != NULL ) ;
			::StringCchCopy( string_ptr, 5, _T( "Test" ) ) ;
			tstring1.ReleaseBuffer() ;
			JT_CHECK( tstring1 == _T( "Test" ) ) ;

			return JT_RETURN() ;
		}

		///
		///	@fn		void LoadString_test()
		///	@brief	LoadString�e�X�g
		///
		DWORD LoadString_test()
		{
			//
			Javelin::TString tstring1 ;
			JT_REQUIRE( tstring1.Load_string( IDS_STRING1 ) == ERROR_SUCCESS ) ;
			JT_CHECK( tstring1 == _T( "Resource Test" ) ) ;

			JT_REQUIRE( tstring1.Load_string( IDS_STRING2 ) == ERROR_SUCCESS ) ;
			JT_CHECK( tstring1 == _T( "���\�[�X�e�X�g" ) ) ;

			return JT_RETURN() ;
		}

		///
		///	@fn		void FormatMessage_test()
		///	@brief	FormatMessage�e�X�g
		///
		DWORD FormatMessage_test()
		{
			//
			Javelin::TString tstring1 ;
			JT_REQUIRE( tstring1.FormatMessage( TSTRING_TEST_FORMAT1, _T( "first" ), _T( "second" ) ) > 0 ) ;
			JT_CHECK( tstring1 == _T( "second / first" ) ) ;

			JT_REQUIRE( tstring1.FormatMessage( TSTRING_TEST_FORMAT2, _T( "��" ), _T( "��" ) ) > 0 ) ;
			JT_CHECK( tstring1 == _T( "��^��" ) ) ;

			JT_REQUIRE( tstring1.FormatMessage( _T( "%2 / %1" ), _T( "first" ), _T( "second" ) ) > 0 ) ;
			JT_CHECK( tstring1 == _T( "second / first" ) ) ;

			JT_REQUIRE( tstring1.FormatMessage( _T( "%2�^%1" ), _T( "��" ), _T( "��" ) ) > 0 ) ;
			JT_CHECK( tstring1 == _T( "��^��" ) ) ;

			return JT_RETURN() ;
		}

		///
		///	@fn		DWORD Test()
		///	@brief	�e�X�g���C��
		///
		DWORD Test()
		{
			JT_TEST_REQUIRE( Constructor_test ) ;
			JT_TEST_CHECK( Operator_test ) ;
			JT_TEST_CHECK( Buffer_test ) ;
			JT_TEST_CHECK( LoadString_test ) ;
			JT_TEST_CHECK( FormatMessage_test ) ;

			return JT_FINISH() ;
		}
	} // namespace TString_test
} // namespace Javelin

// [[[[[ End of this module ]]]]]
