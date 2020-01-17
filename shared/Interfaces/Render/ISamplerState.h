#pragma once

// FORWARD BEGIN
ZN_INTERFACE IShader;
ZN_INTERFACE IShaderParameterSource;
// FORWARD END

ZN_INTERFACE ZN_API ISamplerState
	: public IShaderParameterSource
{
	enum class ZN_API MinFilter
	{
		MinNearest,                // The nearest texel to the sampled texel.
		MinLinear,                 // Linear average of the 4 closest texels.
	};

	enum class ZN_API MagFilter
	{
		MagNearest,                // The nearest texel to the sampled texel.
		MagLinear,                 // Weighted average of the closest texels.
	};

	enum class ZN_API MipFilter
	{
		MipNearest,                // Choose the nearest mip level.
		MipLinear,                 // Linear interpolate between the 2 nearest mip map levels.
	};

	enum class ZN_API WrapMode
	{
		Repeat,                 // Texture is repeated when texture coordinates are out of range.
		Mirror,                 // Texture is mirrored when texture coordinates are out of range.
		Clamp,                  // Texture coordinate is clamped to [0, 1] 
		Border,                 // Texture border color is used when texture coordinates are out of range.
	};

	enum class ZN_API CompareMode
	{
		None,                   // Don't perform any comparison
		CompareRefToTexture,    // Compare the reference value (usually the currently bound depth buffer) to the value in the texture.
	};

	enum class ZN_API CompareFunc
	{
		Never,                  // Never pass the comparison function.
		Less,                   // Pass if the source data is less than the destination data.
		Equal,                  // Pass if the source data is equal to the destination data.
		LessEqual,              // Pass if the source data is less than or equal to the destination data.
		Greater,                // Pass if the source data is greater than the destination data.
		NotEqual,               // Pass if the source data is not equal to the destination data.
		GreaterEqual,           // Pass if the source data is greater than or equal to the destination data.
		Always,                 // Always pass the comparison function.
	};

	virtual ~ISamplerState() {}

	virtual void SetFilter(MinFilter minFilter, MagFilter magFilter, MipFilter mipFilter) = 0;
	virtual void GetFilter(MinFilter& minFilter, MagFilter& magFilter, MipFilter& mipFilter) const = 0;

	virtual void SetWrapMode(WrapMode u = WrapMode::Repeat, WrapMode v = WrapMode::Repeat, WrapMode w = WrapMode::Repeat) = 0;
	virtual void GetWrapMode(WrapMode& u, WrapMode& v, WrapMode& w) const = 0;

	virtual void SetCompareFunction(CompareFunc compareFunc) = 0;
	virtual CompareFunc GetCompareFunc() const = 0;

	/**
	 * Set the offset from the calculated mipmap level.  For example, if mipmap texture 1 should be sampled and
	 * LOD bias is set to 2, then the texture will be sampled at mipmap level 3.
	 */
	virtual void SetLODBias(float lodBias) = 0;
	virtual float GetLODBias() const = 0;

	/**
	 * Set the minimum LOD level that will be sampled.  The highest resolution mip map is level 0.
	 */
	virtual void SetMinLOD(float minLOD) = 0;
	virtual float GetMinLOD() const = 0;

	/**
	 * Set the maximum LOD level that will be sampled. The LOD level increases as the resolution of the mip-map decreases.
	 */
	virtual void SetMaxLOD(float maxLOD) = 0;
	virtual float GetMaxLOD() const = 0;

	/**
	 * Sets the border color to use if the wrap mode is set to Border.
	 */
	virtual void SetBorderColor(cvec4 borderColor) = 0;
	virtual vec4 GetBorderColor() const = 0;

	/**
	 * Enable Anisotropic filtering (where supported).
	 */
	virtual void EnableAnisotropicFiltering(bool enabled) = 0;
	virtual bool IsAnisotropicFilteringEnabled() const = 0;

	/**
	 * When Anisotropic filtering is enabled, use this value to determine the maximum level
	 * of anisotropic filtering to apply.  Valid values are in the range [1, 16].
	 * May not be supported on all platforms.
	 */
	virtual void SetMaxAnisotropy(uint8_t maxAnisotropy) = 0;
	virtual uint8_t GetMaxAnisotropy() const = 0;

	virtual void Bind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const = 0;
	virtual void UnBind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const = 0;
};

typedef std::vector< std::shared_ptr<ISamplerState> > SamplerList;
typedef std::unordered_map<uint8, std::shared_ptr<ISamplerState>> SamplersMap;