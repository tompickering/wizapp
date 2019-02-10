#include "animation_text.h"

AnimationText::AnimationText(float _x, float _y, string _base, int _frames, float _duration)
                            : Animation(_x, _y, "", _frames, _duration) {
    text = _base;
    r = g = b = 0xFF;
}

void AnimationText::advance(float delta) {
    Animation::advance(delta);
}
