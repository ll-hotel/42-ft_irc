#pragma once

#include <cstddef>
#include <stdexcept>

template <class T, class Arg = T>
struct Rc
{
private:
	struct Inner
	{
		std::size_t count;
		union {
			char bytes[sizeof(T)];
		};

		Inner(std::size_t count, Arg const &arg) : count(count)
		{
			::new (&this->get_mut()) T(arg);
		}

		T &get_mut() const throw()
		{
			return reinterpret_cast<T>(bytes);
		}
	};
	Inner *inner;

public:
	explicit Rc(Arg const &arg) : inner(new Inner(1, arg))
	{
	}

	T &deref_mut() const
	{
		if (this->inner == 0) {
			throw std::runtime_error("Rc: attempt to dereference a null pointer");
		}
		return this->inner->get_mut();
	}

	T &operator*() const
	{
		return this->deref_mut();
	}

	T *operator->() const
	{
		return &this->deref_mut();
	}

	Rc() throw() : inner(0)
	{
	}

	Rc(Rc const &other) : inner(other.inner)
	{
		if (this->inner != 0) {
			this->inner->count += 1;
		}
	}

	~Rc() throw()
	{
		if (this->inner == 0) {
			return;
		}
		this->inner->count -= 1;
		if (this->inner->count == 0) {
			this->inner->get_mut().~T();
			delete this->inner;
		}
	}

	Rc &operator=(Rc const &other) throw()
	{
		if (this == &other) {
			return *this;
		}
		this->~Rc();
		::new (this) Rc(other);
		return *this;
	}
};
