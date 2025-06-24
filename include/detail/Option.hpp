#pragma once

#include "../Option.hpp"
#include <stdexcept>

template <typename T>
Option<T>::Option() : kind(NONE)
{
}

template <typename T>
Option<T>::Option(T const &val) : value(val), kind(SOME)
{
}

template <typename T>
Option<T>::Option(Option const &other) : value(other.value), kind(other.kind)
{
}

template <typename T>
Option<T> &Option<T>::operator=(Option const &other)
{
	if (this == &other) {
		return *this;
	}
	if (this->is_some() && other.is_some()) {
		this->value = other.value;
	}
	else if (this->is_some() && other.is_none()) {
		this->value.~T();
	}
	else if (this->is_none() && other.is_some()) {
		::new (&this->value) T(other.value);
	}
	this->kind = other.kind;
	return *this;
}

template <typename T>
bool Option<T>::is_some() const throw()
{
	return this->kind == SOME;
}

template <typename T>
bool Option<T>::is_none() const throw()
{
	return this->kind == NONE;
}
