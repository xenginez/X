/*!
 * \file	ConcurrentPriorityQueue.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/06/08
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CONCURRENTPRIORITYQUEUE_HPP__0AC2C5D0_C70E_420A_996E_07378A8833ED
#define CONCURRENTPRIORITYQUEUE_HPP__0AC2C5D0_C70E_420A_996E_07378A8833ED

#include "Type.h"

BEG_XE_NAMESPACE

template <typename _Ty, typename _Less = std::less<_Ty>, class _Container = XE::Deque<_Ty>> class ConcurrentPriorityQueue
{
public:
	using value_type = _Ty;
	using reference = _Ty &;
	using const_reference = const _Ty &;
	using compare_type = _Less;
	using size_type = XE::uint64;
	using container_type = _Container;
	using allocator_type = typename container_type::allocator_type;

public:
	ConcurrentPriorityQueue( ConcurrentPriorityQueue && _Src )
		: _Mark( _Src._Mark ), _Size( _Src._Size ), _Data( std::move( _Src._Data ) )
	{
		_Src._Mark = 0;
		_Src._Size = 0;
		_Aggregator.InitializeHandler( _FunctorType( this ) );
		_Heapify();
	}
	ConcurrentPriorityQueue( const ConcurrentPriorityQueue & _Src )
		: _Mark( _Src._Mark ), _Size( _Src._Size ), _Data( _Src._Data )
	{
		_Aggregator.InitializeHandler( _FunctorType( this ) );
		_Heapify();
	}
	ConcurrentPriorityQueue( const allocator_type & _Al = allocator_type() )
		: _Mark( 0 ), _Size( 0 ), _Data( _Al )
	{
		_Aggregator.InitializeHandler( _FunctorType( this ) );
	}
	ConcurrentPriorityQueue( ConcurrentPriorityQueue && _Src, const allocator_type & _Al )
		: _Mark( _Src._Mark ), _Size( _Src._Size ), _Data( std::move( _Src._Data ), _Al )
	{
		_Src._Mark = 0;
		_Src._Size = 0;
		_Aggregator.InitializeHandler( _FunctorType( this ) );
		_Heapify();
	}
	ConcurrentPriorityQueue( const ConcurrentPriorityQueue & _Src, const allocator_type & _Al )
		: _Mark( _Src._Mark ), _Data( _Src._Data.begin(), _Src._Data.end(), _Al )
	{
		_Size = _Data.size();
		_Aggregator.InitializeHandler( _FunctorType( this ) );
		_Heapify();
	}
	ConcurrentPriorityQueue( size_type _Init_capacity, const allocator_type & _Al = allocator_type() )
		: _Mark( 0 ), _Size( 0 ), _Data( _Al )
	{
		_Data.reserve( _Init_capacity );
		_Aggregator.InitializeHandler( _FunctorType( this ) );
	}
	template<typename _InputIterator> ConcurrentPriorityQueue( _InputIterator _Begin, _InputIterator _End, const allocator_type & _Al = allocator_type() )
		: _Data( _Begin, _End, _Al )
	{
		_Mark = 0;
		_Size = _Data.size();
		_Aggregator.InitializeHandler( _FunctorType( this ) );
		_Heapify();
	}

public:
	ConcurrentPriorityQueue & operator=( ConcurrentPriorityQueue && _Src )
	{
		if ( this != &_Src )
		{
			_Data = std::move( _Src._Data );
			_Mark = _Src._Mark;
			_Size = _Src._Size;
			_Src._Mark = 0;
			_Src._Size = 0;
		}
		return *this;
	}
	ConcurrentPriorityQueue & operator=( const ConcurrentPriorityQueue & _Src )
	{
		if ( this != &_Src )
		{
			std::vector<value_type, allocator_type>( _Src._Data.begin(), _Src._Data.end(), _Src._Data.get_allocator() ).swap( _Data );
			_Mark = _Src._Mark;
			_Size = _Src._Size;
		}
		return *this;
	}

public:
	bool empty() const
	{
		return ( _Size == 0 );
	}
	size_type size() const
	{
		return _Size;
	}

public:
	void clear()
	{
		_Data.clear();
		_Mark = 0;
		_Size = 0;
	}
	void push( const value_type & _Elem )
	{
		_CpqOperation op_data( _Elem, _PUSH_OP_COPY );
		_Aggregator.Execute( &op_data );
		if ( op_data._Status == _FAILED )
		{
			std::rethrow_exception( op_data._ExceptionPtr );
		}
	}
	void push( value_type && _Elem )
	{
		_CpqOperation op_data( _Elem, _PUSH_OP_MOVE );
		_Aggregator.Execute( &op_data );
		if ( op_data._Status == _FAILED )
		{
			std::rethrow_exception( op_data._ExceptionPtr );
		}
	}
	bool try_pop( reference _Elem )
	{
		_CpqOperation op_data( _POP_OP );
		op_data._Elem = &_Elem;
		_Aggregator.Execute( &op_data );
		return ( op_data._Status == _SUCCEEDED );
	}

public:
	void swap( ConcurrentPriorityQueue & _Queue )
	{
		_Data.swap( _Queue._Data );
		std::swap( _Mark, _Queue._Mark );
		std::swap( _Size, _Queue._Size );
	}

public:
	allocator_type get_allocator() const
	{
		return _Data.get_allocator();
	}

private:
	enum _OperationType
	{
		_INVALID_OP,
		_PUSH_OP_COPY,
		_PUSH_OP_MOVE,
		_POP_OP
	};

	enum _OperationStatus
	{
		_WAIT = 0,
		_SUCCEEDED,
		_FAILED
	};

	template <typename _Derived> class _AggregatedOperation
	{
	public:
		int _Status;
		_Derived * _Next;

		_AggregatedOperation()
			: _Status( 0 ), _Next( nullptr )
		{
		}
	};

	template < typename _OperationType, typename _Handler_type > class _AggregatorType
	{
	public:
		_AggregatorType()
			: _HandlerBusy( 0 )
		{
			_PendingOperations = nullptr;
		}

		~_AggregatorType()
		{
		}

		void InitializeHandler( _Handler_type _Handler )
		{
			_HandleOperations = _Handler;
		}

		void Execute( _OperationType * _Op )
		{
			_OperationType * res;

			do
			{
				_Op->_Next = res = _PendingOperations;
			} while ( !std::atomic_compare_exchange_weak( _PendingOperations, _Op, res ) );

			if ( res == nullptr )
			{
				StartHandleOperations();
			}
			else
			{
				while ( _Op->_Status == 0 )
				{
					std::this_thread::yield();
				}
			}
		}

	private:
		void StartHandleOperations()
		{
			_OperationType * op_list;

			while ( _HandlerBusy == 1 )
			{
				std::this_thread::yield();
			}
			
			XE::int32 old_val = std::atomic_exchange( &_HandlerBusy, 1 );

			op_list = reinterpret_cast<_OperationType *>( std::atomic_exchange( reinterpret_cast<void * *>( &_PendingOperations ), nullptr ) );

			_HandleOperations( op_list );

			old_val = std::atomic_exchange( &_HandlerBusy, 0 );
		}

	private:
		_OperationType * _PendingOperations;
		XE::int32 _HandlerBusy;
		_Handler_type _HandleOperations;
	};

	class _CpqOperation : public _AggregatedOperation<_CpqOperation>
	{
	public:
		_OperationType _Type;
		union
		{
			value_type * _Elem;
			size_type	_Size;
		};
		std::exception_ptr _ExceptionPtr;

		_CpqOperation( const_reference _E, _OperationType _T ) :
			_Type( _T ), _Elem( const_cast<value_type *>( &_E ) )
		{
		}
		_CpqOperation( value_type && _E, _OperationType _T ) :
			_Type( _T ), _Elem( const_cast<value_type *>( &_E ) )
		{
		}
		_CpqOperation( _OperationType _T ) : _Type( _T ) {}
	};

	class _FunctorType
	{
		ConcurrentPriorityQueue<_Ty, _Less, _Container> * _PCpq;
	public:
		_FunctorType() {}
		_FunctorType( ConcurrentPriorityQueue<_Ty, _Less, _Container> * _PCpq )
			: _PCpq( _PCpq )
		{}

		void operator()( _CpqOperation * _POp_list )
		{
			_PCpq->_HandleOperations( _POp_list );
		}
	};

private:
	void _Reheap()
	{
		size_type cur_pos = 0, child = 1;
		while ( child < _Mark )
		{
			size_type target = child;
			if ( child + 1 < _Mark && _Less( _Data[child], _Data[child + 1] ) )
			{
				++target;
			}

			if ( _Less( _Data[target], _Data[_Data.size() - 1] ) )
			{
				break;
			}

			_Data[cur_pos] = std::move( _Data[target] );
			cur_pos = target;
			child = ( cur_pos << 1 ) + 1;
		}

		_Data[cur_pos] = std::move( _Data[_Data.size() - 1] );
		_Data.pop_back();

		if ( _Mark > _Data.size() )
		{
			_Mark = _Data.size();
		}
	}

	void _Heapify()
	{
		if ( _Mark == 0 && _Size > 0 )
		{
			_Mark = 1;
		}

		for ( ; _Mark < _Size; ++_Mark )
		{
			size_type cur_pos = _Mark;
			value_type to_place = std::move( _Data[_Mark] );

			do
			{
				size_type parent = ( cur_pos - 1 ) >> 1;
				if ( !_Less( _Data[parent], to_place ) )
				{
					break;
				}
				_Data[cur_pos] = std::move( _Data[parent] );
				cur_pos = parent;

			} while ( cur_pos != 0 );

			_Data[cur_pos] = std::move( to_place );
		}
	}

	void _HandleOperations( _CpqOperation * _POp_list )
	{
		_CpqOperation * tmp, * pop_list = nullptr;

		while ( _POp_list != nullptr )
		{
			tmp = _POp_list;
			_POp_list = _POp_list->_Next;
			if ( tmp->_Type == _PUSH_OP_COPY )
			{
				try
				{
					_Data.push_back( *( tmp->_Elem ) );
					++_Size;
					std::atomic_exchange( (XE::int32 *)&tmp->_Status, _SUCCEEDED );
				}
				catch ( ... )
				{
					tmp->_ExceptionPtr = std::current_exception();
					std::atomic_exchange( (XE::int32 *)&tmp->_Status, _FAILED );
				}
			}
			else if ( tmp->_Type == _PUSH_OP_MOVE )
			{
				try
				{
					_Data.push_back( std::move( *( tmp->_Elem ) ) );
					++_Size;
					std::atomic_exchange( (XE::int32 *)&tmp->_Status, _SUCCEEDED );
				}
				catch ( ... )
				{
					tmp->_ExceptionPtr = std::current_exception();
					std::atomic_exchange( (XE::int32 *)&tmp->_Status, _FAILED );
				}
			}
			else
			{
				if ( _Mark < _Size && _Less( _Data[0], _Data[_Size - 1] ) )
				{
					*( tmp->_Elem ) = std::move( _Data[_Size - 1] );
					--_Size;
					std::atomic_exchange( (XE::int32 *)&tmp->_Status, _SUCCEEDED );
					_Data.pop_back();
				}
				else
				{
					tmp->_Next = pop_list;
					pop_list = tmp;
				}
			}
		}

		while ( pop_list != nullptr )
		{
			tmp = pop_list;
			pop_list = pop_list->_Next;

			if ( _Size == 0 )
			{
				std::atomic_exchange( (XE::int32 *)&tmp->_Status, _FAILED );
			}
			else
			{
				if ( _Mark < _Size && _Less( _Data[0], _Data[_Size - 1] ) )
				{
					*( tmp->_Elem ) = std::move( _Data[_Size - 1] );
					--_Size;
					std::atomic_exchange( (XE::int32 *)&tmp->_Status, _SUCCEEDED );
					_Data.pop_back();
				}
				else
				{
					*( tmp->_Elem ) = std::move( _Data[0] );
					--_Size;
					std::atomic_exchange( (XE::int32 *)&tmp->_Status, _SUCCEEDED );
					_Reheap();
				}
			}
		}

		if ( _Mark < _Size )
		{
			_Heapify();
		}
	}

private:
	_AggregatorType< _CpqOperation, _FunctorType > _Aggregator;
	char _Padding1[64 - sizeof( _AggregatorType< _CpqOperation, _FunctorType > )];
	size_type _Mark;
	size_type _Size;
	compare_type _Less;
	char _Padding2[64 - sizeof( size_type ) - sizeof( _Less )];
	container_type _Data;
};

END_XE_NAMESPACE

#endif//CONCURRENTPRIORITYQUEUE_HPP__0AC2C5D0_C70E_420A_996E_07378A8833ED