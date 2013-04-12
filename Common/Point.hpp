///
///	@file	Point.hpp
///	@brief	Point�N���X
///	@note	�ˑ��FWin32 API
///
//	2008/10/16	�^�c	�V�K�쐬
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
			LPARAM lparam ;	// operator�p
	} ;
}

// [[[[[ End of this header ]]]]]
