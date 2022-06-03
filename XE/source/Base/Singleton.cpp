#include "Singleton.h"

namespace
{
	class SigletonMap : public XE::Map< XE::uint64, std::tuple< void *, XE::uint64, XE::Delegate< void( void * ) > > >
	{
	public:
		~SigletonMap()
		{
			for( const auto & it : *this )
			{
				std::get<2>( it.second )( std::get<0>( it.second ) );
				std::pmr::get_default_resource()->deallocate( std::get<0>( it.second ), std::get<1>( it.second ) );
			}
		}
	};
}

XE::Pair<void *, bool> XE::SingletonBase::NewSigleton( XE::uint64 hash, XE::uint64 size, const XE::Delegate<void( void * )> & destructor )
{
	static std::mutex _Lock;
	static SigletonMap _Map;

	std::unique_lock< std::mutex > lock( _Lock );

	auto it = _Map.find( hash );
	if( it != _Map.end() )
	{
		return { std::get<0>( it->second ), true };
	}

	void * p = std::pmr::get_default_resource()->allocate( size );

	_Map.insert( { hash, { p, size, destructor } } );

	return { p, false };
}
