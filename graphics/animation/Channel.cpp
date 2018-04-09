#include "Channel.h"
#include <cmath>

Channel::Channel() {

}

Channel::~Channel() {

}

void Channel::addKeyframe(float time, float value, std::string tangentIn, std::string tangentOut) {
	keyframes.push_back(new Keyframe(time, value, tangentIn, tangentOut));
}

float Channel::evaluate(float time) {
	//cout << "channel evaluate " << time << endl;
	if (time < keyframes.at(0)->time) { //t before first key, extrapolate
		//cout << "extrapin" << endl;
		if (extrapIn == "constant") {
			return keyframes.at(0)->value;
		}
		else if (extrapIn == "linear") {
			return keyframes.at(0)->value + ((keyframes.at(1)->value - keyframes.at(0)->value) / 
				(keyframes.at(1)->time - keyframes.at(0)->time)) * (time - keyframes.at(0)->time);
		}
		else if (extrapIn == "cycle") {
			int multiplier = 1;
			float timeFromLastKey = keyframes.at(keyframes.size() - 1)->time - time;
			float range = keyframes.at(keyframes.size() - 1)->time - keyframes.at(0)->time;
			while (range * multiplier < std::abs(timeFromLastKey)) {
				multiplier++;
			}

			float adjustedTime = timeFromLastKey - ((multiplier - 1) * range);
			return computeValue(adjustedTime + keyframes.at(0)->time);
		}
		else if (extrapIn == "cycle_offset") {
			int multiplier = 1;
			float timeFromLastKey = keyframes.at(keyframes.size() - 1)->time - time;
			float range = keyframes.at(keyframes.size() - 1)->time - keyframes.at(0)->time;
			while (range * multiplier < std::abs(timeFromLastKey)) {
				multiplier++;
			}

			float adjustedTime = timeFromLastKey - ((multiplier - 1) * range);
			return computeValue(adjustedTime + keyframes.at(0)->time) -
				(keyframes.at(keyframes.size() - 1)->value - keyframes.at(0)->value) * (multiplier - 1);
		}
		else if (extrapIn == "bounce") {
			int multiplier = 1;
			float timeFromLastKey = keyframes.at(keyframes.size() - 1)->time - time;
			float range = keyframes.at(keyframes.size() - 1)->time - keyframes.at(0)->time;
			while (range * multiplier < std::abs(timeFromLastKey)) {
				multiplier++;
			}

			float adjustedTime = timeFromLastKey - ((multiplier - 1) * range);
			if ((multiplier - 1) % 2 != 0) {
				return computeValue((range - adjustedTime) + keyframes.at(0)->time);
			}
			else {
				return computeValue(adjustedTime + keyframes.at(0)->time);
			}
		}

		return 0;
	}
	else if (time > keyframes.at(keyframes.size()-1)->time) { //t after last key, extrapolate
		//cout << "extrapout " << extrapOut << endl;
		if (extrapOut == "constant") {
			return keyframes.at(keyframes.size() - 1)->value;
		}
		else if (extrapOut == "linear") {
			return keyframes.at(keyframes.size() - 1)->value + ((keyframes.at(keyframes.size() - 1)->value - keyframes.at(keyframes.size() - 2)->value) /
				(keyframes.at(keyframes.size() - 1)->time - keyframes.at(keyframes.size() - 2)->time)) * (time - keyframes.at(keyframes.size() - 1)->time);
		}
		else if (extrapOut == "cycle") {
			int multiplier = 1;
			float timeFromFirstKey = time - keyframes.at(0)->time;
			float range = keyframes.at(keyframes.size() - 1)->time - keyframes.at(0)->time;
			while ((range * multiplier) < (std::abs(timeFromFirstKey))) {
				multiplier++;
			}

			float adjustedTime = timeFromFirstKey - ((multiplier - 1) * range);
			return computeValue(adjustedTime + keyframes.at(0)->time);
		}
		else if (extrapOut == "cycle_offset") {
			int multiplier = 1;
			float timeFromFirstKey = time - keyframes.at(0)->time;
			float range = keyframes.at(keyframes.size() - 1)->time - keyframes.at(0)->time;
			while ((range * multiplier) < (std::abs(timeFromFirstKey))) {
				multiplier++;
			}

			float adjustedTime = timeFromFirstKey - ((multiplier - 1) * range);
			return computeValue(adjustedTime + keyframes.at(0)->time) + 
				(keyframes.at(keyframes.size() - 1)->value - keyframes.at(0)->value) * (multiplier - 1);
		}
		else if (extrapOut == "bounce") {
			int multiplier = 1;
			float timeFromFirstKey = time - keyframes.at(0)->time;
			float range = keyframes.at(keyframes.size() - 1)->time - keyframes.at(0)->time;
			while ((range * multiplier) < (std::abs(timeFromFirstKey))) {
				multiplier++;
			}

			float adjustedTime = timeFromFirstKey - ((multiplier - 1) * range);
			if ((multiplier - 1) % 2 != 0) {
				return computeValue((range - adjustedTime) + keyframes.at(0)->time);
			}
			else {
				return computeValue(adjustedTime + keyframes.at(0)->time);
			}
		}

		cout << "extrap" << endl;
	}
	else {
		return computeValue(time);
	}

	return 0;
}

float Channel::computeValue(float time) {
	//cout << "compute value" << endl;
	for (int i = 0; i < keyframes.size() - 1; i++) {
		float keyTime = keyframes.at(i)->time;
		if (time == keyTime) { //time is equal to time of key frame, return value
			return keyframes.at(i)->value;
		}

		else { //check if time is in between this keyframe and next keyframe's time
			if (time > keyframes.at(i)->time && time < keyframes.at(i + 1)->time) { //in between this keyframe and next keyframe
				//evaluate cubic equation
				Keyframe * firstFrame = keyframes.at(i);
				Keyframe * nextFrame = keyframes.at(i + 1);

				float vout, vin;

				if (firstFrame->tangentOut == "flat") {
					vout = 0;
				}
				else if (firstFrame->tangentOut == "linear") {
					vout = (keyframes.at(i + 1)->value - keyframes.at(i)->value) / (keyframes.at(i + 1)->time - keyframes.at(i)->time);
				}
				else if (firstFrame->tangentOut == "smooth") {
					if (i == 0) { //first frame, smooth doesn't work, use linear
						vout = (keyframes.at(i + 1)->value - keyframes.at(i)->value) / (keyframes.at(i + 1)->time - keyframes.at(i)->time);
					}
					else {
						cout << "f" << endl;
						vout = (keyframes.at(i + 1)->value - keyframes.at(i - 1)->value) / (keyframes.at(i + 1)->time - keyframes.at(i - 1)->time);
					}
				}
				else {
					vout = std::atof(firstFrame->tangentOut.c_str());
				}

				if (nextFrame->tangentIn == "flat") {
					vin = 0;
				}
				else if (nextFrame->tangentIn == "linear") {
					vin = (keyframes.at(i + 1)->value - keyframes.at(i)->value) / (keyframes.at(i + 1)->time - keyframes.at(i)->time);
				}
				else if (nextFrame->tangentIn == "smooth") {
					if (i + 1 == keyframes.size() - 1) { //last frame, smooth doesn't work, use linear
						vin = (keyframes.at(i + 1)->value - keyframes.at(i)->value) / (keyframes.at(i + 1)->time - keyframes.at(i)->time);
					}
					else {
						vin = (keyframes.at(i + 2)->value - keyframes.at(i)->value) / (keyframes.at(i + 2)->time - keyframes.at(i)->time);
					}
				}
				else {
					vin = std::atof(nextFrame->tangentIn.c_str());
				}

				glm::mat4x4 matr = glm::mat4x4(
					2, -3, 0, 1,
					-2, 3, 0, 0,
					1, -2, 1, 0,
					1, -1, 0, 0
				);

				glm::vec4 cubicCoeff = matr * glm::vec4(firstFrame->value, nextFrame->value, 
					(nextFrame->time - firstFrame->time) * vout, (nextFrame->time - firstFrame->time) * vin);

				float u = (time - firstFrame->time) / (nextFrame->time - firstFrame->time);
				float span = cubicCoeff.x * pow(u, 3) + cubicCoeff.y * pow(u, 2) + cubicCoeff.z * u + cubicCoeff.w;

				cout << span << endl;
				return span;
			}
		}
	}

	if (time == keyframes.at(keyframes.size() - 1)->time) { //check if time equal to last key frame time
		return keyframes.at(keyframes.size() - 1)->value;
	}
}