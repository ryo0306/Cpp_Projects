
#include "Stage.h"

using namespace frameworks::object;


Stage::Stage() {
  blocks.clear();
  textureID.clear();

  // �摜�̃e�[�u��
  // NOTICE: �ǂ�ȉ摜���́A���\�[�X�t�H���_���m�F���Ă�������
  const std::string TextureTable[] = {
    "res/png/ashiba_A.png",    // [0]�u���b�N�̏�������
    "res/png/ashiba_B.png",    // [1]��
    "res/png/ashiba_C.png",    // [2]��
    "res/png/ashiba_D.png",    // [3]�E
    "res/png/ashiba_E.png",    // [4]�Ȃ��F�\����p
    "res/png/ashiba_kA.png",   // [5]��ƉE
    "res/png/ashiba_kB.png",   // [6]�E�Ɖ�
    "res/png/ashiba_kC.png",   // [7]���ƍ�
    "res/png/ashiba_kD.png",   // [8]���Ə�
  };

  // �e�[�u������f�[�^��o�^�A�֘A�t�����ꂽ ID ��ێ�
  for (auto& data : TextureTable) {
    textureID.push_back(Asset().Append().Texture(data));
  }
  
  // �X�e�[�W�̏�����
  switch (GameData::Get().GetStageID()) {
    default: // *Debug Mode Only* Initialized Stage Data.
    case StageID::Stage1:

      // �摜 ID �̎��o����
      // textureID[] �ɁA��L�e�[�u���\�ɑΉ������ԍ�������� ID �������Ă��܂�

      // textureID[0] �ŁA��������u���b�N�����o����
      // ���������u���b�N�́AtextureID[1] �Ŏ��o���܂�
      // ����Ȋ����Ŏg�������摜�� ID ��R�Â����Ă�������
      blocks.push_back({ Vec2f(0, 0), Vec2f(100, 100), textureID[0] });

      break;

    case StageID::Stage2:
      break;

    case StageID::Stage3:
      break;
  }
}


void Stage::Update() {
  // do not something.
}


void Stage::Draw() {
  const float Size = 108.0f;
  for (const auto& block : blocks) {
    const auto texture = Asset().Find().Texture(block.id);
    drawTextureBox(block.pos.x(), block.pos.y(),
                   block.size.x(), block.size.y(),
                   0, 0, Size, Size, *texture);
  }
}
