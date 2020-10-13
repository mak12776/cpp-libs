#pragma once

namespace scl
{
	template<bool B, class T = void, class E = void>
	struct if_else {};

	template<class T, class E>
	struct if_else<true, T, E> { typedef T type; };

	template<class T, class E>
	struct if_else<false, T, E> { typedef E type; };

	template<bool B, class T = void, class E = void>
	using if_else_t = typename if_else<B, T, E>::type;
}