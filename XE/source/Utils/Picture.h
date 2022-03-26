/*!
 * \file	Picture.h
 *
 * \author	ZhengYuanQing
 * \date	2021/07/26
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef PICTURE_H__57B94437_3845_47D5_BC2A_93775C8969AC
#define PICTURE_H__57B94437_3845_47D5_BC2A_93775C8969AC

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Picture : public XE::NonCopyable
{
public:
    Picture();

    ~Picture();

public:
    bool Load( std::istream & val, bool filp_y = false );

    bool Save( std::ostream & val, XE::ImageType type, bool filp_y = false );

public:
    bool Resize( XE::uint32 w, XE::uint32 h );

public:
    XE::uint32 GetWidth() const;

    XE::uint32 GetHeight() const;

    XE::uint32 GetChannels() const;

    const XE::Array< XE::uint8 > & GetPixels() const;

public:
    XE::uint32 GetPixel( XE::uint32 x, XE::uint32 y ) const;

    void SetPixel( XE::uint32 x, XE::uint32 y, XE::uint32 pixel );

    void SetPixels( XE::BasicMemoryView< XE::uint32 > pixels );

private:
    XE::uint32 _Width = 0;
    XE::uint32 _Height = 0;
    XE::uint32 _Channels = 0;
    XE::Array< XE::uint8 > _Pixels;
};

END_XE_NAMESPACE

#endif // PICTURE_H__57B94437_3845_47D5_BC2A_93775C8969AC
