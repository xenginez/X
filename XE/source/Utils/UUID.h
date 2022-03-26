/*!
 * \file	UUID.h
 *
 * \author	ZhengYuanQing
 * \date	2021/03/26
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef UUID_H__E164A39F_98E8_4E72_8408_7BD981800EC5
#define UUID_H__E164A39F_98E8_4E72_8408_7BD981800EC5

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API UUID
{
public:
	UUID();

	UUID( const XE::UUID & val );

	UUID( const XE::String & val );

public:
	UUID & operator=( const UUID & val );

public:
	bool operator < ( const UUID & val ) const;

	bool operator <= ( const UUID & val ) const;

	bool operator > ( const UUID & val ) const;

	bool operator >= ( const UUID & val ) const;

	bool operator == ( const UUID & val ) const;

	bool operator != ( const UUID & val ) const;

public:
	static UUID Create();

public:
	/// < summary >
	/// format
	/// raw data: 0123456789abcdef0123456789abcdef
	/// format string: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx		container: 0123456789abcdef0123456789abcdef
	/// format string: xxxxxxxxxxxxxxxxxxxxxxxx				container: 0123456789abcdef01234567
	/// format string: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx	container: 01234567-89ab-cdef-0123-456789abcdef
	/// format string: XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX	container: 01234567-89AB-CDEF-0123-456789ABCDEF
	/// < /summary >
	/// < param name="fmt" >format string< /param >
	XE::String ToString( std::string_view fmt = "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" ) const;

	void FromString( const XE::String & container, std::string_view fmt = "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" );

private:
	union
	{
		struct
		{
			XE::uint32 Data1;
			XE::uint16 Data2;
			XE::uint16 Data3;
			XE::uint8  Data4[8];
		} _GUID;

		XE::uint8 _Datas[16];
	};
};
DECL_XE_CLASS( UUID );

XE_INLINE XE::String ToString(const XE::UUID & _Val, std::string_view fmt = "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" )
{
	return _Val.ToString( fmt );
}

XE_INLINE bool FromString( XE::UUID & _Val, const XE::String & _Str, std::string_view fmt = "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" )
{
	_Val.FromString( _Str, fmt );

	return true;
}

template< > struct XE::Serializable< XE::UUID >
{
public:
	static void Serialize( OArchive & arc, const XE::UUID & val )
	{
		arc << val.ToString();
	}

	static void Deserialize( IArchive & arc, XE::UUID & val )
	{
		XE::String suuid;
		arc >> suuid;
		val.FromString( suuid );
	}
};

END_XE_NAMESPACE

#endif // UUID_H__E164A39F_98E8_4E72_8408_7BD981800EC5
