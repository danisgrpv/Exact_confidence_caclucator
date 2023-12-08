#include "normal_cdf.h"

int get_success(double mean, double std, double lower_fact, double upper_fact, double survival)
{
	double lower_bound;
	double upper_bound;
	double coverage;
	double success;

	lower_bound = mean - std * lower_fact;
	upper_bound = mean + std * upper_fact;
	coverage = normal_cdf(upper_bound) - normal_cdf(lower_bound);

	if (coverage >= survival)
	{
		success = 1;
	}
	else
	{
		success = 0;
	}

	return success;
}