#include "Coin.h"
#include "../../../Common/ModelMng.h"
#include "../../../Common/AnimationController.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Common/SoundMng.h"
#include "../../../../_debug/_DebugConOut.h"

Coin::Coin()
{
}

Coin::Coin(Scene sceneID, Vector3 pos, int num)
{
	nowSceneID_ = sceneID;
	num_ = num;
	Init(pos);
}

Coin::~Coin()
{
}

bool Coin::ReSet(Vector3 pos)
{
	pos_ = pos;
	nowSceneID_ = Scene::Result;
	MV1SetPosition(lpModelMng.GetID("./source/model/Coin/Coin.mv1")[num_], { pos_.x, pos_.y, pos_.z });
	MV1SetPosition(lpModelMng.GetID("./source/model/Coin/Coin_Trans.mv1")[num_], { pos_.x, pos_.y, pos_.z });
	MV1SetScale(lpModelMng.GetID("./source/model/Coin/Coin.mv1")[num_], { 1,1,1 });
	MV1SetScale(lpModelMng.GetID("./source/model/Coin/Coin_Trans.mv1")[num_], { 1,1,1 });
	clearFlg = false;
	mCoinState = CoinState::Idle;
	IsAlive = true;
	animController_->Play((int)CoinState::Idle);
	return false;
}

bool Coin::Init(void)
{
	return false;
}

bool Coin::Init(Vector3 pos)
{
	pos_ = pos;
	MV1SetPosition(lpModelMng.GetID("./source/model/Coin/Coin.mv1")[num_], pos_.toVECTOR());
	MV1SetPosition(lpModelMng.GetID("./source/model/Coin/Coin_Trans.mv1")[num_], pos_.toVECTOR());
	MV1SetScale(lpModelMng.GetID("./source/model/Coin/Coin.mv1")[num_], { 0.7f,0.7f,0.7f });
	MV1SetScale(lpModelMng.GetID("./source/model/Coin/Coin_Trans.mv1")[num_], { 1.0f,1.0f,1.0f });
	MV1RefreshCollInfo(lpModelMng.GetID("./source/model/Coin/Coin.mv1")[num_]);
	clearFlg = false;
	mCoinState = CoinState::Idle;
	IsAlive = true;
	GetFlg = false;

	animController_ = std::make_unique<AnimationController>(lpModelMng.GetID("./source/model/Coin/Coin.mv1")[num_]);
	animController_->Add((int)CoinState::Idle, "./source/animation/Coin/CoinRotateAnim.mv1", 30.0f);
	animController_->Add((int)CoinState::Get, "./source/animation/Coin/GetAnim.mv1", 30.0f);

	int pso = LoadPixelShader("./source/shader/ps_Sph.pso");
	int vso = LoadVertexShader("./source/shader/vs.vso");

	texmap.diffuse = LoadGraph("./source/Textures/Coin/CoinTextue.png");
	texmap.norm = LoadGraph("./source/Textures/Player/MergedBake_normal.png");
	texmap.rough = LoadGraph("./source/Textures/Coin/Coin_roughness.png");
	texmap.metallic = LoadGraph("./source/Textures/Player/MergedBake_metalness.png");
	texmap.shp = LoadGraph("./source/Textures/Player/sph.png");
	return true;
}
void Coin::Update(void)
{
	if (!IsAlive)
	{
		return;
	}
	if (!GetFlg)//コインの取得フラグ
	{
		animController_->Play((int)CoinState::Idle);
	}
	else
	{
		if (nowSceneID_ == Scene::Result)
		{
			animController_->Play((int)CoinState::Idle);

		}
		else
		{
			if (CheckSoundMem(lpSoundMng.GetID("./source/Sound/coinget.mp3")[0]) == 0)
			{
				PlaySoundMem(lpSoundMng.GetID("./source/Sound/coinget.mp3")[0], DX_PLAYTYPE_BACK);
			}
			animController_->Play((int)CoinState::Get, false);
			if (animController_->IsEnd())
			{
				IsAlive = false;
			}
		}
	}
	animController_->Update(lpSceneMng.GetDeltaTime());
}


void Coin::Draw(void)
{
	if (nowSceneID_ == Scene::Result)
	{
		if (GetFlg)
		{
			MV1DrawModel(lpModelMng.GetID("./source/model/Coin/Coin.mv1")[num_]);
		}
		else
		{
			MV1DrawModel(lpModelMng.GetID("./source/model/Coin/Coin_Trans.mv1")[num_]);
		}
	}
	else
	{
		if (IsAlive)
		{
			MV1DrawModel(lpModelMng.GetID("./source/model/Coin/Coin.mv1")[num_]);
		}
	}
}

void Coin::SetCoinPos(int handle, Vector3 Coinpos)
{
	MV1SetPosition(handle, { Coinpos.x, Coinpos.y, Coinpos.z });
}

Vector3 Coin::GetCoinPos()
{
	return pos_;
}

void Coin::SetCoinState(CoinState state)
{
	mCoinState = state;
}

CoinState Coin::GetCoinState(void)
{
	return mCoinState;
}

void Coin::SetCoinfFlg(bool flg)
{
	GetFlg = flg;
}

bool Coin::GetCoinFlg()
{
	return GetFlg;
}

std::string Coin::GetCoinPath(void)
{
	return "./source/model/Coin/Coin.mv1";
}

bool Coin::GetClearFlg()
{
	return clearFlg;
}

int Coin::GetCoinNumber(void)
{
	return num_;
}

int Coin::GetCoinHandle(void)
{
	return lpModelMng.GetID("./source/model/Coin/Coin.mv1")[num_];
}
