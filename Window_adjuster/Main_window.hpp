///	@file	Main_window.hpp
///	@brief	メインウィンドウクラス
//
//	2007/02/21	真田	新規作成

#pragma once

#include "Window.hpp"
#include "Notify_icon.hpp"
#include "TPath.hpp"

#define	LWM_TRAY_CALLBACK	( WM_USER + 0x100 )

///	@brief	メインウィンドウクラス
class Main_window : public Javelin::Window
{
public :
	Main_window() ;

protected :
	virtual LRESULT Window_proc( UINT message, WPARAM wparam, LPARAM lparam ) ;

	void OnCreate() ;
	void OnDestroy() ;
	void On_notify_icon( LPARAM lParam ) ;

	void On_layout_save() ;
	void On_layout_load() ;
	void On_layout_clear() ;

	void On_adjust(WPARAM wparam, LPARAM lparam);

//	HRESULT Get_application_data_folder( Javelin::TPath& path ) ;
	void Get_file_name( Javelin::TString& file_name ) ;
	void Get_file_name2( Javelin::TString& file_name ) ;

private :
	Javelin::Notify_icon Notify_icon ;
	UINT RWM_TASKBARCREATED ;
	UINT RWM_ADJUST;
	UINT RWM_SET_PARAM;
	UINT RWM_SET_PARAM2;
	MOUSEHOOKSTRUCT Mouse_hook_struct;
	BOOL Show_dialog;

	static LPCTSTR Save_file_name_format ;
	static LPCTSTR Save_file_name_format2 ;
} ;

// [EOF]
