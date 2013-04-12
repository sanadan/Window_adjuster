///
///	@file	Comb_sort.hpp
///	@brief	�R���\�[�g
///	@note	�ˑ��FSTL
//
//	2007/05/21	�^�c	�V�K�쐬
//

#pragma once

#include <algorithm>

template < class Bidirectional_iterator, class Predicate >
void Comb_sort( Bidirectional_iterator first, Bidirectional_iterator last, Predicate pr )
{
	int gap = static_cast< int >( std::distance( first, last ) ) ;
	if ( gap < 1 ) return ;

	bool swapped = false ;
	do
	{
		gap = gap * 10 / 13 ;
		if ( gap < 1 ) gap = 1 ;
		if ( gap == 9 || gap == 10 ) gap = 11 ;

		Bidirectional_iterator target1 = first ;
		Bidirectional_iterator target2 = first ;
		std::advance( target2, gap ) ;
		swapped = false ;
		for ( ; target2 != last ; target1++, target2++ )
		{
			if ( pr( *target2, *target1 ) )
			{
				std::iter_swap( target1, target2 ) ;
				swapped = true ;
			}
		}
	} while ( gap > 1 || swapped ) ;
}

template < class Bidirectional_iterator >
void Comb_sort( Bidirectional_iterator first, Bidirectional_iterator last )
{
	int gap = static_cast< int >( std::distance( first, last ) ) ;
	if ( gap < 1 ) return ;

	bool swapped = false ;
	do
	{
		gap = gap * 10 / 13 ;
		if ( gap < 1 ) gap = 1 ;
		if ( gap == 9 || gap == 10 ) gap = 11 ;

		Bidirectional_iterator target1 = first ;
		Bidirectional_iterator target2 = first ;
		std::advance( target2, gap ) ;
		swapped = false ;
		for ( ; target2 != last ; target1++, target2++ )
		{
			if ( *target2 < *target1 )
			{
				std::iter_swap( target1, target2 ) ;
				swapped = true ;
			}
		}
	} while ( gap > 1 || swapped ) ;
}

// [[[[[ End of this header ]]]]]
