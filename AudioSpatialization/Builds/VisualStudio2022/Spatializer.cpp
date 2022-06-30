#include "Spatializer.h"

float Spatializer::ComputeSpatial(float input, int channel)
{
	float returnValue = ComputeInterauralDelay(input, channel);
	returnValue = CalculateBlauertschBands(returnValue, channel);
	returnValue = CalculateDistanceFalloff(returnValue, channel);

	return returnValue;
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
	leftDelayBuffer[leftWriteIndex] = sample;

	float delayedSample = leftDelayBuffer[leftReadIndex];

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

	return delayedSample;
}
 
float Spatializer::RightEarDelay(float sample)
{
	rightDelayBuffer[rightWriteIndex] = sample;

	float delayedSample = rightDelayBuffer[rightReadIndex];
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

	return delayedSample;
}

void Spatializer::PrepareToPlay(Position sourcePos, Position listenerPos, double sampleRate)
{
	bufferSize = sampleRate * 2;
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

void Spatializer::ChangeDelay(Position sourcePos, Position listenerPos, double sampleRate)
{
	int leftDelayGrow = 0;
	int rightDelayGrow = 0;

	float xV = sourcePos.x;
	float yV = sourcePos.y;

	if (yV > 0)
		front = true;
	else
		front = false;

	r_distance = sqrt(pow(xV + 0.1, 2) + pow(yV, 2));
	float r_delay = r_distance / 343;

	l_distance = sqrt(pow(xV - 0.1, 2) + pow(yV, 2));
	float l_delay = l_distance / 343;

	auto leftNewDelay = l_delay * ((float)sampleRate);
	auto rightNewDelay = r_delay * ((float)sampleRate);

	if (leftReadIndex > rightReadIndex)
	{
		leftReadIndex = rightReadIndex;
	}
	else
	{
		rightReadIndex = leftReadIndex;
	}
	int l = 0;
	int r = 0;

	if (leftNewDelay > rightNewDelay)
	{
		int diff = leftNewDelay - rightNewDelay;
		leftReadIndex -= diff;
	}
	else
	{
		int diff = rightNewDelay - leftNewDelay;
		rightReadIndex -= diff;
	}

	
	if (rightReadIndex < 0)
	{
		rightReadIndex += bufferSize;
	}

	if (leftReadIndex < 0)
	{
		leftReadIndex += bufferSize;
	}
}

float Spatializer::CalculateDistanceFalloff(float sample, int channel)
{
	float distanceValue = 0;
	if (channel == 0)
		distanceValue = l_distance;
	else if (channel == 1)
		distanceValue = r_distance;

	float damping = 1 / (distanceValue * 0.5);
	return sample * damping;
}

float Spatializer::CalculateBlauertschBands(float sample, int channel)
{
	float val = sample;
	if (channel == 0)
	{
		if (front)
		{
			val = l_Damp_1000Hz.ProcessSample(
					l_Boost_3500Hz.ProcessSample(
						l_Boost_350Hz.ProcessSample(sample)));
		}
		else
		{
			val = l_Boost_11000Hz.ProcessSample(
					l_Boost_1000Hz.ProcessSample(sample));
		}
	}
	else if (channel == 1)
	{
		if (front)
		{
			val = r_Damp_1000Hz.ProcessSample(
					r_Boost_3500Hz.ProcessSample(
						r_Boost_350Hz.ProcessSample(sample)));
		}
		else
		{
			val = r_Boost_11000Hz.ProcessSample(
					r_Boost_1000Hz.ProcessSample(sample));
		}
	}
	
	return val;
}