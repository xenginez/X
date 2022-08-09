/*!
 * \file	RenderShader.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/21
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef RENDERSHADER_H__69113B18_585B_4E74_A6BA_4C5A535247AD
#define RENDERSHADER_H__69113B18_585B_4E74_A6BA_4C5A535247AD

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API RenderShader : public XE::Object
{
	OBJECT( RenderShader, XE::Object )

public:
	RenderShader();

	~RenderShader() override;

public:


private:
	XE::GraphicsShaderModulePtr _ShaderModule;
	XE::Map< XE::GraphicsShaderStage, XE::String > _EntryPoints;
	XE::Array< std::tuple< XE::String, XE::Variant > > _Parameters;
};

END_XE_NAMESPACE

#endif//RENDERSHADER_H__69113B18_585B_4E74_A6BA_4C5A535247AD
