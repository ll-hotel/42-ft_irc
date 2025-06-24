#pragma once

#include "../MaybeUninit.hpp"

template <typename T, typename TArg>
MaybeUninit<T, TArg>::MaybeUninit(TArg const &arg) : value(arg)
{
}

template <typename T, typename TArg>
T MaybeUninit<T, TArg>::assume_init() throw()
{
	return this->value.into_inner();
}

template <typename T, typename TArg>
void MaybeUninit<T, TArg>::assume_init_drop() throw()
{
	this->value.drop();
}

template <typename T, typename TArg>
T const &MaybeUninit<T, TArg>::assume_init_ref() const throw()
{
	return this->value.get_ref();
}

template <typename T, typename TArg>
T &MaybeUninit<T, TArg>::assume_init_mut() throw()
{
	return this->value.get_mut();
}
