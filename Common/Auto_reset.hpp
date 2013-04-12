///
///	@file	Auto_reset.hpp
///	@brief	�������Z�b�g�N���X\n
///			�R���X�g���N�g���ɃZ�b�g�l��ݒ肵�A�f�X�g���N�g���Ƀ��Z�b�g�l��ݒ肷��B
///
//	2007/05/24	�^�c	�V�K�쐬
//

#pragma once

namespace Javelin
{
	template < typename T > class Auto_reset
	{
	public :
		///
		///	@fn		Auto_reset( T& target, const T set, const T reset )
		///	@brief	�R���X�g���N�^
		///	@param	target	�Ώەϐ�
		///	@param	set		�Z�b�g�l
		///	@param	reset	���Z�b�g�l
		///
		Auto_reset( T& target, const T set, const T reset ) : Target( target )
		{
			Target = set ;
			Reset = reset ;
		}

		///
		///	@fn		~Auto_reset()
		///	@brief	�f�X�g���N�^
		///
		~Auto_reset()
		{
			Target = Reset ;
		}

	private :
		T& Target ;	///< �Ώەϐ��̎Q��
		T Reset ;	///< ���Z�b�g�l

		// �R�s�[�֎~
		Auto_reset( const Auto_reset& ) ;
		Auto_reset& operator=( const Auto_reset& ) ;
	} ;
}	// namespace Javelin

// [[[[[ End of this header ]]]]]
