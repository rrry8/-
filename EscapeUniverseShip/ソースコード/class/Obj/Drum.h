#pragma once
#include<vector>
#include<chrono>
#include"Gimmick.h"
#include"../../Geometry.h"

class Camera;

//�h�����̍\����
struct DrumObj
{
    Vector2 pos;     //�ʒu
    double accel;    //�����x
    double angle;    //�p�x
    double time;     //�_�ł̕b��
    int alpha;       //���l
    bool alive = false;    //�����t���O
    std::chrono::system_clock::time_point elapTime;    //�������Ă���o��������
};

class Drum:
    public Gimmick
{
public:
    Drum();
    ~Drum();

    // ����������
    void Init(TmxObj& tmx) override;

    //�󋵃��Z�b�g
    void Reset(TmxObj& tmx);

    // �X�V�X�e�b�v
    void Update(void) override;

    // �X�V�X�e�b�v
    void UpdateAlive(TmxObj& tmx);

    // �`�揈��
    void Draw(Vector2 offset) override;

    // �������
    void Release(void) override;

    //�h�����ʂ̂h�c���ǂ����`�F�b�N
    bool CheckDrum(int gid);

private:

    //�^�C���n���h��
    int tileImage_;

    //�h�����ʂ̃n���h��
    int drumImage_;

    //�h�����ʂ̏��
    std::vector<DrumObj> drum_;

    //�f���^
    std::chrono::system_clock::time_point nowTime_, oldTime_;
};