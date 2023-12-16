/*
  ==============================================================================

    Parameters.h
    Created: 12 Dec 2023 10:09:05pm
    Author:  NeZvers

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Parameters {
	/*
	sources:
		https://github.com/Mrugalla/ParametersExample/blob/master/Source/Params.h
	*/
	using APVTS = juce::AudioProcessorValueTreeState;
	using Layout = APVTS::ParameterLayout;
	using RAP = juce::RangedAudioParameter;
	using UniqueRAP = std::unique_ptr<RAP>;
	using UniqueRAPVector = std::vector<UniqueRAP>;
	using APP = juce::AudioProcessorParameter;
	using APPCategory = APP::Category;
	using APF = juce::AudioParameterFloat;
	using RangeF = juce::NormalisableRange<float>;
	using String = juce::String;

	enum class Unit
	{
		Db,
		Hz,
		NumUnits
	};

	inline String toString(Unit unit)
	{
		switch (unit)
		{
		case Unit::Db: return "dB";
		case Unit::Hz: return "hz";
		default: return "Unknown";
		}
	}

	namespace range
	{
		inline RangeF biased(float start, float end, float bias) noexcept
		{
			// https://www.desmos.com/calculator/ps8q8gftcr
			const auto a = bias * .5f + .5f;
			const auto a2 = 2.f * a;
			const auto aM = 1.f - a;

			const auto r = end - start;
			const auto aR = r * a;
			if (bias != 0.f)
				return
			{
					start, end,
					[a2, aM, aR](float min, float, float x)
					{
						const auto denom = aM - x + a2 * x;
						if (denom == 0.f)
							return min;
						return min + aR * x / denom;
					},
					[a2, aM, aR](float min, float, float x)
					{
						const auto denom = a2 * min + aR - a2 * x - min + x;
						if (denom == 0.f)
							return 0.f;
						auto val = aM * (x - min) / denom;
						return val > 1.f ? 1.f : val;
					},
					[](float min, float max, float x)
					{
						return x < min ? min : x > max ? max : x;
					}
			};
			else return { start, end };
		}

		inline RangeF stepped(float start, float end, float steps = 1.f) noexcept
		{
			return { start, end, steps };
		}

		inline RangeF toggle() noexcept
		{
			return stepped(0.f, 1.f);
		}

		inline RangeF lin(float start, float end) noexcept
		{
			const auto range = end - start;

			return
			{
				start,
				end,
				[range](float min, float, float normalized)
				{
					return min + normalized * range;
				},
				[inv = 1.f / range](float min, float, float denormalized)
				{
					return (denormalized - min) * inv;
				},
				[](float min, float max, float x)
				{
					return juce::jlimit(min, max, x);
				}
			};
		}

		inline RangeF withCentre(float start, float end, float centre) noexcept
		{
			const auto r = end - start;
			const auto v = (centre - start) / r;

			return biased(start, end, 2.f * v - 1.f);
		}
	}

	using ValueToString = std::function<String(float, int)>;
	using StringToValue = std::function<float(const String&)>;

	namespace valToStr
	{
		inline ValueToString db()
		{
			return [](float val, int)
			{
				return String(val, 2) + " dB";
			};
		}

		inline ValueToString hz()
		{
			return [](float val, int)
			{
				if (val < 100.f)
					return String(val, 2) + " hz";
				else if (val < 1000.f)
					return String(val, 1) + " hz";
				else if (val >= 1000.f)
				{
					auto k = val / 1000.f;
					return String(k, 1) + " khz";
				}
				return String();
			};
		}

		inline ValueToString pan() {
			return [](float val, int) {
				if (val < 0.5f) {
					return String((1.0f - val / 0.5f) * 100.0f, 2) + " L";
				}
				else if (val > 0.5f) {
					return String((val - 0.5f) / 0.5f * 100.0f, 2) + " R";
				}
				else {
					return String("C");
				}
			};
		}

		inline ValueToString mix() {
			return [](float val, int) {
				return String((float)juce::roundFloatToInt(val * 100.0f), 0) + " %";
			};
		}
	}

	namespace strToVal
	{
		inline StringToValue db()
		{
			return [](const String& str)
			{
				return str.removeCharacters(toString(Unit::Db)).getFloatValue();
			};
		}

		inline StringToValue hz()
		{
			return [](const String& str)
			{
				auto s = str.removeCharacters(toString(Unit::Hz));
				if (s.endsWith("k"))
				{
					s = s.dropLastCharacters(1);
					return s.getFloatValue() * 1000.f;
				}
				return s.getFloatValue();
			};
		}

		inline StringToValue pan() {
			return [](const String& str) {
				if (str.endsWith(" L")) {
					juce::String s = str.removeCharacters(" L");
					float val = s.getFloatValue();
					return (1.0f - (val / 100.0f)) * 0.5f;
				}
				else if (str.endsWith(" R")) {
					juce::String s = str.removeCharacters(" L");
					float val = s.getFloatValue();
					return (val / 100.0f) * 0.5f + 0.5f;
				}
				else {
					return 0.5f;
				}
			};
		}

		inline StringToValue mix() {
			return [](const String& str) {
				juce::String s = str.removeCharacters(" %");
				float val = s.getFloatValue();
				return (val * 0.01f);
			};
		}
	}
}