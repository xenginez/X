/*!
 * \file   WASI.h
 *
 * \author ZhengYuanQing
 * \date   2022/05/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef WASI_H__8A76CEB4_F41C_491B_A559_32437F29F17A
#define WASI_H__8A76CEB4_F41C_491B_A559_32437F29F17A

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API WASI : public std::enable_shared_from_this< WASI >
{
public:
	WASI() = default;

	virtual ~WASI() = default;

public:

};
DECL_XE_CLASS( WASI );

END_XE_NAMESPACE

#endif // WASI_H__8A76CEB4_F41C_491B_A559_32437F29F17A
