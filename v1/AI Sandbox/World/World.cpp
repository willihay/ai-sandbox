#include "pch.h"
#include "World.h"

using namespace DirectX;

World::World()
{
}

World::~World()
{
}

void World::Update(float elapsedTime)
{
    // TODO: first, apply drag force to all GameObjects (based on world attribute)?

    for (const auto& team : m_playerTeams)
    {
        for (const auto& teamPlayer : team)
        {
            teamPlayer->Update(this, elapsedTime);
        }
    }
}

void World::Render(SpriteBatch* spriteBatch)
{
    for (const auto& team : m_playerTeams)
    {
        for (const auto& teamPlayer : team)
        {
            teamPlayer->Render(spriteBatch);
        }
    }
}

void World::CreateAllTextures(ID3D11Device2* device)
{
    for (const auto& team : m_playerTeams)
    {
        for (const auto& teamPlayer : team)
        {
            teamPlayer->CreateTexture(device);
        }
    }
}

void World::ResetAllTextures()
{
    for (const auto& team : m_playerTeams)
    {
        for (const auto& teamPlayer : team)
        {
            teamPlayer->ResetTexture();
        }
    }
}

void World::CreateTeam()
{
    m_playerTeams.push_back(Team());
}

void World::AddPlayer(std::shared_ptr<GameObject> player, size_t teamNumber)
{
    if (teamNumber < m_playerTeams.size())
    {
        player->SetTeamNumber(teamNumber);
        m_playerTeams[teamNumber].push_back(player);
    }
}

std::shared_ptr<GameObject> World::GetPlayer(size_t teamNumber, size_t playerNumber)
{
    if (teamNumber < m_playerTeams.size() && playerNumber < m_playerTeams[teamNumber].size())
    {
        const auto& team = m_playerTeams[teamNumber];
        auto it = team.begin();
        for (auto i = 0; i < playerNumber; ++i)
        {
            it++;
        }
        return *it;
    }

    return nullptr;
}

void World::RemoveAllPlayers(size_t teamNumber)
{
    if (teamNumber < m_playerTeams.size())
    {
        auto& team = m_playerTeams[teamNumber];
        for (auto& player : team)
        {
            player->SetValidTarget(false);
        }

        m_playerTeams[teamNumber].clear();
    }
}

void World::RemovePlayer(std::shared_ptr<GameObject> player)
{
    if (!player)
        return;

    player->SetValidTarget(false);

    auto teamNumber = player->GetTeamNumber();
    if (teamNumber < m_playerTeams.size())
    {
        auto& team = m_playerTeams[teamNumber];
        for (auto it = team.cbegin(); it != team.cend(); ++it)
        {
            if (*it == player)
            {
                team.erase(it);
                break;
            }
        }
    }
}
