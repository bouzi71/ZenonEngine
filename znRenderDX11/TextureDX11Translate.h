#pragma once

static void ReportAndThrowTextureFormatError(const ITexture::TextureFormat& format, const std::string& file, int line, const std::string& function, const std::string& message)
{
	std::stringstream ss;
	ss << message << std::endl;
	ss << "Components: ";
	switch (format.Components)
	{
	case ITexture::Components::R:
		ss << "R" << std::endl;
		break;
	case ITexture::Components::RG:
		ss << "RG" << std::endl;
		break;
	case ITexture::Components::RGB:
		ss << "RGB" << std::endl;
		break;
	case ITexture::Components::RGBA:
		ss << "RGBA" << std::endl;
		break;
	case ITexture::Components::Depth:
		ss << "Depth" << std::endl;
		break;
	case ITexture::Components::DepthStencil:
		ss << "DepthStencil" << std::endl;
		break;
	default:
		ss << "Unknown" << std::endl;
		break;
	}

	ss << "Type:";
	switch (format.Type)
	{
	case ITexture::Type::Typeless:
		ss << "Typeless" << std::endl;
		break;
	case ITexture::Type::UnsignedNormalized:
		ss << "UnsignedNormalized" << std::endl;
		break;
	case ITexture::Type::SignedNormalized:
		ss << "SignedNormalized" << std::endl;
		break;
	case ITexture::Type::Float:
		ss << "Float" << std::endl;
		break;
	case ITexture::Type::UnsignedInteger:
		ss << "UnsignedInteger" << std::endl;
		break;
	case ITexture::Type::SignedInteger:
		ss << "SignedInteger" << std::endl;
		break;
	default:
		ss << "Unknown" << std::endl;
		break;
	}

	ss << "RedBits:     " << (int32_t)format.RedBits << std::endl;
	ss << "GreenBits:   " << (int32_t)format.GreenBits << std::endl;
	ss << "BlueBits:    " << (int32_t)format.BlueBits << std::endl;
	ss << "AlphaBits:   " << (int32_t)format.AlphaBits << std::endl;
	ss << "DepthBits:   " << (int32_t)format.DepthBits << std::endl;
	ss << "StencilBits: " << (int32_t)format.StencilBits << std::endl;
	ss << "Num Samples: " << (int32_t)format.NumSamples << std::endl;

	Log::Fatal("%s", ss.str().c_str());
}
#define ReportTextureFormatError( fmt, msg ) ReportAndThrowTextureFormatError( (fmt), __FILE__, __LINE__, __FUNCTION__, (msg) )


static DXGI_FORMAT DX11TranslateFormat(const ITexture::TextureFormat& format)
{
	DXGI_FORMAT result = DXGI_FORMAT_UNKNOWN;

	switch (format.Components)
	{
	case ITexture::Components::R:
		switch (format.Type)
		{
		case ITexture::Type::Typeless:
			if (format.RedBits == 8)
			{
				result = DXGI_FORMAT_R8_TYPELESS;
			}
			else if (format.RedBits == 16)
			{
				result = DXGI_FORMAT_R16_TYPELESS;
			}
			else if (format.RedBits == 32)
			{
				result = DXGI_FORMAT_R32_TYPELESS;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose the best format based on the requested format.
				if (format.RedBits > 16)
				{
					result = DXGI_FORMAT_R32_TYPELESS;
				}
				else if (format.RedBits > 8)
				{
					result = DXGI_FORMAT_R16_TYPELESS;
				}
				else
				{
					result = DXGI_FORMAT_R8_TYPELESS;
				}
			}
			break;
		case ITexture::Type::UnsignedNormalized:
			if (format.RedBits == 1)
			{
				result = DXGI_FORMAT_R1_UNORM;
			}
			else if (format.RedBits == 8)
			{
				result = DXGI_FORMAT_R8_UNORM;
			}
			else if (format.RedBits == 16)
			{
				result = DXGI_FORMAT_R16_UNORM;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose a reasonable alternative to the requested format.
				if (format.RedBits > 8)
				{
					result = DXGI_FORMAT_R16_UNORM;
				}
				else if (format.RedBits > 1)
				{
					result = DXGI_FORMAT_R8_UNORM;
				}
				else
				{
					result = DXGI_FORMAT_R1_UNORM;
				}
			}
			break;
		case ITexture::Type::SignedNormalized:
			if (format.RedBits == 8)
			{
				result = DXGI_FORMAT_R8_SNORM;
			}
			else if (format.RedBits == 16)
			{
				result = DXGI_FORMAT_R16_SNORM;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose a reasonable alternative.
				if (format.RedBits > 8)
				{
					result = DXGI_FORMAT_R16_SNORM;
				}
				else
				{
					result = DXGI_FORMAT_R8_SNORM;
				}
			}
			break;
		case ITexture::Type::Float:
			if (format.RedBits == 16)
			{
				result = DXGI_FORMAT_R16_FLOAT;
			}
			else if (format.RedBits == 32)
			{
				result = DXGI_FORMAT_R32_FLOAT;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose a reasonable alternative.
				if (format.RedBits > 16)
				{
					result = DXGI_FORMAT_R32_FLOAT;
				}
				else
				{
					result = DXGI_FORMAT_R16_FLOAT;
				}
			}
			break;
		case ITexture::Type::UnsignedInteger:
			if (format.RedBits == 8)
			{
				result = DXGI_FORMAT_R8_UINT;
			}
			else if (format.RedBits == 16)
			{
				result = DXGI_FORMAT_R16_UINT;
			}
			else if (format.RedBits == 32)
			{
				result = DXGI_FORMAT_R32_UINT;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose a reasonable alternative.
				if (format.RedBits > 16)
				{
					result = DXGI_FORMAT_R32_UINT;
				}
				else if (format.RedBits > 8)
				{
					result = DXGI_FORMAT_R16_UINT;
				}
				else
				{
					result = DXGI_FORMAT_R8_UINT;
				}
			}
			break;
		case ITexture::Type::SignedInteger:
			if (format.RedBits == 8)
			{
				result = DXGI_FORMAT_R8_SINT;
			}
			else if (format.RedBits == 16)
			{
				result = DXGI_FORMAT_R16_SINT;
			}
			else if (format.RedBits == 32)
			{
				result = DXGI_FORMAT_R32_SINT;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose a reasonable alternative
				if (format.RedBits > 16)
				{
					result = DXGI_FORMAT_R32_SINT;
				}
				else if (format.RedBits > 8)
				{
					result = DXGI_FORMAT_R16_SINT;
				}
				else
				{
					result = DXGI_FORMAT_R8_SINT;
				}
			}
			break;
		default:
			ReportTextureFormatError(format, "Unknown texture format.");
			break;
		}
		break;
	case ITexture::Components::RG:
		switch (format.Type)
		{
		case ITexture::Type::Typeless:
			if (format.RedBits == 8 && format.GreenBits == 8)
			{
				result = DXGI_FORMAT_R8G8_TYPELESS;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16)
			{
				result = DXGI_FORMAT_R16G16_TYPELESS;
			}
			else if (format.RedBits == 24 && format.GreenBits == 8)
			{
				result = DXGI_FORMAT_R24G8_TYPELESS;
			}
			else if (format.RedBits == 32 && format.GreenBits == 32)
			{
				result = DXGI_FORMAT_R32G32_TYPELESS;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose the best format based on the requested format.
				if (format.RedBits > 24 || format.GreenBits > 16)
				{
					result = DXGI_FORMAT_R32G32_TYPELESS;
				}
				else if (format.RedBits > 16 && format.GreenBits <= 8)
				{
					result = DXGI_FORMAT_R24G8_TYPELESS;
				}
				else if (format.RedBits > 8 || format.GreenBits > 8)
				{
					result = DXGI_FORMAT_R16G16_TYPELESS;
				}
				else
				{
					result = DXGI_FORMAT_R8G8_TYPELESS;
				}
			}
			break;
		case ITexture::Type::UnsignedNormalized:
			if (format.RedBits == 8 && format.GreenBits == 8)
			{
				result = DXGI_FORMAT_R8G8_UNORM;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16)
			{
				result = DXGI_FORMAT_R16G16_UNORM;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose a reasonable alternative.
				if (format.RedBits > 8 || format.GreenBits > 8)
				{
					result = DXGI_FORMAT_R16G16_UNORM;
				}
				else
				{
					result = DXGI_FORMAT_R8G8_UNORM;
				}
			}
			break;
		case ITexture::Type::SignedNormalized:
			if (format.RedBits == 8 && format.GreenBits == 8)
			{
				result = DXGI_FORMAT_R8G8_SNORM;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16)
			{
				result = DXGI_FORMAT_R16G16_SNORM;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose a reasonable alternative.
				if (format.RedBits > 8 || format.GreenBits > 8)
				{
					result = DXGI_FORMAT_R16G16_SNORM;
				}
				else
				{
					result = DXGI_FORMAT_R8G8_SNORM;
				}
			}
			break;
		case ITexture::Type::Float:
			if (format.RedBits == 16 && format.GreenBits == 16)
			{
				result = DXGI_FORMAT_R16G16_FLOAT;
			}
			else if (format.RedBits == 32 && format.GreenBits == 32)
			{
				result = DXGI_FORMAT_R32G32_FLOAT;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose a reasonable alternative.
				if (format.RedBits > 16 || format.GreenBits > 16)
				{
					result = DXGI_FORMAT_R32G32_FLOAT;
				}
				else
				{
					result = DXGI_FORMAT_R16G16_FLOAT;
				}
			}
			break;
		case ITexture::Type::UnsignedInteger:
			if (format.RedBits == 8 && format.GreenBits == 8)
			{
				result = DXGI_FORMAT_R8G8_UINT;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16)
			{
				result = DXGI_FORMAT_R16G16_UINT;
			}
			else if (format.RedBits == 32 && format.GreenBits == 32)
			{
				result = DXGI_FORMAT_R32G32_UINT;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose a reasonable alternative.
				if (format.RedBits > 16 || format.GreenBits > 16)
				{
					result = DXGI_FORMAT_R32G32_UINT;
				}
				else if (format.RedBits > 8 || format.GreenBits > 8)
				{
					result = DXGI_FORMAT_R16G16_UINT;
				}
				else
				{
					result = DXGI_FORMAT_R8G8_UINT;
				}
			}
			break;
		case ITexture::Type::SignedInteger:
			if (format.RedBits == 8 && format.GreenBits == 8)
			{
				result = DXGI_FORMAT_R8G8_SINT;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16)
			{
				result = DXGI_FORMAT_R16G16_SINT;
			}
			else if (format.RedBits == 32 && format.GreenBits == 32)
			{
				result = DXGI_FORMAT_R32G32_SINT;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose a reasonable alternative.
				if (format.RedBits > 16 || format.GreenBits > 16)
				{
					result = DXGI_FORMAT_R32G32_SINT;
				}
				else if (format.RedBits > 8 || format.GreenBits > 8)
				{
					result = DXGI_FORMAT_R16G16_SINT;
				}
				else
				{
					result = DXGI_FORMAT_R8G8_SINT;
				}
			}
			break;
		default:
			ReportTextureFormatError(format, "Unknown texture format.");
			break;
		}
		break;
	case ITexture::Components::RGB:
		switch (format.Type)
		{
		case ITexture::Type::Typeless:
			if (format.RedBits == 32 && format.GreenBits == 32 && format.BlueBits == 32)
			{
				result = DXGI_FORMAT_R32G32B32_TYPELESS;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// There is only 1 RGB typeless format
				result = DXGI_FORMAT_R32G32B32_TYPELESS;
			}
			break;
		case ITexture::Type::Float:
			if (format.RedBits == 11 && format.GreenBits == 11 && format.BlueBits == 10)
			{
				result = DXGI_FORMAT_R11G11B10_FLOAT;
			}
			else if (format.RedBits == 32 && format.GreenBits == 32 && format.BlueBits == 32)
			{
				result = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose a reasonable alternative.
				if (format.RedBits > 11 || format.GreenBits > 11 || format.BlueBits > 10)
				{
					result = DXGI_FORMAT_R32G32B32_FLOAT;
				}
				else
				{
					result = DXGI_FORMAT_R11G11B10_FLOAT;
				}
			}
			break;
		case ITexture::Type::UnsignedInteger:
			if (format.RedBits == 32 && format.GreenBits == 32 && format.BlueBits == 32)
			{
				result = DXGI_FORMAT_R32G32B32_UINT;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// There is only 1 3-component UINT format.
				result = DXGI_FORMAT_R32G32B32_UINT;
			}
			break;
		case ITexture::Type::SignedInteger:
			if (format.RedBits == 32 && format.GreenBits == 32 && format.BlueBits == 32)
			{
				result = DXGI_FORMAT_R32G32B32_SINT;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// There is only 1 3-component SINT format.
				result = DXGI_FORMAT_R32G32B32_SINT;
			}
			break;
		default:
			ReportTextureFormatError(format, "Unsupported texture format.");
			// Try to choose a reasonable alternative
			switch (format.Type)
			{
			case ITexture::Type::UnsignedNormalized:
				// This is a non-normalized format. May result in unintended behavior.
				result = DXGI_FORMAT_R32G32B32_UINT;
				break;
			case ITexture::Type::SignedNormalized:
				// Non-normalized format. May result in unintended behavior.
				result = DXGI_FORMAT_R32G32B32_SINT;
				break;
			default:
				ReportTextureFormatError(format, "Unknown texture format.");
				break;
			}
			break;
		}
		break;
	case ITexture::Components::RGBA:
		switch (format.Type)
		{
		case ITexture::Type::Typeless:
			if (format.RedBits == 8 && format.GreenBits == 8 && format.BlueBits == 8 && format.AlphaBits == 8)
			{
				result = DXGI_FORMAT_R8G8B8A8_TYPELESS;
			}
			else if (format.RedBits == 10 && format.GreenBits == 10 && format.BlueBits == 10 && format.AlphaBits == 2)
			{
				result = DXGI_FORMAT_R10G10B10A2_TYPELESS;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16 && format.BlueBits == 16 && format.AlphaBits == 16)
			{
				result = DXGI_FORMAT_R16G16B16A16_TYPELESS;
			}
			else if (format.RedBits == 32 && format.GreenBits == 32 && format.BlueBits == 32 && format.AlphaBits == 32)
			{
				result = DXGI_FORMAT_R32G32B32A32_TYPELESS;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose the best format based on the requested format.
				if (format.RedBits > 16 || format.GreenBits > 16 || format.BlueBits > 16 || format.AlphaBits > 16)
				{
					result = DXGI_FORMAT_R32G32B32A32_TYPELESS;
				}
				else if ((format.RedBits > 10 || format.GreenBits > 10 || format.BlueBits > 10) && format.AlphaBits <= 2)
				{
					result = DXGI_FORMAT_R10G10B10A2_TYPELESS;
				}
				else if (format.RedBits > 8 || format.GreenBits > 8 || format.BlueBits > 8 || format.AlphaBits > 8)
				{
					result = DXGI_FORMAT_R16G16B16A16_TYPELESS;
				}
				else
				{
					result = DXGI_FORMAT_R8G8B8A8_TYPELESS;
				}
			}
			break;
		case ITexture::Type::UnsignedNormalized:
			if (format.RedBits == 8 && format.GreenBits == 8 && format.BlueBits == 8 && format.AlphaBits == 8)
			{
				result = DXGI_FORMAT_R8G8B8A8_UNORM;
			}
			else if (format.RedBits == 10 && format.GreenBits == 10 && format.BlueBits == 10 && format.AlphaBits == 2)
			{
				result = DXGI_FORMAT_R10G10B10A2_UNORM;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16 && format.BlueBits == 16 && format.AlphaBits == 16)
			{
				result = DXGI_FORMAT_R16G16B16A16_UNORM;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose a reasonable alternative.
				if (format.RedBits > 10 || format.GreenBits > 10 || format.BlueBits > 10 || format.AlphaBits > 8)
				{
					result = DXGI_FORMAT_R16G16B16A16_UNORM;
				}
				else if ((format.RedBits > 8 || format.GreenBits > 8 || format.BlueBits > 8) && format.AlphaBits <= 2)
				{
					result = DXGI_FORMAT_R10G10B10A2_UNORM;
				}
				else
				{
					result = DXGI_FORMAT_R8G8B8A8_UNORM;
				}
			}
			break;
		case ITexture::Type::SignedNormalized:
			if (format.RedBits == 8 && format.GreenBits == 8 && format.BlueBits == 8 && format.AlphaBits == 8)
			{
				result = DXGI_FORMAT_R8G8B8A8_SNORM;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16 && format.BlueBits == 16 && format.AlphaBits == 16)
			{
				result = DXGI_FORMAT_R16G16B16A16_SNORM;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose a reasonable alternative.
				if (format.RedBits > 8 || format.GreenBits > 8 || format.BlueBits > 8 || format.AlphaBits > 8)
				{
					result = DXGI_FORMAT_R16G16B16A16_SNORM;
				}
				else
				{
					result = DXGI_FORMAT_R8G8B8A8_SNORM;
				}
			}
			break;
		case ITexture::Type::Float:
			if (format.RedBits == 32 && format.GreenBits == 32 && format.BlueBits && format.AlphaBits == 32)
			{
				result = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16 && format.BlueBits == 16 && format.AlphaBits == 16)
			{
				result = DXGI_FORMAT_R16G16B16A16_FLOAT;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose a reasonable alternative.
				if (format.RedBits > 16 || format.GreenBits > 16 || format.BlueBits > 16 || format.AlphaBits > 16)
				{
					result = DXGI_FORMAT_R32G32B32A32_FLOAT;
				}
				else
				{
					result = DXGI_FORMAT_R16G16B16A16_FLOAT;
				}
			}
			break;
		case ITexture::Type::UnsignedInteger:
			if (format.RedBits == 8 && format.GreenBits == 8 && format.BlueBits == 8 && format.AlphaBits == 8)
			{
				result = DXGI_FORMAT_R8G8B8A8_UINT;
			}
			else if (format.RedBits == 10 && format.GreenBits == 10 && format.BlueBits == 10 && format.AlphaBits == 2)
			{
				result = DXGI_FORMAT_R10G10B10A2_UINT;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16 && format.BlueBits == 16 && format.AlphaBits == 16)
			{
				result = DXGI_FORMAT_R16G16B16A16_UINT;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose a reasonable alternative.
				if (format.RedBits > 10 || format.GreenBits > 10 || format.BlueBits > 10 || format.AlphaBits > 10)
				{
					result = DXGI_FORMAT_R16G16B16A16_UINT;
				}
				else if ((format.RedBits > 8 || format.GreenBits > 8 || format.BlueBits > 8) && format.AlphaBits <= 2)
				{
					result = DXGI_FORMAT_R10G10B10A2_UINT;
				}
				else
				{
					result = DXGI_FORMAT_R8G8B8A8_UINT;
				}
			}
			break;
		case ITexture::Type::SignedInteger:
			if (format.RedBits == 8 && format.GreenBits == 8 && format.BlueBits == 8 && format.AlphaBits == 8)
			{
				result = DXGI_FORMAT_R8G8B8A8_SINT;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16 && format.BlueBits == 16 && format.AlphaBits == 16)
			{
				result = DXGI_FORMAT_R16G16B16A16_SINT;
			}
			else if (format.RedBits == 32 && format.GreenBits == 32 && format.BlueBits == 32 && format.AlphaBits == 32)
			{
				result = DXGI_FORMAT_R32G32B32A32_SINT;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Try to choose a reasonable alternative.
				if (format.RedBits > 16 || format.GreenBits > 16 || format.BlueBits > 16 || format.AlphaBits > 16)
				{
					result = DXGI_FORMAT_R32G32B32A32_SINT;
				}
				if (format.RedBits > 8 || format.GreenBits > 8 || format.BlueBits > 8 || format.AlphaBits > 8)
				{
					result = DXGI_FORMAT_R16G16B16A16_SINT;
				}
				else
				{
					result = DXGI_FORMAT_R8G8B8A8_SINT;
				}
			}
			break;
		default:
			ReportTextureFormatError(format, "Unknown texture format.");
			break;
		}
		break;
	case ITexture::Components::Depth:
		switch (format.Type)
		{
		case ITexture::Type::UnsignedNormalized:
			if (format.DepthBits == 16)
			{
				result = DXGI_FORMAT_D16_UNORM;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Only 1 format that could match.
				result = DXGI_FORMAT_D16_UNORM;
			}
			break;
		case ITexture::Type::Float:
			if (format.DepthBits == 32)
			{
				result = DXGI_FORMAT_D32_FLOAT;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
				// Only 1 format that could match.
				result = DXGI_FORMAT_D32_FLOAT;
			}
			break;
		default:
			// There are no SNORM, SINT, or UINT depth-only formats.
			ReportTextureFormatError(format, "Unknown texture format.");
			break;
		}
		break;
	case ITexture::Components::DepthStencil:
		// For Depth/Stencil formats, we'll ignore the type and try to deduce the format based on the bit-depth values.
		if (format.DepthBits == 24 && format.StencilBits == 8)
		{
			result = DXGI_FORMAT_D24_UNORM_S8_UINT;
		}
		else if (format.DepthBits == 32 && format.StencilBits == 8)
		{
			result = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		}
		else
		{
			ReportTextureFormatError(format, "Unsupported texture format.");
			// Try to choose a reasonable alternative...
			if (format.DepthBits > 24)
			{
				result = result = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
			}
			else
			{
				result = DXGI_FORMAT_D24_UNORM_S8_UINT;
			}
		}
		break;
	default:
		ReportTextureFormatError(format, "Unknown texture format.");
		break;
	}

	return result;
}

static ITexture::TextureFormat DX11TranslateFormat(DXGI_FORMAT format, uint8_t numSamples)
{
	ITexture::TextureFormat result;
	result.NumSamples = numSamples;

	switch (format)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::Typeless;
		result.RedBits = 32;
		result.GreenBits = 32;
		result.BlueBits = 32;
		result.AlphaBits = 32;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::Float;
		result.RedBits = 32;
		result.GreenBits = 32;
		result.BlueBits = 32;
		result.AlphaBits = 32;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R32G32B32A32_UINT:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::UnsignedInteger;
		result.RedBits = 32;
		result.GreenBits = 32;
		result.BlueBits = 32;
		result.AlphaBits = 32;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R32G32B32A32_SINT:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::SignedInteger;
		result.RedBits = 32;
		result.GreenBits = 32;
		result.BlueBits = 32;
		result.AlphaBits = 32;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R32G32B32_TYPELESS:
		result.Components = ITexture::Components::RGB;
		result.Type = ITexture::Type::Typeless;
		result.RedBits = 32;
		result.GreenBits = 32;
		result.BlueBits = 32;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R32G32B32_FLOAT:
		result.Components = ITexture::Components::RGB;
		result.Type = ITexture::Type::Float;
		result.RedBits = 32;
		result.GreenBits = 32;
		result.BlueBits = 32;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R32G32B32_UINT:
		result.Components = ITexture::Components::RGB;
		result.Type = ITexture::Type::UnsignedInteger;
		result.RedBits = 32;
		result.GreenBits = 32;
		result.BlueBits = 32;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R32G32B32_SINT:
		result.Components = ITexture::Components::RGB;
		result.Type = ITexture::Type::SignedInteger;
		result.RedBits = 32;
		result.GreenBits = 32;
		result.BlueBits = 32;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::Typeless;
		result.RedBits = 16;
		result.GreenBits = 16;
		result.BlueBits = 16;
		result.AlphaBits = 16;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::Float;
		result.RedBits = 16;
		result.GreenBits = 16;
		result.BlueBits = 16;
		result.AlphaBits = 16;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R16G16B16A16_UINT:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::UnsignedInteger;
		result.RedBits = 16;
		result.GreenBits = 16;
		result.BlueBits = 16;
		result.AlphaBits = 16;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R16G16B16A16_SNORM:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::SignedNormalized;
		result.RedBits = 16;
		result.GreenBits = 16;
		result.BlueBits = 16;
		result.AlphaBits = 16;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R16G16B16A16_SINT:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::SignedInteger;
		result.RedBits = 16;
		result.GreenBits = 16;
		result.BlueBits = 16;
		result.AlphaBits = 16;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R32G32_TYPELESS:
		result.Components = ITexture::Components::RG;
		result.Type = ITexture::Type::Typeless;
		result.RedBits = 32;
		result.GreenBits = 32;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R32G32_FLOAT:
		result.Components = ITexture::Components::RG;
		result.Type = ITexture::Type::Float;
		result.RedBits = 32;
		result.GreenBits = 32;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R32G32_UINT:
		result.Components = ITexture::Components::RG;
		result.Type = ITexture::Type::UnsignedInteger;
		result.RedBits = 32;
		result.GreenBits = 32;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R32G32_SINT:
		result.Components = ITexture::Components::RG;
		result.Type = ITexture::Type::SignedInteger;
		result.RedBits = 32;
		result.GreenBits = 32;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		result.Components = ITexture::Components::DepthStencil;
		result.Type = ITexture::Type::Float;
		result.RedBits = 0;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 32;
		result.StencilBits = 8;
		break;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::Typeless;
		result.RedBits = 10;
		result.GreenBits = 10;
		result.BlueBits = 10;
		result.AlphaBits = 2;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R10G10B10A2_UNORM:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::UnsignedNormalized;
		result.RedBits = 10;
		result.GreenBits = 10;
		result.BlueBits = 10;
		result.AlphaBits = 2;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R10G10B10A2_UINT:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::UnsignedInteger;
		result.RedBits = 10;
		result.GreenBits = 10;
		result.BlueBits = 10;
		result.AlphaBits = 2;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R11G11B10_FLOAT:
		result.Components = ITexture::Components::RGB;
		result.Type = ITexture::Type::Float;
		result.RedBits = 11;
		result.GreenBits = 11;
		result.BlueBits = 10;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::Typeless;
		result.RedBits = 8;
		result.GreenBits = 8;
		result.BlueBits = 8;
		result.AlphaBits = 8;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::UnsignedNormalized;
		result.RedBits = 8;
		result.GreenBits = 8;
		result.BlueBits = 8;
		result.AlphaBits = 8;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R8G8B8A8_UINT:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::UnsignedInteger;
		result.RedBits = 8;
		result.GreenBits = 8;
		result.BlueBits = 8;
		result.AlphaBits = 8;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R8G8B8A8_SNORM:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::SignedNormalized;
		result.RedBits = 8;
		result.GreenBits = 8;
		result.BlueBits = 8;
		result.AlphaBits = 8;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R8G8B8A8_SINT:
		result.Components = ITexture::Components::RGBA;
		result.Type = ITexture::Type::SignedInteger;
		result.RedBits = 8;
		result.GreenBits = 8;
		result.BlueBits = 8;
		result.AlphaBits = 8;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R16G16_TYPELESS:
		result.Components = ITexture::Components::RG;
		result.Type = ITexture::Type::Typeless;
		result.RedBits = 16;
		result.GreenBits = 16;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R16G16_FLOAT:
		result.Components = ITexture::Components::RG;
		result.Type = ITexture::Type::Float;
		result.RedBits = 16;
		result.GreenBits = 16;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R16G16_UNORM:
		result.Components = ITexture::Components::RG;
		result.Type = ITexture::Type::UnsignedNormalized;
		result.RedBits = 16;
		result.GreenBits = 16;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R16G16_UINT:
		result.Components = ITexture::Components::RG;
		result.Type = ITexture::Type::UnsignedInteger;
		result.RedBits = 16;
		result.GreenBits = 16;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R16G16_SINT:
		result.Components = ITexture::Components::RG;
		result.Type = ITexture::Type::SignedInteger;
		result.RedBits = 16;
		result.GreenBits = 16;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R32_TYPELESS:
		result.Components = ITexture::Components::R;
		result.Type = ITexture::Type::Typeless;
		result.RedBits = 32;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_D32_FLOAT:
		result.Components = ITexture::Components::Depth;
		result.Type = ITexture::Type::Float;
		result.RedBits = 0;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 32;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R32_FLOAT:
		result.Components = ITexture::Components::R;
		result.Type = ITexture::Type::Float;
		result.RedBits = 32;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R32_UINT:
		result.Components = ITexture::Components::R;
		result.Type = ITexture::Type::UnsignedInteger;
		result.RedBits = 32;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R32_SINT:
		result.Components = ITexture::Components::R;
		result.Type = ITexture::Type::SignedInteger;
		result.RedBits = 32;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R24G8_TYPELESS:
		result.Components = ITexture::Components::RG;
		result.Type = ITexture::Type::Typeless;
		result.RedBits = 24;
		result.GreenBits = 8;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		result.Components = ITexture::Components::DepthStencil;
		result.Type = ITexture::Type::UnsignedNormalized;
		result.RedBits = 0;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 24;
		result.StencilBits = 8;
		break;
	case DXGI_FORMAT_R8G8_TYPELESS:
		result.Components = ITexture::Components::RG;
		result.Type = ITexture::Type::Typeless;
		result.RedBits = 8;
		result.GreenBits = 8;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R8G8_UNORM:
		result.Components = ITexture::Components::RG;
		result.Type = ITexture::Type::UnsignedNormalized;
		result.RedBits = 8;
		result.GreenBits = 8;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R8G8_UINT:
		result.Components = ITexture::Components::RG;
		result.Type = ITexture::Type::UnsignedInteger;
		result.RedBits = 8;
		result.GreenBits = 8;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R8G8_SNORM:
		result.Components = ITexture::Components::RG;
		result.Type = ITexture::Type::SignedNormalized;
		result.RedBits = 8;
		result.GreenBits = 8;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R8G8_SINT:
		result.Components = ITexture::Components::RG;
		result.Type = ITexture::Type::SignedInteger;
		result.RedBits = 8;
		result.GreenBits = 8;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R16_TYPELESS:
		result.Components = ITexture::Components::R;
		result.Type = ITexture::Type::Typeless;
		result.RedBits = 16;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R16_FLOAT:
		result.Components = ITexture::Components::R;
		result.Type = ITexture::Type::Float;
		result.RedBits = 16;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_D16_UNORM:
		result.Components = ITexture::Components::Depth;
		result.Type = ITexture::Type::UnsignedNormalized;
		result.RedBits = 0;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 16;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R16_UNORM:
		result.Components = ITexture::Components::R;
		result.Type = ITexture::Type::UnsignedNormalized;
		result.RedBits = 16;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R16_UINT:
		result.Components = ITexture::Components::R;
		result.Type = ITexture::Type::UnsignedInteger;
		result.RedBits = 16;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R16_SNORM:
		result.Components = ITexture::Components::R;
		result.Type = ITexture::Type::SignedNormalized;
		result.RedBits = 16;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R16_SINT:
		result.Components = ITexture::Components::R;
		result.Type = ITexture::Type::SignedInteger;
		result.RedBits = 16;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R8_TYPELESS:
		result.Components = ITexture::Components::R;
		result.Type = ITexture::Type::Typeless;
		result.RedBits = 8;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R8_UNORM:
		result.Components = ITexture::Components::R;
		result.Type = ITexture::Type::UnsignedNormalized;
		result.RedBits = 8;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R8_UINT:
		result.Components = ITexture::Components::R;
		result.Type = ITexture::Type::UnsignedInteger;
		result.RedBits = 8;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R8_SNORM:
		result.Components = ITexture::Components::R;
		result.Type = ITexture::Type::SignedNormalized;
		result.RedBits = 8;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R8_SINT:
		result.Components = ITexture::Components::R;
		result.Type = ITexture::Type::SignedInteger;
		result.RedBits = 8;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	case DXGI_FORMAT_R1_UNORM:
		result.Components = ITexture::Components::R;
		result.Type = ITexture::Type::UnsignedNormalized;
		result.RedBits = 1;
		result.GreenBits = 0;
		result.BlueBits = 0;
		result.AlphaBits = 0;
		result.DepthBits = 0;
		result.StencilBits = 0;
		break;
	default:
		throw CznRenderException("Unsupported DXGI format.");
		break;
	}

	return result;
}

//--

static DXGI_FORMAT DX11GetTextureFormat(DXGI_FORMAT format)
{
	DXGI_FORMAT result = format;

	switch (format)
	{
	case DXGI_FORMAT_D16_UNORM:
		result = DXGI_FORMAT_R16_TYPELESS;
		break;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		result = DXGI_FORMAT_R24G8_TYPELESS;
		break;
	case DXGI_FORMAT_D32_FLOAT:
		result = DXGI_FORMAT_R32_TYPELESS;
		break;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		result = DXGI_FORMAT_R32G8X24_TYPELESS;
		break;
	}

	return result;
}

static DXGI_FORMAT DX11GetDSVFormat(DXGI_FORMAT format)
{
	DXGI_FORMAT result = format;

	switch (format)
	{
	case DXGI_FORMAT_R16_TYPELESS:
		result = DXGI_FORMAT_D16_UNORM;
		break;
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		result = DXGI_FORMAT_D24_UNORM_S8_UINT;
		break;
	case DXGI_FORMAT_R32_TYPELESS:
		result = DXGI_FORMAT_D32_FLOAT;
		break;
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		result = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		break;
	}

	return result;
}

static DXGI_FORMAT DX11GetSRVFormat(DXGI_FORMAT format)
{
	DXGI_FORMAT result = format;
	switch (format)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
		result = DXGI_FORMAT_R32G32B32A32_FLOAT;
		break;
	case DXGI_FORMAT_R32G32B32_TYPELESS:
		result = DXGI_FORMAT_R32G32B32_FLOAT;
		break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
		result = DXGI_FORMAT_R16G16B16A16_UNORM;
		break;
	case DXGI_FORMAT_R32G32_TYPELESS:
		result = DXGI_FORMAT_R32G32_FLOAT;
		break;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
		result = DXGI_FORMAT_R10G10B10A2_UNORM;
		break;
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		result = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case DXGI_FORMAT_R16G16_TYPELESS:
		result = DXGI_FORMAT_R16G16_UNORM;
		break;
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_D16_UNORM:
		result = DXGI_FORMAT_R16_UNORM;
		break;
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		result = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		break;
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
		result = DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		result = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		break;
	case DXGI_FORMAT_R8G8_TYPELESS:
		result = DXGI_FORMAT_R8G8_UNORM;
		break;
	case DXGI_FORMAT_R8_TYPELESS:
		result = DXGI_FORMAT_R8_UNORM;
		break;
	}

	return result;
}

static DXGI_FORMAT DX11GetRTVFormat(DXGI_FORMAT format)
{
	DXGI_FORMAT result = format;

	switch (format)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
		result = DXGI_FORMAT_R32G32B32A32_FLOAT;
		break;
	case DXGI_FORMAT_R32G32B32_TYPELESS:
		result = DXGI_FORMAT_R32G32B32_FLOAT;
		break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
		result = DXGI_FORMAT_R16G16B16A16_UNORM;
		break;
	case DXGI_FORMAT_R32G32_TYPELESS:
		result = DXGI_FORMAT_R32G32_FLOAT;
		break;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
		result = DXGI_FORMAT_R10G10B10A2_UNORM;
		break;
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		result = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case DXGI_FORMAT_R16G16_TYPELESS:
		result = DXGI_FORMAT_R16G16_UNORM;
		break;
	case DXGI_FORMAT_R8G8_TYPELESS:
		result = DXGI_FORMAT_R8G8_UNORM;
		break;
	case DXGI_FORMAT_R32_TYPELESS:
		result = DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT_R8_TYPELESS:
		result = DXGI_FORMAT_R8_UNORM;
		break;
	}

	return result;
}

static DXGI_FORMAT DX11GetUAVFormat(DXGI_FORMAT format)
{
	return DX11GetRTVFormat(format);
}

static uint8_t DX11GetBPP(DXGI_FORMAT format)
{
	uint8_t bpp = 0;

	switch (format)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		bpp = 128;
		break;
	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		bpp = 96;
		break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
		bpp = 64;
		break;
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
		bpp = 64;
		break;
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		bpp = 64;
		break;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
		bpp = 32;
		break;
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
		bpp = 32;
		break;
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
		bpp = 32;
		break;
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
		bpp = 32;
		break;
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		bpp = 32;
		break;
	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
		bpp = 16;
		break;
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
		bpp = 16;
		break;
	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
		bpp = 8;
		break;
	case DXGI_FORMAT_R1_UNORM:
		bpp = 1;
		break;
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		bpp = 32;
		break;
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		bpp = 32;
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		bpp = 16;
		break;
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		bpp = 16;
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
		bpp = 32;
		break;
	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
		bpp = 32;
		break;
	case DXGI_FORMAT_B8G8R8A8_TYPELESS:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8X8_TYPELESS:
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
		bpp = 32;
		break;
	default:
		throw CznRenderException("Unsupported texture format.");
		break;
	}

	return bpp;
}
