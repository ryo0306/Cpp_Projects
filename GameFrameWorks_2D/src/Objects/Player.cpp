
#include "Player.h"

using namespace frameworks::object;


Player::Player(const Vec2f& pos) : Object(pos) {
	transform.pos = pos;
	time = 0;
	direction = Bottom;
	accela = Vec2f(0.5, 0.5);
	velocity = Vec2f(0, 0);
}

void Player::Update() {
	
	//  移動処理
	//  重力が上下だった場合
	if (direction == Top || direction == Bottom){
		if (Env().isPressKey(KEY_L)){
			transform.pos.x() -= Move;
		}
		if (Env().isPressKey(KEY_R)){
			transform.pos.x() += Move;
		}
	}

	//  重力が左右だった場合
	if (direction == Right || direction == Left){
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
			if (direction == Bottom){
				direction = Left;
			}
			else if (direction == Left){
				direction = Top;
			}
			else if (direction == Top){
				direction = Right;
			}
			else if (direction == Right){
				direction = Bottom;
			}
			GravityReset();
			time = KeyActiveTime;
		}
	}



	

	if (direction == Top){
		transform.pos.y() += velocity.y();
	}

	if (direction == Right){
		transform.pos.x() += velocity.x();
	}

	if (direction == Bottom){
		transform.pos.y() -= velocity.y();
	}

	if (direction == Left){
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
				100,100,Color::red);
}


void Player::GravityReset(){
	velocity = Vec2f(0, 0);
}