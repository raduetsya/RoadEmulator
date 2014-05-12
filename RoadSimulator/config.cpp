#include "config.h"

// Singleton pattern
Config* Config::_instance = nullptr;

Config& Config::get()
{
    if (_instance == nullptr) {
        _instance = new Config;
    }
    return *_instance;
}
