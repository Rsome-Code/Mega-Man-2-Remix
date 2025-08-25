#include "enemy.cpp"
#pragma once

class Wolf :public enemy {
	using enemy::enemy;
	animation* teleportAnim;

	animation* idleAnim_down;
	animation* idleAnim_up;
	animation* shootAnim_up;
	animation* shootAnim_down;

	animTimer* teleportTimer;
	animTimer* idleTimer;

	bool teleporting = true;

public:
	void initial() {
		sprite->setRect(IntRect(0, 732, 67, 55));
		hp = 20;

		teleportAnim = new animation(list<IntRect>{IntRect(646, 727, 14, 80), IntRect(662, 760, 64, 14), IntRect(727, 751, 48, 32)}, sprite);
		idleAnim_down = new animation(list<IntRect>{IntRect(0, 728, 67, 59), IntRect(146, 728, 67, 59), IntRect(292, 728, 67, 59)}, sprite);
		idleAnim_up = new animation(list < IntRect>{ IntRect(73, 728, 67, 59), IntRect(219, 728, 67, 59), IntRect(365, 728, 67, 59) }, sprite);
		shootAnim_up = new animation(list<IntRect>{IntRect(511, 728, 67, 59)}, sprite);
		shootAnim_down = new animation(list<IntRect>{IntRect(438, 728, 67, 59)}, sprite);

		teleportTimer = new animTimer(teleportAnim, 15, false);
		idleTimer = new animTimer(idleAnim_down, 15, true);
		
	}
};