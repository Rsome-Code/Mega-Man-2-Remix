#include "enemy bullet.cpp"
#pragma once

class LeafShieldBoss : public EnemyBullet {

	

	LeafShieldBoss(Texture* t, Vector2f pos) {
		mov = new movable(string("eBullet"), t, IntRect(3, 389, 47, 47), pos, Vector2f(4, 4));

	}
};