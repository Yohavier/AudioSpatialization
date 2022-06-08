#pragma once
#include "JuceHeader.h"
#include <cmath>
#include "Utils.h"

class Panner : public juce::Component
{
private:
    void paint(juce::Graphics& g) override;

    void mouseDown(const juce::MouseEvent& mouse) override;

    void CalculateSourcePoint(const juce::MouseEvent& mouse);

    void mouseDrag(const juce::MouseEvent& mouse) override;

    void DrawRaster(juce::Graphics& g);

    void DrawListener(juce::Graphics& g);

    void DrawCoordinate(juce::Graphics& g);

    void DrawSourcePoint(juce::Graphics& g);

    void CalculateAngle();

public:
    Position sourcePosition = Position(0, 0); 
    Position listenerPosition = Position(0, 0);

    bool sourceChanged = false;

    float azimuthAngle = 0;

private:
    float relativeX = 0;
    float relativeY = 0;
    float gridSize = 0;
    int horizontalLines = 10;
};