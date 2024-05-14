#pragma once

#include <nameof/nameof.hpp>

#include <optional>

namespace nameof
{

template<class E>
inline std::optional<E> enumof_name(const char *name)
{
	using D = std::decay_t<E>;
	static_assert(detail::nameof_enum_supported<D>::value, "nameof::nameof_enum unsupported compiler (https://github.com/Neargye/nameof#compiler-compatibility).");
	static_assert(detail::count_v<D> > 0, "nameof::nameof_enum requires enum implementation and valid max and min.");

	for (std::size_t i = 0; i < detail::count_v<D>; ++i)
	{
		if (detail::names_v<D>[i] == name)
		{
			return detail::enum_value<D>(i);
		}
	}

	return std::nullopt;
}

} // namespace nameof
