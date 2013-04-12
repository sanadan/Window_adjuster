///
///	@file	Buffer_test.hpp
///	@brief	�o�b�t�@�N���X�@�e�X�g
///	@note	�ˑ��FWin32 API�^Javelin::Buffer�^Javelin::Test_frame
///
//	2006/01/09	�^�c	�V�K�쐬
//

#pragma once

//	�C���N���[�h
#include "Buffer.hpp"
#include "Test_frame.hpp"
#include <strsafe.h>

namespace Javelin
{
	namespace Buffer_test
	{
		///
		///	@fn		void Constructor_test()
		///	@brief	�R���X�g���N�^�e�X�g
		///
		DWORD Constructor_test()
		{
			// ��ł��邱�Ƃ��m�F
			Javelin::Buffer buffer1 ;
			JT_REQUIRE( buffer1.Get_size() == 0 ) ;
			JT_REQUIRE( buffer1.Get_ptr() == NULL ) ;

			// �o�C�i���f�[�^����̃R���X�g���N�g
			const CHAR test_str[] = "Test String" ;
			Javelin::Buffer buffer2( test_str, sizeof ( test_str ) ) ;
			JT_REQUIRE( buffer2.Get_size() == sizeof ( test_str ) ) ;
			JT_REQUIRE( ::strcmp( ( LPSTR )buffer2.Get_ptr(), test_str ) == 0 ) ;

			// �R�s�[�R���X�g���N�^
			Javelin::Buffer buffer3( buffer2 ) ;
			JT_REQUIRE( buffer3.Get_size() == buffer2.Get_size() ) ;
			JT_REQUIRE( ::strcmp( ( LPSTR )buffer3.Get_ptr(), ( LPSTR )buffer2.Get_ptr() ) == 0 ) ;
			JT_REQUIRE( buffer3.Get_ptr() != buffer2.Get_ptr() ) ;	// ���͈̂Ⴄ���Ƃ��m�F

			return JT_RETURN() ;
		}

		///
		///	@fn		void Alloc_test()
		///	@brief	�������m�ۃe�X�g
		///
		DWORD Alloc_test()
		{
			// 0�o�C�g�m��
			Javelin::Buffer buffer1 ;
			JT_REQUIRE( buffer1.Set_size( 0 ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == 0 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != NULL ) ;

			// �J��
			JT_CHECK( buffer1.Free() == ERROR_SUCCESS ) ;
			JT_CHECK( buffer1.Get_size() == 0 ) ;
			JT_CHECK( buffer1.Get_ptr() == NULL ) ;

			// 1�o�C�g�m��
			JT_REQUIRE( buffer1.Set_size( 1 ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == 1 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != NULL ) ;

			// �J��
			JT_CHECK( buffer1.Free() == ERROR_SUCCESS ) ;
			JT_CHECK( buffer1.Get_size() == 0 ) ;
			JT_CHECK( buffer1.Get_ptr() == NULL ) ;

			// 1K�o�C�g�m��
			JT_REQUIRE( buffer1.Set_size( 1024 ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == 1024 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != NULL ) ;

			// �J��
			JT_CHECK( buffer1.Free() == ERROR_SUCCESS ) ;
			JT_CHECK( buffer1.Get_size() == 0 ) ;
			JT_CHECK( buffer1.Get_ptr() == NULL ) ;

			// 1M�o�C�g�m��
			JT_REQUIRE( buffer1.Set_size( 1024 * 1024 ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == 1024 * 1024 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != NULL ) ;

			// �J��
			JT_CHECK( buffer1.Free() == ERROR_SUCCESS ) ;
			JT_CHECK( buffer1.Get_size() == 0 ) ;
			JT_CHECK( buffer1.Get_ptr() == NULL ) ;

			return JT_RETURN() ;
		}

		///
		///	@fn		void Resize_test()
		///	@brief	�������Ċm�ۃe�X�g
		///
		DWORD Resize_test()
		{
			// 0�o�C�g�m��
			Javelin::Buffer buffer1 ;
			JT_REQUIRE( buffer1.Set_size( 0 ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == 0 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != NULL ) ;

			// 1�o�C�g�Ċm��
			JT_REQUIRE( buffer1.Resize( 1 ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == 1 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != NULL ) ;

			// �e�X�g�p�f�[�^�ݒ�
			PBYTE data_ptr = ( PBYTE )buffer1.Get_ptr() ;
			*data_ptr = 0x55 ;

			// 256�o�C�g�Ċm��
			JT_REQUIRE( buffer1.Resize( 256 ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == 256 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != NULL ) ;
			data_ptr = ( PBYTE )buffer1.Get_ptr() ;
			JT_REQUIRE( *data_ptr == 0x55 ) ;

			// �e�X�g�p�f�[�^�ݒ�
			const CHAR test_str[] = "Test String" ;
			LPSTR buffer_ptr = ( LPSTR )buffer1.Get_ptr() ;
			::StringCbCopyA( buffer_ptr, buffer1.Get_size(), test_str ) ;

			// 128�o�C�g�Ċm��
			JT_REQUIRE( buffer1.Resize( 128 ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == 128 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != NULL ) ;
			buffer_ptr = ( LPSTR )buffer1.Get_ptr() ;
			JT_REQUIRE( ::strcmp( buffer_ptr, test_str ) == 0 ) ;

			return JT_RETURN() ;
		}

		///
		///	@fn		DWORD Copy_test()
		///	@brief	�R�s�[�e�X�g
		///
		DWORD Copy_test()
		{
			// �|�C���^����R�s�[
			const CHAR test_str[] = "Test String" ;
			Javelin::Buffer buffer1 ;
			JT_REQUIRE( buffer1.Copy( test_str, sizeof ( test_str ) ) == ERROR_SUCCESS ) ;
			JT_REQUIRE( buffer1.Get_size() == sizeof ( test_str ) ) ;
			JT_REQUIRE( ::strcmp( ( LPCSTR )buffer1.Get_ptr(), test_str ) == 0 ) ;
			JT_REQUIRE( buffer1.Get_ptr() != test_str ) ;

			// ����ɂ��R�s�[
			Javelin::Buffer buffer2 ;
			buffer2 = buffer1 ;
			JT_REQUIRE( buffer2.Get_size() == buffer1.Get_size() ) ;
			JT_REQUIRE( ::strcmp( ( LPCSTR )buffer2.Get_ptr(), ( LPCSTR )buffer1.Get_ptr() ) == 0 ) ;
			JT_REQUIRE( buffer2.Get_ptr() != buffer1.Get_ptr() ) ;

			return JT_RETURN() ;
		}

		///
		///	@fn		DWORD Test()
		///	@brief	�e�X�g
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
