#pragma once

template<typename Enum>
class Flags
{
	using Value = std::underlying_type<Enum>::type;

public:
	constexpr Flags() = default;
	constexpr Flags(const Enum flag);
	constexpr Flags(const Flags& flags);

	/// \brief Returns true if the specified flag is raised.
	constexpr bool Has(const Enum flag) const;
	/// \brief Returns true if the all the specified flags are raised.
	constexpr bool HasAll(const Flags& flags) const;
	/// \brief Returns true if any flags are raised.
	constexpr bool HasAny() const;
	/// \brief Returns true if any of the specified flags are raised.
	constexpr bool HasAny(const Flags& flags) const;
	/// \brief Returns true if only the specified flag is raised.
	constexpr bool HasOnly(const Enum flag) const;
	/// \brief Returns true if only the specified flags are raised.
	constexpr bool HasOnly(const Flags& flags) const;
	/// \brief Returns true if no flags are raised.
	constexpr bool HasNone() const;
	/// \brief Returns true if none of the specified flags are raised.
	constexpr bool HasNone(const Flags& flags) const;

	/// \brief Clears the specified flag.
	constexpr void Clear(const Enum flag);
	/// \brief Clears all the specified flags.
	constexpr void Clear(const Flags& flags);
	/// \brief Clears all flags.
	constexpr void ClearAll();
	/// \brief Raises the specified flag.
	constexpr void Raise(const Enum flag);
	/// \brief Raises all the specified flags.
	constexpr void Raise(const Flags& flags);
	/// \brief Raises all flags.
	constexpr void RaiseAll();
	/// \brief Raises or clears the specified flag based of the value.
	constexpr void Set(const Enum flag, const bool value);
	/// \brief Raises or clears the specified flag based of the value.
	constexpr void Toggle(const Enum flag);

private:
	Value m_Value = 0;
};

#include "Flags.inl"