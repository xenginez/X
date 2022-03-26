#include "Picture.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <stb/stb_image_resize.h>

XE::Picture::Picture()
{

}

XE::Picture::~Picture()
{

}

bool XE::Picture::Load( std::istream & val, bool filp_y /*= false */ )
{
	stbi_io_callbacks callback;

	callback.read = []( void * user, char * data, int size )
	{
		std::istream * stream = ( std::istream * )( user );

		return stream->get();
	};
	callback.skip = []( void * user, int n )
	{
		std::istream * stream = ( std::istream * )( user );

		stream->seekg( n, std::ios::beg );
	};
	callback.eof = []( void * user )
	{
		std::istream * stream = ( std::istream * )( user );

		return stream->eof() ? 1 : 0;
	};

	int x, y, comp; x = y = comp = 0;

	stbi_set_flip_vertically_on_load( filp_y );

	stbi_uc * data = stbi_load_from_callbacks( &callback, ( void * ) &val, &x, &y, &comp, 0 );

	if( x != 0 && y != 0 && comp != 0 )
	{
		_Width = x; _Height = y; _Channels = comp;

		_Pixels.resize( XE::uint64( _Width ) * XE::uint64( _Channels ) * XE::uint64( _Height ) );

		std::memcpy( _Pixels.data(), data, _Pixels.size() );

		stbi_image_free( data );

		return true;
	}

	return false;
}

bool XE::Picture::Save( std::ostream & val, XE::ImageType type, bool filp_y /*= false */ )
{
	auto func = []( void * context, void * data, int size )
	{
		std::ostream * stream = ( std::ostream * )( context );

		stream->write( ( const char * ) data, size );
	};

	int result = 0;

	stbi_flip_vertically_on_write( filp_y );

	switch( type )
	{
	case XE::ImageType::PNG:
		result = stbi_write_png_to_func( func, ( void * ) &val, _Width, _Height, _Channels, _Pixels.data(), 0 );
		break;
	case XE::ImageType::JPG:
		result = stbi_write_jpg_to_func( func, ( void * ) &val, _Width, _Height, _Channels, _Pixels.data(), 90 );
		break;
	case XE::ImageType::TGA:
		result = stbi_write_tga_to_func( func, ( void * ) &val, _Width, _Height, _Channels, _Pixels.data() );
		break;
	case XE::ImageType::BMP:
		result = stbi_write_bmp_to_func( func, ( void * ) &val, _Width, _Height, _Channels, _Pixels.data() );
		break;
	default:
		break;
	}

	return result != 0;
}

bool XE::Picture::Resize( XE::uint32 w, XE::uint32 h )
{
	XE::Array< XE::uint8 > out( XE::uint64( w ) * XE::uint64( _Channels ) * XE::uint64( h ) );

	if( stbir_resize_uint8( _Pixels.data(), _Width, _Height, 0, out.data(), w, h, 0, _Channels ) != 0 )
	{
		_Width = w; _Height = h; _Pixels = out;

		return true;
	}

	return false;
}

XE::uint32 XE::Picture::GetWidth() const
{
	return _Width;
}

XE::uint32 XE::Picture::GetHeight() const
{
	return _Height;
}

XE::uint32 XE::Picture::GetChannels() const
{
	return _Channels;
}

const XE::Array< XE::uint8 > & XE::Picture::GetPixels() const
{
	return _Pixels;
}

XE::uint32 XE::Picture::GetPixel( XE::uint32 x, XE::uint32 y ) const
{
	XE::uint32 pixel = 0;

	XE::uint64 index = ( XE::uint64( x ) + XE::uint64( _Height ) * XE::uint64( y ) ) * XE::uint64( _Channels );

	switch( _Channels )
	{
	case 4:
		pixel |= XE::uint64( _Pixels[index + 3] ) << 24;
	case 3:
		pixel |= XE::uint64( _Pixels[index + 2] ) << 16;
	case 2:
		pixel |= XE::uint64( _Pixels[index + 1] ) << 8;
	case 1:
		pixel |= XE::uint64( _Pixels[index + 0] );
	default:
		break;
	}

	return pixel;
}

void XE::Picture::SetPixel( XE::uint32 x, XE::uint32 y, XE::uint32 pixel )
{
	XE::uint64 index = ( XE::uint64( x ) + XE::uint64( _Height ) * XE::uint64( y ) ) * XE::uint64( _Channels );

	switch( _Channels )
	{
	case 4:
		_Pixels[index + 3] = pixel & 0xFF000000;
	case 3:
		_Pixels[index + 2] = pixel & 0x00FF0000;
	case 2:
		_Pixels[index + 1] = pixel & 0x0000FF00;
	case 1:
		_Pixels[index + 0] = pixel & 0x000000FF;
	default:
		break;
	}
}

void XE::Picture::SetPixels( XE::BasicMemoryView< XE::uint32 > pixels )
{
	std::memcpy( _Pixels.data(), pixels.data(), pixels.size() );
}
