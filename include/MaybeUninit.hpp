#pragma once

#include "ManuallyDrop.hpp"

template <typename T, typename TArg = T>
struct MaybeUninit
{
	// Construct/copy/destruct
	MaybeUninit(TArg const &);

	// Modifiers
	T assume_init() throw();
	void assume_init_drop() throw();

	// Queries
	T const &assume_init_ref() const throw();
	T &assume_init_mut() throw();

private:
	ManuallyDrop<T, TArg> value;
};

#include "detail/MaybeUninit.hpp"
