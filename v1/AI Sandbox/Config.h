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
extern float GameObject_DefaultCoefficientFriction;
extern float GameObject_DefaultCoefficientRestitution;
extern float GameObject_DefaultMass; // kilograms
extern float GameObject_DefaultMaxAcceleration; // meters per second per second
extern float GameObject_DefaultMaxAngularVelocity; // radians per second
extern float GameObject_DefaultMaxSpeed; // meters per second
extern const wchar_t* GameObject_DefaultTextureFile;

// World attributes
extern float World_FrictionCoefficient;
extern float World_Gravity; // meters per second per second
extern float World_ScaleMetersPerPixel; // world scale for displaying sprites
}
