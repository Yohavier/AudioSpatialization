#include "Panner.h"

void Panner::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);

    DrawRaster(g);
    DrawListener(g);
    DrawCoordinate(g);
    DrawSourcePoint(g);
    CalculateAngle();
}

void Panner::mouseDown(const juce::MouseEvent & mouse)
{
    CalculateSourcePoint(mouse);
    repaint();
}

void Panner::mouseDrag(const juce::MouseEvent& mouse)
{
    CalculateSourcePoint(mouse);
    repaint();
}

void Panner::CalculateSourcePoint(const juce::MouseEvent& mouse)
{
    auto relativePos = juce::Point<float>((getWidth() / 2) - mouse.getPosition().x, (getHeight() / 2) - mouse.getPosition().y);
    relativeX = relativePos.x / getWidth();
    relativeY = relativePos.y / getHeight();

    sourcePosition.x = relativeX * 100;
    sourcePosition.y = relativeY * 100;
}

void Panner::DrawRaster(juce::Graphics& g)
{
    g.setColour(juce::Colours::darkseagreen);
    if (getHeight() < getWidth())
    {
        gridSize = getHeight();
    }
    else
    {
        gridSize = getWidth();
    }

    for (int i = 0; i <= 10; i++)
    {
        juce::Line<float> lineH(juce::Point<float>(0, 0 + (gridSize / 10) * i), juce::Point<float>(gridSize, 0 + (gridSize / 10) * i));
        g.drawLine(lineH, 2.0f);

        juce::Line<float> lineV(juce::Point<float>(0 + (gridSize / 10) * i, 0), juce::Point<float>(0 + (gridSize / 10) * i, gridSize));
        g.drawLine(lineV, 2.0f);
    }
}

void Panner::DrawListener(juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    juce::Rectangle<float> listenerArea(getWidth() / 40, getHeight() / 40);
    listenerPosition.Set(getWidth() / 2, getHeight() / 2);
    listenerArea.setCentre(juce::Point<float>(listenerPosition.x, listenerPosition.y));
    g.fillRect(listenerArea);
}

void Panner::DrawCoordinate(juce::Graphics & g)
{
    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::orange);

    g.drawText(std::to_string(sourcePosition.x) + " " + std::to_string(sourcePosition.y), getLocalBounds(), juce::Justification::top, true);
}

void Panner::DrawSourcePoint(juce::Graphics& g)
{
    g.setColour(juce::Colours::yellow);
    juce::Rectangle<float> pointArea(10, 10);

    pointArea.setCentre(juce::Point<float>(relativeX * getWidth(), relativeY * getHeight()));
    float pointSize = 0.025;
    g.fillEllipse(-relativeX * getWidth() + (getWidth() / 2) - (gridSize * pointSize) / 2, -relativeY * getHeight() + (getHeight()/2) - (gridSize * pointSize) / 2, gridSize * pointSize, gridSize * pointSize);
}

void Panner::CalculateAngle()
{
    float vecLenght = sqrt(pow(relativeX * 100, 2) + pow(relativeY * 100, 2));
    float dotProduct = (0 * (relativeX * 100 / vecLenght) + 1 * (relativeY * 100 / vecLenght));

    float angle = acos(dotProduct) * 180 / M_PI;

    azimuthAngle = angle;
}