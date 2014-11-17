///	@file		TString.hpp
///	@brief		文字列クラス
///	@note		依存：STL／Javelin::Buffer\n
///				VC6では警告レベル4でコンパイルすると、4018/4245/4663が出て
///				しまうので注意。
//
//	2006/01/09	真田	新規作成

#pragma once
#pragma warning( push )
// #pragma warning( disable : 4100 4511 4512 )

#include <string>
using std::basic_string ;
using std::char_traits ;
using std::allocator ;
#include "Buffer.hpp"

// namespace宣言
namespace Javelin
{
//	const DWORD Load_string_buffer_length = 65535 ;	///< Load_stringで読み出せる最大文字列
	const DWORD Load_string_buffer_length = 1024 ;	///< Load_stringで読み出せる最大文字列
	const int Int_buffer_length = 24 ;	// 64bitで20文字なので少し余裕を見て24文字にしている

	///	@brief	Win32関数切り換えのベースのテンプレート
	///	@param	_E	要素の型
	template< class _E > class Basic_string_Win32
	{
	} ;

	///	@brief	UNICODE系Win32関数テンプレート
	template<> class Basic_string_Win32< WCHAR >
	{
	public :
		static int Load_string( HINSTANCE instance, UINT resource_ID, LPWSTR string_buf, int buffer_len )
		{
			return ::LoadStringW( instance, resource_ID, string_buf, buffer_len ) ;
		}
		static DWORD Format_message( DWORD flags, LPCVOID source, DWORD message_ID, DWORD language_ID, LPWSTR string_buf, DWORD buffer_len, va_list *arguments )
		{
			return ::FormatMessageW( flags, source, message_ID, language_ID, string_buf, buffer_len, arguments ) ;
		}
		static LPCWSTR To_unicode(LPCWSTR source)
		{
			return source;
		}
	} ;

	///	@brief	ANSI系Win32関数テンプレート
	template<> class Basic_string_Win32< CHAR >
	{
	public :
		static int Load_string( HINSTANCE instance, UINT string_ID, LPSTR string_buf, int buffer_len )
		{
			return ::LoadStringA( instance, string_ID, string_buf, buffer_len ) ;
		}
		static DWORD Format_message( DWORD flags, LPCVOID source, DWORD message_ID, DWORD language_ID, LPSTR string_buf, DWORD buffer_len, va_list *arguments )
		{
			return ::FormatMessageA( flags, source, message_ID, language_ID, string_buf, buffer_len, arguments ) ;
		}
		static LPWSTR To_unicode(LPCSTR source)
		{
			size_t size;
			::mbstowcs_s(&size, NULL, 0, source, _TRUNCATE);
			static Buffer Unicode_buffer;
			Unicode_buffer.Set_size(size);
			::mbstowcs_s(&size, (wchar_t *)Unicode_buffer.Get_ptr(), size, source, _TRUNCATE);
			return (LPWSTR)Unicode_buffer.Get_ptr();
		}
	} ;

	///	@brief	Basic_stringテンプレート
	///	@param	_E	要素の型
	///	@param	_Tr	要素の特性クラス
	///	@param	_A	要素のアロケータ
	template< class _E, class _Tr = char_traits< _E >, class _A = allocator< _E > >
	class Basic_string : public basic_string< _E, _Tr, _A >
	{
	protected :
		Buffer Buffer ;	///< Get_buffer/Release_bufferで使用するバッファ
		HINSTANCE Instance ;	///< リソースアクセス時のモジュールハンドル

	public :
		explicit Basic_string( const _A& _Al = _A() )
			: basic_string< _E, _Tr, _A >( _Al )
		{
			Init() ;
		}
		Basic_string( const _Myt& _X )
			: basic_string< _E, _Tr, _A >( _X )
		{
			Init() ;
		}
		Basic_string( const _Myt& _X, size_type _P, size_type _M, const _A& _Al = _A() )
			: basic_string< _E, _Tr, _A >( _X, _P, _M, _Al )
		{
			Init() ;
		}
		Basic_string( const _E *_S, size_type _N, const _A& _Al = _A() )
			: basic_string< _E, _Tr, _A >( _S, _N, _Al )
		{
			Init() ;
		}
		Basic_string( const _E *_S, const _A& _Al = _A() )
			: basic_string< _E, _Tr, _A >( _S, _Al )
		{
			Init() ;
		}
		Basic_string( size_type _N, _E _C, const _A& _Al = _A() )
			: basic_string< _E, _Tr, _A >( _N, _C, _Al )
		{
			Init() ;
		}
		// typedef const_iterator _It ;	// 親クラスで宣言済
#if _MSC_VER < 1310
		Basic_string( _It _F, _It _L, const _A& _Al = _A() )
			: basic_string< _E, _Tr, _A >( _F, _L, _Al )
		{
			Init() ;
		}
#else
		template< class _It > Basic_string( _It _First, _It _Last )
			: basic_string< _E, _Tr, _A >( _First, _Last )
		{
			Init() ;
		}

		template< class _It > Basic_string( _It _First, _It _Last, const _A& _Al )
			: basic_string< _E, _Tr, _A >( _First, _Last, _Al )
		{
			Init() ;
		}
#endif

		///	@brief	バッファ取得
		///	@param	length	バッファ長（NULL文字を含まない文字数）
		///	@return	バッファのポインタ
		_E* Get_buffer( int length )
		{
			DWORD size = ( length + 1 ) * sizeof ( _E ) ;
			if ( Buffer.Set_size( size ) != ERROR_SUCCESS )
				return NULL ;
			::CopyMemory( Buffer.Get_ptr(), this->c_str(), min( size, ( this->length() + 1 ) * sizeof ( _E ) ) ) ;

			return ( _E * )Buffer.Get_ptr() ;
		}

		///	@brief	バッファ解放
		///	@param	length	バッファ長（NULL文字を含まない文字数）\n
		///					== -1 : バッファの文字数に合わせる
		void Release_buffer( int length = -1 )
		{
			*this = ( _E * )Buffer.Get_ptr() ;
			Buffer.Free() ;

			if ( length != -1 )
			{
				resize( length ) ;
			}
		}

		///	@brief	リソースアクセス用インスタンスハンドル設定\n
		///			デフォルトではGetModuleHandle( NULL )の値が設定されている
		///	@param	hModule	モジュールハンドル
		void Set_instance( HINSTANCE instance )
		{
			Instance = instance ;
		}

		///	@brief	リソースから文字列を取得する\n
		///	@param	uID	リソースID
		///	@note	別のモジュールから取得する場合は、あらかじめSet_instance()でモジュールハンドルを設定しておくこと
		HRESULT Load_string( UINT uID )
		{
			_E temp[ Load_string_buffer_length ] ;
			if ( !Basic_string_Win32< _E >::Load_string( Instance, uID, temp, Load_string_buffer_length ) )
			{
				return ::GetLastError() ;
			}
			*this = temp ;
			return ERROR_SUCCESS ;
		}

		///	@brief	メッセージ文字列を書式化する
		///	@param	dwMessageID	メッセージID
		///	@return	メッセージ文字数
		///	@note	別のモジュールから取得する場合は、あらかじめSet_instance()でモジュールハンドルを設定しておくこと
		DWORD Format_message( DWORD dwMessageID, ... )
		{
			va_list args ;
			va_start( args, dwMessageID ) ;
			DWORD dwRet = Do_format_message( FORMAT_MESSAGE_FROM_HMODULE, Instance, dwMessageID, &args ) ;
			va_end( args ) ;
			return dwRet ;
		}

		///	@brief	メッセージ文字列を書式化する
		///	@param	format	メッセージ書式
		///	@return	メッセージ文字数
		DWORD Format_message( const _E *format, ... )
		{
			va_list args ;
			va_start( args, format ) ;
			DWORD dwRet = Do_format_message( FORMAT_MESSAGE_FROM_STRING, format, 0, &args ) ;
			va_end( args ) ;
			return dwRet ;
		}

		///	@brief	直接キャスト
		operator const _E* ()
		{
			return this->c_str() ;
		}

		///	@brief	intを文字列に変換
		void From_int( int value )
		{
			_E buffer[ Int_buffer_length ] ;
			::_itot_s( value, buffer, 10 ) ;
			*this = buffer ;
		}

		///	@brief	Unicode変換
		///	@return	変換後の文字列
		LPCWSTR To_unicode()
		{
			return Basic_string_Win32< _E >::To_unicode( *this );
		}

	protected:
		///	@brief	メッセージ文字列を書式化する（共通処理）
		///	@param	dwFlag		フォーマットフラグ
		///	@param	pSource		元データ
		///	@param	dwMessageId	メッセージID
		///	@param	args		可変長引数
		///	@return	メッセージ文字数
		DWORD Do_format_message( DWORD dwFlag, LPCVOID pSource, DWORD dwMessageId, va_list *args )
		{
			_E *temp = NULL ;
			DWORD dwRet = Basic_string_Win32< _E >::Format_message( FORMAT_MESSAGE_ALLOCATE_BUFFER | dwFlag, pSource, dwMessageId, 0, ( _E * )&temp, 0, args ) ;
			if ( temp != NULL )
			{
				*this = temp ;
				::LocalFree( temp ) ;
			}
			return dwRet ;
		}

	private :
		///
		///	@fn		void Init()
		///	@brief	メンバ変数初期化
		///
		void Init()
		{
			Instance = ::GetModuleHandle( NULL ) ;
		}
	} ;

	///	@brief	UNICODE用Basic_string
	typedef Basic_string< WCHAR > WString ;

	///	@brief	ANSI用Basic_string
	typedef Basic_string< CHAR > String ;

	///	@brief	TCHAR用Basic_string
	#if defined ( _UNICODE )
	typedef WString TString ;
	#else
	typedef String TString ;
	#endif
}	// namespace Javelin

#pragma warning( pop )

// [[[[[ End of this header ]]]]]
