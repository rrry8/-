#pragma once

#include<vector>
#include<list>
#include"../../../Common/Geometry.h"
#include"../../../Input/Controller.h"

enum class MAGSTATE;

class MagStickTrail
{
public:
    MagStickTrail();
    ~MagStickTrail();

    //������
    bool Init(void);
    //�X�V
    void Update(Line playerpos);
    //�`��
    void Draw(void);
    //���_�f�[�^���擾
    std::vector<VERTEX2DSHADER>& GetVertexData(void);

    //���S�_�擾
    Vector2 GetCenterPos(void);

private:

    //���_�f�[�^��ݒ�
    void SetVertex(const std::list<Vector2>& ePos);

    //�O�Ղ̃f�[�^��vector�ŊǗ�
    std::vector<VERTEX2DSHADER> trail_;

    //�ʒu���L�����Ă���`��(�폜��}�����J��Ԃ�����list)
    std::list<Vector2> posList_;
};
