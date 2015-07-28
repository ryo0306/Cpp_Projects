
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
	
	//  �ړ�����
	//  �d�͂��㉺�������ꍇ
	if (direction == Top || direction == Bottom){
		if (Env().isPressKey(KEY_L)){
			transform.pos.x() -= Move;
		}
		if (Env().isPressKey(KEY_R)){
			transform.pos.x() += Move;
		}
	}

	//  �d�͂����E�������ꍇ
	if (direction == Right || direction == Left){
		if (Env().isPressKey(KEY_U)){
			transform.pos.y() += Move;
		}
		if (Env().isPressKey(KEY_D)){
			transform.pos.y() -= Move;
		}
	}

	//  �M�~�b�N����
	//  5�t���[���͔������Ȃ�
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


	//  time��0�ȉ��ɂȂ�Ȃ��悤��
	if (time > 0) time--;
}


void Player::Draw() {
	drawFillBox(transform.pos.x(),transform.pos.y(),
				100,100,Color::red);
}


void Player::GravityReset(){
	velocity = Vec2f(0, 0);
}