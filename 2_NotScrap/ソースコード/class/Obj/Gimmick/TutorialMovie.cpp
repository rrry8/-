#include "TutorialMovie.h"
#include "../../Common/MovieMng.h"
#include "../Unit/Player.h"
#include "../../Common/ImageMng.h"

TutorialMovie::TutorialMovie(Vector3 pos, std::string str, int num, TutorialType type)
{
	
	MovieName_ = str;
	type_ = type;
	Init(pos, num);
}

TutorialMovie::~TutorialMovie()
{
}

bool TutorialMovie::Init(Vector3 pos, int num)
{
	speed_ = 20;
	pos_ = Vector3{ pos.x,2000,pos.z };
	//pos_ = Vector3{ 800, 600,300 };
	num_ = num;
	movedPosY_ = pos.y;
	moveLimitVal_ = { pos_.y , movedPosY_ };

	MV1SetPosition(lpModelMng.GetID("./source/Model/Stage/signbot2_new.mv1")[num], pos_.toVECTOR());
	MV1SetScale(lpModelMng.GetID("./source/Model/Stage/signbot2_new.mv1")[num], { 0.5,0.5,0.5 }/*{ 1.5,1.5,1.5 }*/);
	SetMovieVolumeToGraph(0, lpMovieMng.GetID(MovieName_)[0]);

	MV1SetTextureGraphHandle(lpModelMng.GetID("./source/Model/Stage/signbot2_new.mv1")[num_], 0, lpMovieMng.GetID(MovieName_)[0], false);
	return true;
}

void TutorialMovie::Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList,bool flag)
{
	cameraPos_ = PlyaerList[0]->GetPos();
	clearflag_ = flag;
	if (/*!GetMovieStateToGraph(lpMovieMng.GetID(MovieName_)[0])*/TellMovieToGraph(lpMovieMng.GetID(MovieName_)[0]) > 4500)
	{
		SeekMovieToGraph(lpMovieMng.GetID(MovieName_)[0], 0);
	}
	/*int su = TellMovieToGraph(lpMovieMng.GetID("move")[0]);
	TRACE("%d\n", su);*/
	
		if (CheckWithinRangeXonly(pos_, cameraPos_,500))
		{
			if (clearflag_)
			{
				MV1SetTextureGraphHandle(lpModelMng.GetID("./source/Model/Stage/signbot2_new.mv1")[num_], 0, lpImageMng.GetID("Smouse")[0], false);
				lpMovieMng.StopDrawMovie(MovieName_);

			}
			else
			{
				lpMovieMng.PlayOnMovie(MovieName_);
			}
				if (pos_.y >= moveLimitVal_.lower)
				{

					pos_.y -= speed_;


					speed_ *= 0.99;
					if (speed_ < 1.0f)
					{
						speed_ = 1.0f;
					}
				}
				

		}
		else
		{
			//if (flag)
			{
				lpMovieMng.StopDrawMovie(MovieName_);
			}
			speed_ = 20;
			if (pos_.y <= moveLimitVal_.upper)
			{
				pos_.y += speed_;
			}
		}
	
	MV1SetPosition(lpModelMng.GetID("./source/Model/Stage/signbot2_new.mv1")[num_], pos_.toVECTOR());
}

void TutorialMovie::Draw(void)
{
	//チュートリアルのほうで範囲チェックしている
	if (CheckWithinRangeXonly(pos_, cameraPos_))
	{
		MV1DrawModel(lpModelMng.GetID("./source/Model/Stage/signbot2_new.mv1")[num_]);
	}
	/*if (!flag_)
	{
		DrawExtendGraph(0, 0, 640, 480, lpMovieMng.GetID(MovieName_)[0], false);
	}*/
}
