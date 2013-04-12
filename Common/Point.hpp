///
///	@file	Point.hpp
///	@brief	Pointクラス
///	@note	依存：Win32 API
///
//	2008/10/16	真田	新規作成
//

#pragma once

namespace Javelin
{
	class Point
	{
		public :
			int x ;
			int y ;

			Point() ;

			operator LPARAM() ;

		private :
			LPARAM lparam ;	// operator用
	} ;
}

// [[[[[ End of this header ]]]]]
