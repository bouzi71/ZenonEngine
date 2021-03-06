#include "stdafx.h"

// General
#include "MaterialModel.h"

const char* cTextureTypeNames[] =
{ 
	"Ambient",
	"Diffuse",
	"Specular",
	"Emissive",

	"Shininess",
	"NormalMap",
	"Bump",
	"Transparency",
	
	"Reflection",
	"Displacement"
};

MaterialModel::MaterialModel(const IBaseManager& BaseManager)
	: MaterialProxieT(BaseManager.GetApplication().GetRenderDevice().GetObjectsFactory().CreateMaterial("MaterialModel"))
	, m_BaseManager(BaseManager)
{
	// Ambient
	{
		auto prop = MakeShared(CPropertyWrapped<ColorRGB>, "AmbientColor", "descr", ColorRGB(0.0f));
		prop->SetValueSetter(std::bind(&MaterialModel::SetAmbientColor, this, std::placeholders::_1));
		prop->SetValueGetter(std::bind(&MaterialModel::GetAmbientColor, this));
		GetProperties()->AddProperty(prop);
	}

	// Diffuse
	{
		auto prop = MakeShared(CPropertyWrapped<ColorRGB>, "DiffuseColor", "descr", ColorRGB(1.0f));
		prop->SetValueSetter(std::bind(&MaterialModel::SetDiffuseColor, this, std::placeholders::_1));
		prop->SetValueGetter(std::bind(&MaterialModel::GetDiffuseColor, this));
		GetProperties()->AddProperty(prop);
	}

	// Specular
	{
		auto prop = MakeShared(CPropertyWrapped<ColorRGB>, "SpecularColor", "descr", ColorRGB(1.0f));
		prop->SetValueSetter(std::bind(&MaterialModel::SetSpecularColor, this, std::placeholders::_1));
		prop->SetValueGetter(std::bind(&MaterialModel::GetSpecularColor, this));
		GetProperties()->AddProperty(prop);
	}

	// Emissive
	{
		auto prop = MakeShared(CPropertyWrapped<ColorRGB>, "EmissiveColor", "descr", ColorRGB(0.0f));
		prop->SetValueSetter(std::bind(&MaterialModel::SetEmissiveColor, this, std::placeholders::_1));
		prop->SetValueGetter(std::bind(&MaterialModel::GetEmissiveColor, this));
		GetProperties()->AddProperty(prop);
	}
}

MaterialModel::~MaterialModel()
{

}



void MaterialModel::SetAmbientColor(ColorRGB Color)
{
	MaterialData().Ambient = Color;
}

ColorRGB MaterialModel::GetAmbientColor() const
{
	return MaterialDataReadOnly().Ambient;
}

void MaterialModel::SetAmbientFactor(float Factor)
{
	MaterialData().AmbientFactor = Factor;
}



void MaterialModel::SetDiffuseColor(ColorRGB Color)
{
	MaterialData().Diffuse = Color;
}

ColorRGB MaterialModel::GetDiffuseColor() const
{
	return MaterialDataReadOnly().Diffuse;
}

void MaterialModel::SetDiffuseFactor(float Factor)
{
	MaterialData().DiffuseFactor = Factor;
}



void MaterialModel::SetSpecularColor(ColorRGB Color)
{
	MaterialData().Specular = Color;
}

ColorRGB MaterialModel::GetSpecularColor() const
{
	return MaterialDataReadOnly().Specular;
}

void MaterialModel::SetSpecularFactor(float Factor)
{
	MaterialData().SpecularFactor = Factor;
}



void MaterialModel::SetEmissiveColor(ColorRGB Color)
{
	MaterialData().Emissive = Color;
}

ColorRGB MaterialModel::GetEmissiveColor() const
{
	return MaterialDataReadOnly().Emissive;
}

void MaterialModel::SetEmissiveFactor(float Factor)
{
	MaterialData().EmissiveFactor = Factor;
}



void MaterialModel::SetShininess(float Factor)
{
	MaterialData().Shininess = Factor;
}

void MaterialModel::SetBumpFactor(float Factor)
{
	MaterialData().BumpFactor = Factor;
}



void MaterialModel::SetEnviorementMappingEnable(bool Value)
{
	MaterialData().IsEnviorementMappingEnable = Value;
}



void MaterialModel::SetTexture(ETextureType TextureType, std::shared_ptr<ITexture> texture)
{
	switch (TextureType)
	{
		case ETextureType::TextureAmbient:
		{
			MaterialData().HasTextureAmbient = true;
		}
		break;
		case ETextureType::TextureDiffuse:
		{
			MaterialData().HasTextureDiffuse = true;
		}
		break;
		case ETextureType::TextureSpecular:
		{
			MaterialData().HasTextureSpecular = true;
		}
		break;
		case ETextureType::TextureEmissive:
		{
			MaterialData().HasTextureEmissive = true;
		}
		break;
		case ETextureType::TextureShininess:
		{
			MaterialData().HasTextureShininess = true;
		}
		break;
		case ETextureType::TextureNormalMap:
		{
			MaterialData().HasTextureNormalMap = true;
		}
		break;
		case ETextureType::TextureBump:
		{
			MaterialData().HasTextureBump = true;
		}
		break;
		case ETextureType::TextureTransparency:
		{
			MaterialData().HasTextureTransparency = true;
		}
		break;
		case ETextureType::TextureReflection:
		{
			MaterialData().HasTextureReflection = true;
		}
		break;
		case ETextureType::TextureDisplacement:
		{
			MaterialData().HasTextureDisplacement = true;
		}
		break;
		default:
			_ASSERT(false);
	};

	__super::SetTexture((uint32)TextureType, texture);
}

std::shared_ptr<ITexture> MaterialModel::GetTexture(ETextureType TextureType) const
{
	return __super::GetTexture((uint32)TextureType);
}



//
// IObjectLoadSave
//
void MaterialModel::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	SetName(Reader->GetStrAttribute("Name"));

	auto propertiesReaderXML = Reader->GetChild("Properties");
	GetProperties()->Load(propertiesReaderXML);

	auto texturesReader = Reader->GetChild("Textures");
	for (const auto& textureNode : texturesReader->GetChilds())
	{
		MaterialModel::ETextureType textureID = GetTextureIDByTextureTypeName(textureNode->GetName().c_str());

		if (__super::GetTexture(textureID) != nullptr)
		{
			Log::Error("MaterialModel: Texture with ID '%d' already set.", textureID);
			continue;
		}

		std::string textureFileName = textureNode->GetValue();

		auto texture = m_BaseManager.GetManager<IznTexturesFactory>()->LoadTexture2D(textureFileName);
		if (texture == nullptr)
		{
			Log::Error("MaterialModel: Unable load texture: '%s' with ID '%d' for material '%s'.", textureFileName.c_str(), textureID, GetName().c_str());
			texture = m_BaseManager.GetManager<IznTexturesFactory>()->GetDefaultTexture();
		}

		SetTexture(textureID, texture);
	}
}

void MaterialModel::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	Writer->SetStrAttribute(GetName(), "Name");

	auto propertiesWriterXML = Writer->CreateChild("Properties");
	GetProperties()->Save(propertiesWriterXML);

	auto texturesWriter = Writer->CreateChild("Textures");
	for (size_t i = 0; i < ETextureType::Count; i++)
	{
		auto texture = __super::GetTexture(i);
		if (texture == nullptr)
			continue;

		const auto textureFileName = texture->GetFilename();
		if (textureFileName.empty())
			throw CException("Empty filename for texture with ID '%d'.", i);

		auto textureWriter = texturesWriter->CreateChild(GetTextureTypeNameByTextureID(static_cast<MaterialModel::ETextureType>(i)));
		textureWriter->SetValue(textureFileName);
	}
}



//
// Protected
//
void MaterialModel::PrintInfo()
{
	Log::Info("Ambient: [%f, %f, %f]. Factor [%f].", MaterialDataReadOnly().Ambient.r, MaterialDataReadOnly().Ambient.g, MaterialDataReadOnly().Ambient.b, MaterialDataReadOnly().AmbientFactor);
	Log::Info("Diffuse: [%f, %f, %f]. Factor [%f].", MaterialDataReadOnly().Diffuse.r, MaterialDataReadOnly().Diffuse.g, MaterialDataReadOnly().Diffuse.b, MaterialDataReadOnly().DiffuseFactor);
	Log::Info("Specular: [%f, %f, %f]. Factor [%f].", MaterialDataReadOnly().Specular.r, MaterialDataReadOnly().Specular.g, MaterialDataReadOnly().Specular.b, MaterialDataReadOnly().SpecularFactor);
	Log::Info("Emissive: [%f, %f, %f]. Factor [%f].", MaterialDataReadOnly().Emissive.r, MaterialDataReadOnly().Emissive.g, MaterialDataReadOnly().Emissive.b, MaterialDataReadOnly().EmissiveFactor);
}

std::string MaterialModel::GetTextureTypeNameByTextureID(ETextureType ID) const
{
	for (size_t i = 0; i < ETextureType::Count; i++)
		if (i == ID)
			return cTextureTypeNames[i];
	throw CException("Unknown texture ID '%d'.", ID);
}

MaterialModel::ETextureType MaterialModel::GetTextureIDByTextureTypeName(const char * TypeName) const
{
	for (size_t i = 0; i < ETextureType::Count; i++)
		if (::stricmp(cTextureTypeNames[i], TypeName) == 0)
			return static_cast<MaterialModel::ETextureType>(i);
	throw CException("Unknown texture type name '%s'.", TypeName);
}
