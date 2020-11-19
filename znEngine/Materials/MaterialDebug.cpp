#include "stdafx.h"

// General
#include "MaterialDebug.h"

MaterialDebug::MaterialDebug(IRenderDevice& RenderDevice)
	: MaterialProxieT(RenderDevice, "MaterialDebug")
{}

MaterialDebug::~MaterialDebug()
{}



//
// MaterialDebug
//
void MaterialDebug::SetDiffuseColor(const glm::vec4& diffuse)
{
	MaterialData().DiffuseColor = diffuse;
	MarkMaterialDataDirty();
}

glm::vec4 MaterialDebug::GetDiffuseColor() const
{
	return MaterialDataReadOnly().DiffuseColor;
}

void MaterialDebug::SetTexture(std::shared_ptr<ITexture> Texture)
{
	MaterialData().HasTextureDiffuse = (Texture != nullptr);
	MarkMaterialDataDirty();

	if (MaterialData().HasTextureDiffuse)
		__super::SetTexture(0, Texture);
}

std::shared_ptr<ITexture> MaterialDebug::GetTexture() const
{
	return __super::GetTexture(0);
}