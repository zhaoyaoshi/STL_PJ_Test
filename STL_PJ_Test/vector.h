#pragma once
using namespace std;
#include <memory>
#include<atlalloc.h>
template<class _Ty, class _A = allocator<_Ty> >
class vector
{
public:
		/*
		typedef typename _A::size_type size_type;
		typedef typename _A::pointer pointer;
		typedef typename _A::const_pointer const_pointer;
		typedef typename _A::reference reference;
		typedef typename _A::const_reference const_referencre;
		typedef typename _A::value_type value_type;
		//typedef pointer iterator;
		typedef typename _Ty* iterator;
		*/
		using size_type = typename _A::size_type ;
		using pointer = typename _A::pointer;
		using const_pointe = typename _A::const_pointer;
		using reference= typename _A::reference ;
		using const_reference = typename _A::const_reference;
		using value_type = typename _A::value_type;
		//typedef pointer iterator;
		using iterator = typename _Ty*;
public:
	explicit vector(const _A& _A1 = _A()) :allocator(_A1), _First(0), _Last(0), _End(0)
	{
	}
	explicit vector(size_type _N, const _Ty& _V = _Ty(), const _A& _A1 = _A()):allocator(_A1)
	{
		_First = allocator.allocate(_N, (void*)0);
		_Ufill(_First, _N, _V);
		_Last = _First + _N;
		_End = _Last;
	}
	~vector()
	{
		std::cout << "释放向量" << endl;
		_Destroy(_First, _Last);
		allocator.deallocate(_First, _End - _First);
		_First = _Last = _End = nullptr;
	}
public:
	iterator begin()
	{
		return _First;
	}
	iterator end()
	{
		return _Last;
	}
	size_t size() const
	{
		return _Last - _First;
	}
	size_type capacity() const
	{
		return _First == nullptr ? 0: _End - _First;
	}
	bool empty()const
	{
		return size() == 0;
	}

	reference operator[](size_type index)
	{
		if (index >= size())
			_Xran();
		return *(_First + index);
	}
	reference at(size_type index)
	{
		if (index >= size())
			_Xran();
			//_THROW(x,y) thorw x(y)
			
		return *(begin() + index);
	}
	reference front()
	{
		return *begin();
	}

	reference back()
	{
		return *(end() - 1);
	}

	void push_back(const _Ty& _X)
	{
		insert(end(), _X);
	}
	void pop_back()
	{
		eraser(end() - 1);
	}
	void clear()
	{
		eraser(begin(), end());
	}

	void resize(size_type _N, const _Ty& _X = _Ty())
	{
		if (_N > size())
			insert(end(), _N - size(), _X);
	    else if (_N < size())
			eraser(begin() + _N, end());
	}
	void reserve(size_type _N)
	{
		if (_N > capacity())
		{
			iterator _S = allocator.allocate(_N, (void*)0);
			_Ucopy(_First, _Last, _S);
			_Destroy(_First, _Last);
			_Last = _S + size();
			_First = _S;
			_End = _S + _N;
		}
	}

	void _Xran() const
	{
		throw(out_of_range("invalid vector<bool> subscript"));
	}
	iterator insert(iterator _P, const _Ty& _X = _Ty())
	{
		size_type _0 = _P - begin();
		insert(_P, 1, _X);
		return begin() + _0;
	}

	void insert(iterator _P, size_type _M, const _Ty& _X)
	{
		if (_M > _End - _Last)
		{
			size_type _N = size() + (_M < size() ? size() : _M);
			iterator _S = allocator.allocate(_N, (void*)0);//申请空间
			iterator _Q = _Ucopy(_First, _P, _S);//填充原数据
			_Ufill(_Q, _M, _X);//填充新数据
			_Ucopy(_P, _Last, _Q + _M);
			_Destroy(_First, _Last);
			allocator.deallocate(_First, _End - _First);
			_End = _S + _N;
			_Last = _S + size() + _M;
			_First = _S;

		}
		else if(_M > _Last - _P)
		{
			_Ucopy(_P, _Last, _P + _M);
			_Ufill(_Last, _M - (_Last - _P), _X);
			fill(_P, _Last, _X);
			_Last += _M;

		}
		else if(_M > 0)
		{
			_Ucopy(_Last-_M, _Last, _Last);
			copy_backward(_P, _Last - _M, _Last);
			fill(_P, _P + _M, _X);
			_Last += _M;

		}

	}

	void insert(iterator _p, iterator _F, iterator _L);

	iterator eraser(iterator _P)
	{
		copy(_P + 1, end(), _P);
		_Destroy(_Last - 1, _Last);
		--_Last;
		return _P;
	}

	iterator eraser(iterator _F, iterator _L)
	{
		iterator _S = copy(_L, end(), _F);
		//copy(_L, end(), _F);
		_Destroy(_S,_End());
		_Last =_S;
	}



protected:
	void _Destroy(iterator _F, iterator _L)
	{
		for (; _F != _L; ++_F)
			allocator.destroy(_F);
	}

	iterator _Ucopy(iterator _F, const iterator _L,iterator _P)
	{
		for (; _F != _L; ++_P, ++_F) 
			allocator.construct(_P, *_F);
		return _P;
	}

	void _Ufill(iterator _F, size_type _N, const _Ty& _X)
	{
		for (;0<_N;--_N, ++_F)
		{
			allocator.construct(_F, _X);
		}
	}


private:
	_A allocator;
	iterator _First, _Last, _End;
};