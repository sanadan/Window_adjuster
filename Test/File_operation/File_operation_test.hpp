///
///	@file	File_operation_test.hpp
///	@brief	�t�@�C������N���X�@�e�X�g
///	@note	�ˑ��FWin32 API
///
//	2007/01/22	�c���@��	�V�K�쐬
//

//	�C���N���[�h
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
		///	@brief	�R���X�g���N�^�e�X�g
		///
		DWORD Constructor_test()
		{
			Javelin::File_operation fo ;

			return JT_RETURN() ;
		}

		///	@brief	�R�s�[�e�X�g
		DWORD Copy_test()
		{
			Javelin::File_operation fo ;

//			Javelin::TString str ;
//			str << 2 ;
//			::OutputDebugString( str ) ;
//			JT_CHECK( fo.Copy() == ERROR_SUCCESS ) ;
			
			return JT_RETURN() ;
		}

		/// @brief	�폜�e�X�g

		/// @brief	�ړ��e�X�g

		///	@brief	���O�ύX�e�X�g

		///	@brief	��n��
		///
		///	@fn		DWORD Test()
		///	@brief	�e�X�g���C��
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
