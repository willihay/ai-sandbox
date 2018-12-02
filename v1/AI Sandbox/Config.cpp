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
float GameObject_DefaultCoefficientFriction = 0.4f;
float GameObject_DefaultCoefficientRestitution = 0.9f; 
float GameObject_DefaultMass = 5.f; // kilograms
float GameObject_DefaultMaxAcceleration = 100.f; // meters per second per second
float GameObject_DefaultMaxAngularVelocity = 6.f; // radians per second
float GameObject_DefaultMaxSpeed = 300.f; // meters per second
const wchar_t* GameObject_DefaultTextureFile = L"Assets\\DefaultGameObject.png";

// World attributes
float World_FrictionCoefficient = 0.5f;
float World_Gravity = 9.8f; // meters per second per second
float World_ScaleMetersPerPixel = 0.1f; // world scale for displaying sprites
}
