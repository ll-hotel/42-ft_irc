#pragma once

#include "../ManuallyDrop.hpp"
#include <cstddef>

template <typename T, typename TArg>
ManuallyDrop<T, TArg>::ManuallyDrop()
{
}

template <typename T, typename TArg>
ManuallyDrop<T, TArg>::ManuallyDrop(TArg const &arg)
{
	::new (&this->into_inner()) T(arg);
}

template <typename T, typename TArg>
void ManuallyDrop<T, TArg>::drop() throw()
{
	this->into_inner().~T();
}

template <typename T, typename TArg>
T ManuallyDrop<T, TArg>::into_inner() throw()
{
	return this->get_ref();
}

template <typename T, typename TArg>
T const &ManuallyDrop<T, TArg>::get_ref() const throw()
{
	std::size_t addr = reinterpret_cast<std::size_t>(&this->bytes);
	return *reinterpret_cast<T const *>(addr);
}

template <typename T, typename TArg>
T &ManuallyDrop<T, TArg>::get_mut() throw()
{
	std::size_t addr = reinterpret_cast<std::size_t>(&this->bytes);
	return *reinterpret_cast<T *>(addr);
}
