
static inline float randf(float scale)
{
	// return a float between 0 and scale
	return (float)(random(1000001))/1000000.0f*scale;
}

static inline float randfRange(float min, float max)
{
	// return a float between min and max
	float scale = max-min;
	return min + (float)(random(1000001))/1000000.0f*scale;
}

static inline float randfRangeMaybe(float min, float max, float likelihood, float def)
{
	if (randf(1) < likelihood)
		return def;

	// return a float between min and max
	float scale = max-min;
	return min + (float)(random(1000001))/1000000.0f*scale;
}

#define randi random

static inline float randiRange(uint32_t min, uint32_t max)
{
	// return an int between min and max
	return min + random(max-min);
}

