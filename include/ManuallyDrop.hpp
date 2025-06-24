#pragma once

template <typename T, typename TArg = T>
struct ManuallyDrop
{
	ManuallyDrop();
	ManuallyDrop(TArg const &);

	// Modifiers
	void drop() throw();

	// Queries
	T into_inner() throw();
	T const &get_ref() const throw();
	T &get_mut() throw();

private:
	char bytes[sizeof(T)];
};

#include "detail/ManuallyDrop.hpp"
