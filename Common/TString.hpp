///	@file		TString.hpp
///	@brief		������N���X
///	@note		�ˑ��FSTL�^Javelin::Buffer\n
///				VC6�ł͌x�����x��4�ŃR���p�C������ƁA4018/4245/4663���o��
///				���܂��̂Œ��ӁB
//
//	2006/01/09	�^�c	�V�K�쐬

#pragma once
#pragma warning( push )
// #pragma warning( disable : 4100 4511 4512 )

#include <string>
using std::basic_string ;
using std::char_traits ;
using std::allocator ;
#include "Buffer.hpp"

// namespace�錾
namespace Javelin
{
//	const DWORD Load_string_buffer_length = 65535 ;	///< Load_string�œǂݏo����ő啶����
	const DWORD Load_string_buffer_length = 1024 ;	///< Load_string�œǂݏo����ő啶����
	const int Int_buffer_length = 24 ;	// 64bit��20�����Ȃ̂ŏ����]�T������24�����ɂ��Ă���

	///	@brief	Win32�֐��؂芷���̃x�[�X�̃e���v���[�g
	///	@param	_E	�v�f�̌^
	template< class _E > class Basic_string_Win32
	{
	} ;

	///	@brief	UNICODE�nWin32�֐��e���v���[�g
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

	///	@brief	ANSI�nWin32�֐��e���v���[�g
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

	///	@brief	Basic_string�e���v���[�g
	///	@param	_E	�v�f�̌^
	///	@param	_Tr	�v�f�̓����N���X
	///	@param	_A	�v�f�̃A���P�[�^
	template< class _E, class _Tr = char_traits< _E >, class _A = allocator< _E > >
	class Basic_string : public basic_string< _E, _Tr, _A >
	{
	protected :
		Buffer Buffer ;	///< Get_buffer/Release_buffer�Ŏg�p����o�b�t�@
		HINSTANCE Instance ;	///< ���\�[�X�A�N�Z�X���̃��W���[���n���h��

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
		// typedef const_iterator _It ;	// �e�N���X�Ő錾��
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

		///	@brief	�o�b�t�@�擾
		///	@param	length	�o�b�t�@���iNULL�������܂܂Ȃ��������j
		///	@return	�o�b�t�@�̃|�C���^
		_E* Get_buffer( int length )
		{
			DWORD size = ( length + 1 ) * sizeof ( _E ) ;
			if ( Buffer.Set_size( size ) != ERROR_SUCCESS )
				return NULL ;
			::CopyMemory( Buffer.Get_ptr(), this->c_str(), min( size, ( this->length() + 1 ) * sizeof ( _E ) ) ) ;

			return ( _E * )Buffer.Get_ptr() ;
		}

		///	@brief	�o�b�t�@���
		///	@param	length	�o�b�t�@���iNULL�������܂܂Ȃ��������j\n
		///					== -1 : �o�b�t�@�̕������ɍ��킹��
		void Release_buffer( int length = -1 )
		{
			*this = ( _E * )Buffer.Get_ptr() ;
			Buffer.Free() ;

			if ( length != -1 )
			{
				resize( length ) ;
			}
		}

		///	@brief	���\�[�X�A�N�Z�X�p�C���X�^���X�n���h���ݒ�\n
		///			�f�t�H���g�ł�GetModuleHandle( NULL )�̒l���ݒ肳��Ă���
		///	@param	hModule	���W���[���n���h��
		void Set_instance( HINSTANCE instance )
		{
			Instance = instance ;
		}

		///	@brief	���\�[�X���當������擾����\n
		///	@param	uID	���\�[�XID
		///	@note	�ʂ̃��W���[������擾����ꍇ�́A���炩����Set_instance()�Ń��W���[���n���h����ݒ肵�Ă�������
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

		///	@brief	���b�Z�[�W�����������������
		///	@param	dwMessageID	���b�Z�[�WID
		///	@return	���b�Z�[�W������
		///	@note	�ʂ̃��W���[������擾����ꍇ�́A���炩����Set_instance()�Ń��W���[���n���h����ݒ肵�Ă�������
		DWORD Format_message( DWORD dwMessageID, ... )
		{
			va_list args ;
			va_start( args, dwMessageID ) ;
			DWORD dwRet = Do_format_message( FORMAT_MESSAGE_FROM_HMODULE, Instance, dwMessageID, &args ) ;
			va_end( args ) ;
			return dwRet ;
		}

		///	@brief	���b�Z�[�W�����������������
		///	@param	format	���b�Z�[�W����
		///	@return	���b�Z�[�W������
		DWORD Format_message( const _E *format, ... )
		{
			va_list args ;
			va_start( args, format ) ;
			DWORD dwRet = Do_format_message( FORMAT_MESSAGE_FROM_STRING, format, 0, &args ) ;
			va_end( args ) ;
			return dwRet ;
		}

		///	@brief	���ڃL���X�g
		operator const _E* ()
		{
			return this->c_str() ;
		}

		///	@brief	int�𕶎���ɕϊ�
		void From_int( int value )
		{
			_E buffer[ Int_buffer_length ] ;
			::_itot_s( value, buffer, 10 ) ;
			*this = buffer ;
		}

		///	@brief	Unicode�ϊ�
		///	@return	�ϊ���̕�����
		LPCWSTR To_unicode()
		{
			return Basic_string_Win32< _E >::To_unicode( *this );
		}

	protected:
		///	@brief	���b�Z�[�W�����������������i���ʏ����j
		///	@param	dwFlag		�t�H�[�}�b�g�t���O
		///	@param	pSource		���f�[�^
		///	@param	dwMessageId	���b�Z�[�WID
		///	@param	args		�ϒ�����
		///	@return	���b�Z�[�W������
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
		///	@brief	�����o�ϐ�������
		///
		void Init()
		{
			Instance = ::GetModuleHandle( NULL ) ;
		}
	} ;

	///	@brief	UNICODE�pBasic_string
	typedef Basic_string< WCHAR > WString ;

	///	@brief	ANSI�pBasic_string
	typedef Basic_string< CHAR > String ;

	///	@brief	TCHAR�pBasic_string
	#if defined ( _UNICODE )
	typedef WString TString ;
	#else
	typedef String TString ;
	#endif
}	// namespace Javelin

#pragma warning( pop )

// [[[[[ End of this header ]]]]]
