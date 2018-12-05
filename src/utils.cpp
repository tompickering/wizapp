int clamp(int v, int min, int max) {
    if (v < min) v = min;
    if (v > max) v = max;
    return v;
}

float clamp(float v, float min, float max) {
    if (v < min) v = min;
    if (v > max) v = max;
    return v;
}

float float_abs(float v) {
    if (v < 0.f)
        return -v;
    return v;
}
