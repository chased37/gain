#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>


class GainAudioProcessor : public juce::AudioProcessor
{
public:
    GainAudioProcessor()
    {
        
        // Create the input and output gain parameters
        inputGainParam = new juce::AudioParameterFloat("input_gain", "Input Gain", 0.0f, 2.0f, 1.0f);
        outputGainParam = new juce::AudioParameterFloat("output_gain", "Output Gain", 0.0f, 2.0f, 1.0f);
        
        // Create the compressor parameters
        thresholdParam = new juce::AudioParameterFloat("threshold", "Threshold", -60.0f, 0.0f, -12.0f);
        ratioParam = new juce::AudioParameterFloat("ratio", "Ratio", 1.0f, 20.0f, 4.0f);
        attackParam = new juce::AudioParameterFloat("attack", "Attack", 1.0f, 1000.0f, 20.0f);
        releaseParam = new juce::AudioParameterFloat("release", "Release", 10.0f, 10000.0f, 200.0f);
        
        // Add the parameters to the processor
        addParameter(inputGainParam);
        addParameter(outputGainParam);
        addParameter(thresholdParam);
        addParameter(ratioParam);
        addParameter(attackParam);
        addParameter(releaseParam);
    }
    
    void prepareToPlay(double sampleRate, int samplesPerBlock) override {}
    void releaseResources() override {}
    
    
    // PROCESS BLOCK //
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        const float inputGain = inputGainParam->get();
        const float outputGain = outputGainParam->get();
        const float threshold = thresholdParam->get();
        const float ratio = ratioParam->get();
        const float attack = attackParam->get();
        const float release = releaseParam->get();
        
        const int numSamples = buffer.getNumSamples();
        const int numChannels = buffer.getNumChannels();
        
        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* channelData = buffer.getWritePointer(channel);
            
            for (int i = 0; i < numSamples; ++i)
            {
                // apply input gain
                channelData[i] *= inputGain;
                
                // calculate the envelope of the signal
                float envelope = std::abs(channelData[i]);
                
                // apply compression if necessary
                if (envelope > threshold)
                {
                    // calculate gain reduction
                    float dbAboveThreshold = juce::Decibels::gainToDecibels(envelope / threshold);
                    float dbGainReduction = dbAboveThreshold * (1.0f - 1.0f / ratio);
                    float gainReduction = juce::Decibels::decibelsToGain(-dbGainReduction);
                    
                    // apply gain reduction with fast attack and release
                    float attackTime = std::max(0.001f, attack / 1000.0f);
                    float releaseTime = std::max(0.001f, release / 1000.0f);
                    float time = envelope > gainReduction * threshold ? attackTime : releaseTime;
                    float gain = std::exp(-1.0f / (time * getSampleRate()));
                    channelData[i] *= gainReduction * gain;
                }
                
                // apply output gain
                channelData[i] *= outputGain;
            }
        }
    }
    
    
    
    juce::AudioProcessorEditor* createEditor() override
    {
        return new juce::GenericAudioProcessorEditor(*this);
    }
    
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return "Gain"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    int getNumParameters() override { return 2; }
    
    
    float getParameter(int index) override
    {
        if (index == 0)
            return *inputGainParam;
        else if (index == 1)
            return *outputGainParam;
        
        return 0.0f;
    }
    
    void setParameter(int index, float newValue) override
    {
        if (index == 0)
            *inputGainParam = newValue;
        else if (index == 1)
            *outputGainParam = newValue;
    }
    const juce::String getParameterName(int index) override
    {
        if (index == 0)
            return "Input Gain";
        else if (index == 1)
            return "Output Gain";
        
        return {};
    }
    
    const juce::String getParameterText(int index) override
    {
        if (index == 0)
            return juce::String(*inputGainParam, 2);
        else if (index == 1)
            return juce::String(*outputGainParam, 2);
        
        return {};
    }
    
    
    juce::AudioProcessorParameter* getParameter(int index) const
    {
        if (index == 0)
            return inputGainParam;
        else if (index == 1)
            return outputGainParam;
        
        return nullptr;
    }
    
    bool isParameterAutomatable(int index) const override { return true; }
    int getNumInputChannels() const { return 2; }
    int getNumOutputChannels() const { return 2; }
    
    
    
    void getStateInformation(juce::MemoryBlock& destData) override
    {
        float inputGain = inputGainParam->get();
        float outputGain = outputGainParam->get();
        destData.append(&inputGain, sizeof(float));
        destData.append(&outputGain, sizeof(float));
    }
    
    
    
    void setStateInformation(const void* data, int size) override
    {
        if (size != 2*sizeof(float))
            return;
        
        float inputGain;
        memcpy(&inputGain, data, sizeof(float));
        inputGainParam->setValueNotifyingHost(inputGain);
        
        float outputGain;
        memcpy(&outputGain, static_cast<const char*>(data) + sizeof(float), sizeof(float));
        outputGainParam->setValueNotifyingHost(outputGain);
    }
    

    
private:
    juce::AudioParameterFloat* inputGainParam; // input gain parameter
    juce::AudioParameterFloat* outputGainParam; // output gain parameter
    juce::AudioParameterFloat*  thresholdParam;
    juce::AudioParameterFloat*  ratioParam;
    juce::AudioParameterFloat*  attackParam;
    juce::AudioParameterFloat* releaseParam;
    
};

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GainAudioProcessor();
}




