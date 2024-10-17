#include <chrono>
#include <thread>
#include <iostream>
#pragma once

using namespace std;


template <
	class result_t = std::chrono::milliseconds,
	class clock_t = std::chrono::steady_clock,
	class duration_t = std::chrono::milliseconds
>
auto since(std::chrono::time_point<clock_t, duration_t> const& start)
{
	return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}

class timer {

	public: auto timerStart() {
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		return start;
	}
	public: double checkTimer(std::chrono::steady_clock::time_point* start) {
		int temp = since<std::chrono::nanoseconds>(*start).count();
		double temp1 = temp;
		temp1 = temp1 / 1000000000;
		return temp1;
	}

	public:void frameLimiter(double targetRate, std::chrono::steady_clock::time_point* start) {
		double targetDelta = (1 / targetRate) * 1000000000;
		double delta = checkTimer(start);

		while (delta <= targetDelta) {
			delta = since<std::chrono::nanoseconds>(*start).count();
		}
		double delta1 = since<std::chrono::nanoseconds>(*start).count();
		int frame = 1 / (delta1 / 1000000000);
		//cout << frame;
		//cout << ", ";
	}
};