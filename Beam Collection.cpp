#include "beam.cpp"
#pragma once

class BeamCollection : public GameObject{
	
	list<Beam*> beamList = {};

	float untilStart = 0.5;
	float untilStart_left = untilStart/4;

	list<Beam*>::iterator beamIt;

public:
	BeamCollection(list<Beam*> beams) {
		bool sorted = false;
		
		float aboveThis = -99999;
		
		Beam* lowestBeam;

		for (int i = 0; i < beams.size(); i++) {
			float lowestY = 9999999999;
			for (Beam* beam : beams) {
				if (beam->getPosition().y > aboveThis) {
					if (beam->getPosition().y < lowestY) {
						lowestBeam = beam;
					}
				}
			}

			beamList.push_back(lowestBeam);
			aboveThis = lowestBeam->getPosition().y;
		}

		beamIt = beamList.begin();

		code = "beam collection";
	}

	BeamCollection() {
		code = "beam collection";
	}

	void setSoundPointer(Sound* sou) {
		for (Beam* b : beamList) {
			b->setSoundPointer(sou);
		}
	}

	void addBeam(Beam* b) {
		if (!beamList.empty()) {
			list<Beam*>::iterator it = beamList.begin();
			for (Beam* current : beamList) {

				if (current->getInitialPosition().y >= b->getInitialPosition().y) {
					beamList.insert(it, b);
					break;
				}
				it = next(it);
				if (it == beamList.end()) {
					beamList.push_back(b);
					break;
				}
			}
		}
		else {
			beamList.push_back(b);
		}

		beamIt = beamList.begin();
	}

	void eachFrame(float* deltaT, player* player, camera* cam) {

		if (beamIt != beamList.end()) {
			untilStart_left -= *deltaT;
			if (untilStart_left <= 0) {
				untilStart_left = untilStart;
				Beam* b = *beamIt;
				b->moveStart();
				beamIt = next(beamIt);

			}
		}

	}

};