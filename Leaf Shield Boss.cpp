#include "enemy bullet.cpp"
#pragma once

class LeafShieldBoss : public EnemyBullet {

	

	LeafShieldBoss(shared_ptr<Texture> t, Vector2f pos) {
		mov = shared_ptr<movable>(new movable(string("eBullet"), t, IntRect(3, 389, 47, 47), pos, Vector2f(4, 4)));

	}
};