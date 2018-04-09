#include "Keyframe.h"

Keyframe::Keyframe(float time, float value, std::string tangentIn, std::string tangentOut) {
	this->time = time;
	this->value = value;
	this->tangentIn = tangentIn;
	this->tangentOut = tangentOut;
}

Keyframe::~Keyframe() {

}