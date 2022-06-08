#include "Panner.h"

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
	float InterpolateLastValue(float lastSample, float currentSample);

private:
	float* leftDelayBuffer;
	float* rightDelayBuffer;

	int rightReadIndex;
	int rightWriteIndex;

	int leftReadIndex;
	int leftWriteIndex;

	int bufferSize = 0;

	float leftDelayInSamples = 0;
	float rightDelayInSamples = 0;

	float leftDelayGrowthParameter = 0;
	float rightDelayGrowthParameter = 0;
};

