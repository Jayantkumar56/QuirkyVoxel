

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <type_traits>


namespace Mct {

	template<typename Key, typename ...Types>
	struct IsOneOf : std::bool_constant<(std::is_same_v<Key, Types> || ...)> {};

	template<typename Key, typename ...Types>
	inline constexpr bool IsOneOf_V = IsOneOf<Key, Types...>::value;

}
