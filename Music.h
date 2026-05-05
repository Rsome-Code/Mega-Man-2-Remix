#include <SFML/Audio.hpp>

#pragma once

class Music {

	std::shared_ptr<sf::Music> music;

public:
	Music() {
		music = std::shared_ptr<sf::Music>(new sf::Music());
	}

	Music(std::string path) {
		music = std::shared_ptr<sf::Music>(new sf::Music());
		music->openFromFile(path);
	}

	void setPath(std::string path) {
		music->openFromFile(path);
	}

	void openFromFile(std::string path) {
		music->openFromFile(path);
	}

	void setLoopPoints(float start, float end) {
		music->setLoopPoints({ sf::seconds(start), sf::seconds(end) });
	}

	void setLoop(bool l) {
		music->setLoop(l);
	}

	void play() {
		music->play();
	}

	void setVolume(int v) {
		music->setVolume(v);
	}

	void stop() {
		music->stop();
	}

	bool hasStopped() {
		return music->getStatus() == sf::Music::Stopped;
	}
};