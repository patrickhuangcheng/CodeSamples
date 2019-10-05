#ifndef WAVE_SPAWNER_H
#define WAVE_SPAWNER_H
#include <vector>
#include <array>        // std::array

#include "2D\Vector2D.h"
#include "Misc\Util.h"
#include "GameEntities\EnemyPlayer.h"

class EnemyPlayer;

class WaveSpawner 
{
public:
	enum State {None, Spawn, Battle, Reward, Evaluate};
	WaveSpawner()
		:populationSize(10),
		generation(1),
		m_fGenerateTime(0.f),
		m_fDestroyedTime(0.f),
		m_fGenerateWait(0.5f),
		m_fRewardWait(0.5f),
		Enemies{nullptr},
		EnemyDNAs{nullptr},
		GenerationHeights{ 1, 4, 7 },
		MeteoriteSpawnPoses{},
		State(None)
	{}
	~WaveSpawner() 
	{
		DeleteSTLContainer(Enemies);
		Enemies.clear();
	}

	void Update();

protected:
	//////////////////////////////////////////////////////////////////////////
	// State Machine Transitions
	bool OnSpawn();

	bool OnBattle();

	bool OnReward();

	bool OnEvaluate();
	//////////////////////////////////////////////////////////////////////////
	bool RandomGenEnemies();
	bool GenerateEnemies();
	Vector2D RandomSpawnPosition(GameConfig::Positions position = GameConfig::Positions::None);

	int RandomLevel() { return RandInt(0, 2); }

	void ShuffleHeights();

private:
	int populationSize;
	int generation;
	float m_fGenerateTime;
	float m_fDestroyedTime;
	const float m_fGenerateWait;
	const float m_fRewardWait;
	std::vector<EnemyPlayer*> Enemies;
	std::vector<EnemyPlayer::EnemyDNA*> EnemyDNAs;
	std::vector<EnemyPlayer::EnemyDNA*>::iterator curDNA;
	std::array<int, 3> GenerationHeights;
	std::vector<Vector2D> MeteoriteSpawnPoses;
	WaveSpawner::State State;

};

#endif