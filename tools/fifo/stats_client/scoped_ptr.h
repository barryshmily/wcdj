#ifndef TDF_RUNTIME_BASE_SCOPED_PTR_H_
#define TDF_RUNTIME_BASE_SCOPED_PTR_H_

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include <algorithm>  // For std::swap().
#include <iosfwd>

template <class T >
class scoped_ptr 
{
public:
	typedef T element_type;

	scoped_ptr() : impl_(NULL) {}
	~scoped_ptr()
	{
		delete impl_;
	}

	// Constructor.  Takes ownership of p.
	explicit scoped_ptr(element_type* p) : impl_(p) {}

	scoped_ptr& operator = (element_type* p)
	{
		reset(p);
		return *this;
	}

	void reset(element_type* p = NULL) 
	{
		if (p != impl_)
		{
			delete impl_;
		}

		impl_ = p;
	}

	element_type& operator*() const 
	{
		assert(impl_ != NULL);
		return *impl_;
	}
	element_type* operator->() const  
	{
		assert(impl_ != NULL);
		return impl_;
	}
	element_type* get() const { return impl_; }

	bool operator == (const element_type* p) const { return impl_ == p; }
	bool operator != (const element_type* p) const { return impl_ != p; }

	void swap(scoped_ptr& p2) 
	{
		element_type* tmp = impl_;
		impl_ = p2.impl_;
		p2.impl_ = tmp;	
	}

	element_type* release()  
	{
		element_type* tmp = impl_;
		impl_ = NULL;
		return tmp;
	}

private:
	element_type* impl_;

	template <class U> bool operator==(scoped_ptr<U> const& p2) const;
	template <class U> bool operator!=(scoped_ptr<U> const& p2) const;
};

// Free functions
template <class T>
void swap(scoped_ptr<T>& p1, scoped_ptr<T>& p2) 
{
	p1.swap(p2);
}

template <class T>
bool operator==(T* p1, const scoped_ptr<T>& p2) 
{
	return p1 == p2.get();
}

template <class T>
bool operator != (T* p1, const scoped_ptr<T>& p2) 
{
	return p1 != p2.get();
}

template <typename T>
scoped_ptr<T> make_scoped_ptr(T* ptr) 
{
	return scoped_ptr<T>(ptr);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const scoped_ptr<T>& p) 
{
	return out << p.get();
}

#endif  // TDF_RUNTIME_BASE_SCOPED_PTR_H_
