#pragma once

#include<string>
#include<map>
#include<vector>
#include<list>
#include"../Geometry.h"
#include"rapidxml.hpp"
#include"rapidxml_utils.hpp"
#include"../class/Other/Collision.h"

//string��name�Eint��csv�̃f�[�^�E
using MapData = std::map <std::string, std::vector<int>>;

class TmxObj
{
public:
	TmxObj();
	~TmxObj();
	bool LoadTsx(std::string FileName);
	bool LoadTmx(std::string FileName);
	bool SetMap(void);

	const Vector2& GetWorldArea(void);
	const Vector2& GetTileSize(void);
	const unsigned int GetLayerSize(void);
	const unsigned int GetFirstGID(void);

	//�}�b�v�f�[�^�擾
	const MapData& GetMapData(void);
	const int GetMapData(std::string layer, Vector2 pos);	//pos�͍��W
	const int GetMapData(std::string layer, int x, int y);	//x,y�͏���

	//�R���W�����擾
	CollList& GetCollList(void);		
	CollList& GetCollTrap(void);		//�g���b�v
	CollList& GetCollCheck(void);		//�`�F�b�N�|�C���g
	CollList& GetCollRotateTrap(void);	//��]����g���b�v
	DrumCollList& GetCollDrum(void);

private:
	bool CheckTiledVersion(rapidxml::xml_node<>* node);

	std::map<std::string, int> version_;

	//TMX
	rapidxml::xml_document<> tmxDoc_;
	unsigned int firstGID_;		//GID
	unsigned int layerSize_;	//nextlayerID-1
	Vector2 worldArea_;			//width
	Vector2 tileSize_;			//tilesize
	MapData mapData_;

	//TSX
	rapidxml::xml_document<> tsxDoc_;

	//�����蔻��
	CollList collList_;		//�X�e�[�W�S��
	CollList collTrap_;		//�̂�����
	CollList collRotTrap_;	//��]�̂�����
	CollList collCheck_;	//���ԃ|�C���g
	DrumCollList collDrum_;	//�h������
};
