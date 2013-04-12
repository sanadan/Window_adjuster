///
///	@file	Test_frame.hpp
///	@brief	簡易テストフレームワーク
///
//	2005/12/23	真田	新規作成
//

#pragma once

#include <windows.h>
#include <tchar.h>

// テスト記述用マクロ
#define	JT_MESSAGE( x )	{ Javelin::Current_test_frame_ptr->Indent() ; Javelin::Current_test_frame_ptr->Output( x ) ; }

#define	JT_CHECK( x )	{ Javelin::Current_test_frame_ptr->Count() ; if ( !( x ) ) Javelin::Current_test_frame_ptr->Error( _T( __FILE__ ), __LINE__, Javelin::Check_macro_name, _T( #x ) ) ; }
#define	JT_REQUIRE( x )	{ Javelin::Current_test_frame_ptr->Count() ; if ( !( x ) ) { Javelin::Current_test_frame_ptr->Error( _T( __FILE__ ), __LINE__, Javelin::Require_macro_name, _T( #x ) ) ; return JT_RETURN() ; } }

#define	JT_DUMP( x, y )	Javelin::Current_test_frame_ptr->Dump( _T( #x ), x, y )

#define	JT_ERROR( x )	{ Javelin::Current_test_frame_ptr->Count() ; Javelin::Current_test_frame_ptr->Error( _T( __FILE__ ), __LINE__, Javelin::Error_macro_name, _T( #x ) ) ; }
#define	JT_FAULT( x )	{ Javelin::Current_test_frame_ptr->Count() ; Javelin::Current_test_frame_ptr->Error( _T( __FILE__ ), __LINE__, Javelin::Falut_macro_name, _T( #x ) ) ; return JT_RETURN() ; }

#define	JT_RETURN()	Javelin::Current_test_frame_ptr->Get_test_error_num()

#define	JT_TEST_CHECK( x )	{ Javelin::Test_frame *prev_test_frame_ptr = Javelin::Current_test_frame_ptr ; Javelin::Test_frame current ; Javelin::Current_test_frame_ptr = &current ; Javelin::Current_test_frame_ptr->Start( _T( #x ) ) ; x() ; Javelin::Current_test_frame_ptr->End( prev_test_frame_ptr ) ; Javelin::Current_test_frame_ptr = prev_test_frame_ptr ; }
#define	JT_TEST_REQUIRE( x )	{ Javelin::Test_frame *prev_test_frame_ptr = Javelin::Current_test_frame_ptr ; Javelin::Test_frame current ; Javelin::Current_test_frame_ptr = &current ; Javelin::Current_test_frame_ptr->Start( _T( #x ) ) ; DWORD dwRet = x() ; Javelin::Current_test_frame_ptr->End( prev_test_frame_ptr ) ; Javelin::Current_test_frame_ptr = prev_test_frame_ptr ; if ( dwRet != 0 ) return JT_FINISH() ; }
#define	JT_FINISH()	Javelin::Current_test_frame_ptr->Get_total_error_num()

// namespace宣言
namespace Javelin
{
	const TCHAR Check_macro_name[] = _T( "JT_CHECK" ) ;
	const TCHAR Require_macro_name[] = _T( "JT_REQUIRE" ) ;
	const TCHAR Error_macro_name[] = _T( "JT_ERROR" ) ;
	const TCHAR Fault_macro_name[] = _T( "JT_FAULT" ) ;

	class Test_frame
	{
	public :
		Test_frame() ;

		// 関数宣言
		void Output( LPCTSTR szMessage ) ;
		void Error( LPCTSTR szFilePath, DWORD dwLine, LPCTSTR szType, LPCTSTR szTest ) ;
		void Start( LPCTSTR szName ) ;
		int End( Test_frame *test_frame_ptr ) ;
		void Dump( LPCTSTR szTitle, const void *pvData, DWORD dwLength ) ;
		void Count() ;
		DWORD Get_total_error_num() ;
		DWORD Get_test_error_num() ;
		void Indent() ;

	private :
		TCHAR Test_name[ MAX_PATH ] ;
		DWORD Test_num ;
		DWORD Test_error_num ;
	} ;

	extern Test_frame *Current_test_frame_ptr ;
}	// namespace Javelin

// [[[[[ End of this header ]]]]]
