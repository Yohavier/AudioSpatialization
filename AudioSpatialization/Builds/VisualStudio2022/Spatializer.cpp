#include "Spatializer.h"

float Spatializer::ComputeSpatial(float input, int channel)
{
	return ComputeInterauralDelay(input, channel);
}

float Spatializer::ComputeInterauralDelay(float sample, int channel)
{
	float xn = sample;
	float yn = 0;

	if (channel == 0)
	{
		yn = LeftEarDelay(sample);
	}
	else if (channel == 1)
	{
		yn = RightEarDelay(sample);
	}
	return yn;
}

float Spatializer::LeftEarDelay(float sample)
{
	float y = leftDelayBuffer[leftReadIndex];
	leftDelayBuffer[leftWriteIndex] = sample;
	leftWriteIndex++;
	if (leftWriteIndex >= bufferSize)
	{
		leftWriteIndex = 0;
	}

	leftReadIndex++;
	if (leftReadIndex >= bufferSize)
	{
		leftReadIndex = 0;
	}

	return y;
}

float Spatializer::RightEarDelay(float sample)
{
	float y = rightDelayBuffer[rightReadIndex];
	rightDelayBuffer[rightWriteIndex] = sample;
	rightWriteIndex++;
	if (rightWriteIndex >= bufferSize)
	{
		rightWriteIndex = 0;
	}

	rightReadIndex++;
	if (rightReadIndex >= bufferSize)
	{
		rightReadIndex = 0;
	}

	return y;
}

void Spatializer::CookVariables(Position sourcePos, Position listenerPos, double sampleRate)
{
	bufferSize = sampleRate * 4;
	leftDelayBuffer = new float[bufferSize];
	rightDelayBuffer = new float[bufferSize];

	memset(leftDelayBuffer, 0, bufferSize * sizeof(float));
	memset(rightDelayBuffer, 0,bufferSize * sizeof(float));

	rightReadIndex = 0;
	rightWriteIndex = 0;

	leftReadIndex = 0;
	leftWriteIndex = 0;

	float xV = sourcePos.x;
	float yV = sourcePos.y;

	float r_distance = sqrt(pow(xV + 0.1, 2) + pow(yV, 2));
	float r_delay = r_distance / 343;

	float l_distance = sqrt(pow(xV - 0.1, 2) + pow(yV, 2));
	float l_delay = l_distance / 343;


	rightDelayInSamples = r_delay * ((float)sampleRate);
	leftDelayInSamples = l_delay * ((float)sampleRate);

	rightReadIndex = rightWriteIndex - (int)rightDelayInSamples;

	if (rightReadIndex < 0)
	{
		rightReadIndex += bufferSize;
	}

	leftReadIndex = leftWriteIndex - (int)leftDelayInSamples;

	if (leftReadIndex < 0)
	{
		leftReadIndex += bufferSize;
	}
}
