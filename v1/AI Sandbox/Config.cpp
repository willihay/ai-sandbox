#include "pch.h"
#include "Config.h"

bool Config::LoadConfigFile()
{
    // TODO : load configuration from file, creating the file using default values if needed
    return false;
}

namespace Config
{
// General
int ConfigVersion = 1;

// Behavior Modules
char BehaviorModule_DefaultPriorityLevel = 5;
float Follow_DefaultDistance = 20.f;
char PlayerInput_DefaultPriorityLevel = 1;

// Game Objects
float GameObject_DefaultMaxAcceleration = 50.f;
float GameObject_DefaultMaxAngularRotation = 2.f;
float GameObject_DefaultMaxSpeed = 200.f;
const wchar_t* GameObject_DefaultTextureFile = L"Assets\\DefaultGameObject.png";
}
