#ifndef LIST_H
#define LIST_H
#include<assert.h>
#include"Xmemory.h"
template <typename _Ty, class _A = Allocator<_Ty> >
class list {
	public:
		struct _Node;
		typedef _Node* _Nodeptr;
		struct _Node {
			_Ty _Value;
			_Nodeptr _Next;
			_Nodeptr _Prev;
		};
		struct _Acc
		{
			typedef _Nodeptr& _Nodepref;
			typedef _Ty& _Vref;
			static _Nodepref _Next(_Nodeptr _P)
			{
				return ((_Nodepref)(*_P)._Next);
			}
			static _Nodepref _Prev(_Nodeptr _P)
			{
				return ((_Nodepref)(*_P)._Prev);
			}
			static _Vref _Value(_Nodeptr _P)
			{
				return ((_Vref)(*_P)._Value);
			}

		};

		typedef typename _A::size_type     size_type;
		typedef typename _A::value_type        value_type;
		typedef typename _A::pointer       pointer_type;
		typedef typename _A::reference       reference_type;
		typedef typename _A::const_pointer const_pointer;
		typedef typename _A::const_reference const_reference;
		typedef typename _A::difference_type       difference_type;
	/*
		typedef size_t     size_type;
		typedef _Ty        value_type;
		typedef _Ty* pointer;
		typedef _Ty& reference;
		typedef const _Ty* const_pointer;
		typedef const _Ty& const_reference;
		typedef int        difference_type;
	*/


		typedef list<_Ty, _A> _Myt;
	public:
		class iterator;
		class const_iterator;
		typedef const_iterator _IT;
		class const_iterator {
		public:
			const_iterator() {}
			const_iterator(_Nodeptr _P) :_Ptr(_P)
			{
			}


			bool operator== (const const_iterator & _X)const
			{
				return _Ptr == _X._Ptr;
			}
			bool operator!= (const const_iterator & _X)const
			{
				return !(*this==_X);
			}

			const_iterator& operator++()
			{
				std::cout << "const before ++" << std::endl;
				this->_Ptr = _Acc::_Next(this->_Ptr);
				return *this;
			}
			const_iterator operator++(int)
			{
				std::cout << "const  ++" << std::endl;
				const_iterator _Tmp = *this;
				this->_Ptr = _Acc::_Next(this->_Ptr);
				return _Tmp;
			}
			const_reference operator*()
			{
				return _Acc::_Value(this->_Ptr);
			}
			const_iterator operator--(int)
			{
				std::cout << "norm --" << std::endl;
				const_iterator _Tmp = *this;
				this->_Ptr = _Acc::_Prev(this->_Ptr);
				return _Tmp;
			}

			const_iterator& operator--()
			{
				std::cout << "norm before --" << std::endl;
				this->_Ptr = _Acc::_Prev(this->_Ptr);
				return *this;
			}

			_Nodeptr _Mynode() const
			{
				return _Ptr;
			}

		protected:
			_Nodeptr _Ptr;
		};

	public:
		class iterator : public const_iterator {
		public:
			iterator() {}
			iterator(_Nodeptr _P):const_iterator(_P)
			{
			}
			iterator(const iterator& _X) :const_iterator(_X._Ptr)
			{
				//std::cout << "copy" << std::endl;
			}
			iterator operator=(iterator& _X)
			{
				if (this != &_X) {
					this->_Ptr = _X._Ptr;
				}
				std::cout << "assign" << std::endl;
				return *this;
			}
			iterator operator++(int)
			{
				std::cout << "norm ++" << std::endl;
				iterator _Tmp = *this;
				this->_Ptr = _Acc::_Next(this->_Ptr);
				return _Tmp;
			}
			
			iterator& operator++()
			{
				std::cout << "norm before ++" << std::endl;
				this->_Ptr = _Acc::_Next(this->_Ptr);
				return *this;
			}
			iterator operator--(int)
			{
				std::cout << "norm --" << std::endl;
				iterator _Tmp = *this;
				this->_Ptr = _Acc::_Prev(this->_Ptr);
				return _Tmp;
			}

			iterator& operator--()
			{
				std::cout << "norm before --" << std::endl;
				this->_Ptr = _Acc::_Prev(this->_Ptr);
				return *this;
			}
			reference_type operator* ()
			{
				return _Acc::_Value(this->_Ptr);	
			}
			_Nodeptr _Mynode()
			{
				return this->_Ptr;
			}
		};

	public:
		explicit list() :_Head(_Buynode()),_Size(0) {}
		explicit list(size_type _N, const _Ty& value = _Ty()):_Head(_Buynode()),_Size(0)
		{
			insert(begin(), _N, value);
		}
		explicit list(const _Ty *_F, const _Ty* _L):_Head(_Buynode()), _Size(0)
		{
			insert(begin(), _F, _L);
		}
	public:

		iterator insert(iterator _P, const_reference _X)
		{
			_Nodeptr _S = _P._Mynode();
			_Nodeptr _N = _Buynode(_S, _Acc::_Prev(_S));
		
			_Acc::_Next(_Acc::_Prev(_S)) = _N;
			_Acc::_Prev(_S) = _N;

			//_Acc::_Value(_N) = _X;
			My_allocator.construct(&_Acc::_Value(_N),_X);
			_Size++;
			return iterator(_S);
		}
		iterator erase(iterator _P)
		{
			_Nodeptr _S = (_P++)._Mynode();
			_Acc::_Prev(_Acc::_Next(_S)) = _Acc::_Prev(_S);
			_Acc::_Next(_Acc::_Prev(_S)) = _Acc::_Next(_S);
			My_allocator.destroy(&_Acc::_Value(_S));
			_Freenode(_S);
			_Size--;
			//free(_S);
			return iterator(_P);
		}

		void insert(iterator _P, size_type _M, const _Ty& _X = _Ty())
		{
			for (; 0 < _M; --_M)
				insert(_P, _X);
		}

		void insert(iterator _P, const _Ty* _F, const _Ty* _L)
		{
			for (; _L != _F; ++_F)
			{
				insert(_P, *_F);
			}
		}

		void insert(iterator _P, _IT _F, _IT _L)
		{
			for (; _L != _F; ++_F)
			{
				insert(_P, *_F);
			}
		}

		void push_front(const _Ty& v)
		{
			insert(begin(), v);
		}

		void push_back(const _Ty& v)
		{
			insert(end(), v);
		}
		bool empty()
		{
			return _Size == 0 ? true : false;
		}
		void pop_front(const _Ty& v)
		{
			erase(begin());
		}
		void pop_back(const _Ty& v)
		{
			erase(--end());
		}
		void clear()
		{
			std::cout << "Clear" << std::endl;
			erase(begin(), end());
		}
		void assign(_IT _F, _IT _L)
		{
			clear();
			insert(begin(), _F, _L);
		}

		void splice(iterator _P, _Myt& _X)
		{
			if (!_X.empty())
			{
				_Splice(_P,_X, _X.begin(), _X.end());
				_Size += _X._Size;
				_X._Size = 0;
			}
		}

		iterator erase(iterator _F, iterator _L)
		{
			while (_F != _L)
			{
				erase(_F++);
				//_F++;
			}
			return _F;
		}


		const_iterator begin() const
		{
			return const_iterator(_Acc::_Next(_Head));
		}
		iterator begin()
		{ 
			return iterator(_Acc::_Next(_Head));
		}
		const_iterator end() const
		{
			return const_iterator(_Head);
		}
		iterator end()
		{
			return iterator(_Head);
		}

	protected:
		_Nodeptr _Buynode(_Nodeptr _Narg=0,_Nodeptr _Parg=0) {
			//_Nodeptr _S = (_Nodeptr)malloc(sizeof(_Node));
			_Nodeptr _S = (_Nodeptr)My_allocator._Charalloc(sizeof(_Node));
			assert(_S != NULL);
			_Acc::_Next(_S) = _Narg ? _Narg  : _S;
			_Acc::_Prev(_S) = _Parg ? _Parg : _S;
			return _S;
		}
		void _Freenode(_Nodeptr _S)
		{
			My_allocator.deallocate(_S,1);
		}

		void _Splice(iterator _P, _Myt& _X, iterator _F, iterator _L)
		{
			if (My_allocator == _X.My_allocator)
			{
				_Acc::_Next(_Acc::_Prev(_L._Mynode())) = _P._Mynode();
				_Acc::_Next(_Acc::_Prev(_F._Mynode())) = _L._Mynode();
				_Acc::_Next(_Acc::_Prev(_P._Mynode())) = _F._Mynode();
				_Nodeptr _S = _Acc::_Prev(_P._Mynode());
				_Acc::_Prev(_P._Mynode()) = _Acc::_Prev(_L._Mynode());
				_Acc::_Prev(_L._Mynode()) = _Acc::_Prev(_F._Mynode());
				_Acc::_Prev(_F._Mynode()) = _S;
			}
			else
			{
				insert(_P, _F, _L);
				_X.erase(_F, _L);
			}
		}
	private:
		_A My_allocator;
		
		_Nodeptr _Head; // Pointer to the head of the list
		size_type _Size;
	public:
		~list()
		{
			clear();
			_Freenode(_Head);
			_Head = nullptr;
			std::cout << "Îö¹¹º¯Êý" << std::endl;
		}
	};
#endif
