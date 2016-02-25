#pragma once

// Return a float between 0 and scale inclusive.
static inline float randf(float scale)
{
	return (float)(random(1000001))/1000000.0f*scale;
}

// return a float between min and max inclusive.
static inline float randfRange(float min, float max)
{
	float scale = max-min;
	return min + (float)(random(1000001))/1000000.0f*scale;
}

// At a given probabiliy, return a float between min and max inclusive,
// otherwise return the default.
static inline float randfRangeMaybe(float min, float max, float likelihood, float def)
{
	if (randf(1) < likelihood)
		return def;

	float scale = max-min;
	return min + (float)(random(1000001))/1000000.0f*scale;
}

static inline long randi(int32_t max)
{
	return random(max+1);
}

static inline long randiRange(int32_t min, int32_t max)
{
	// return an int between min and max inclusive.
	return min + random(max-min+1);
}

