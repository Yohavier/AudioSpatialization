/*
  ==============================================================================

	Biquad.cpp
	Created: 28 Jun 2022 9:59:59pm
	Author:  Jonas

  ==============================================================================
*/

#include "Biquad.h"
void Biquad::ComputeCoeff(const float frequency, const float qualityFactor, const float peakGain)
{
	double const factorV = pow(10.0, (fabs(peakGain) / 20.0));
	double const factorK = tan(M_PI * (frequency / m_sampleRate));
	double const factorKSpuare = factorK * factorK;

	switch (type)
	{
	case Lowpass:
	{
		double const norm = 1.0 / (1.0 + factorK / qualityFactor + factorKSpuare);
		coeff_a0 = static_cast<float>(factorKSpuare * norm);
		coeff_a1 = static_cast<float>(2.0 * coeff_a0);
		coeff_a2 = coeff_a0;
		coeff_b1 = static_cast<float>(2.0 * (factorKSpuare - 1.0) * norm);
		coeff_b2 = static_cast<float>((1.0 - factorK / qualityFactor + factorKSpuare) * norm);
		break;
	}
	case Highpass:
	{
		double const norm = 1.0 / (1.0 + factorK / qualityFactor + factorKSpuare);
		coeff_a0 = static_cast<float>(1 * norm);
		coeff_a1 = static_cast<float>(-2.0f * coeff_a0);
		coeff_a2 = coeff_a0;
		coeff_b1 = static_cast<float>(2 * (factorKSpuare - 1) * norm);
		coeff_b2 = static_cast<float>(norm * (1 - factorK / qualityFactor + factorKSpuare));
		break;
	}
	case Peak:
	{
		double const x1 = factorK / qualityFactor;
		double const x2 = factorK * factorV / qualityFactor;

		if (peakGain >= 0)
		{
			double const norm = 1 / (1 + x1 + factorKSpuare);
			coeff_a0 = static_cast<float>((1 + x2 + factorKSpuare) * norm);
			coeff_a1 = static_cast<float>(2 * (factorKSpuare - 1) * norm);
			coeff_a2 = static_cast<float>((1 - x2 + factorKSpuare) * norm);
			coeff_b1 = coeff_a1;
			coeff_b2 = static_cast<float>((1 - x1 + factorKSpuare) * norm);
		}
		else
		{
			double const norm = 1 / (1 + x2 + factorKSpuare);
			coeff_a0 = static_cast<float>((1 + x1 + factorKSpuare) * norm);
			coeff_a1 = static_cast<float>((2 * (factorKSpuare - 1)) * norm);
			coeff_a2 = static_cast<float>((1 - x1 + factorKSpuare) * norm);
			coeff_b1 = coeff_a1;
			coeff_b2 = static_cast<float>((1 - x2 + factorKSpuare) * norm);
		}
		break;
	}
	case Bandpass:
	{
		double const norm = 1 / (1 + factorK / qualityFactor + factorKSpuare);
		coeff_a0 = static_cast<float>(factorK / qualityFactor * norm);
		coeff_a1 = 0;
		coeff_a2 = -coeff_a0;
		coeff_b1 = static_cast<float>(2 * (factorKSpuare - 1) * norm);
		coeff_b2 = static_cast<float>((1 - factorK / qualityFactor + factorKSpuare) * norm);
		break;
	}
	case Notch:
	{
		double const norm = 1 / (1 + factorK / qualityFactor + factorKSpuare);
		coeff_a0 = static_cast<float>((1 + factorKSpuare) * norm);
		coeff_a1 = static_cast<float>(2 * factorKSpuare - 1) * norm;
		coeff_a2 = coeff_a0;
		coeff_b1 = coeff_a1;
		coeff_b2 = static_cast<float>((1 - factorK / qualityFactor + factorKSpuare) * norm);
		break;
	}
	case Highshelf:
	{
		double const x1 = sqrt(2 * factorV) * factorK;
		double const x2 = sqrt(2) * factorK;
		if (peakGain >= 0)
		{
			double const norm = 1 / (1 + x2 + factorKSpuare);
			coeff_a0 = static_cast<float>((factorV + x1 + factorKSpuare) * norm);
			coeff_a1 = static_cast<float>(2.0 * (factorKSpuare - factorV) * norm);
			coeff_a2 = static_cast<float>((factorV - x1 + factorKSpuare) * norm);
			coeff_b1 = static_cast<float>(2.0 * (factorKSpuare - 1) * norm);
			coeff_b2 = static_cast<float>((1.0 - x2 + factorKSpuare) * norm);
		}
		else
		{
			double const norm = 1.0 / (factorV + x1 + factorKSpuare);
			coeff_a0 = static_cast<float>((1.0 + x1 + factorKSpuare) * norm);
			coeff_a1 = static_cast<float>(2.0 * (factorKSpuare - 1.0) * norm);
			coeff_a2 = static_cast<float>((1.0 - x1 + factorKSpuare) * norm);
			coeff_b1 = static_cast<float>(2.0 * (factorKSpuare - factorV) * norm);
			coeff_b2 = static_cast<float>((factorV - x1 + factorKSpuare) * norm);
		}
		break;
	}
	case Lowshelf:
	{
		double const x3 = sqrt(2) * factorK;
		double const x1 = sqrt(2.0 * factorV) * factorK;
		double const x2 = factorV * factorKSpuare;
		if (peakGain >= 0)
		{
			double const norm = 1.0 / (1.0 + x3 + factorKSpuare);
			coeff_a0 = static_cast<float>((1.0 + x1 + x2) * norm);
			coeff_a1 = static_cast<float>(2.0 * (x2 - 1.0) * norm);
			coeff_a2 = static_cast<float>((1.0 - x1 + x2) * norm);
			coeff_b1 = static_cast<float>(2.0 * (factorKSpuare - 1.0) * norm);
			coeff_b2 = static_cast<float>((1.0 - x3 + factorKSpuare) * norm);
		}
		else
		{
			double const norm = 1.0 / (1.0 + x1 + x2);
			coeff_a0 = static_cast<float>((1.0 + x3 + factorKSpuare) * norm);
			coeff_a1 = static_cast<float>(2.0 * (factorKSpuare - 1.0) * norm);
			coeff_a2 = static_cast<float>((1.0 - x3 + factorKSpuare) * norm);
			coeff_b1 = static_cast<float>(2.0 * (x2 - 1.0) * norm);
			coeff_b2 = static_cast<float>((1.0 - x1 + x2) * norm);
		}
		break;
	}
	}
}

float Biquad::ProcessSample(float const sample)
{
	if (type != BiquadType::None)
	{
		float const fCurOut = sample * coeff_a0 + in_z1;
		in_z1 = sample * coeff_a1 + in_z2 - coeff_b1 * fCurOut;
		in_z2 = sample * coeff_a2 - coeff_b2 * fCurOut;
		return fCurOut;
	}
	else
	{
		return sample;
	}
}