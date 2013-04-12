///	@file	Accelerator.hpp
///	@brief	アクセラレータクラス
//
// 2006/01/22	真田	新規作成

#pragma once

namespace Javelin
{
	///	@brief	アクセラレータクラス
	class Accelerator
	{
	public :
		///	@brief	コンストラクタ
		Accelerator()
		{
			Instance = NULL ;
			Accel_table = NULL ;
		}

		///	@brief	デストラクタ
		~Accelerator()
		{
			Destroy() ;
		}

		///	@brief	インスタンス設定
		///	@param	instance	インスタンスハンドル
		void Set_instance( HINSTANCE instance )
		{
			Instance = instance ;
		}

		///	@brief	リソースからロード
		///	@param	resource	リソースID
		///	@return	winerror.h準拠
		HRESULT Load( int resource )
		{
			Destroy() ;
			Accel_table = ::LoadAccelerators( Instance, MAKEINTRESOURCE( resource ) ) ;
			if ( Accel_table != NULL )
			{	// エラー
				return ::GetLastError() ;
			}

			return ERROR_SUCCESS ;
		}

		///	@brief	破棄
		///	@return	winerror.h準拠
		HRESULT Destroy()
		{
			HRESULT result = ERROR_SUCCESS ;

			if ( Accel_table != NULL )
			{
				if ( !::DestroyAcceleratorTable( Accel_table ) )
				{	// エラー
					result = ::GetLastError() ;
				}
				Accel_table = NULL ;
			}

			return result ;
		}

		///	@brief	アクセラレータハンドルへのキャスト
		///	@return	アクセラレータハンドル
		operator HACCEL()
		{
			return Accel_table ;
		}

		///	@brief	アクセラレータのトランスレート
		///	@param	msg	メッセージ
		///	@return	winerror.h準拠
		HRESULT Translate( MSG& msg )
		{
			if ( Accel_table == NULL ) return FALSE ;
			if ( !::TranslateAccelerator( msg.hwnd, Accel_table, &msg ) )
			{	// エラー
				return ::GetLastError() ;
			}
			return ERROR_SUCCESS ;
		}

	private :
		HINSTANCE Instance ;
		HACCEL Accel_table ;

		// コピー禁止処理
		Accelerator( const Accelerator& ) ;
		Accelerator& operator =( const Accelerator& ) ;
	} ;
}	// namespace Javelin

// [EOF]
