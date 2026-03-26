#pragma once
#include<iostream>
#include <memory> // Ensure the allocator template is included
#define _DEQUESIZ (4096<sizeof(_Ty)?1:4096/sizeof(_Ty))
#define _DEQUEMAPSIZ 2

template<class _Ty, class _A = std::allocator<_Ty> >
class deque
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
	using size_type = typename _A::size_type;
	using pointer = typename _A::pointer;
	using const_pointe = typename _A::const_pointer;
	using reference = typename _A::reference;
	using const_reference = typename _A::const_reference;
	using value_type = typename _A::value_type;
	using difference_type = typename _A::size_type;
	//typedef pointer iterator;
	typedef pointer* _Mapptr;
public:
	class iterator
	{ 
		friend class deque<_Ty,_A>;
	public:
		iterator():_First(nullptr),_Last(nullptr),_Next(nullptr){}
		iterator(pointer _P, _Mapptr _M)
			:_First(*_M),_Last(*_M+_DEQUESIZ),_Next(_P),_Map(_M)
		{};
	protected:
		pointer _First, _Last, _Next;
		_Mapptr _Map;
	};

public:

	explicit deque(const _A &_A1=_A()): allocator(_A1),_First(),_Last(), _Map(0), _Mapsize(0), _Size(0) { }

public:
	size_type size()
	{
		return _Size;
	}
	bool empty()
	{
		return size() == 0;
	}



public:
	void push_front(const _Ty& _X)
	{
		if (empty() || _First._Next == _First._First)
			_Buyfront();
		allocator.construct(--_First._Next, _X);
		++_Size;
	}


	void push_back(const _Ty& _X)
	{
		if (empty() || _Last._Next == _Last._Last)//空间不足
		{
			_Buyback();
			allocator.construct(_Last._Next++,_X);
		}
		else if (_Last._Next + 1 == _Last._Last)//只有一个空间
		{
			allocator.construct(_Last._Next++, _X);
			_Buyback();
		}
		else
		{
			allocator.construct(_Last._Next++, _X);
		}
		++_Size;
	}

	void pop_front()
	{
		allocator.destroy(_First._Next++);
		--_Size;
		if (empty() || _First._Next == _First._Last)
			_Freefront();
	}

	void pop_back()
	{
		allocator.destroy(--_Last._Next);
		--_Size;
		if (empty() || _Last._Next == _Last._First)
		{
			_Freeback();
		}
	}

protected:
	void _Buyback()
	{
		pointer _P = allocator.allocate(_DEQUESIZ);
		if (empty())
		{

			_Mapsize = _DEQUEMAPSIZ;//2个
			size_type _N = _Mapsize / 2;
			_Getmap();
			_Setptr(_Map + _N, _P);
			_First = iterator(_P + _DEQUESIZ / 2, _Map + _N);
			_Last = _First;

		}
		else if (_Last._Map < _Map + (_Mapsize - 1))
		{

		}
		else {

			difference_type _I = _Last._Map - _First._Map + 1;
			_Mapptr _M = _Growmap(2 * _I);
			_Setptr(_M + 1, _P);
			_First = iterator(_First._Next, _M);

		}

	}
	void _Buyfront()
	{
		pointer _P = allocator.allocate(_DEQUESIZ, (void*)0);
		if (empty())
		{
			_Mapsize = _DEQUEMAPSIZ;//2个
			size_type _N = _Mapsize / 2;
			_Getmap();
			_Setptr(_Map + _N, _P);
			_First = iterator(_P +(_DEQUESIZ / 2+1), _Map + _N);
			_Last = _First;
		}
		else
		{

		}
	}
	void _Freefront()
	{
		//allocator.deallocate
		_Freeptr(_First._Map++);
		if (empty())
		{
			_First = iterator();
			_Last = _First;
			_Freemap();
		}
		else
		{
			_First = iterator(*_First._Map, _First._Map);
		}
	}

	void _Freeback()
	{
		_Free(_Last._Map--);
		if (empty())
		{
			_First = iterator();
			_Last = _First;
			_Freemap();
		}
		else {
			_Last = iterator(*_Last._Map+_DEQUEMAPSIZ, _Last._Map);
		}
	}

	void _Freeptr(_Mapptr _M)
	{

		allocator.deallocate(*_M, _DEQUESIZ);
		
	}

	void _Freemap()
	{

		allocator.deallocate(_Map, _Mapsize);

	}

	void _Getmap()
	{

		//_Map = (_Mapptr)allocator.allocate(_Mapsize, pointer);
		_Map = Map_allocator.allocate(_Mapsize);
		
	}

	void _Setptr(_Mapptr _M, pointer _P)
	{

		*_M = _P;

	}
	
	_Mapptr _Growmap(size_type _Newsize)
	{

		_Mapptr _M = Map_allocator.allocate(_Newsize);



		std::copy(_First._Map, _Last._Map + 1,_M+_Newsize/4);
		Map_allocator.deallocate(_Map,_Mapsize);
		_Map = _M;
		_Mapsize = _Newsize;
		return (_M + _Newsize/4);

	}
	public:
	void print()
	{

		while (_Last._Next != _Last._First)
		{
			_Last._Next--;
			std::cout << _Last._Next << std::endl;
			std::cout<<":" << *_Last._Next << std::endl;
			//_First._Next++;
		}

	}
private:
	_A allocator;
	std::allocator<pointer> Map_allocator;
	iterator _First, _Last;
	_Mapptr _Map;//二级指针Map管理
	size_type _Mapsize, _Size;

};