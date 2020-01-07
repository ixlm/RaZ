#ifndef __CONFIG_MANAGER_H__
#define __CONFIG_MANAGER_H__
#include <unordered_map>

class Config;

class ConfigManager
{
public:
   static ConfigManager* createInstance();
   static ConfigManager* getInstance();
   static void destroyInstance();

   Config* loadConfig(const std::string& conf_name, const std::string& file_path);
   Config* getDefaultConfig() const;
protected:
    ConfigManager();
    ~ConfigManager();
private:
    typedef std::unordered_map<std::string, Config* > ConfigMap;
    ConfigMap   m_conf_map;
    Config*     m_default_conf = nullptr;
    static ConfigManager* m_instance ;
};

#endif