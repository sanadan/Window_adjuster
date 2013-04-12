///
///	@file	Auto_reset.hpp
///	@brief	自動リセットクラス\n
///			コンストラクト時にセット値を設定し、デストラクト時にリセット値を設定する。
///
//	2007/05/24	真田	新規作成
//

#pragma once

namespace Javelin
{
	template < typename T > class Auto_reset
	{
	public :
		///
		///	@fn		Auto_reset( T& target, const T set, const T reset )
		///	@brief	コンストラクタ
		///	@param	target	対象変数
		///	@param	set		セット値
		///	@param	reset	リセット値
		///
		Auto_reset( T& target, const T set, const T reset ) : Target( target )
		{
			Target = set ;
			Reset = reset ;
		}

		///
		///	@fn		~Auto_reset()
		///	@brief	デストラクタ
		///
		~Auto_reset()
		{
			Target = Reset ;
		}

	private :
		T& Target ;	///< 対象変数の参照
		T Reset ;	///< リセット値

		// コピー禁止
		Auto_reset( const Auto_reset& ) ;
		Auto_reset& operator=( const Auto_reset& ) ;
	} ;
}	// namespace Javelin

// [[[[[ End of this header ]]]]]
