#include <Script/GlobalState.hpp>
#include <System/Loaders.hpp>
#include <System/Path.hpp>

namespace obe::Script
{
    kaguya::State ScriptEngine;
    void InitScriptEngine()
    {
        System::Path("Lib/Internal/LuaCore.lua")
            .load(System::Loaders::luaLoader, ScriptEngine);
        System::Path("Lib/Internal/Environment.lua")
            .load(System::Loaders::luaLoader, ScriptEngine);
        System::Path("Lib/Internal/ScriptInit.lua")
            .load(System::Loaders::luaLoader, ScriptEngine);
        System::Path("Lib/Internal/Triggers.lua")
            .load(System::Loaders::luaLoader, ScriptEngine);
        ScriptEngine["Hook"] = kaguya::NewTable();
        ScriptEngine.dofile("Lib/Internal/Canvas.lua");
        ScriptEngine.setErrorHandler([](int statusCode, const char* message) {
            Debug::Log->error("<LuaError>({0}) : {1}", statusCode, message);
        });
    }

    unsigned int CreateNewEnvironment()
    {
        return ScriptEngine["LuaCore"]["CreateNewEnv"]();
    }

    void executeFile(unsigned int envIndex, const std::string& file)
    {
        ScriptEngine["LuaCore"]["ExecuteFileOnEnv"](System::Path(file).find(), envIndex);
    }

    void executeString(unsigned int envIndex, const std::string& string)
    {
        ScriptEngine["LuaCore"]["ExecuteStringOnEnv"](string, envIndex);
    }
} // namespace obe::Script
