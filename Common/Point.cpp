///
///	@file	Point.cpp
///	@brief	Pointクラス
///	@note	依存：Win32 API
///
//	2008/10/16	真田	新規作成
//

#include "stdafx.h"
#include "Point.hpp"

namespace Javelin
{
	Point::Point()
	{
		x = 0 ;
		y = 0 ;
	}

	Point::operator LPARAM()
	{
		lparam = ( LPARAM )( ( WORD )y ) << 8 + ( WORD )x ;
		return lparam ;
	}
}
