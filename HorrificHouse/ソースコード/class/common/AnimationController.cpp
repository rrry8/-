#include <DxLib.h>
#include "AnimationController.h"

AnimationController::AnimationController(int modelId)
{
	model_ = modelId;
	animType_ = -1;
	mIsLoop = false;
	mIsStop = false;
	playType = PLAY_TYPE::NORMAL;
	animState_ = ANIM_STATE::STOPPING;
}

AnimationController::~AnimationController()
{
	playAnim_.attachNo = MV1DetachAnim(model_, playAnim_.attachNo);
}

void AnimationController::Add(int type, std::string path, float speed)
{
	Animation anim;

	anim.model = MV1LoadModel(path.c_str());
	anim.attachNo = type;
	anim.speed = speed;

	if (animations_.count(type) == 0)
	{
		// 追加
		animations_.emplace(type, anim);
	}
	else
	{
		// 入れ替え
		animations_[type].model = anim.model;
		animations_[type].attachNo = anim.attachNo;
		animations_[type].totalTime = anim.totalTime;
	}
}

void AnimationController::Add(int type, int handle, float speed)
{
	Animation anim;
	//ハンドル
	anim.model = handle;
	//アニメーション種類
	anim.attachNo = type;
	//再生速度
	anim.speed = speed;

	if (animations_.count(type) == 0)
	{
		// 追加
		animations_.emplace(type, anim);
	}
	else
	{
		// 入れ替え
		animations_[type].model = anim.model;
		animations_[type].attachNo = anim.attachNo;
		animations_[type].totalTime = anim.totalTime;
	}
}

void AnimationController::Play(int type, bool isLoop)
{
	if (animType_ != type) {

		if (animType_ != -1)
		{
			// モデルからアニメーションを外す
			playAnim_.attachNo = MV1DetachAnim(model_, playAnim_.attachNo);
		}

		// アニメーション種別を変更
		animType_ = type;
		playAnim_ = animations_[type];

		// 初期化
		playAnim_.frame = 0.0f;

		// モデルにアニメーションを付ける
		playAnim_.attachNo = MV1AttachAnim(model_, 0, playAnim_.model);

		// アニメーション総時間の取得
		playAnim_.totalTime = MV1GetAttachAnimTotalTime(model_, playAnim_.attachNo);
		
		//再生方法
		playType = PLAY_TYPE::NORMAL;
		animState_ = ANIM_STATE::PLAYING;

		// アニメーションループ
		mIsLoop = isLoop;
	}
}

void AnimationController::Play(int type, int num, bool isLoop)
{
	if (animType_ != type) {

		if (animType_ != -1)
		{
			// モデルからアニメーションを外す
			playAnim_.attachNo = MV1DetachAnim(model_, playAnim_.attachNo);
		}

		// アニメーション種別を変更
		animType_ = type;
		playAnim_ = animations_[type];

		// 初期化
		playAnim_.frame = 0.0f;

		// モデルにアニメーションを付ける
		playAnim_.attachNo = MV1AttachAnim(model_, num, playAnim_.model);

		// アニメーション総時間の取得
		playAnim_.totalTime = MV1GetAttachAnimTotalTime(model_, playAnim_.attachNo);

		//再生方法
		playType = PLAY_TYPE::NORMAL;
		animState_ = ANIM_STATE::PLAYING;

		// アニメーションループ
		mIsLoop = isLoop;
	}
}

//逆再生
void AnimationController::PlayBack(int type, bool isLoop)
{
	if (animType_ != type) {

		if (animType_ != -1)
		{
			// モデルからアニメーションを外す
			playAnim_.attachNo = MV1DetachAnim(model_, playAnim_.attachNo);
		}

		// アニメーション種別を変更
		animType_ = type;
		playAnim_ = animations_[type];

		// モデルにアニメーションを付ける
		playAnim_.attachNo = MV1AttachAnim(model_, type, playAnim_.model);

		// アニメーション総時間の取得
		playAnim_.totalTime = MV1GetAttachAnimTotalTime(model_, playAnim_.attachNo);
		
		// 初期化
		//※逆再生なのでフレーム数を最大値に
		playAnim_.frame = playAnim_.totalTime;


		//再生方法
		playType = PLAY_TYPE::REVERSE;

		// アニメーションループ
		mIsLoop = isLoop;
	}
}

void AnimationController::Update(double delta)
{
	// アニメーション終了判定
	bool isEnd = false;

	if (!mIsStop)
	{
		// 通常再生の場合
		if (playType == PLAY_TYPE::NORMAL)
		{
			playAnim_.frame += (delta * playAnim_.speed);

			if (playAnim_.frame > playAnim_.totalTime)
			{
				isEnd = true;
			}
			if (isEnd)
			{
				// アニメーションが終了したら
				if (mIsLoop)
				{
					// ループ再生
					playAnim_.frame = 0.0f;
				}
				else
				{
					animState_ = ANIM_STATE::STOPPING;
					// ループしない
					playAnim_.frame = playAnim_.totalTime;
				}
			}

		}
		// 逆再生の場合
		//else if (playType == PLAY_TYPE::REVERSE)
		//{
		//	playAnim_.frame -= (delta * playAnim_.speed);

		//	if (playAnim_.frame < 0.0f)
		//	{
		//		isEnd = true;
		//	}
		//	if (isEnd)
		//	{
		//		// アニメーションが終了したら
		//		if (mIsLoop)
		//		{
		//			// ループ再生
		//			playAnim_.frame = playAnim_.totalTime;
		//		}
		//		else
		//		{
		//			// ループしない
		//			playAnim_.frame = 0.0f;
		//		}
		//	}
		//}
	}

	// アニメーション設定
	MV1SetAttachAnimTime(model_, playAnim_.attachNo, playAnim_.frame);
}


void AnimationController::Release(void)
{
	for (auto& anim : animations_)
	{
		MV1DeleteModel(anim.second.model);
	}
}


ANIM_STATE AnimationController::GetPlayState(void)
{
	return animState_;
}


