#include "AnimationPlayer.h"

AnimationPlayer::AnimationPlayer() {

}

AnimationPlayer::~AnimationPlayer() {

}

void AnimationPlayer::setClip(Animation * anim) {
	this->anim = anim;
	this->time = anim->startTime;
}

void AnimationPlayer::Update() {
	anim->evaluate(time);
	time = time + 0.01f;
}