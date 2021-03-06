#pragma once

#include <Audio/AudioManager.hpp>
#include <Config/Config.hpp>
#include <Engine/ResourceManager.hpp>
#include <Input/InputManager.hpp>
#include <Scene/Scene.hpp>
#include <System/Cursor.hpp>
#include <System/Plugin.hpp>
#include <System/Window.hpp>
#include <Time/FramerateManager.hpp>
#include <Triggers/TriggerManager.hpp>
#include <sol/sol.hpp>

namespace obe::Bindings
{
    void IndexAllBindings(sol::state_view state);
}

namespace obe::Engine
{
    class Engine
    {
    protected:
        sol::state m_lua;
        std::vector<std::unique_ptr<System::Plugin>> m_plugins;
        std::unique_ptr<Scene::Scene> m_scene;
        std::unique_ptr<System::Cursor> m_cursor;
        std::unique_ptr<System::Window> m_window;

        // Managers
        Audio::AudioManager m_audio {};
        Config::ConfigurationManager m_config {};
        ResourceManager m_resources {};
        std::unique_ptr<Input::InputManager> m_input {};
        std::unique_ptr<Time::FramerateManager> m_framerate;
        std::unique_ptr<Triggers::TriggerManager> m_triggers;

        // TriggerGroups
        Triggers::TriggerGroupPtr t_game {};

        // Initialization
        void initConfig();
        void initLogger() const;
        void initScript();
        void initTriggers();
        void initInput();
        void initFramerate();
        void initResources();
        void initWindow();
        void initCursor();
        void initPlugins();
        void initScene();

        // Main loop
        void clean();
        void handleWindowEvents();
        void update();
        void render() const;

    public:
        Engine();
        void run();

        /**
         * \bind{Audio}
         * \asproperty
         */
        Audio::AudioManager& getAudioManager();
        /**
         * \bind{Configuration}
         * \asproperty
         */
        Config::ConfigurationManager& getConfigurationManager();
        /**
         * \bind{Resources}
         * \asproperty
         */
        ResourceManager& getResourceManager();
        /**
         * \bind{Input}
         * \asproperty
         */
        Input::InputManager& getInputManager();
        /**
         * \bind{Framerate}
         * \asproperty
         */
        Time::FramerateManager& getFramerateManager();
        /**
         * \bind{Triggers}
         * \asproperty
         */
        Triggers::TriggerManager& getTriggerManager();

        /**
         * \bind{Scene}
         * \asproperty
         */
        Scene::Scene& getScene();
        /**
         * \bind{Cursor}
         * \asproperty
         */
        System::Cursor& getCursor();
        /**
         * \bind{Window}
         * \asproperty
         */
        System::Window& getWindow();
    };
}