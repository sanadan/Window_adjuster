///	@brief	���C���N���X
//
//	2007/05/08	�^�c	�V�K�쐬

#pragma once

#include "resource.h"
#include "Application.hpp"
#include "TPath.hpp"

class Main : public Javelin::Application
{
public :
	virtual HRESULT Initialize() ;

	static int Error_message_box( LPCTSTR message ) ;
	static LPCTSTR Get_title() ;
	static LPCTSTR Get_internal_name() ;
	static LPCTSTR Get_internal_id() ;
	static LPCTSTR Get_application_data_folder() ;
	static UINT Get_quit_message();

private :
	static LPCTSTR Title ;
	static LPCTSTR Internal_name ;
	static LPCTSTR Internal_id ;

	Javelin::TPath Application_data_folder ; 
	static UINT RWM_QUIT;
} ;

// [EOF]
