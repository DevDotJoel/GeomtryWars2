#pragma once
class CBBox {
public:
    float width = 0.0f;
    float height = 0.0f;

    CBBox() {}
    CBBox(float w, float h) : width(w), height(h) {}
};