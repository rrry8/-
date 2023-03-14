#pragma once
#include <string>
#include <iostream>
#include <string>
#include <algorithm>
#include "Unit/Unit.h"
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"
#include "../Common/ModelMng.h"

struct CollisionRay
{

	MV1_COLL_RESULT_POLY operator()(Line& line, const rapidxml::xml_node<>* node,int id)
	{
		// �����ID���Ȃ��ꍇ�̊֐�
		// Unit�̃R���W�����̍X�V�͂���ĂȂ�
		// �X�V�������ꍇ���̊֐���ǂ��
		// ���΂̃I�u�W�F�N�g�Ɣ�������

		std::string name = node->first_attribute("name")->value();
		auto w = static_cast<float>(atof(node->first_attribute("w")->value()));//���C�̒���

		auto clampX = atof(node->first_attribute("clampX")->value());//���S�����X
		auto clampY = atof(node->first_attribute("clampY")->value());//���S�����Y
		Line posLine;
		//���C���ɕύX
		posLine = { line.p,line.end };
		//�v���C���[�̒��S���W�܂ŌŒ�l�ň����グ�Ă�
		auto centerX = atof(node->first_attribute("Checkcenterx")->value());//���S�����X
		auto centerY = atof(node->first_attribute("Checkcentery")->value());//���S�����Y
		//�Ԃ��p
		MV1_COLL_RESULT_POLY hitData{};
		
			auto hit = MV1CollCheck_Line(id, -1, posLine.p.toVECTOR(), posLine.end.toVECTOR());
			if (hit.HitFlag > 0)
			{
				hitData = hit;
			}
			//MV1DeleteModel(id);
		return hitData;
	}

	MV1_COLL_RESULT_POLY operator()(Line& line, const rapidxml::xml_node<>* node,std::string path,int num)
	{
		// �����ID���Ȃ��ꍇ�̊֐�
		// Unit�̃R���W�����̍X�V�͂���ĂȂ�
		// �X�V�������ꍇ���̊֐���ǂ��
		// ���΂̃I�u�W�F�N�g�Ɣ�������

		std::string name = node->first_attribute("name")->value();
		auto w = static_cast<float>(atof(node->first_attribute("w")->value()));//���C�̒���

		auto clampX = atof(node->first_attribute("clampX")->value());//���S�����X
		auto clampY = atof(node->first_attribute("clampY")->value());//���S�����Y
		Line posLine;
		//���C���ɕύX
		posLine = { line.p,line.end };
		//�v���C���[�̒��S���W�܂ŌŒ�l�ň����グ�Ă�
		auto centerX = atof(node->first_attribute("Checkcenterx")->value());//���S�����X
		auto centerY = atof(node->first_attribute("Checkcentery")->value());//���S�����Y
		//�Ԃ��p
		MV1_COLL_RESULT_POLY hitData{};
		
			auto hit = MV1CollCheck_Line(lpModelMng.GetID(path)[num], -1, posLine.p.toVECTOR(), posLine.end.toVECTOR());
			if (hit.HitFlag > 0)
			{
				hitData = hit;
			}
			//MV1DeleteModel(id);
		return hitData;
	}

	MV1_COLL_RESULT_POLY operator()(Line& line, int id)
	{
		MV1_COLL_RESULT_POLY hitData{};

		auto hit = MV1CollCheck_Line(id, -1, line.p.toVECTOR(), line.end.toVECTOR());
		if (hit.HitFlag > 0)
		{
			hitData = hit;
		}
		return hitData;
	}

	MV1_COLL_RESULT_POLY operator()(Line& line, const rapidxml::xml_node<>* node, std::vector<int>list)
	{
		// �����ID���Ȃ��ꍇ�̊֐�
		// Unit�̃R���W�����̍X�V�͂���ĂȂ�
		// �X�V�������ꍇ���̊֐���ǂ��
		// ���΂̃I�u�W�F�N�g�Ɣ�������
		std::string name = node->first_attribute("name")->value();
		auto w = static_cast<float>(atof(node->first_attribute("w")->value()));//���C�̒���

		auto clampX = atof(node->first_attribute("clampX")->value());//���S�����X
		auto clampY = atof(node->first_attribute("clampY")->value());//���S�����Y
		Line posLine;
		//���C���ɕύX
		posLine = { line.p,line.end };
		//�v���C���[�̒��S���W�܂ŌŒ�l�ň����グ�Ă�
		auto centerX = atof(node->first_attribute("Checkcenterx")->value());//���S�����X
		auto centerY = atof(node->first_attribute("Checkcentery")->value());//���S�����Y
		//�Ԃ��p
		MV1_COLL_RESULT_POLY hitData{};
		for (auto StageCollId : list)
		{
			auto hit = MV1CollCheck_Line(StageCollId, -1, posLine.p.toVECTOR(), posLine.end.toVECTOR());
			if (hit.HitFlag > 0)
			{
				hitData = hit;
			}
		}
		return hitData;
	}

	MV1_COLL_RESULT_POLY operator()(Vector3& pos, Vector3 moveVec ,Vector3 movePower, const rapidxml::xml_node<>* node, std::vector<std::vector<int>>list)
	{
		//�}�b�v�S�Ăɓ����蔻�������֐�
		auto VecPow = moveVec * movePower;
		//MV1RefreshCollInfo(thisId);
		std::string name = node->first_attribute("name")->value();
		auto w = static_cast<float>(atof(node->first_attribute("w")->value()));//���C�̒���

		auto clampX = static_cast<float>(atof(node->first_attribute("clampX")->value()));//���S�����X
		auto clampY = static_cast<float>(atof(node->first_attribute("clampY")->value()));//���S�����Y

		if (clampX != 0 && clampY != 0)
		{
			VecPow.x = std::clamp(VecPow.x, -clampX, clampX);
			VecPow.y = std::clamp(VecPow.y, -clampY, clampY);
		}
		Line posLine;
		//���C���ɕύX
		posLine = { pos,pos + (VecPow * w) };
		//�v���C���[�̒��S���W�܂ŌŒ�l�ň����グ�Ă�
		auto centerX = atof(node->first_attribute("Checkcenterx")->value());//���S�����X
		auto centerY = atof(node->first_attribute("Checkcentery")->value());//���S�����Y
		posLine.p.y += static_cast<float>(centerY);
		posLine.end.y += static_cast<float>(centerY);
		//�Ԃ��p
		MV1_COLL_RESULT_POLY hitData{};
		for (auto& Stagelist : list)
		{
			for (auto StageCollId : Stagelist)
			{
				auto hit = MV1CollCheck_Line(StageCollId, -1, posLine.p.toVECTOR(), posLine.end.toVECTOR());
				if (hit.HitFlag > 0)
				{
					hitData = hit;
				}
			}

		}
		return hitData;
	}

	MV1_COLL_RESULT_POLY operator()(Vector3& pos, Vector3 moveVec, Vector3 movePower, const rapidxml::xml_node<>* node, std::vector<std::vector<int>>list,bool flag)
	{
		//�}�b�v�S�Ăɓ����蔻�������֐�
		auto VecPow = moveVec * movePower;
		//MV1RefreshCollInfo(thisId);
		std::string name = node->first_attribute("name")->value();
		auto w = static_cast<float>(atof(node->first_attribute("w")->value()));//���C�̒���

		auto clampX = static_cast<float>(atof(node->first_attribute("clampX")->value()));//���S�����X
		auto clampY = static_cast<float>(atof(node->first_attribute("clampY")->value()));//���S�����Y

		if (clampX != 0 && clampY != 0)
		{
			VecPow.x = std::clamp(VecPow.x, -clampX, clampX);
			VecPow.y = std::clamp(VecPow.y, -clampY, clampY);
		}
		Line posLine;
		//���C���ɕύX
		posLine = { pos,pos + (VecPow * w) };
		//�v���C���[�̒��S���W�܂ŌŒ�l�ň����グ�Ă�
		auto centerX = atof(node->first_attribute("Checkcenterx")->value());//���S�����X
		auto centerY = atof(node->first_attribute("Checkcentery")->value());//���S�����Y
		posLine.p.y +=static_cast<float>(centerY);
		posLine.end.y += static_cast<float>(centerY);
		//�Ԃ��p
		MV1_COLL_RESULT_POLY hitData{};
		for (auto& Stagelist : list)
		{
			for (auto StageCollId : Stagelist)
			{
				auto hit = MV1CollCheck_Line(StageCollId, -1, posLine.p.toVECTOR(), posLine.end.toVECTOR());
				if (hit.HitFlag > 0)
				{
					hitData = hit;
				}
			}

		}
		return hitData;
	}
	MV1_COLL_RESULT_POLY operator()(Vector3& pos, Vector3 moveVec, Vector3 movePower, const rapidxml::xml_node<>* node, std::vector<int>list)
	{
		//���΂ȂǁA����̕��Ɠ����蔻�肷��֐�
		auto VecPow = moveVec * movePower;
		//MV1RefreshCollInfo(thisId);
		std::string name = node->first_attribute("name")->value();
		auto w = static_cast<float>(atof(node->first_attribute("w")->value()));//���C�̒���

		auto clampX = static_cast<float>(atof(node->first_attribute("clampX")->value()));//���S�����X
		auto clampY = static_cast<float>(atof(node->first_attribute("clampY")->value()));//���S�����Y

		if (clampX != 0 && clampY != 0)
		{
			VecPow.y = std::clamp(VecPow.y, -clampY, clampY);
		}
		Line posLine;
		//���C���ɕύX
		posLine = { pos,pos + (VecPow * (w)) };
		//�v���C���[�̒��S���W�܂ŌŒ�l�ň����グ�Ă�
		auto centerX = atof(node->first_attribute("Checkcenterx")->value());//���S�����X
		auto centerY = atof(node->first_attribute("Checkcentery")->value());//���S�����Y
		/*posLine.p.x += centerX;
		posLine.end.x += centerX;*/
		posLine.p.y += static_cast<float>(centerY);
		posLine.end.y += static_cast<float>(centerY);
		//�Ԃ��p
		MV1_COLL_RESULT_POLY hitData{};
		for (auto StageCollId : list)
		{
			auto hit = MV1CollCheck_Line(StageCollId, -1, posLine.p.toVECTOR(), posLine.end.toVECTOR());
			if (hit.HitFlag > 0)
			{
				hitData = hit;
			}
		}
		return hitData;
	}

	MV1_COLL_RESULT_POLY operator()(Vector3& pos, Vector3 moveVec, Vector3 movePower, const rapidxml::xml_node<>* node, std::vector<int>list,int& hitid)
	{
		//���΂ȂǁA����̕��Ɠ����蔻�肷��֐�
		auto VecPow = moveVec * movePower;
		//MV1RefreshCollInfo(thisId);
		std::string name = node->first_attribute("name")->value();
		auto w = static_cast<float>(atof(node->first_attribute("w")->value()));//���C�̒���

		auto clampX = static_cast<float>(atof(node->first_attribute("clampX")->value()));//���S�����X
		auto clampY = static_cast<float>(atof(node->first_attribute("clampY")->value()));//���S�����Y

		if (clampX != 0 && clampY != 0)
		{
			VecPow.y = std::clamp(VecPow.y, -clampY, clampY);
		}
		Line posLine;
		//���C���ɕύX
		posLine = { pos,pos + (VecPow * (w)) };
		//�v���C���[�̒��S���W�܂ŌŒ�l�ň����グ�Ă�
		auto centerX = atof(node->first_attribute("Checkcenterx")->value());//���S�����X
		auto centerY = atof(node->first_attribute("Checkcentery")->value());//���S�����Y
		/*posLine.p.x += centerX;
		posLine.end.x += centerX;*/
		posLine.p.y += static_cast<float>(centerY);
		posLine.end.y += static_cast<float>(centerY);
		//�Ԃ��p
		MV1_COLL_RESULT_POLY hitData{};
		for (auto StageCollId : list)
		{
			auto hit = MV1CollCheck_Line(StageCollId, -1, posLine.p.toVECTOR(), posLine.end.toVECTOR());
			if (hit.HitFlag > 0)
			{
				hitid = StageCollId;
				hitData = hit;
				return hitData;
			}
		}
		return hitData;
	}
};

//�}�E�X�J�[�\����
//UI�Ƃ̓����蔻��
struct CheckCollM
{
	bool operator()(Raycast& raycast_, Vector2Int& pos, const rapidxml::xml_node<>* node, const Collision& coll)
	{

		//��ŏ�������
		auto w = static_cast<float>(atof(node->first_attribute("w")->value()));//���C�̒���
		float speedX = 5;

		auto centerX = atof(node->first_attribute("Checkcenterx")->value());//���S�����X
		auto centerY = atof(node->first_attribute("Checkcentery")->value());//���S�����Y

		////�܂Ƃ߂Ă�
		//Vector2 halfsize = Vector2Int{ static_cast<float>(centerX),static_cast<float> (centerY) };
		//Vector2Int cntminmax = { static_cast<int>(centerX),static_cast<int>(centerY) };//���[�v�̉񐔂̍ő��-�̍ŏ�

		//coll�̃f�o�b�O�\��
		//_dbgDrawBox(coll.first.x, coll.first.y, coll.first.x + coll.second.x, coll.first.y + coll.second.y, 0xfffff0, false);//�R���W�����̕`��
		//�������Ă邩
		if (pos.x > coll.first.x &&
			pos.y > coll.first.y &&
			pos.x < coll.first.x + coll.second.x &&
			pos.y < coll.first.y + coll.second.y)
		{
			//�������Ă�����

			return true;
		}

		//�������ĂȂ�������
		return false;
	}
};


struct CollisionSphere
{
	/// <summary>
	/// �e�ƓG�̓����蔻��
	/// </summary>
	/// <param name="spherepos">���̈ʒu</param>
	/// <param name="radius">���̔��a</param>
	/// <param name="path">���f���p�X</param>
	/// <param name="num">���f���i���o�[</param>
	/// <returns>true:������/false:�������Ă��Ȃ�</returns>
	bool operator()(Vector3 spherepos, float radius, std::string path, int num)
	{
		bool hitFlag = false;
		MV1_COLL_RESULT_POLY_DIM colls{};

		colls = MV1CollCheck_Sphere(lpModelMng.GetID(path)[num], -1, spherepos.toVECTOR(), radius);

		if (colls.HitNum > 0)
		{
			hitFlag = true;
		}

		//�����蔻��̌�n��������
		MV1CollResultPolyDimTerminate(colls);

		return hitFlag;
	}

	bool operator()(Vector3 spherepos, float radius,std::vector<int> stage)
	{
		bool hitF = false;
		MV1_COLL_RESULT_POLY_DIM colls{};
		for (auto& s : stage)
		{
			colls = MV1CollCheck_Sphere(s, -1, spherepos.toVECTOR(), radius);

			if (colls.HitNum > 0)
			{
				hitF = true;
			}
		}

		//�����蔻��̌�n��������
		MV1CollResultPolyDimTerminate(colls);

		return hitF;
	}

};