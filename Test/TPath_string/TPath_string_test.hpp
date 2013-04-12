///
///	@file	TPath_string_test.hpp
///	@brief	������N���X�@�e�X�g
///	@note	�ˑ��FWin32 API�^Javelin::TPath_string�^Javelin::TString�^Javelin::TestFrame
///
//	2006/01/09	�c���@��	�V�K�쐬
//

//	�C���N���[�h
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
		///	@brief	�R���X�g���N�^�e�X�g
		///
		DWORD Constructor_test()
		{
			// ��ł��邱�Ƃ��m�F
			Javelin::TPath_string TPath_string1 ;
			JT_CHECK( TPath_string1 == _T( "" ) ) ;

			// �R�s�[�R���X�g���N�^
			Javelin::TPath_string TPath_string2( _T( "Test" ) ) ;
			JT_CHECK( TPath_string2 == _T( "Test" ) ) ;

			return JT_RETURN() ;
		}

		// ����

		// �t�H���_�p�X�擾

		// �t�@�C�����擾


		///
		///	@fn		DWORD Test()
		///	@brief	�e�X�g���C��
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
