#pragma once
#include"../../Geometry.h"

class TmxObj;

class Gimmick
{
public:
    Gimmick();
    virtual ~Gimmick();

    // ����������
    virtual void Init(TmxObj& tmx) = 0;

    // �X�V�X�e�b�v
    virtual void Update(void) = 0;

    // �`�揈��
    virtual void Draw(Vector2 offset) = 0;

    // �������
    virtual void Release(void) = 0;

};
