#include "ConfigManager.h"

ConfigManager *ConfigManager::m_instance = nullptr;

ConfigManager *ConfigManager::createInstance()
{
    if (m_instance == nullptr)
        m_instance = new ConfigManager();
    return m_instance;
}

ConfigManager *ConfigManager::getInstance() { return m_instance; }
void ConfigManager::destroyInstance()
{
    if (m_instance)
        delete m_instance;
    m_instance = nullptr;
}

ConfigManager::ConfigManager()
{
}

ConfigManager::~ConfigManager()
{}

Config *ConfigManager::loadConfig(const std::string &conf_name,
                                  const std::string &file_path)
{
}

Config *ConfigManager::getDefaultConfig() const
{
    return m_default_conf;
}