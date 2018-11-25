#pragma once

namespace Config
{
bool LoadConfigFile();

// General
extern int ConfigVersion;

// Behavior modules
extern char BehaviorModule_DefaultPriorityLevel;
extern float Follow_DefaultDistance;
extern char PlayerInput_DefaultPriorityLevel;

// Game objects
extern float GameObject_DefaultMaxAcceleration;
extern float GameObject_DefaultMaxAngularRotation; // radians per second
extern float GameObject_DefaultMaxSpeed;
extern const wchar_t* GameObject_DefaultTextureFile;
}
