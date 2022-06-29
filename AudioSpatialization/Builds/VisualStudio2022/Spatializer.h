#include "Panner.h"
#include "Biquad.h"

class Spatializer 
{
public:
	Spatializer() {}

	float ComputeSpatial(float input, int channel);
	void PrepareToPlay(Position sourcePos, Position listenerPos, double sampleRate);
	void ChangeDelay(Position sourcePos, Position listenerPos, double sampleRate);

private:
	float ComputeInterauralDelay(float sample, int channel);
	float LeftEarDelay(float sample);
	float RightEarDelay(float sample);
	float CalculateDistanceFalloff(float sample, int channel);
	float CalculateBlauertschBands(float sample, int channel);

private:
	float* leftDelayBuffer;
	float* rightDelayBuffer;

	float r_distance;
	float l_distance;

	int rightReadIndex;
	int rightWriteIndex;

	int leftReadIndex;
	int leftWriteIndex;

	int bufferSize = 0;

	float leftDelayInSamples = 0;
	float rightDelayInSamples = 0;

	float leftDelayGrowthParameter = 0;
	float rightDelayGrowthParameter = 0;

	bool front = false;
	
	//Front Left
	Biquad l_Boost_350Hz = Biquad(48000, BiquadType::Peak, 350, 1, 6);
	Biquad l_Damp_1000Hz = Biquad(48000, BiquadType::Peak, 1000, 1, -6);
	Biquad l_Boost_3500Hz = Biquad(48000, BiquadType::Peak, 3500, 1, 6);

	//Front Right
	Biquad r_Boost_350Hz = Biquad(48000, BiquadType::Peak, 350, 1, 6);
	Biquad r_Damp_1000Hz = Biquad(48000, BiquadType::Peak, 1000, 1, -6);
	Biquad r_Boost_3500Hz = Biquad(48000, BiquadType::Peak, 3500, 1, 6);


	//Back Left
	Biquad l_Boost_1000Hz = Biquad(48000, BiquadType::Peak, 1000, 1, 6);
	Biquad l_Boost_11000Hz = Biquad(48000, BiquadType::Peak, 11000, 1, 6);

	//Back Right
	Biquad r_Boost_1000Hz = Biquad(48000, BiquadType::Peak, 1000, 1, 6);
	Biquad r_Boost_11000Hz = Biquad(48000, BiquadType::Peak, 11000, 1, 6);
	
};

