#include "GUIService.h"

#include "Core/CoreFramework.h"
#include "Core/ThreadService.h"

#include "Canvas.h"

IMPLEMENT_META( XE::GUIService );

struct XE::GUIService::Private
{
	XE::ConcurrentQueue< XE::CanvasPtr > _FreeCanvas;
	XE::ConcurrentList< XE::Pair< XE::CanvasPtr, bool > > _Canvas;
};

XE::GUIService::GUIService()
	:_p( XE::New< Private >() )
{

}

XE::GUIService::~GUIService()
{
	XE::Delete( _p );
}

void XE::GUIService::Prepare()
{
	ImGui::SetAllocatorFunctions( 
		[]( size_t sz, void * user_data ) { return XE::MemoryResource::Alloc( sz ); },
		[]( void * ptr, void * user_data ) { XE::MemoryResource::Free( ptr ); },
		nullptr );
}

void XE::GUIService::Startup()
{

}

void XE::GUIService::Update()
{
	using Iterator = XE::ConcurrentList< XE::Pair< XE::CanvasPtr, bool > >::iterator;
	GetFramework()->GetServiceT< XE::ThreadService >()->ParallelTask( _p->_Canvas.begin(), _p->_Canvas.end(), [this]( Iterator it )
	{
		if ( it->second == false )
		{
			it->first->Startup();
			it->second = true;
		}

		it->first->Update();

		// TODO: ImGui::GetDrawData();

	} ).wait();

	XE::CanvasPtr canvas;
	while ( _p->_FreeCanvas.try_pop( canvas ) )
	{
		canvas->Clearup();
	}
}

void XE::GUIService::Clearup()
{
	for ( const auto & it : _p->_Canvas )
	{
		if ( it.second )
		{
			it.first->Clearup();
		}
	}
	_p->_Canvas.clear();

	XE::CanvasPtr canvas;
	while ( _p->_FreeCanvas.try_pop( canvas ) )
	{
		canvas->Clearup();
	}
}

XE::CanvasPtr XE::GUIService::FindCanvas( const XE::String & val )
{
	auto it = std::find_if( _p->_Canvas.begin(), _p->_Canvas.end(), [&]( const auto & it ) { return it.first->GetName() == val; } );

	return it != _p->_Canvas.end() ? it->first : nullptr;
}

XE::Disposable XE::GUIService::RegisterCanvas( const XE::CanvasPtr & val )
{
	XE::Pair< XE::CanvasPtr, bool > pair( val, false );
	auto it = _p->_Canvas.emplace_back( pair );
	return { [this, it]() { _p->_FreeCanvas.push( it->first ); _p->_Canvas.erase( it ); } };
}
