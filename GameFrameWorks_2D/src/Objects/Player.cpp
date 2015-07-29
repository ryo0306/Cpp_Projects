
#include "Player.h"

using namespace frameworks;
using namespace frameworks::object;


Player::Player() :
time(0),
gravityState(GravityDirection::Bottom),
direction(Move_R),
acceleration(0.25f),
velocity(0.0f) {
  textureID.clear();

  transform.scale = Vec2f::Ones() * 50.0f;
  transform.rotate = transform.angle = 0.0f;

  const std::string TextureTable[] = {
    "res/png/ochirukun_red.png",
    "res/png/ochirukun_blue.png",
    "res/png/ochirukun_yellow.png",
    "res/png/ochirukun_green.png",
  };

  for (const auto& data : TextureTable) {
    textureID.push_back(Asset().Append().Texture(data));
  }
}


void Player::Update() {
  using namespace utility;

  // �L�[���͗L���̃^�C���J�E���^�����炷
  if (IsKeyActive()) time--;

  // �M�~�b�N����
  // 5 �t���[���͔������Ȃ�
  if (!IsKeyActive() && Env().isPushKey(SPACE)) {
    time = KeyActiveTime;
  }

  // �d��
  GravityUpdate();

  // �u���b�N�Ƃ̏Փ˔���
  bool isHit = false;
  bool enableMove = false;

  const auto& pos = transform.pos;
  const auto& size = transform.scale;

  for (auto& block : stageBlocks) {
    isHit = IsHitRectToRect(pos, size, block.pos, block.size);
    if (!isHit) continue;

    velocity -= acceleration;
    switch (gravityState) {
      case GravityDirection::Top:
        transform.pos.y() -= velocity;
        break;

      case GravityDirection::Bottom:
        transform.pos.y() += velocity;
        break;

      case GravityDirection::Left:
        transform.pos.x() += velocity;
        break;

      case GravityDirection::Right:
        transform.pos.x() -= velocity;
        break;

      default: break;
    }

    GravityReset();
    enableMove = true;
  }

  if (!isHit && !enableMove) return;

  // �ړ�����
  if (gravityState == GravityDirection::Top ||
      gravityState == GravityDirection::Bottom) {
    if (Env().isPressKey(KEY_L)) { Move(Vec2f(-MoveSpeed, 0)); }
    if (Env().isPressKey(KEY_R)) { Move(Vec2f(MoveSpeed, 0)); }
  }
  if (gravityState == GravityDirection::Left ||
      gravityState == GravityDirection::Right) {
    if (Env().isPressKey(KEY_D)) { Move(Vec2f(0, -MoveSpeed)); }
    if (Env().isPressKey(KEY_U)) { Move(Vec2f(0, MoveSpeed)); }
  }
}


void Player::Draw() {
  const float Size = 512.0f;
  const auto texture = Asset().Find().Texture(gravityState);
  const auto pos = transform.pos + transform.scale * 0.5f;

  drawTextureBox(pos.x(), pos.y(),
                 transform.scale.x(), transform.scale.y(),
                 0, 0, Size, Size,
                 *texture, Color::white,
                 gravityState * (M_PI / 2),
                 Vec2f(direction, 1),
                 transform.scale * 0.5f);
}


void Player::GravityUpdate() {
  switch (gravityState) {
    case GravityDirection::Top:
      transform.pos.y() += velocity;
      break;

    case GravityDirection::Bottom:
      transform.pos.y() -= velocity;
      break;

    case GravityDirection::Left:
      transform.pos.x() -= velocity;
      break;

    case GravityDirection::Right:
      transform.pos.x() += velocity;
      break;

    default: break;
  }

  velocity += acceleration;
  if (velocity > 10.0f) velocity = 10.0f;
}


void Player::Move(const Vec2f& moveSpeed) {
  switch (gravityState) {
    case GravityDirection::Top:
      direction = moveSpeed.x() > 0 ? Move_L : Move_R;
      break;

    case GravityDirection::Bottom:
      direction = moveSpeed.x() > 0 ? Move_R : Move_L;
      break;

    case GravityDirection::Left:
      direction = moveSpeed.y() > 0 ? Move_L : Move_R;
      break;

    case GravityDirection::Right:
      direction = moveSpeed.y() > 0 ? Move_R : Move_L;
      break;

    default: break;
  }

  transform.pos += moveSpeed;
  if (DisableMove()) { transform.pos -= moveSpeed; }
}


const bool Player::DisableMove() {
  using namespace utility;
  const auto& Pos = transform.pos;
  const auto& Size = transform.scale;

  bool hitL, hitR, hitT, hitB;
  hitL = hitR = hitT = hitB = false;

  for (auto& block : stageBlocks) {
    const bool hit = IsHitRectToRect(Pos, Size,
                                     block.pos, block.size);

    if (!hit) continue;

    switch (gravityState) {
      case GravityDirection::Top:
        hitL = (Pos.x() + Size.x()) > block.pos.x();
        hitR = Pos.x() < (block.pos.x() + block.size.x());
        return hitL && hitR;

      case GravityDirection::Bottom:
        hitR = (Pos.x() + Size.x()) > block.pos.x();
        hitL = Pos.x() < (block.pos.x() + block.size.x());
        return hitL && hitR;

      case GravityDirection::Left:
        hitT = (Pos.y() + Size.y()) > block.pos.y();
        hitB = Pos.y() < (block.pos.y() + block.size.y());
        return hitB && hitT;

      case GravityDirection::Right:
        hitB = (Pos.y() + Size.y()) > block.pos.y();
        hitT = Pos.y() < (block.pos.y() + block.size.y());
        return hitB && hitT;

      default:;
    }
  }

  return false;
}
