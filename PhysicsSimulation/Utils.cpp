#include "Utils.h"
#include <regex>
#include <sstream>

#ifndef SQR
#define SQR(x)	((x) * (x))
#endif

namespace utils {

	const double M_PI = 3.141592653589;
	const float FLOAT_TOL = 1e-6f;

	/**
	 * Generate a random number in [0, 1).
	 * Note: 1 is exluded!
	 */
	float genRand() {
		return (float)(rand() % 1000) / 1000.0f;
	}

	/**
	 * Generate a random number in [0, b).
	 * Note: b is excluded!
	 */
	float genRand(float v) {
		return genRand() * v;
	}

	/**
	 * Generate a random number in [a, b).
	 * Note: b is excluded!
	 */
	float genRand(float a, float b) {
		return genRand(b - a) + a;
	}

}