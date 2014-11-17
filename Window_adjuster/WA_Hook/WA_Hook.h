///	@fn		WA_Hook.h
///	@brief	フックDLLエントリ
//
//	2007/04/19	真田	新規作成

#pragma once

// 以下の ifdef ブロックは DLL からのエクスポートを容易にするマクロを作成するための 
// 一般的な方法です。この DLL 内のすべてのファイルは、コマンド ラインで定義された WA_HOOK_EXPORTS
// シンボルでコンパイルされます。このシンボルは、この DLL を使うプロジェクトで定義することはできません。
// ソースファイルがこのファイルを含んでいる他のプロジェクトは、 
// WA_HOOK_API 関数を DLL からインポートされたと見なすのに対し、この DLL は、このマクロで定義された
// シンボルをエクスポートされたと見なします。
#ifdef WA_HOOK_EXPORTS
#define WA_HOOK_API __declspec(dllexport)
#else
#define WA_HOOK_API __declspec(dllimport)
#endif

#define	RWM_ADJUST_MESSAGE	_T("Javelin/Window_adjuster/RWM_ADJUST")
#define	RWM_SET_PARAM_MESSAGE	_T("Javelin/Window_adjuster/RWM_SET_PARAM")
#define	RWM_SET_PARAM2_MESSAGE	_T("Javelin/Window_adjuster/RWM_SET_PARAM2")

WA_HOOK_API HRESULT WA_Enable_hook( HWND hwnd ) ;
WA_HOOK_API HRESULT WA_Disable_hook() ;

// [EOF]
