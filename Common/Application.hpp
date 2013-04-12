///	@file	Application.hpp
///	@brief	�A�v���P�[�V�����N���X
//
//	2006/01/22	�^�c	�V�K�쐬

#pragma once

#include "Accelerator.hpp"
#include "Mutex.hpp"

namespace Javelin
{
	class Application
	{
	public :
		Application() ;

		HINSTANCE Get_instance() const ;

	protected :
		virtual HRESULT Initialize() ;
		BOOL Is_already_exists( LPCTSTR name ) ;
		int Get_show_command() const ;
		HRESULT Load_accelerator( WORD resource ) ;

	private :
		HINSTANCE Instance ;
		int Show_command ;
		Accelerator Accel ;
		Mutex Exist_mutex ;

		HRESULT Run() ;

		friend int WINAPI ::WinMain( HINSTANCE instance, HINSTANCE, LPSTR, int show_command ) ;

		// �R�s�[�֎~����
		Application( const Application& ) ;
		Application& operator =( const Application& ) ;
	} ;
}	// namespace Javelin

// [EOF]
