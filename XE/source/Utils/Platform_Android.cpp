#include "Platform.h"

#if PLATFORM_OS & OS_ANDROID

XE::Language XE::Platform::GetDefaultLanguage()
{
	return Language::ENGLISH;
}

XE::float32 XE::Platform::GetDpiFactor()
{
	return 1.0f;
}

#endif