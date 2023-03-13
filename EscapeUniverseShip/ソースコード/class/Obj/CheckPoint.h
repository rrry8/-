#pragma once
#include "Gimmick.h"

class CheckPoint :
    public Gimmick
{
public:
    CheckPoint();
    ~CheckPoint();

    // ����������
    void Init(TmxObj& tmx) override;

    // �X�V�X�e�b�v
    void Update(void) override;

    //�`�F�b�N�|�C���g��ID�`�F�b�N
    bool CheckCheckPoint(int gid);

    //ID�̕ύX
    void ChangeID(int& gid);

    // �`�揈��
    void Draw(Vector2 offset) override;

    // �������
    void Release(void) override;

    //���ԃ|�C���g���B�Z�b�g
    void SetFlag(bool flag);

    //���ԃ|�C���g�ɓ��B���Ă��邩�Q�b�g
    bool GetFlag(void);

private:
    //�摜�n���h��
    int tileImage_;           //�^�C��
    int redCheckImage_;       //���ԃ|�C���g(�ʉߑO)
    int greenCheckImage_;     //���ԃ|�C���g(�ʉߌ�)

    //�`�F�b�N�|�C���g�ɓ��B���Ă��邩�ǂ���
    bool checkPointFlag_;
};
