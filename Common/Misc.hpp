///	@file	Misc.hpp
///	@brief	����
//
//	2007/04/16	�^�c	�V�K�쐬

#pragma once

#include <memory.h>

#define	FOR_ONCE()	for ( int _i = 0 ; _i < 1 ; _i++ )

namespace Javelin
{
	template<class T> void Initialize(T &x)
	{
		::memset(&x, 0, sizeof(x));
	}
}

// [EOF]
