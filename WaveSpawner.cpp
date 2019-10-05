#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include "GameSystem\WaveSpawner.h"
#include "GameEntities\EnemyPlayer.h"
#include "GameEntities\Meteorite.h"
#include "UI/HUD.h"

void WaveSpawner::Update()
{
	switch (State)
	{
		case WaveSpawner::None:
			{
				Enemies.clear();
				EnemyDNAs.clear();
				if(RandomGenEnemies())
					OnSpawn();
			}
			break;
		case WaveSpawner::Spawn:
			{
				if ( Simple2D::GetGameTime() - m_fGenerateTime  >= m_fGenerateWait && GenerateEnemies())
				{
					OnBattle();
				}
			}
			break;
		case WaveSpawner::Battle:
			{
				std::vector<EnemyPlayer*>::iterator curEnemy;
				for (curEnemy = Enemies.begin(); curEnemy != Enemies.end(); ++curEnemy)
				{
					if ((*curEnemy)!= NULL && !(*curEnemy)->IsDead())
					{
						return;
					}
				}
				OnReward();
			}
			break;
		case WaveSpawner::Reward:
			{
				if(Simple2D::GetGameTime() - m_fDestroyedTime >= m_fRewardWait)
					OnEvaluate();
			}
			break;
		case WaveSpawner::Evaluate:
			{

				if (RandomGenEnemies())
					OnSpawn();
			}
			break;
	}

	
}

bool WaveSpawner::OnSpawn()
{
	m_fGenerateTime = Simple2D::GetGameTime();
	State = State::Spawn;
	return true;
}

bool WaveSpawner::OnBattle()
{
	State = State::Battle;
	return true;
}

bool WaveSpawner::OnReward()
{
	State = State::Reward;
	m_fDestroyedTime = Simple2D::GetGameTime();
	return true;
}

bool WaveSpawner::OnEvaluate()
{
	State = State::Evaluate;
	return true;
}

bool WaveSpawner::RandomGenEnemies()
{

	DeleteSTLContainer(Enemies);
	Enemies.clear();
	ShuffleHeights();
	int AreaOffset = 0;
	int AreaIndex = GenerationHeights[0];
	MeteoriteSpawnPoses.clear();
	while (AreaOffset < 3)
	{
		for (int i = 0; i < 2; i++)
		{
			MeteoriteSpawnPoses.push_back(RandomSpawnPosition((GameConfig::Positions)(AreaIndex + AreaOffset)));
		}
		AreaOffset++;
	}
	AreaIndex = GenerationHeights[1];

	int NumOfDrones =  (int)((float)populationSize * 0.6f);
	for (int i = 0; i < NumOfDrones; i++)
	{
		EnemyPlayer::EnemyDNA  * pDna = new EnemyPlayer::EnemyDNA
		(RandomSpawnPosition((GameConfig::Positions)(AreaIndex)),
			EnemyPlayer::LEVEL::drone,
			-1.f);
		EnemyDNAs.push_back(pDna);
	}
	int NumOfFeeder =  (int)((float)populationSize * 0.2f);
	for (int i = 0; i < NumOfFeeder; i++)
	{
		EnemyPlayer::EnemyDNA  * pDna = new EnemyPlayer::EnemyDNA
		(RandomSpawnPosition((GameConfig::Positions)(AreaIndex + 1)),
			EnemyPlayer::LEVEL::feeder,
			-1.f);
		EnemyDNAs.push_back(pDna);
	}

	AreaIndex = GenerationHeights[2];
	int NumOfCarrier = (int)((float)populationSize * 0.2f);
	for (int i = 0; i < NumOfCarrier; i++)
	{
		EnemyPlayer::EnemyDNA  * pDna = new EnemyPlayer::EnemyDNA
		(RandomSpawnPosition((GameConfig::Positions)(AreaIndex + 2)),
			EnemyPlayer::LEVEL::carrier,
			-1.f);
		EnemyDNAs.push_back(pDna);
	}

	curDNA = EnemyDNAs.begin();
	return true;
}

bool WaveSpawner::GenerateEnemies()
{
	m_fGenerateTime = Simple2D::GetGameTime();

	if (curDNA == EnemyDNAs.end())
	{
		DeleteSTLContainer(EnemyDNAs);
		EnemyDNAs.clear();
		for (auto var : MeteoriteSpawnPoses)
		{
			new Meteorite(var, (Meteorite::METEORITE)RandInt(0, 2));
		}
		return true;
	}
	else
	{
		Enemies.push_back(EnemyFactory::Get()->CreateEnemy(**curDNA));
		curDNA++;
		
		return false;
	}

}


Vector2D WaveSpawner::RandomSpawnPosition(GameConfig::Positions position/* = GameConfig::Positions::None*/)
{
	float X, Y = 0;
	switch (position)
	{
	case GameConfig::Positions::None:
		{
			X = (float)(GameConfig::EnemyWidth[RandInt(0, 8)]);
			Y = (float)(GameConfig::EnemyHeight[RandInt(0, 8)]);
		}
		break;
	case GameConfig::Positions::Center:
		{
			X = (float)(GameConfig::EnemyWidth[RandInt(3, 5)]);
			Y = (float)(GameConfig::EnemyHeight[RandInt(3, 5)]);
		}
		break;
	case GameConfig::Positions::North:
		{
			X = (float)(GameConfig::EnemyWidth[RandInt(3, 5)]);
			Y = (float)(GameConfig::EnemyHeight[RandInt(6, 8)]);
		}
		break;
	case GameConfig::Positions::NorthEast:
		{
			X = (float)(GameConfig::EnemyWidth[RandInt(6, 8)]);
			Y = (float)(GameConfig::EnemyHeight[RandInt(6, 8)]);
		}
		break;
	case GameConfig::Positions::East:
		{
			X = (float)(GameConfig::EnemyWidth[RandInt(6, 8)]);
			Y = (float)(GameConfig::EnemyHeight[RandInt(3, 5)]);
		}
		break;
	case GameConfig::Positions::SouthEast:
		{
			X = (float)(GameConfig::EnemyWidth[RandInt(6, 8)]);
			Y = (float)(GameConfig::EnemyHeight[RandInt(0, 2)]);
		}
		break;
	case GameConfig::Positions::South:
		{
			X = (float)(GameConfig::EnemyWidth[RandInt(3, 5)]);
			Y = (float)(GameConfig::EnemyHeight[RandInt(0, 2)]);
		}
		break;
	case GameConfig::Positions::SouthWest:
		{
			X = (float)(GameConfig::EnemyWidth[RandInt(0, 2)]);
			Y = (float)(GameConfig::EnemyHeight[RandInt(0, 2)]);
		}
		break;
	case GameConfig::Positions::West:
		{
			X = (float)(GameConfig::EnemyWidth[RandInt(0, 2)]);
			Y = (float)(GameConfig::EnemyHeight[RandInt(3, 5)]);
		}
		break;
	case GameConfig::Positions::NorthWest:
		{
			X = (float)(GameConfig::EnemyWidth[RandInt(0, 2)]);
			Y = (float)(GameConfig::EnemyHeight[RandInt(6, 8)]);
		}
		break;
	default:
		{
			X = (float)(GameConfig::EnemyWidth[RandInt(0, 8)]);
			Y = (float)(GameConfig::EnemyHeight[RandInt(0, 8)]);
		}
		break;
	}
  	X += 1024.f;
	return Vector2D(X,Y);
}

void WaveSpawner::ShuffleHeights()
{
	unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(GenerationHeights.begin(), GenerationHeights.end(), std::default_random_engine(seed));
}

