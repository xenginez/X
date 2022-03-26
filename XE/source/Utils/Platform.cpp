#include "Platform.h"

#include <cpuinfo/cpuinfo.h>

namespace XE
{
	XE::String ToString( cpuinfo_vendor vendor )
	{
	#define CASE( VALUE ) \
	case VALUE: \
	return #VALUE;

		switch( vendor )
		{
			CASE( cpuinfo_vendor_unknown )
				CASE( cpuinfo_vendor_intel )
				CASE( cpuinfo_vendor_amd )
				CASE( cpuinfo_vendor_arm )
				CASE( cpuinfo_vendor_qualcomm )
				CASE( cpuinfo_vendor_apple )
				CASE( cpuinfo_vendor_samsung )
				CASE( cpuinfo_vendor_nvidia )
				CASE( cpuinfo_vendor_mips )
				CASE( cpuinfo_vendor_ibm )
				CASE( cpuinfo_vendor_ingenic )
				CASE( cpuinfo_vendor_via )
				CASE( cpuinfo_vendor_cavium )
				CASE( cpuinfo_vendor_broadcom )
				CASE( cpuinfo_vendor_apm )
				CASE( cpuinfo_vendor_huawei )
				CASE( cpuinfo_vendor_hygon )
				CASE( cpuinfo_vendor_texas_instruments )
				CASE( cpuinfo_vendor_marvell )
				CASE( cpuinfo_vendor_rdc )
				CASE( cpuinfo_vendor_dmp )
				CASE( cpuinfo_vendor_motorola )
				CASE( cpuinfo_vendor_transmeta )
				CASE( cpuinfo_vendor_cyrix )
				CASE( cpuinfo_vendor_rise )
				CASE( cpuinfo_vendor_nsc )
				CASE( cpuinfo_vendor_sis )
				CASE( cpuinfo_vendor_nexgen )
				CASE( cpuinfo_vendor_umc )
				CASE( cpuinfo_vendor_dec )
		default:
			break;
		}

		return "";

	#undef CASE
	}
}

bool g_cpu_info_init = cpuinfo_initialize();

XE::String XE::Platform::CpuInfo()
{
	XE::String result( R"(
Name:{%0} Vendor:{%1} Core:{%2}
Support:)" );

	result = XE::Format( result,
						 cpuinfo_get_package( 0 )->name,
						 cpuinfo_get_core( 0 )->vendor,
						 cpuinfo_get_cores_count() );

#define HAS( SUPPORT ) \
	if( cpuinfo_has_##SUPPORT() ) \
	{ \
		result += "\n\t"#SUPPORT; \
	}

	HAS( x86_rdtsc );
	HAS( x86_rdtscp );
	HAS( x86_rdpid );
	HAS( x86_clzero );
	HAS( x86_mwait );
	HAS( x86_mwaitx );
	HAS( x86_fxsave );
	HAS( x86_xsave );
	HAS( x86_fpu );
	HAS( x86_mmx );
	HAS( x86_mmx_plus );
	HAS( x86_3dnow );
	HAS( x86_3dnow_plus );
	HAS( x86_3dnow_geode );
	HAS( x86_prefetch );
	HAS( x86_prefetchw );
	HAS( x86_prefetchwt1 );
	HAS( x86_daz );
	HAS( x86_sse );
	HAS( x86_sse2 );
	HAS( x86_sse3 );
	HAS( x86_ssse3 );
	HAS( x86_sse4_1 );
	HAS( x86_sse4_2 );
	HAS( x86_sse4a );
	HAS( x86_misaligned_sse );
	HAS( x86_avx );
	HAS( x86_fma3 );
	HAS( x86_fma4 );
	HAS( x86_xop );
	HAS( x86_f16c );
	HAS( x86_avx2 );
	HAS( x86_avx512f );
	HAS( x86_avx512pf );
	HAS( x86_avx512er );
	HAS( x86_avx512cd );
	HAS( x86_avx512dq );
	HAS( x86_avx512bw );
	HAS( x86_avx512vl );
	HAS( x86_avx512ifma );
	HAS( x86_avx512vbmi );
	HAS( x86_avx512vbmi2 );
	HAS( x86_avx512bitalg );
	HAS( x86_avx512vpopcntdq );
	HAS( x86_avx512vnni );
	HAS( x86_avx512bf16 );
	HAS( x86_avx512vp2intersect );
	HAS( x86_avx512_4vnniw );
	HAS( x86_avx512_4fmaps );
	HAS( x86_hle );
	HAS( x86_rtm );
	HAS( x86_xtest );
	HAS( x86_mpx );
	HAS( x86_cmov );
	HAS( x86_cmpxchg8b );
	HAS( x86_cmpxchg16b);
	HAS( x86_clwb);
	HAS( x86_movbe);
	HAS( x86_lahf_sahf);
	HAS( x86_lzcnt);
	HAS( x86_popcnt);
	HAS( x86_tbm);
	HAS( x86_bmi);
	HAS( x86_bmi2);
	HAS( x86_adx);
	HAS( x86_aes);
	HAS( x86_vaes);
	HAS( x86_pclmulqdq);
	HAS( x86_vpclmulqdq);
	HAS( x86_gfni);
	HAS( x86_rdrand);
	HAS( x86_rdseed);
	HAS( x86_sha);

	HAS( arm_thumb );
	HAS( arm_thumb2 );
	HAS( arm_v5e );
	HAS( arm_v6 );
	HAS( arm_v6k );
	HAS( arm_v7 );
	HAS( arm_v7mp );
	HAS( arm_v8 );
	HAS( arm_idiv );
	HAS( arm_vfpv2 );
	HAS( arm_vfpv3 );
	HAS( arm_vfpv3_d32 );
	HAS( arm_vfpv3_fp16 );
	HAS( arm_vfpv3_fp16_d32 );
	HAS( arm_vfpv4 );
	HAS( arm_vfpv4_d32 );
	HAS( arm_wmmx );
	HAS( arm_wmmx2 );
	HAS( arm_neon );
	HAS( arm_neon_fp16 );
	HAS( arm_neon_fma );
	HAS( arm_neon_v8 );
	HAS( arm_atomics );
	HAS( arm_neon_rdm );
	HAS( arm_neon_fp16_arith );
	HAS( arm_fp16_arith );
	HAS( arm_neon_dot );
	HAS( arm_jscvt );
	HAS( arm_fcma );
	HAS( arm_aes );
	HAS( arm_sha1 );
	HAS( arm_sha2 );
	HAS( arm_pmull );
	HAS( arm_crc32 );
	HAS( arm_sve );
	HAS( arm_sve2 );

	return result;

#undef HAS
}

XE::String XE::Platform::CpuName()
{
	return cpuinfo_get_package( 0 )->name;
}

XE::uint64 XE::Platform::CpuCoreSize()
{
	return cpuinfo_get_cores_count();
}

XE::uint64 XE::Platform::CpuCacheL1Size()
{
	return cpuinfo_get_processor( 0 )->cache.l1d->size;
}

XE::uint64 XE::Platform::CpuCacheL2Size()
{
	return cpuinfo_get_processor( 0 )->cache.l2->size;
}
