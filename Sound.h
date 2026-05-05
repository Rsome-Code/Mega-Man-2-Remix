#include <SFML/Audio.hpp>
#pragma once


class Sound {
	std::shared_ptr<sf::SoundBuffer> soundB;

	std::shared_ptr<sf::Sound> sound;

public:
	Sound() {
		soundB = std::shared_ptr<sf::SoundBuffer>(new sf::SoundBuffer());

		sound = std::shared_ptr<sf::Sound>(new sf::Sound());
	}

	Sound(std::string path) {
		soundB = std::shared_ptr<sf::SoundBuffer>(new sf::SoundBuffer());
		soundB->loadFromFile(path);

		sound = std::shared_ptr<sf::Sound>(new sf::Sound());
		sound->setBuffer(*soundB);
	}

	bool play() {
		sound->play();
	}

	void setLoop(bool b) {
		sound->setLoop(b);
	}

	void stop() {
		sound->stop();
	}

	bool checkPlaying() {
		return sound->getStatus() == sf::Sound::Playing;
	}

};