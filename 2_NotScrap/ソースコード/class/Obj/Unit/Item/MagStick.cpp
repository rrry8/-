#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include "MagStick.h"
#include"../../../../_debug/_DebugConOut.h"
#include"../Enemy.h"
#include"BulletManager.h"
#include"MagStickTrail.h"
#include "../../Gimmick/Gimmick.h"
#include "../../../Common/ImageMng.h"
#include "../EnemyShoot.h"

constexpr float ROD_LENGTH = 512.0f;	//棒の最大の長さ
constexpr float MOUSEICON_HALFSIZE = 32.0f;	//マウスアイコンの大きさの半分
constexpr float ROD_OFFSET_Y = 100.0f;		//プレイヤーの位置は足元なのでオフセットを足す
constexpr float ROD_MINI_OFFSET_Y = 8.0f;	//描画は少しだけずらす
constexpr float ROD_OFFSET_Z = 40.0f;		//プレイヤーの向いている方向で腕の位置を変える
constexpr float ARM_SCALE = 0.35f;			//プレイヤーの拡大率
constexpr float LINE_WIDTH = 30.0f;			//ライン同士の横幅
constexpr int LOWER_LIMIT = 200;			//傾きの最低数値
constexpr float WITHIN_RANGE = 3000.0f;		//判定する範囲
constexpr float CENTER_POS_X = 960.0f;	//中心点ｘ
constexpr float CENTER_POS_Y = 670.0f;	//中心点ｙ

MagStick::MagStick(Vector3 playerpos,CntType type,CntData cntdata, int num)
{
	num_ = num;
	line_.p = playerpos;
	cntType_ = type;
	cntData_ = cntdata;
	Init();
}

MagStick::~MagStick()
{
	StopEffect();
}

bool MagStick::Init(void)
{
	armPosz_ = -ROD_OFFSET_Z;
	angle_ = 0.0;
	rapidxml::file<> moduleFileName = "tmx/CheckHitObj.tsx";
	stateVec_ = moduleFileName.vecData();
	stateDoc.parse<0>(stateVec_.data());
	stateNode_ = stateDoc.first_node("MagStickmodule");
	state_ = MAGSTATE::None;
	playerState_ = State::Idle;
	playerMoveVec_ = {0,0,0};
	rodLength_ = ROD_LENGTH;

	//エフェクト初期化
	northEffect_.resource = LoadEffekseerEffect("./source/effect/north01.efkefc",20.0f);
	southEffect_.resource = LoadEffekseerEffect("./source/effect/south01.efkefc",20.0f);
	noneEffect_.resource = LoadEffekseerEffect("./source/effect/none01.efkefc", 20.0f);
	Effekseer_InitDistortion();
	effectAngle_ = 0.0f;

	endPos_ = line_.p;
	line_.p.x += 1.0f;
	oldAddPos_ = Vector2(ROD_LENGTH, 0.0f);

	trail_ = std::make_unique<MagStickTrail>();
	MV1SetPosition(lpModelMng.GetID("./source/model/Player/armonly_new.mv1")[num_], (pos_.toVECTOR()));
	MV1SetScale(lpModelMng.GetID("./source/model/Player/armonly_new.mv1")[num_], { ARM_SCALE,ARM_SCALE,ARM_SCALE });

	MV1SetPosition(lpModelMng.GetID("./source/model/Player/Chocolate-Stick.mv1")[num_], (pos_.toVECTOR()));
	MV1SetScale(lpModelMng.GetID("./source/model/Player/Chocolate-Stick.mv1")[num_], { 30,30,30 });

	MV1SetPosition(lpModelMng.GetID("./source/model/Player/Stick.mv1")[num_], (pos_.toVECTOR()));
	MV1SetScale(lpModelMng.GetID("./source/model/Player/Stick.mv1")[num_], { 40,100,100 });
	MV1SetOpacityRate(lpModelMng.GetID("./source/model/Player/Stick.mv1")[num_], 0.5);
	return false;
}

void MagStick::Update()
{
}

void MagStick::Update(GimmickList& gimmicklist,MagBoxList& boxList, Vector3 pos, Vector3 dir,State state,const CntData& cntdata, const EneymyList& enemy)
{
	//磁石の箱情報取得
	boxList_ = boxList;

	//プレイヤーの位置に棒の始点
	line_.p = pos;
	line_.p.y += ROD_OFFSET_Y;	//オフセットを足す

	//プレイヤーの向いている方向によって腕を移動させる
	if (dir.x > 0.0f)
	{
		armPosz_ = -ROD_OFFSET_Z;
	}
	else if (dir.x < 0.0f)
	{
		armPosz_ = ROD_OFFSET_Z;
	}

	//状態取得
	playerState_ = state;

	//コントローラデータ更新
	cntData_ = cntdata;

	state_ = MAGSTATE::None;
	if (cntData_[InputID::BtnN][static_cast<int>(Trg::Now)])
	{
		state_ = MAGSTATE::North;
		MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/Stick.mv1")[num_], 1, lpImageMng.GetID("N")[0], false);
	}
	else if (cntData_[InputID::BtnS][static_cast<int>(Trg::Now)])
	{
		state_ = MAGSTATE::South;
		MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/Stick.mv1")[num_], 1, lpImageMng.GetID("S")[0], false);
	}
	else
	{
		MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/Stick.mv1")[num_], 1, lpImageMng.GetID("NONtex")[0], false);
	}

	//エフェクトの再生
	PlayEffect();

	//ラインの情報
	SetLineInfo();

	SlowEnemyBullet(enemy);

	//パッドの場合は棒の先端をマウスの位置にする
	if (cntType_ == CntType::Pad)
	{
		SetMouseIconPosition();
	}
	else
	{
		endPos_ = GetCurrentMosusePosition();
	}
	
	//位置と回転情報更新
	MV1SetPosition(lpModelMng.GetID("./source/model/Player/armonly_new.mv1")[num_], VGet(line_.p.x, line_.p.y + ROD_MINI_OFFSET_Y, line_.p.z + armPosz_));
	MV1SetRotationXYZ(lpModelMng.GetID("./source/model/Player/armonly_new.mv1")[num_], { 0, Deg2RadF(-90.0f), (float)angle_ });

	MV1SetPosition(lpModelMng.GetID("./source/model/Player/Chocolate-Stick.mv1")[num_], VGet(line_.p.x, line_.p.y + ROD_MINI_OFFSET_Y, line_.p.z + armPosz_ + 20));
	MV1SetRotationXYZ(lpModelMng.GetID("./source/model/Player/Chocolate-Stick.mv1")[num_], { 0, 0, (float)angle_ });

	MV1SetPosition(lpModelMng.GetID("./source/model/Player/Stick.mv1")[num_], VGet(line_.p.x, line_.p.y + ROD_MINI_OFFSET_Y, line_.p.z + armPosz_ + 35));
	MV1SetRotationXYZ(lpModelMng.GetID("./source/model/Player/Stick.mv1")[num_], { 0, 0, (float)angle_ });

	//各オブジェクトとの判定
	CollMagnet();
	CollMagnetBox(gimmicklist);
	CollAccessory(gimmicklist);
	CollEnemy(enemy);

	//軌跡の更新
	trail_->Update(line_);
}

void MagStick::Draw(void)
{
	//プレイヤーが死んでいるとき、描画をしない
	if (playerState_ == State::Death || playerState_ == State::DismemberDeath)
	{
		return;
	}

	MV1DrawModel(lpModelMng.GetID("./source/model/Player/Chocolate-Stick.mv1")[num_]);
	MV1DrawModel(lpModelMng.GetID("./source/model/Player/armonly_new.mv1")[num_]);

	//軌跡
	trail_->Draw();
}

void MagStick::Release(void)
{
	DeleteEffekseerEffect(northEffect_.resource);
	DeleteEffekseerEffect(southEffect_.resource);
}

Vector3 MagStick::GetPlayerMoveVec(void)
{
	return playerMoveVec_;
}

Line MagStick::GetLine(void)
{
	return line_;
}

Vector3 MagStick::GetMagBoxVec(void)
{
	return magBoxVec_;
}

Vector3 MagStick::GetCurrentMosusePosition(void)
{
	VECTOR endPos;
	VECTOR screenPos;
	int mx, my;
	GetMousePoint(&mx, &my);
	auto screenLinePos =ConvertVECTORtoVector3(ConvWorldPosToScreenPos(line_.p.toVECTOR()));
	screenPos.x = (float)mx + MOUSEICON_HALFSIZE;
	screenPos.y = (float)my + MOUSEICON_HALFSIZE;
	screenPos.z = 1.0f;

	endPos = ConvScreenPosToWorldPos(screenPos);
	endPos.z = 0.0f;
	return  ConvertVECTORtoVector3(endPos);
}

void MagStick::CollMagnet(void)
{
	playerMoveVec_ = { 0, 0 ,0 };
	auto node = stateNode_->first_node("module");

	Vector3 centerPos;
	centerPos = ConvertVECTORtoVector3(ConvScreenPosToWorldPos(VGet(trail_->GetCenterPos().x, trail_->GetCenterPos().y, 1.0f)));

	//z軸は使わないのでゼロクリア
	centerPos.z = 0.0f;


	effectPos_ = line_.p;
	//先端からプレイヤーへのベクトル
	Vector3 endToCenterVec = centerPos - endPos_;
	effectAngle_ = atan2(endToCenterVec.y, endToCenterVec.x);

	//sかnの磁石に衝突している場合、そのベクトル方向を取得
	if (CollisionRay()(line_, node, lpStage.GetSouthStageCollList()).HitFlag ||
		CollisionRay()(rightLine_, node, lpStage.GetSouthStageCollList()).HitFlag ||
		CollisionRay()(leftLine_, node, lpStage.GetSouthStageCollList()).HitFlag)
	{
		if (state_ == MAGSTATE::North)
		{
			//引き寄せられる
			effectPos_ = line_.p;
			endToCenterVec = centerPos - endPos_;
			effectAngle_ = atan2(endToCenterVec.y, endToCenterVec.x);
			playerMoveVec_ = -normalVec_;
		}
		else if (state_ == MAGSTATE::South)
		{
			//反発する
			effectPos_ = line_.end;
			endToCenterVec = endPos_ - centerPos;
			effectAngle_ = atan2(endToCenterVec.y, endToCenterVec.x);
			playerMoveVec_ = normalVec_;
		}
		else if (state_ == MAGSTATE::None)
		{
			if (IsEffekseer3DEffectPlaying(noneEffect_.playhandle) == -1)
			{
				noneEffect_.playhandle = PlayEffekseer3DEffect(noneEffect_.resource);
			}
		}
	}
	else if (CollisionRay()(line_, node, lpStage.GetNorthStageCollList()).HitFlag ||
		CollisionRay()(rightLine_, node, lpStage.GetNorthStageCollList()).HitFlag ||
		CollisionRay()(leftLine_, node, lpStage.GetNorthStageCollList()).HitFlag)
	{

		if (state_ == MAGSTATE::North)
		{
			//反発する
			effectPos_ = line_.end;
			endToCenterVec = endPos_ - centerPos;
			effectAngle_ = atan2(endToCenterVec.y, endToCenterVec.x);
			playerMoveVec_ = normalVec_;
		}
		else if (state_ == MAGSTATE::South)
		{
			//引き寄せられる
			effectPos_ = line_.p;
			endToCenterVec = centerPos - endPos_;
			effectAngle_ = atan2(endToCenterVec.y, endToCenterVec.x);
			playerMoveVec_ = -normalVec_;
		}
		else if (state_ == MAGSTATE::None)
		{
			if (IsEffekseer3DEffectPlaying(noneEffect_.playhandle) == -1)
			{
				noneEffect_.playhandle = PlayEffekseer3DEffect(noneEffect_.resource);
			}
		}
	}
	else
	{
		if (IsEffekseer3DEffectPlaying(noneEffect_.playhandle) == 0)
		{
			StopEffekseer3DEffect(noneEffect_.playhandle);
		}
	}
}

void MagStick::CollMagnetBox(GimmickList& gimmicklist)
{
	auto node = stateNode_->first_node("module");
	for (auto& magbox : gimmicklist)
	{
		if (!CheckWithinRange(line_.p, magbox->GetPos()))
		{
			continue;
		}
		int jyougenn = 500;
		if (magbox->GetObjType() == objType::NMagBox)
		{
			
			if (CollisionRay()(line_, node, lpModelMng.GetID("./source/Model/Magnet/N_Box.mv1")[magbox->GetModelH()]).HitFlag ||
				CollisionRay()(rightLine_, node, lpModelMng.GetID("./source/Model/Magnet/N_Box.mv1")[magbox->GetModelH()]).HitFlag ||
				CollisionRay()(leftLine_, node, lpModelMng.GetID("./source/Model/Magnet/N_Box.mv1")[magbox->GetModelH()]).HitFlag)
			{
				btTransform trans;
				magbox->GetRigidBody()->getMotionState()->getWorldTransform(trans);
				normalVec_.y = 0;
				if (state_ == MAGSTATE::North)
				{

					if ((abs(magbox->GetRigidBody()->getInterpolationLinearVelocity().x()) < jyougenn))
					{
						magbox->GetRigidBody()->applyCentralForce(normalVec_.tobtVec()*50000000);

					}
				}
				else if (state_ == MAGSTATE::South)
				{
					
					if ((abs(magbox->GetRigidBody()->getInterpolationLinearVelocity().x()) < jyougenn))
					{
						magbox->GetRigidBody()->applyCentralForce(-normalVec_.tobtVec()*50000000);
					}
				}
			}
			else
			{
				magbox->SetMoveVec(Vector3Zero);
			}
		}

		if (magbox->GetObjType() == objType::SMagBox)
		{
			if (CollisionRay()(line_, node, lpModelMng.GetID("./source/Model/Magnet/S_Box.mv1")[magbox->GetModelH()]).HitFlag ||
				CollisionRay()(rightLine_, node, lpModelMng.GetID("./source/Model/Magnet/S_Box.mv1")[magbox->GetModelH()]).HitFlag ||
				CollisionRay()(leftLine_, node, lpModelMng.GetID("./source/Model/Magnet/S_Box.mv1")[magbox->GetModelH()]).HitFlag)
			{
				btTransform trans;
				magbox->GetRigidBody()->getMotionState()->getWorldTransform(trans);
				normalVec_.y = 0;
				if (state_ == MAGSTATE::North)
				{

					if ((abs(magbox->GetRigidBody()->getInterpolationLinearVelocity().x()) < jyougenn))
					{
						
						magbox->GetRigidBody()->applyCentralForce(-normalVec_.tobtVec() * 50000000);
					}
				}
				else if (state_ == MAGSTATE::South)
				{

					if ((abs(magbox->GetRigidBody()->getInterpolationLinearVelocity().x()) < jyougenn))
					{
						magbox->GetRigidBody()->applyCentralForce(normalVec_.tobtVec() * 50000000);
					}
				}
			}
		}
	}
}

void MagStick::CollAccessory(GimmickList& gimmicklist)
{
	auto node = stateNode_->first_node("module");
	for (auto& acs : gimmicklist)
	{
		if (!CheckWithinRange(line_.p, acs->GetPos()))
		{
			continue;
		}
		if (acs->GetObjType() == objType::Accessory)
		{
			auto lineAcs = line_;
			auto rightlineAcs = rightLine_;
			auto leftlineAcs = leftLine_;
			lineAcs.p.z += MV1GetPosition(lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).z;
			rightlineAcs.p.z += MV1GetPosition(lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).z;
			leftlineAcs.p.z += MV1GetPosition(lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).z;
			
			lineAcs.end.z += MV1GetPosition(lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).z;
			rightlineAcs.end.z += MV1GetPosition(lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).z;
			leftlineAcs.end.z += MV1GetPosition(lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).z;

			if (CollisionRay()(lineAcs, node, lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).HitFlag ||
				CollisionRay()(rightlineAcs, node, lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).HitFlag ||
				CollisionRay()(leftlineAcs, node, lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).HitFlag)
			{
				if (state_ == MAGSTATE::North)
				{
					acs->GetRigidBody()->applyCentralImpulse(normalVec_.tobtVec() * 15);

				}
				else if (state_ == MAGSTATE::South)
				{
					acs->GetRigidBody()->applyCentralImpulse(-normalVec_.tobtVec() * 15);

				}
			}
		}
	}
}

void MagStick::CollEnemy(const EneymyList& enemy)
{
	auto node = stateNode_->first_node("module");
	for (auto &unit : enemy)
	{
		if (unit->GetEnemyType() != EnemyType::WALK)
		{
			continue;
		}
		auto enemy = dynamic_cast<Enemy*>(unit.get());
		enemy->SetRodLine(line_);

		//敵のハンドル取得
		auto name = enemy->GetHandlePath();
		auto id = enemy->GetNumber();
		//磁石が有効で、棒にあたっていた場合、敵の状態を張り付きにする
		if (CollisionRay()(line_,	   node, name,id).HitFlag ||
			CollisionRay()(leftLine_,  node, name,id).HitFlag ||
			CollisionRay()(rightLine_, node, name,id).HitFlag)
		{
			if (state_ != MAGSTATE::None)
			{
				if (enemy->GetState() != State::Stick)
				{
					enemy->SetState(State::Stick);
					auto enemyToplayerVec = enemy->GetPosition() - line_.p;
					rodLength_ = enemyToplayerVec.Magnitude();

					//もし棒の最大範囲を超えようとしていた場合は抑える
					if (rodLength_ > ROD_LENGTH)
					{
						rodLength_ = ROD_LENGTH;
					}
				}
				continue;
			}
		}
		//張り付き状態で磁石が稼働していない場合は敵の状態を落下にする
		if (enemy->GetState() == State::Stick)
		{
			if (state_ == MAGSTATE::None)
			{
				rodLength_ = ROD_LENGTH;
				enemy->SetState(State::Run);
			}
		}
		else
		{
			rodLength_ = ROD_LENGTH;
		}
	}
}

void MagStick::SetLineInfo(void)
{
	Vector3 centerPos = ConvertVECTORtoVector3(ConvScreenPosToWorldPos(VGet(CENTER_POS_X, CENTER_POS_Y, 1.0f)));
	//z軸は使わないのでゼロクリア
	centerPos.z = 0.0f;
	auto d = endPos_ - centerPos;

	//マウスの位置とプレイヤーの位置が一緒だった場合、方向を固定させる
	if (d == Vector3{ 0.0f,0.0f,0.0f })
	{
		d = Vector3{ 1.0f,1.0f,0.0f };
	}

	//角度更新
	angle_ = atan2(-d.y, -d.x);

	//正規化
	normalVec_ = d.Normalized();

	//真ん中の線分の位置更新
	line_.end = line_.p + normalVec_ * rodLength_;
	line_.end.z = 0.0f;

	//棒の方向ベクトルを取得
	auto q = atan2(line_.vec().y, line_.vec().x);

	//90度
	auto deg90 = 90.0f * (DX_PI_F / 180.0f);

	//右の線分の位置更新
	float addx = cos(q + deg90) * LINE_WIDTH;
	float addy = sin(q + deg90) * LINE_WIDTH;
	pos_.x = line_.p.x + addx;
	pos_.y = line_.p.y + addy;
	Vector3 endpos = Vector3{ line_.end.x + addx,line_.end.y + addy,0 };
	rightLine_.p = pos_;
	rightLine_.end = endpos;

	//左の線分の位置更新
	addx = cos(q - deg90) * LINE_WIDTH;
	addy = sin(q - deg90) * LINE_WIDTH;
	pos_.x = line_.p.x + addx;
	pos_.y = line_.p.y + addy;
	endpos = Vector3{ line_.end.x + addx,line_.end.y + addy,0 };
	leftLine_.p = pos_;
	leftLine_.end = endpos;
}

void MagStick::PlayEffect(void)
{
	//磁石の状態でエフェクト変更
	switch (state_)
	{
	case MAGSTATE::None:
		if (!IsEffekseer3DEffectPlaying(northEffect_.playhandle))
		{
			StopEffekseer3DEffect(northEffect_.playhandle);
		}
		if (!IsEffekseer3DEffectPlaying(southEffect_.playhandle))
		{
			StopEffekseer3DEffect(southEffect_.playhandle);
		}
		SetPosPlayingEffekseer3DEffect(noneEffect_.playhandle, effectPos_.x, effectPos_.y, effectPos_.z - 40.0f);
		SetRotationPlayingEffekseer3DEffect(noneEffect_.playhandle, 0, 0, effectAngle_);
		break;
	case MAGSTATE::South:
	{
		if (IsEffekseer3DEffectPlaying(southEffect_.playhandle))
		{
			if (!IsEffekseer3DEffectPlaying(northEffect_.playhandle))
			{
				StopEffekseer3DEffect(northEffect_.playhandle);
			}
			southEffect_.playhandle = PlayEffekseer3DEffect(southEffect_.resource);
		}
		SetPosPlayingEffekseer3DEffect(southEffect_.playhandle, effectPos_.x, effectPos_.y, effectPos_.z - 40.0f);
		SetRotationPlayingEffekseer3DEffect(southEffect_.playhandle, 0, 0,effectAngle_);
		break;
	}
	case MAGSTATE::North:
		if (IsEffekseer3DEffectPlaying(northEffect_.playhandle))
		{
			if (!IsEffekseer3DEffectPlaying(southEffect_.playhandle))
			{
				StopEffekseer3DEffect(southEffect_.playhandle);
			}
			northEffect_.playhandle = PlayEffekseer3DEffect(northEffect_.resource);
		}
		SetPosPlayingEffekseer3DEffect(northEffect_.playhandle, effectPos_.x, effectPos_.y, effectPos_.z - 40.0f);
		SetRotationPlayingEffekseer3DEffect(northEffect_.playhandle, 0, 0, effectAngle_);
		break;
	default:
		break;
	}
}

bool MagStick::CheckWithinRange(Vector3 playerpos, Vector3 gimmickpos)
{
	if (playerpos.x - WITHIN_RANGE < gimmickpos.x && gimmickpos.x < playerpos.x + WITHIN_RANGE &&
		playerpos.y - WITHIN_RANGE < gimmickpos.y && gimmickpos.y < playerpos.y + WITHIN_RANGE)
	{
		return true;
	}
	return false;
}

void MagStick::SlowEnemyBullet(const EneymyList& enemy)
{
	//弾を減速させる
	for (auto& e : enemy)
	{
		//弾を撃つ敵のデータ以外はcontinue
		if (e->GetEnemyType() != EnemyType::SHOOT)
		{
			continue;
		}

		auto shoot = dynamic_cast<EnemyShoot*>(e.get());
		auto bullet = shoot->GetBulletData();
		if (state_ != MAGSTATE::None)
		{
			bullet->CollisionRodAndBullet(line_, rightLine_, leftLine_);
		}
	}
}

void MagStick::SetMouseIconPosition(void)
{
	//PAD接続中
	DINPUT_JOYSTATE rStick_Input;
	//右スティックの入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &rStick_Input);
	if ((rStick_Input.Rx != 0 || rStick_Input.Ry != 0) &&
		(abs(rStick_Input.Rx) >= LOWER_LIMIT || abs(rStick_Input.Ry) >= LOWER_LIMIT))
	{
		Vector2 vec = Vector2{ static_cast<float>(rStick_Input.Rx),static_cast<float>(rStick_Input.Ry) };
		vec.Normalize();
		Vector2 addPos = vec * ROD_LENGTH;
		auto screenPos = ConvWorldPosToScreenPos(VGet(line_.p.x, line_.p.y, line_.p.z));
		auto totalWorldPos = ConvScreenPosToWorldPos(VGet(screenPos.x + addPos.x, screenPos.y + addPos.y, screenPos.z));
		//マウスの位置更新
		endPos_.x = totalWorldPos.x;
		endPos_.y = totalWorldPos.y;
		endPos_.z = 0.0f;

		oldAddPos_ = addPos;
	}
	else
	{
		//傾けていない場合、前フレームのプレイヤーの位置から足す
		auto screenPos = ConvWorldPosToScreenPos(VGet(line_.p.x, line_.p.y, line_.p.z));
		auto totalWorldPos = ConvScreenPosToWorldPos(VGet(screenPos.x + oldAddPos_.x, screenPos.y + oldAddPos_.y, screenPos.z));
		endPos_.x = totalWorldPos.x;
		endPos_.y = totalWorldPos.y;
	}
}

void MagStick::StopEffect(void)
{
	if (!IsEffekseer3DEffectPlaying(southEffect_.playhandle))
	{
		StopEffekseer3DEffect(southEffect_.playhandle);
	}
	if (!IsEffekseer3DEffectPlaying(northEffect_.playhandle))
	{
		StopEffekseer3DEffect(northEffect_.playhandle);
	}
	if (!IsEffekseer3DEffectPlaying(noneEffect_.playhandle))
	{
		StopEffekseer3DEffect(noneEffect_.playhandle);
	}
}

std::vector<VERTEX2DSHADER>& MagStick::GetTrailData(void)
{
	return trail_->GetVertexData();
}
