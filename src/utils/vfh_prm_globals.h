//
// Copyright (c) 2015, Chaos Software Ltd
//
// V-Ray For Houdini
//
// ACCESSIBLE SOURCE CODE WITHOUT DISTRIBUTION OF MODIFICATION LICENSE
//
// Full license text: https://github.com/ChaosGroup/vray-for-houdini/blob/master/LICENSE
//

#ifndef VRAY_FOR_HOUDINI_PRM_GLOBALS_H
#define VRAY_FOR_HOUDINI_PRM_GLOBALS_H

#include "vfh_typedefs.h"

#include <PRM/PRM_Name.h>


namespace VRayForHoudini {
namespace Parm {

extern StringSet RenderSettingsPlugins;
extern StringSet RenderGIPlugins;

extern PRM_Name  parm_render_net_render_channels;
extern PRM_Name  parm_render_net_environment;

} // namespace Parm
} // namespace VRayForHoudini

#endif // VRAY_FOR_HOUDINI_PRM_GLOBALS_H
