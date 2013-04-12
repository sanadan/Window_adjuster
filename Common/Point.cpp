///
///	@file	Point.cpp
///	@brief	Point�N���X
///	@note	�ˑ��FWin32 API
///
//	2008/10/16	�^�c	�V�K�쐬
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
