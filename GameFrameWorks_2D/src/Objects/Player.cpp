
#include "Player.h"

using namespace frameworks::object;


Player::Player(const Vec2f& pos) : Object(pos) {
	transform.pos = pos;
	transform.scale = Vec2f(100, 100);
	time = 0;
	direction = Bottom;
	accela = Vec2f(0.5, 0.5);
	velocity = Vec2f(0, 0);
}

void Player::Update() {
	
	//  移動処理
	//  重力が上下だった場合
	if (direction == GravityDirection::Top || direction == GravityDirection::Bottom){
		if (Env().isPressKey(KEY_L)){
			transform.pos.x() -= Move;
		}
		if (Env().isPressKey(KEY_R)){
			transform.pos.x() += Move;
		}
	}

	//  重力が左右だった場合
	if (direction == GravityDirection::Right || direction == GravityDirection::Left){
		if (Env().isPressKey(KEY_U)){
			transform.pos.y() += Move;
		}
		if (Env().isPressKey(KEY_D)){
			transform.pos.y() -= Move;
		}
	}

	//  ギミック発動
	//  5フレームは発動しない
	if (time == 0){
		if (Env().isPushKey(SPACE)){
			if (direction == GravityDirection::Bottom){
				direction = GravityDirection::Left;
			}
			else if (direction == GravityDirection::Left){
				direction = GravityDirection::Top;
			}
			else if (direction == GravityDirection::Top){
				direction = GravityDirection::Right;
			}
			else if (direction == GravityDirection::Right){
				direction = GravityDirection::Bottom;
			}
			GravityReset();
			time = KeyActiveTime;
		}
	}



	

	if (direction == GravityDirection::Top){
		transform.pos.y() += velocity.y();
	}

	if (direction == GravityDirection::Right){
		transform.pos.x() += velocity.x();
	}

	if (direction == GravityDirection::Bottom){
		transform.pos.y() -= velocity.y();
	}

	if (direction == GravityDirection::Left){
		transform.pos.x() -= velocity.x();
	}


	velocity += accela;

	if (transform.pos.y() <= -400)
	{
		transform.pos.y() = -400;
	}

	if (transform.pos.y() >= 400)
	{
		transform.pos.y() = 400;
	}

	if (transform.pos.x() <= -400)
	{
		transform.pos.x() = -400;
	}

	if (transform.pos.x() >= 400)
	{
		transform.pos.x() = 400;
	}


	//  timeが0以下にならないように
	if (time > 0) time--;
}


void Player::Draw() {
	drawFillBox(transform.pos.x(),transform.pos.y(),
				transform.scale.x(),transform.scale.y(),Color::red);
}


void Player::GravityReset(){
	velocity = Vec2f(0, 0);
}

void Player::Translate(frameworks::Transform transform){

  if (direction == Top){
    this->transform.pos.y() -= velocity.y()-accela.y();
  }

  if (direction == Right){
    this->transform.pos.x() -= velocity.x() - accela.x();
  }
  if (direction == Bottom){
    this->transform.pos.y() += velocity.y() - accela.y();
  }
  if (direction == Left){
    this->transform.pos.x() += velocity.x() - accela.x();
  }

   
}