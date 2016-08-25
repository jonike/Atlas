#include "Engine.h"
#include "LogManager.h"
#include "PluginManager.h"
#include "ResourceManager.h"
#include "ProfilingManager.h"
#include "ECSManager.h"
#include "Resources/Geometry.h"

namespace atlas {

LogManager          *Engine::_logManager = 0x0;
ProfilingManager    *Engine::_profilingManager = 0x0;
PluginManager       *Engine::_pluginManager = 0x0;
ResourceManager     *Engine::_resourceManager = 0x0;
ECSManager          *Engine::_ecsManager = 0x0;



bool Engine::init()
{
    if (_logManager == 0x0) _logManager = new LogManager();
    if (_profilingManager == 0x0) _profilingManager = new ProfilingManager();
    if (_pluginManager == 0x0) _pluginManager = new PluginManager();
    if (_resourceManager == 0x0) _resourceManager = new ResourceManager();
    if (_ecsManager == 0x0) _ecsManager = new ECSManager();


    resMan().registerResType( (int)ResourceTypes::Geometry, "Geometry", GeometryResource::initializationFunc,
        GeometryResource::releaseFunc, GeometryResource::factoryFunc );

    return true;
}

} // namespace Atlas


