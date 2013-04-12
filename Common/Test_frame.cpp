///
///	@file	Test_frame.cpp
///	@brief	�ȈՃe�X�g�t���[�����[�N
///
//	2005/12/23	�^�c	�V�K�쐬
//

#include "stdafx.h"
#include "Test_frame.hpp"

// namespace�錾
namespace Javelin
{
	// �O���[�o���ϐ�
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
	///	@brief	�R���\�[���ƃA�E�g�v�b�g�Ƀ��b�Z�[�W�o��
	///	@param	message	���b�Z�[�W
	///
	void Test_frame::Output( LPCTSTR message )
	{
		// ���b�Z�[�W�o��
		::OutputDebugString( message ) ;
#if _MSC_VER >= 1400
		::_tprintf_s( message ) ;
#else
		::_tprintf( message ) ;
#endif
	}

	///
	///	@fn		void Test_frame::Error( LPCTSTR file_path, DWORD line_num, LPCTSTR type_name, LPCTSTR test_content )
	///	@brief	�G���[�^�O�W�����v�ł���`���ŃG���[���b�Z�[�W�\��
	///	@param	file_path		�G���[�̔��������t�@�C���̃p�X
	///	@param	line_num		�G���[�s�ԍ�
	///	@param	type_name		�G���[���
	///	@param	test_content	�e�X�g���e
	///
	void Test_frame::Error( LPCTSTR file_path, DWORD line_num, LPCTSTR type_name, LPCTSTR test_content )
	{
		Indent() ;

		// ���b�Z�[�W�o��
		TCHAR message[ MAX_PATH ] ;
		::wsprintf( message, _T( "%s(%d) : %s( %s ) is failed.\n" ), file_path, line_num, type_name, test_content ) ;
		Output( message ) ;

		// �G���[�J�E���g
		Test_error_num++ ;
		Total_error_num++ ;
	}

	///
	///	@fn		void Test_frame::Start( LPCTSTR name )
	///	@brief	�e�X�g�J�n����
	///	@param	name	�e�X�g��
	///
	void Test_frame::Start( LPCTSTR name )
	{
		// �����ݒ�
		Test_error_num = 0 ;
		::lstrcpyn( Test_name, name, sizeof ( Test_name ) / sizeof ( Test_name[ 0 ] ) ) ;
		Test_num = 0 ;

		// �J�n���b�Z�[�W�o��
		Indent() ;
		Output( Test_name ) ;
		Output( _T( " start.\n" ) ) ;

		Indent_num++ ;
	}

	///
	///	@fn		int Test_frame::End( Test_frame test_frame_ptr )
	///	@brief	�e�X�g�I������
	///
	int Test_frame::End( Test_frame *test_frame_ptr )
	{
		if ( Indent_num > 0 ) Indent_num-- ;

		// �I�����b�Z�[�W�o��
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
	///	@brief	�o�C�i���_���v
	///	@param	title	�_���v�̕\��
	///	@param	data_ptr	�_���v�f�[�^�|�C���^
	///	@param	size	�_���v�f�[�^�T�C�Y�i�o�C�g�j
	///
	void Test_frame::Dump( LPCTSTR title, const void *data_ptr, DWORD size )
	{
		// �_���v�\��^�A�h���X�^�T�C�Y�o��
		Indent() ;
		TCHAR message[ MAX_PATH ] ;
		::wsprintf( message, _T( "%s = 0x%x / size = 0x%x\n" ), title, data_ptr, size ) ;
		Output( message ) ;

		if ( data_ptr == NULL )
		{	// NULL�|�C���^������
			// �������Ȃ�
			return ;
		}

		// �_���v�o��
		const BYTE *dump_ptr = ( const BYTE * )data_ptr ;
		DWORD pos = 0 ;
		TCHAR szBuf[ 8 ] ;
		while ( pos < size )
		{
			// ���΃A�h���X���x���ݒ�
			::wsprintf( message, _T( "%04x :" ), pos ) ;
			for ( int i = 0 ; i < 16 ; i++ )
			{
				if ( pos >= size )
				{	// �I�[�ɒB����
					// ���[�v�𔲂���
					break ;
				}

				// �_���v�f�[�^�ǉ�
				::wsprintf( szBuf, _T( " %02x" ), dump_ptr[ pos ] ) ;
				::lstrcat( message, szBuf ) ;

				pos++ ;
			}

			// �_���v�f�[�^�o��
			Indent() ;
			Output( message ) ;
			Output( _T( "\n" ) ) ;
		}
	}

	///
	///	@fn		void TestCount()
	///	@brief	�e�X�g�J�E���g
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
