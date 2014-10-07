///	@file	Adjust_dialog.hpp
///	@note	調整ダイアログクラス
//
//	2007/04/15	真田	新規作成

#pragma once

#include "Dialog.hpp"

///	@brief	調整ダイアログクラス
class Adjust_dialog : public Javelin::Dialog
{
public :
	Adjust_dialog() ;

	void Set_position( POINT& point ) ;

protected :
	virtual INT_PTR Dialog_proc( UINT message, WPARAM wparam, LPARAM lparam ) ;

	void On_initdialog() ;
	void On_command( WPARAM wparam ) ;
	void On_cancel() ;
	void On_adjust_button( WPARAM wparam ) ;

	void Set_icon( int control_id, int icon_id ) ;

private :
	WINDOWPLACEMENT Last_window_placement ;
	POINT Point ;
	HICON Icon03 ;
	HICON Icon06 ;

	// コピー禁止処理
	Adjust_dialog( const Adjust_dialog& ) ;
	Adjust_dialog& operator =( const Adjust_dialog& ) ;
} ;

// [EOF]
