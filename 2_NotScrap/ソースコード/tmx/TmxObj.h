#pragma once
#include<string>
#include<map>
#include<vector>
#include<list>
#include"../class/Common/Geometry.h"
#include"rapidxml.hpp"
#include"rapidxml_utils.hpp"
#include"../class/Common/Collision.h"

//stringはname・intはcsvのデータ・
using MapData = std::map <std::string, std::vector<int>>;

class TmxObj
{
public:
	TmxObj();
	~TmxObj();
	bool LoadTsx(std::string FileName);
	bool LoadTmx(std::string FileName);
	bool SetMap(void);

	const Vector2Int& GetWorldArea(void);
	const Vector2Int& GetTileSize(void);
	const unsigned int GetLayerSize(void);
	const unsigned int GetFirstGID(void);

	const MapData& GetMapData(void);
	const int GetMapData(std::string layer, Vector2Int pos);	//posは座標
	const int GetMapData(std::string layer, int x, int y);	//x,yは升目
	CollList& GetCollList(void);
	CollList& GetSouthCollList(void);
	CollList& GetNorthCollList(void);
	CollList& GetItemCollList(void);
	CollList& GetSubItemCollList(void);
	CollList& GetAllCollList(void);
private:
	bool CheckTiledVersion(rapidxml::xml_node<>* node);

	std::map<std::string, int> version_;

	//TMX
	rapidxml::xml_document<> tmxDoc_;
	unsigned int firstGID_;		//GID
	unsigned int layerSize_;	//nextlayerID-1
	Vector2Int worldArea_;			//width
	Vector2Int tileSize_;			//tilesize
	MapData mapData_;

	//TSX
	rapidxml::xml_document<> tsxDoc_;

	//当たり判定
	CollList collListAll_;
	CollList floorCollList_;
	CollList southMagnetCollList_;
	CollList northMagnetCollList_;
	CollList ItemCollList_;
	CollList subItemCollList_;
};

