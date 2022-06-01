#include "MainComponent.h"
#include <math.h>

MainComponent::MainComponent()
    : state(Stopped)
{
    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open...");
    openButton.onClick = [this] { openButtonClicked(); };

    addAndMakeVisible(&playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    playButton.setEnabled(false);

    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    stopButton.setEnabled(false);

    addAndMakeVisible(panner);
    setSize (600, 600);

    formatManager.registerBasicFormats();       
    transportSource.addChangeListener(this); 

    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::openButtonClicked()
{
    chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...", juce::File{}, "*.wav");
    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)           
    {
        auto file = fc.getResult();

        if (file != juce::File{})                                                      
        {
            auto* reader = formatManager.createReaderFor(file);                

            if (reader != nullptr)
            {
                auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);   
                transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);       
                playButton.setEnabled(true);                                                    
                readerSource.reset(newSource.release());                                         
            }
        }
    });
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    float gridSize = 0;

    if (getHeight() - 100 < getWidth())
    {
        gridSize = getHeight() - 100;
    }
    else
    {
        gridSize = getWidth();
    }

    openButton.setBounds(10, 10, getWidth() - 20, 20);
    playButton.setBounds(10, 40, getWidth() - 20, 20);
    stopButton.setBounds(10, 70, getWidth() - 20, 20);

    panner.setBounds(getWidth() * 0.05, getHeight() * 0.05 + 100, gridSize * 0.9, gridSize * 0.9);
}

void MainComponent::playButtonClicked()
{
    spatializer.CookVariables(panner.sourcePosition, panner.listenerPosition, transportSource.sampleRate);
    changeState(Starting);
}

void MainComponent::changeState(TransportState newState)
{
    if (state != newState)
    {
        state = newState;

        switch (state)
        {
            case Stopped:                           
                stopButton.setEnabled(false);
                playButton.setEnabled(true);
                transportSource.setPosition(0.0);
                break;

            case Starting:                          
                playButton.setEnabled(false);
                transportSource.start();
                break;

            case Playing:                           
                stopButton.setEnabled(true);
                break;

            case Stopping:                         
                transportSource.stop();
                break;
        }
    }
}

void MainComponent::stopButtonClicked()
{
    changeState(Stopping);
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
            changeState(Playing);
        else
            changeState(Stopped);
    }
}

void MainComponent::releaseResources()
{
    transportSource.releaseResources();
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (state == Playing)
    {
        if (readerSource.get() == nullptr)
        {
            bufferToFill.clearActiveBufferRegion();
            return;
        }

        transportSource.getNextAudioBlock(bufferToFill);
        for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
            auto* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

            for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                buffer[sample] = spatializer.ComputeSpatial(buffer[sample], channel);
            }
        }
    }
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}