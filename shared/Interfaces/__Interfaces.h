#pragma once

//---------------------------------------------------------//
//--                       BASIC                         --//
//---------------------------------------------------------//

#ifdef OW_ENGINE_EXPORTS
#define ZN_API __declspec(dllexport) 
#else
#define ZN_API __declspec(dllimport) 
#endif

#define ZN_INTERFACE struct

// General types
typedef signed char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

// GLM
#pragma warning( push )
#pragma warning( disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
#define GLM_FORCE_SWIZZLE
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/constants.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/quaternion.hpp>
#include <glm/glm/gtc/random.hpp>
#include <glm/glm/gtx/vector_angle.hpp>
#include <glm/glm/gtx/compatibility.hpp>
#include <glm/glm/gtx/matrix_operation.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtx/quaternion.hpp>
#include <glm/glm/gtx/euler_angles.hpp>
#include <glm/glm/gtx/norm.hpp>
#pragma warning( pop ) 

typedef glm::vec2 vec2;
typedef const vec2& cvec2;
typedef glm::vec3 vec3;
typedef const vec3& cvec3;
typedef glm::vec4 vec4;
typedef const vec4& cvec4;
typedef glm::mat4x4 mat4;
typedef const mat4& cmat4;
typedef glm::quat quat;
typedef const quat& cquat;


#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN 1
#endif

#ifndef NOMINMAX
#   define NOMINMAX
#endif

// STL Types
#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <mutex>
#include <algorithm>
#include <codecvt>
#include <functional>
#include <fstream>
#include <cmath>
#include <memory>
#include <atomic>
#include <future>
#include <chrono>

// Usefull macros
#define __PACK_BEGIN  "../shared/pack_begin.h"
#define __PACK_END  "../shared/pack_end.h"

//---------------------------------------------------------//
//--                   Base Types                        --//
//---------------------------------------------------------//
class BoundingBox;
typedef const BoundingBox& cbbox;
class BoundingRect;
typedef const BoundingRect& cbrect;
class BoundingSphere;
class Frustum;
class Plane;
class Rect;

#include "Interfaces/Types/KeyCodes.h"
#include "Interfaces/Types/Object.h"
#include "Interfaces/Types/Events.h"


//---------------------------------------------------------//
//--                Base Interfaces                      --//
//---------------------------------------------------------//
#include "Interfaces/BaseManager.h"

#include "Interfaces/Application.h"
#include "Interfaces/CameraController.h"
#include "Interfaces/Console.h"
#include "Interfaces/DebugOutput.h"
#include "Interfaces/Files.h"
#include "Interfaces/GameState.h"
#include "Interfaces/GameStateManager.h"
#include "Interfaces/Loader.h"
#include "Interfaces/Log.h"
#include "Interfaces/Networking.h"
#include "Interfaces/Plugins.h"
#include "Interfaces/Settings.h"
#include "Interfaces/Visitor.h"
#include "Interfaces/WindowObject.h"


//---------------------------------------------------------//
//--                   Render Types                      --//
//---------------------------------------------------------//
#include "Interfaces/Render/Types/BufferBinding.h"
#include "Interfaces/Render/Types/ClearFlags.h"
#include "Interfaces/Render/Types/CPUAccess.h"
#include "Interfaces/Render/Types/InputSemantic.h"
#include "Interfaces/Render/Types/PrimitiveTopology.h"
#include "Interfaces/Render/Types/Viewport.h"
#include "Interfaces/Render/Types/CustomVertexElement.h"
#include "Interfaces/Render/Types/Ray.h"
#include "Interfaces/Render/Types/RaycastHit.h"

//---------------------------------------------------------//
//--                Render Interfaces                    --//
//---------------------------------------------------------//
#include "Interfaces/Render/IRender.h"

#include "Interfaces/Render/IShader.h"
#include "Interfaces/Render/IShaderParameter.h"
#include "Interfaces/Render/IShaderInputLayout.h"
#include "Interfaces/Render/ITexture.h"
#include "Interfaces/Render/ISamplerState.h"
#include "Interfaces/Render/IBuffer.h"
#include "Interfaces/Render/IQuery.h"
#include "Interfaces/Render/IMaterial.h"
#include "Interfaces/Render/IMesh.h"
#include "Interfaces/Render/IRenderTarget.h"
#include "Interfaces/Render/IRenderWindow.h"
#include "Interfaces/Render/IBlendState.h"
#include "Interfaces/Render/IDepthStencilState.h"
#include "Interfaces/Render/IRasterizerState.h"
#include "Interfaces/Render/IPipelineState.h"
#include "Interfaces/Render/IRenderDevice.h"
#include "Interfaces/Render/ICamera.h"
#include "Interfaces/Render/IRenderPass.h"



//---------------------------------------------------------//
//--                    Scene Types                      --//
//---------------------------------------------------------//


//---------------------------------------------------------//
//--                 Scene Interfaces                    --//
//---------------------------------------------------------//
#include "Interfaces/Scene/IActions.h"
#include "Interfaces/Scene/IProperties.h"
#include "Interfaces/Scene/ISceneNodeComponents.h"
#include "Interfaces/Scene/ISceneNode.h"
#include "Interfaces/Scene/IScene.h"
#include "Interfaces/Scene/ISceneNodeProvider.h"