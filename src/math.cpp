	#include "math.h"

/********************************************************************************
		Math Functions:
********************************************************************************/

float pow(float base, float power)
{
	if (power == 0)
		return 1;

	float result = base;
	float root_power = mod(abs(power), 1);
	int int_power = power - root_power;
	// takes care of integer exponent:
	for (int count = abs(int_power); count != 0; count--)
		result *= base;
	// takes care of decimal exponent (in form pow/root):
	float dx = 0.0001;
	int numerator;
	int denominator;
	for (numerator = 1; numerator < denominator; numerator++)
		for (; (numerator/denominator) - root_power <= dx; denominator++)
		{}

	return result * (base * numerator) * root(base, denominator);
}
float root(float m_nValue, float index, float guess, float pc)
{
   float result = guess;
   float result_next;
   do
   {
       result_next = (1.0/index)*((index-1.0)*result+(m_nValue)/(pow(result,(index-1.0))));
       result = result_next;
       pc--;
   }while(pc>1);
   return result;
}
float abs(float num)
{
	if (num >= 0)
		return num;
	else
		return -num;
}
float sign(float num)
{
	if (num > 0)
		return 1;
	if (num < 0)
		return -1;
	else
		return 0;
}
// Two method function. Returns the modulus or the decimal of a double.
float mod(float num1, float num2)
{
	float remainder = num1;

	if (num2 == 1)
	{
		while (remainder > 1)
			remainder -= 1;
		return remainder;
	}

	while (remainder >= num2)
		remainder /= num2;
	return remainder;
}