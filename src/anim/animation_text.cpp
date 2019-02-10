#include "animation_text.h"

AnimationText::AnimationText(void* _font, float _x, float _y, string _base, int _frames, float _duration)
                            : Animation(_x, _y, "", _frames, _duration) {
    text = _base;
    font = _font;
    r = g = b = 0xFF;
}

void AnimationText::advance(float delta) {
    Animation::advance(delta);
}
