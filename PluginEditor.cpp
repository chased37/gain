/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainAudioProcessorEditor::GainAudioProcessorEditor (GainAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible (inputGainSlider);
    inputGainSlider.setRange(0.0, 1.0, 0.01);
    inputGainSlider.setSkewFactorFromMidPoint(1.0); // Set midpoint to 0.5
    inputGainSlider.setValue(1.0f); // Set default value to 1.0
    inputGainSlider.setTextBoxStyle (juce::Slider::TextBoxLeft, false, 50, 20);
    inputGainSlider.addListener (this);
    
    addAndMakeVisible(outputGainSlider); // New slider added here
    outputGainSlider.setRange (0.0, 1.0, 0.01);
    outputGainSlider.setTextBoxStyle (juce::Slider::TextBoxLeft, false, 50, 20);
    outputGainSlider.addListener (this);
    
    
    addAndMakeVisible(thresholdSlider);
    thresholdSlider.setSliderStyle(juce::Slider::LinearVertical);
    thresholdSlider.setRange(-60.0f, 0.0f);
    thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    thresholdSlider.setValue(0.5);
    thresholdSlider.addListener(this);
    
    addAndMakeVisible(ratioSlider);
    ratioSlider.setSliderStyle(juce::Slider::LinearVertical);
    ratioSlider.setRange(1.0f, 20.0f);
    ratioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    ratioSlider.setValue(0.5);
    ratioSlider.addListener(this);
    
    addAndMakeVisible(attackSlider);
    attackSlider.setSliderStyle(juce::Slider::LinearVertical);
    attackSlider.setRange(1.0f, 1000.0f);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    attackSlider.setValue(0.5);
    attackSlider.addListener(this);
    
    addAndMakeVisible(releaseSlider);
    releaseSlider.setSliderStyle(juce::Slider::LinearVertical);
    releaseSlider.setRange(10.0f, 10000.0f);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    releaseSlider.setValue(0.5);
    releaseSlider.addListener(this);
    
    // make sure to add a listener for this new slider
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
}

GainAudioProcessorEditor::~GainAudioProcessorEditor()
{
}

//==============================================================================
void GainAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    g.setColour (juce::Colours::red);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void GainAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    outputGainSlider.setBounds (10, 40, getWidth() - 20, 20);
    inputGainSlider.setBounds (10, 40, getWidth() - 20, 20);
    thresholdSlider.setBounds(230, 10, 100, getHeight() - 20);
    ratioSlider.setBounds(340, 10, 100, getHeight() - 20);
    attackSlider.setBounds(450, 10, 100, getHeight() - 20);
    releaseSlider.setBounds(560, 10, 100, getHeight() - 20);
}
