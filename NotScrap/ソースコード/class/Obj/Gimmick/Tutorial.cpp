#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include "Tutorial.h"
#include "../../Common/ImageMng.h"
#include "../../Common/MovieMng.h"
#include "../../Scene/SceneManager.h"
#include "PressureDoor.h"
#include"../../../_debug/_DebugConOut.h"
#include "../Unit/Enemy.h"
#include "../Unit/EnemyShoot.h"
#include "../Gimmick/TutorialMovie.h"
#include "../../Input/PadInput.h"
#include "../../Input/KeyInput.h"

constexpr float MOVE_SPEED = 0.5f;	//“®‚­‘¬“x

Tutorial::Tutorial(Vector3 pos, Vector3 doorPos, int num , EneymyList& list, TutorialType type)
{
	Enemylist_ = list;
	doorPos_ = doorPos;
	type_ = type;
	movedPosY_ = pos.y;
	num_ = num;
	switch (type_)
	{
	case TutorialType::TutorialN:
		offset_ = Vector3{ -30,-60,-140 };
		TutorialMovie_ = std::make_unique<TutorialMovie>(pos, "magWS", num, type_);

		break;
	case TutorialType::TutorialS:
		offset_ = Vector3{ 30,-60,-140 };

		TutorialMovie_ = std::make_unique<TutorialMovie>(pos, "magWN", num, type_);

		break;
	case TutorialType::TutorialKey:
		offset_ = Vector3{ -30,-60,-140 };
		TutorialMovie_ = std::make_unique<TutorialMovie>(pos, "move", num, type_);

		break;
	case TutorialType::TutorialShift:
		offset_ = Vector3{ -30,-60,-140 };
		TutorialMovie_ = std::make_unique<TutorialMovie>(pos, "magSS", num, type_);

		break;
	case TutorialType::TutorialMagBox:
		offset_ = Vector3{ -30,-60,-140 };
		TutorialMovie_ = std::make_unique<TutorialMovie>(pos, "magbox", num, type_);

		break;
	case TutorialType::TutorialBullet:
		TutorialMovie_ = std::make_unique<TutorialMovie>(pos, "bullet", num, type_);
		break;
	case TutorialType::TutorialDeath:
		TutorialMovie_ = std::make_unique<TutorialMovie>(pos, "death", num, type_);
		break;
	case TutorialType::TutorialStick:
		TutorialMovie_ = std::make_unique<TutorialMovie>(pos, "stick", num, type_);
		break;
	default:
		break;
	}
	Init(pos, num);
}

Tutorial::~Tutorial()
{
}

bool Tutorial::Init(Vector3 pos, int num )
{
	door_ = std::make_unique<PressureDoor>(doorPos_, lpStage.GetDoorList().size() + num);
	speed_ = 20;
	pos_ = Vector3{pos.x,2000,0};
	if (type_ == TutorialType::TutorialDeath)
	{
		pos_.x = pos.x + 1200;
		pos_.y = 2200;
	}
	wave_.pos = pos_ + Vector3{0,0,-500};
	size_ = { 30,90,30 };
	MV1SetPosition(lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1")[num], pos_.toVECTOR());
	MV1SetScale(lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1")[num], { 0.5,0.5,0.5 }/*{1.5,1.5,1.5 }*/);
	time_ = 0;

	moveLimitVal_ = { pos_.y , movedPosY_ };
	effcnt_ = 0;
	flag_ = false;
	flg_ = false;
	OpenflagR_ = false;
	OpenflagL_ = false;

	if (GetJoypadNum() > 0)
	{
		controller_ = std::make_unique<PadInput>();
	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}

	return false;
}

void Tutorial::Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)
{
	controller_->Update();

	cameraPos_ = PlyaerList[0]->GetPos();
	if ((time_ / 50) % 2 == 0)
	{
		if (type_ == TutorialType::TutorialS)
		{

			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1")[num_], 0, lpImageMng.GetID("Smouse")[0], false);

		}
		else if (type_ == TutorialType::TutorialN)
		{
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1")[num_], 0, lpImageMng.GetID("Nmouse")[0], false);


		}
		else if (type_ == TutorialType::TutorialKey)
		{
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1")[num_], 0, lpImageMng.GetID("key2")[0], false);

			offset_ = { -0,-0,-100000 };
		}
		else if (type_ == TutorialType::TutorialShift)
		{
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1")[num_], 0, lpImageMng.GetID("shiftM")[0], false);

			offset_ = { -0,-0,-100000 };

		}
		else if (type_ == TutorialType::TutorialMagBox)
		{
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1")[num_], 0, lpImageMng.GetID("Nmouse")[0], false);

			offset_ = { -0,-0,-10000 };

		}
		else if (type_ == TutorialType::TutorialDeath)
		{
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1")[num_], 0, lpImageMng.GetID("RR")[0], false);

			offset_ = { -0,-0,-10000 };

		}
		else
		{
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1")[num_], 0, lpImageMng.GetID("Smouse")[0], false);

		}
	}
	else if ((time_ / 50) % 2 == 1)
	{
		if (type_ == TutorialType::TutorialS)
		{

			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1")[num_], 0, lpImageMng.GetID("NONmouse")[0], false);

		}
		else if (type_ == TutorialType::TutorialN)
		{
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1")[num_], 0, lpImageMng.GetID("NONmouse")[0], false);


		}
		else if (type_ == TutorialType::TutorialKey)
		{
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1")[num_], 0, lpImageMng.GetID("key3")[0], false);

			offset_ = { -0,-0,-100000 };
		}
		else if (type_ == TutorialType::TutorialShift)
		{
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1")[num_], 0, lpImageMng.GetID("shiftMN")[0], false);

			offset_ = { -0,-0,-100000 };

		}
		else if (type_ == TutorialType::TutorialMagBox)
		{
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1")[num_], 0, lpImageMng.GetID("NONmouse")[0], false);

			offset_ = { -0,-0,-10000 };

		}
		else if (type_ == TutorialType::TutorialDeath)
		{
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1")[num_], 0, lpImageMng.GetID("R")[0], false);

			offset_ = { -0,-0,-10000 };

		}
	}
	auto cntData = controller_->GetCntData();

	if (CheckWithinRangeXonly(pos_, cameraPos_))
	{
		if (!flg_)
		{
			for (auto& player : PlyaerList)
			{
				switch (type_)
				{
				case TutorialType::TutorialN:
					OpenflagL_ = true;
					if (cntData[InputID::BtnN][static_cast<int>(Trg::Now)] && player->GetState() == State::MagnetWeak)
					{
						OpenflagR_ = true;
					}
					break;
				case TutorialType::TutorialS:
					OpenflagL_ = true;
					if (cntData[InputID::BtnS][static_cast<int>(Trg::Now)] && player->GetState() == State::MagnetWeak)
					{
						OpenflagR_ = true;
					}
					break;
				case TutorialType::TutorialKey:
					if (cntData[InputID::Right][static_cast<int>(Trg::Now)])
					{
						OpenflagR_ = true;
					}

					if (cntData[InputID::Left][static_cast<int>(Trg::Now)])
					{
						OpenflagL_ = true;
					}
					break;
				case TutorialType::TutorialShift:
					OpenflagL_ = true;
					if (cntData[InputID::Change][static_cast<int>(Trg::Now)] && player->GetState() == State::Magnet)
					{
						OpenflagR_ = true;
					}
					break;
				case TutorialType::TutorialMagBox:
					OpenflagL_ = true;
					for (auto& gim : gimmickList)
					{
						if (gim->GetObjType() == objType::PressurePlate)
						{
							OpenflagR_ = gim->flg_;
						}
					}
					break;
				case TutorialType::TutorialBullet:
					OpenflagL_ = true;
					for (auto& Unit : Enemylist_)
					{
						if (Unit->GetEnemyType() != EnemyType::SHOOT)
						{
							continue;
						}
						auto enemy = dynamic_cast<EnemyShoot*>(Unit.get());
						OpenflagR_ = !enemy->GetisAlive();

					}
					break;
				case TutorialType::TutorialStick:
					OpenflagL_ = true;
					for (auto& Unit : Enemylist_)
					{
						if (Unit->GetEnemyType() != EnemyType::WALK)
						{
							continue;
						}
						auto enemy = dynamic_cast<Enemy*>(Unit.get());
						if (enemy->GetState() == State::Stick)
						{
							OpenflagR_ = true;
						}
					}
					break;
				case TutorialType::TutorialDeath:
					OpenflagL_ = true;
					if (player->GetState() == State::DismemberDeath)
					{
						OpenflagR_ = true;
					}
					break;
				default:
					break;
				}
			}
		}

		if (OpenflagR_ && OpenflagL_)
		{
			flg_ = true;

		}

		if (type_ == TutorialType::TutorialDeath)
		{
			if (flg_)
			{
				if (pos_.y >= moveLimitVal_.lower)
				{

					pos_.y -= speed_;


					speed_ *= 0.99f;
					if (speed_ < 1.0f)
					{
						speed_ = 1.0f;
					}
				}
			}
		}
		else
		{
			if (pos_.y >= moveLimitVal_.lower)
			{

				pos_.y -= speed_;


				speed_ *= 0.99f;
				if (speed_ < 1.0f)
				{
					speed_ = 1.0f;
				}
			}
		}
		if ((time_ / 10) % 8 == 3)
		{
			if (!flag_)
			{
				flag_ = true;
			}
		}
		else
		{
			flag_ = false;
		}
	}
	else
	{

		speed_ = 20;
		if (pos_.y <= moveLimitVal_.upper)
		{
			pos_.y += speed_;
		}
	}

	door_->Update(flg_);
	TutorialMovie_->Update(PlyaerList, gimmickList, flg_);
	MV1SetPosition(lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1")[num_], pos_.toVECTOR());

	time_++;
}

void Tutorial::Draw(void)
{
	if (CheckWithinRangeXonly(pos_, cameraPos_))
	{
		door_->Draw();
		TutorialMovie_->Draw();
		auto pos = pos_ + Vector3{ 0, -110, -80 };
	}
	auto pos = pos_ + Vector3{ 500, 0, 0 };

	if (CheckWithinRangeXonly(pos, cameraPos_))
	{
		door_->Draw();
	}
}
