#include<sstream>
#include"../class/Other/ImageMng.h"
#include "TmxObj.h"
#include"../_debug/_DebugConOut.h"

TmxObj::TmxObj()
{
	version_["1.6.0"] = 1;
	version_["1.4.1"] = 1;
	firstGID_ = -1;
	layerSize_ = -1;
}

TmxObj::~TmxObj()
{
}

bool TmxObj::LoadTsx(std::string FileName)
{
	rapidxml::file<> xmlFileName = FileName.c_str();
	tsxDoc_.parse<0>(xmlFileName.data());
	rapidxml::xml_node<>* tileset = tsxDoc_.first_node("tileset");
	
	if (!CheckTiledVersion(tileset))
	{
		return false;
	}

	auto node = tsxDoc_.first_node();
	while (node)
	{
		if (node->name_size())
		{
			TRACE("%s→", node->name());
		}
		auto atr = node->first_attribute();
		while (atr)
		{
			if (atr->name_size())
			{
				TRACE("%s:", atr->name());
			}
			TRACE("%s ", atr->value());
			atr = atr->next_attribute();
		}
		TRACE("\n");
		node = node->next_sibling();
	}

	auto imageNode = tileset->first_node("image");
	std::string source = imageNode->first_attribute("source")->value();
	auto keyName = source.substr(source.find_last_of("/") + 1);
	
	auto tilewidth = atoi(tileset->first_attribute("tilewidth")->value());
	auto tileheight = atoi(tileset->first_attribute("tileheight")->value());

	auto columns = atoi(tileset->first_attribute("columns")->value());
	auto rows = atoi(tileset->first_attribute("tilecount")->value()) / columns;

	lpImageMng.GetmapID(source, keyName, { static_cast<float>(tilewidth),static_cast<float>(tileheight) }, { static_cast<float>(columns) ,static_cast<float>(rows) });
	return true;
}

bool TmxObj::LoadTmx(std::string FileName)
{
	rapidxml::file<> filename = FileName.c_str();
	tmxDoc_.parse<0>(filename.data());
	rapidxml::xml_node<>* mapnode =tmxDoc_.first_node("map");

	if (!CheckTiledVersion(mapnode))
	{
		return false;
	}

	auto tileset = mapnode->first_node("tileset");

	auto pass = FileName.substr(0, FileName.find_last_of("/") + 1);
	std::string source = pass + tileset->first_attribute("source")->value();

	if (!LoadTsx(source))
	{
		return false;
	}

	firstGID_ = atoi(tileset->first_attribute("firstgid")->value());
	layerSize_ = atoi(mapnode->first_attribute("nextlayerid")->value()) - 1;
	worldArea_ = { static_cast<float>(atoi(mapnode->first_attribute("width")->value())),static_cast<float>(atoi(mapnode->first_attribute("height")->value())) };
	tileSize_ = { static_cast<float>(atoi(mapnode->first_attribute("tilewidth")->value())),static_cast<float>(atoi(mapnode->first_attribute("tileheight")->value())) };

	return 	SetMap();
}

bool TmxObj::SetMap(void)
{
	rapidxml::xml_node<>* mapnode = tmxDoc_.first_node("map");
	if (mapnode == nullptr)
	{
		TRACE("mapノード取得エラー");
		return false;
	}

	for (auto LayerNode = mapnode->first_node("layer");
		LayerNode != nullptr;
		LayerNode = LayerNode->next_sibling())
	{

		if (LayerNode == nullptr)
		{
			TRACE("mapノード→layerノード取得エラー");
			return false;
		}

		std::string attributeName = LayerNode->first_attribute("name")->value();
		if (attributeName == "TrapColl")
		{
			continue;
		}
		if (attributeName == "DrumColl")
		{
			continue;
		}
		if (attributeName == "Coll")
		{
			continue;
		}
		if (attributeName == "CheckPointColl")
		{
			continue;
		}
		if (attributeName == "TrapCollRot")
		{
			continue;
		}

		auto layer = mapData_.try_emplace(LayerNode->first_attribute("name")->value());
		if (layer.second)
		{
			layer.first->second.resize(static_cast<int>(worldArea_.x * worldArea_.y));
		}

		auto layerData = LayerNode->first_node("data")->first_node();

		std::stringstream dataStream;
		dataStream << layerData->value();

		for (auto& data : layer.first->second)
		{
			std::string numstr;
			std::getline(dataStream, numstr, ',');
			data = atoi(numstr.c_str());
		}

		for (const auto& data : layer.first->second)
		{
			TRACE("%d ", data);
		}

		TRACE("No Problem\n");

	}

	float collX = 0.0f;
	float collY = 0.0f;
	float collW = 0.0f;
	float collH = 0.0f;

	auto CollNode = mapnode->first_node("objectgroup");
	if (CollNode == nullptr)
	{
		return true;
	}
	for (auto objectNode = CollNode->first_node();
		objectNode != nullptr;
		objectNode = objectNode->next_sibling())
	{
		collX = static_cast<float>(atof(objectNode->first_attribute("x")->value()));
		collY = static_cast<float>(atof(objectNode->first_attribute("y")->value()));
		collW = static_cast<float>(atof(objectNode->first_attribute("width")->value()));
		collH = static_cast<float>(atof(objectNode->first_attribute("height")->value()));

		collList_.push_back(
			{
				{ collX,collY },
				{ collW,collH }
			}
		);
	}
	auto collTrap = CollNode->next_sibling();
	auto center = 28.0f;
	for (auto objectNode = collTrap->first_node();
		objectNode != nullptr;
		objectNode = objectNode->next_sibling())
	{
		collX = static_cast<float>(atof(objectNode->first_attribute("x")->value()));
		collY = static_cast<float>(atof(objectNode->first_attribute("y")->value()));
		collW = static_cast<float>(atof(objectNode->first_attribute("width")->value()));
		collH = static_cast<float>(atof(objectNode->first_attribute("height")->value()));

		collTrap_.push_back(
			{
				{ collX + center,collY + center },
				{ collW / 2.0f,collH / 2.0f }
			}
		);
	}

	auto collDrum = collTrap->next_sibling();
	for (auto objectNode = collDrum->first_node();
		objectNode != nullptr;
		objectNode = objectNode->next_sibling())
	{
		collX = static_cast<float>(atof(objectNode->first_attribute("x")->value()));
		collY = static_cast<float>(atof(objectNode->first_attribute("y")->value()));
		collW = static_cast<float>(atof(objectNode->first_attribute("width")->value()));
		collH = static_cast<float>(atof(objectNode->first_attribute("height")->value()));

		collDrum_.push_back(
			{
				{{ collX ,collY  },
				{collW,collH}},true
			}
		);
	}

	auto collCheck = collDrum->next_sibling();
	for (auto objectNode = collCheck->first_node();
		objectNode != nullptr;
		objectNode = objectNode->next_sibling())
	{
		collX = static_cast<float>(atof(objectNode->first_attribute("x")->value()));
		collY = static_cast<float>(atof(objectNode->first_attribute("y")->value()));
		collW = static_cast<float>(atof(objectNode->first_attribute("width")->value()));
		collH = static_cast<float>(atof(objectNode->first_attribute("height")->value()));

		collCheck_.push_back(
			{
				{ collX,collY },
				{ collW,collH  }
			}
		);
	}
	auto collrot = collCheck->next_sibling();
	for (auto ObjectNode = collrot->first_node();
		ObjectNode != nullptr;
		ObjectNode = ObjectNode->next_sibling())
	{
		collX = static_cast<float>(atof(ObjectNode->first_attribute("x")->value()));
		collY = static_cast<float>(atof(ObjectNode->first_attribute("y")->value()));
		collW = static_cast<float>(atof(ObjectNode->first_attribute("width")->value()));
		collH = static_cast<float>(atof(ObjectNode->first_attribute("height")->value()));

		collRotTrap_.push_back(
			{
				{ collX + center,collY + center },
				{ collW / 2.0f,collH / 2.0f }
			}
		);
	}
	return true;
}

const Vector2& TmxObj::GetWorldArea(void)
{
	return worldArea_;
}

const Vector2& TmxObj::GetTileSize(void)
{
	return tileSize_;
}

const unsigned int TmxObj::GetLayerSize(void)
{
	return layerSize_;
}

const unsigned int TmxObj::GetFirstGID(void)
{
	return firstGID_;
}

const MapData& TmxObj::GetMapData(void)
{
	return mapData_;
}

const int TmxObj::GetMapData(std::string layer, Vector2 pos)
{
	return GetMapData(layer, static_cast<int>(pos.x / tileSize_.x), static_cast<int>(pos.y / tileSize_.y));
}

const int TmxObj::GetMapData(std::string layer, int x, int y)
{
	if (mapData_.count(layer) == 0)
	{
		return 0;
	}
	int point = y * static_cast<int>(worldArea_.x) + x;			//何番目の情報か
	if (point >= 0 && point < mapData_[layer].size())
	{
		return mapData_[layer][point];
	}
	else
	{
		return 0;
	}
}

CollList& TmxObj::GetCollList(void)
{
	return collList_;
}

CollList& TmxObj::GetCollTrap(void)
{
	return collTrap_;
}

CollList& TmxObj::GetCollCheck(void)
{
	return collCheck_;
}

CollList& TmxObj::GetCollRotateTrap(void)
{
	return collRotTrap_;
}

DrumCollList& TmxObj::GetCollDrum(void)
{
	return collDrum_;
}

bool TmxObj::CheckTiledVersion(rapidxml::xml_node<>* node)
{
	if (node == nullptr)
	{
		return false;
	}

	auto tiledversion = node->first_attribute("tiledversion");
	if (!version_.count(tiledversion->value()))
	{
		return false;
	}
	if (!version_[tiledversion->value()])
	{
		return false;
	}

	return true;
}
