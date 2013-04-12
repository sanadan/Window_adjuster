///	@file	Accelerator.hpp
///	@brief	�A�N�Z�����[�^�N���X
//
// 2006/01/22	�^�c	�V�K�쐬

#pragma once

namespace Javelin
{
	///	@brief	�A�N�Z�����[�^�N���X
	class Accelerator
	{
	public :
		///	@brief	�R���X�g���N�^
		Accelerator()
		{
			Instance = NULL ;
			Accel_table = NULL ;
		}

		///	@brief	�f�X�g���N�^
		~Accelerator()
		{
			Destroy() ;
		}

		///	@brief	�C���X�^���X�ݒ�
		///	@param	instance	�C���X�^���X�n���h��
		void Set_instance( HINSTANCE instance )
		{
			Instance = instance ;
		}

		///	@brief	���\�[�X���烍�[�h
		///	@param	resource	���\�[�XID
		///	@return	winerror.h����
		HRESULT Load( int resource )
		{
			Destroy() ;
			Accel_table = ::LoadAccelerators( Instance, MAKEINTRESOURCE( resource ) ) ;
			if ( Accel_table != NULL )
			{	// �G���[
				return ::GetLastError() ;
			}

			return ERROR_SUCCESS ;
		}

		///	@brief	�j��
		///	@return	winerror.h����
		HRESULT Destroy()
		{
			HRESULT result = ERROR_SUCCESS ;

			if ( Accel_table != NULL )
			{
				if ( !::DestroyAcceleratorTable( Accel_table ) )
				{	// �G���[
					result = ::GetLastError() ;
				}
				Accel_table = NULL ;
			}

			return result ;
		}

		///	@brief	�A�N�Z�����[�^�n���h���ւ̃L���X�g
		///	@return	�A�N�Z�����[�^�n���h��
		operator HACCEL()
		{
			return Accel_table ;
		}

		///	@brief	�A�N�Z�����[�^�̃g�����X���[�g
		///	@param	msg	���b�Z�[�W
		///	@return	winerror.h����
		HRESULT Translate( MSG& msg )
		{
			if ( Accel_table == NULL ) return FALSE ;
			if ( !::TranslateAccelerator( msg.hwnd, Accel_table, &msg ) )
			{	// �G���[
				return ::GetLastError() ;
			}
			return ERROR_SUCCESS ;
		}

	private :
		HINSTANCE Instance ;
		HACCEL Accel_table ;

		// �R�s�[�֎~����
		Accelerator( const Accelerator& ) ;
		Accelerator& operator =( const Accelerator& ) ;
	} ;
}	// namespace Javelin

// [EOF]
