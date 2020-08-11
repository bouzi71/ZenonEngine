#pragma once

#define ONLY_SIMPLE_UI
#define ZN_FBX_SDK_ENABLE

#include "FBX/FBXInterfaces.h"

const ObjectClass cSceneDefault = UINT16_MAX - 301u;

ZN_INTERFACE ZN_API IMaterialModelPass
{
	virtual ~IMaterialModelPass() {}

	virtual IShaderParameter* GetLightsShaderParameter() const = 0;
};



struct ZN_API LightColors
{
	enum List : uint8
	{
		LIGHT_COLOR_GLOBAL_DIFFUSE = 0,
		LIGHT_COLOR_GLOBAL_AMBIENT,

		LIGHT_COLOR_SKY_0,
		LIGHT_COLOR_SKY_1,
		LIGHT_COLOR_SKY_2,
		LIGHT_COLOR_SKY_3,
		LIGHT_COLOR_SKY_4,

		LIGHT_COLOR_FOG,
		LIGHT_COLOR_UNK0,
		LIGHT_COLOR_SUN,
		LIGHT_COLOR_SUN_HALO,
		LIGHT_COLOR_UNK1,
		LIGHT_COLOR_CLOUD,
		LIGHT_COLOR_UNK2,

		LIGHT_COLOR_OCEAN_LIGHT,
		LIGHT_COLOR_OCEAN_DARK,

		LIGHT_COLOR_RIVER_LIGHT,
		LIGHT_COLOR_RIVER_DARK,

		COUNT
	};
};

struct ZN_API LightFogs
{
	enum List : uint8
	{
		LIGHT_FOG_DISTANCE = 0,    // Fog distance multiplied by 36 - distance at which everything will be hidden by the fog
		LIGHT_FOG_MULTIPLIER,      // Fog multiplier - fog distance * fog multiplier = fog start distance. 0 - 0, 999...
		LIGHT_FOG_CELESTIAL_GLOW,  // Celestial Glow Through - the brightness of the sun and moon as it shows through cloud cover.Note that this effect only appears when the Sun or Moon is being obscured by clouds. 0 - 1
		LIGHT_FOG_CLOUD_DESINTY,   // Cloud Density - Controls the density of cloud cover in the area.Value range is 0.0 to 1.0.
		LIGHT_FOG_UNK0,
		LIGHT_FOG_UNK1,

		COUNT
	};
};

ZN_INTERFACE __declspec(uuid("8D86DF19-0FB3-47F3-B0BC-215788EA5500")) ZN_API ISkyManager
	: public IManager
{
	virtual ~ISkyManager() = 0 {};

	virtual void Calculate(const ICameraComponent3D* camera, uint32 _time) = 0;
	virtual bool HasSkies() const = 0;

	virtual glm::vec3 GetColor(LightColors::List _color) const = 0;
	virtual float GetFog(LightFogs::List _fog) const = 0;
	virtual float GetGlow() const = 0;
	virtual float GetWaterShallowAlpha() const = 0;
	virtual float GetWaterDarkAlpha() const = 0;
	virtual float GetOceanShallowAlpha() const = 0;
	virtual float GetOceanDarkAlpha() const = 0;
};