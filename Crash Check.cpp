#include "crash tile.cpp"
#include "crash1.cpp"
#include "crash2.cpp"
#include "crash3.cpp"
#include "crash4.cpp"
#include "crash5.cpp"
#include "crash6.cpp"
#include "crash7.cpp"
#include "crash8.cpp"
#include "crash9.cpp"
#include "crash10.cpp"
#include "crash11.cpp"
#include "crash12.cpp"
#include "crash13.cpp"
#include "crash14.cpp"
#include "crash15.cpp"
#include "crash16.cpp"
#pragma once


//The function "checkCrash" will alter both crash tiles depending on their position relative to each other while keeping collision consistent.

static class CrashCheck {

public:
	static void checkCrash(shared_ptr<CrashTile>* newT, shared_ptr<CrashTile>* existing, Vector2f loc, shared_ptr<Texture> tex) {

		if (newT->get()->getLocation().x == existing->get()->getLocation().x) {
			if (newT->get()->getLocation().y == existing->get()->getLocation().y - 1) {
				checkPos(*newT, *existing, above);
			}
			else if (newT->get()->getLocation().y == existing->get()->getLocation().y + 1) {
				checkPos(*newT, *existing, below);
			}
		}
		if (newT->get()->getLocation().y == existing->get()->getLocation().y) {
			if (newT->get()->getLocation().x == existing->get()->getLocation().x - 1) {
				checkPos(*newT, *existing, leftOf);
			}
			else if (newT->get()->getLocation().x == existing->get()->getLocation().x + 1) {
				checkPos(*newT, *existing, rightOf);
			}
		}


		setTile(newT, loc, tex);
		setTile(existing, loc, tex);
	}

private:
	static void setTile(shared_ptr<CrashTile>* newT, Vector2f loc, shared_ptr<Texture> tex) {

		shared_ptr<Crash1> checkT = shared_ptr<Crash1>(new Crash1(loc, tex, 0));
		if (matchTile(newT, checkT)) {
			copyTile(newT, checkT);
		}

		else{
			shared_ptr<Crash2> checkT = shared_ptr<Crash2>(new Crash2(loc, tex, 1));
			if (matchTile(newT, checkT)) {
				copyTile(newT, checkT);
			}
			else {
				shared_ptr<Crash3> checkT = shared_ptr<Crash3>(new Crash3(loc, tex, 2));
				if (matchTile(newT, checkT)) {
					copyTile(newT, checkT);
				}
				else {
					shared_ptr<Crash4> checkT = shared_ptr<Crash4>(new Crash4(loc, tex, 3));
					if (matchTile(newT, checkT)) {
						copyTile(newT, checkT);
					}

					else {

						shared_ptr<Crash5> checkT = shared_ptr<Crash5>(new Crash5(loc, tex, 4));
						if (matchTile(newT, checkT)) {
							copyTile(newT, checkT);
						}
						else {
							shared_ptr<Crash6> checkT = shared_ptr<Crash6>(new Crash6(loc, tex, 5));
							if (matchTile(newT, checkT)) {
								copyTile(newT, checkT);
							}
							else {
								shared_ptr<Crash7> checkT = shared_ptr<Crash7>(new Crash7(loc, tex, 6));
								if (matchTile(newT, checkT)) {
									copyTile(newT, checkT);
								}
								else {
									shared_ptr<Crash8> checkT = shared_ptr<Crash8>(new Crash8(loc, tex, 7));
									if (matchTile(newT, checkT)) {
										copyTile(newT, checkT);
									}
									else {
										shared_ptr<Crash9> checkT = shared_ptr<Crash9>(new Crash9(loc, tex, 8));
										if (matchTile(newT, checkT)) {
											copyTile(newT, checkT);
										}
										else {
											shared_ptr<Crash10> checkT = shared_ptr<Crash10>(new Crash10(loc, tex, 9));
											if (matchTile(newT, checkT)) {
												copyTile(newT, checkT);
											}
											else {
												shared_ptr<Crash11> checkT = shared_ptr<Crash11>(new Crash11(loc, tex, 10));
												if (matchTile(newT, checkT)) {
													copyTile(newT, checkT);
												}
												else {
									
													shared_ptr<Crash12> checkT = shared_ptr<Crash12>(new Crash12(loc, tex, 11));
													if (matchTile(newT, checkT)) {
														copyTile(newT, checkT);
													}
													else {
														shared_ptr<Crash13> checkT = shared_ptr<Crash13>(new Crash13(loc, tex, 12));
														if (matchTile(newT, checkT)) {
															copyTile(newT, checkT);
														}
														else{
															shared_ptr<Crash14> checkT = shared_ptr<Crash14>(new Crash14(loc, tex, 13));
															if (matchTile(newT, checkT)) {
																copyTile(newT, checkT);
															}
															else {
																shared_ptr<Crash15> checkT = shared_ptr<Crash15>(new Crash15(loc, tex, 14));
																if (matchTile(newT, checkT)) {
																	copyTile(newT, checkT);
																}
																else {
																	shared_ptr<Crash16> checkT = shared_ptr<Crash16>(new Crash16(loc, tex, 15));
																	if (matchTile(newT, checkT)) {
																		copyTile(newT, checkT);
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

	}

	static void copyTile(shared_ptr<CrashTile>* newT, shared_ptr<CrashTile> checkT) {
		checkT->setLocation(newT->get()->getLocation());
		checkT->hitboxCopy(*newT);
		*newT = checkT;
		newT->get()->crashSetup();
	}

	static bool matchTile(shared_ptr<CrashTile>* newT, shared_ptr<CrashTile> checkT) {
		checkT->crashSetup();
		
		if (checkT->getAbove() == newT->get()->getAbove()) {
			if (checkT->getBelow() == newT->get()->getBelow()) {
				if (checkT->getRightB() == newT->get()->getRightB()) {
					if (checkT->getLeftB() == newT->get()->getLeftB()) {
						return true;
						checkT->setLocation(newT->get()->getLocation());
						checkT->hitboxCopy(*newT);
						*newT = checkT;
						newT->get()->crashSetup();
					}
				}
			}
		}
		return false;
	}

	static void checkPos(shared_ptr<CrashTile> newT, shared_ptr<tile> existing, Placement p) {


		if (p == above) {
			newT->setBelow(true);
			existing->setAbove(true);
		}
		else if (p == rightOf) {
			newT->setLeft(true);
			existing->setRight(true);
		}
		else if (p == below) {
			newT->setAbove(true);
			existing->setBelow(true);
		}
		else if (p == leftOf) {
			newT->setRight(true);
			existing->setLeft(true);
		}

		/*if (p == above) {
			if (existing->above) {
				newT->below = true;
			}
		}*/
	}
};