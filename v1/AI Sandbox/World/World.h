#pragma once

#include "GameObject.h"

typedef std::list<std::shared_ptr<GameObject>> Team;
typedef std::vector<Team> Teams;

class World
{
public:
    World();
    ~World();

    // Common functions
    void Update(float elapsedTime);
    void Render(DirectX::SpriteBatch* spriteBatch);

    // World attributes
    float GetFrictionCoefficient() { return m_frictionCoefficient; }
    DirectX::SimpleMath::Vector2 GetWorldBoundary() { return m_worldBoundary; }

    void SetWorldBoundary(DirectX::SimpleMath::Vector2 boundary) { m_worldBoundary = boundary; }

    // World object functions
    void CreateAllTextures(ID3D11Device2* device);
    void ResetAllTextures();

    // Team functions
    void CreateTeam();
    const Teams& GetAllTeams() { return m_playerTeams; }
    size_t GetNumberOfTeams() { return m_playerTeams.size(); }
    const Team& GetTeam(size_t teamNumber) { return m_playerTeams[teamNumber]; }
    void RemoveTeam(size_t teamNumber);

    // Player functions
    void AddPlayer(std::shared_ptr<GameObject> player, size_t teamNumber);
    std::shared_ptr<GameObject> GetPlayer(size_t teamNumber, size_t playerNumber);
    void MovePlayer(std::shared_ptr<GameObject> player, size_t newTeamNumber);
    void RemoveAllPlayers(size_t teamNumber);
    void RemovePlayer(std::shared_ptr<GameObject> player);

private:
    // World objects
    Teams m_playerTeams; // "all the world's a stage, and [we are] merely players"

    // World characteristics
    float                           m_frictionCoefficient;
    DirectX::SimpleMath::Vector2    m_worldBoundary;
};
