
#include "Gimmick.h"

using namespace frameworks::object;


StageGimmick::StageGimmick(int direction,Vec2f pos) {
	this->direction = direction;
	transform.pos = pos;
}


void StageGimmick::Draw(){

	drawFillCircle(transform.pos.x(), transform.pos.y(),
					25, 25, 20, GetColor());

}


Color StageGimmick::GetColor(){
	Color color;

	switch (direction)
	{
	case Top:
		color = Color::red;
		break;
	case Right:
		color = Color::blue;
		break;
	case Bottom:
		color = Color::yellow;
		break;
	case Left:
		color = Color::green;
		break;
	default:
		color = Color::white;
		break;
	}

	return color;
}