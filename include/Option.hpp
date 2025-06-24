#pragma once

#include "MaybeUninit.hpp"

template <typename T>
struct Option
{
private:
	MaybeUninit<T> value;
	enum { NONE, SOME } kind;

public:
	// Construct/copy/destruct
	Option();
	Option(T const &val);  
	Option(Option const &other);
	Option &operator=(Option const &other);
	~Option() throw();

	// Modifiers
	T unwrap();

	// Queries
	bool is_some() const throw();
	bool is_none() const throw();

	static const Option<T> None = Option();
};

#include "detail/Option.hpp"
