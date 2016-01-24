#include "math.h"

/********************************************************************************
		Math Functions:
********************************************************************************/

double pow(double base, double power)
{
	if (power == 0)
		return 1;

	double result = base;
	double root_power = mod(abs(power), 1);
	int int_power = power - root_power;
	// takes care of integer exponent:
	for (int count = abs(int_power); count != 0; count--)
		result *= base;
	// takes care of decimal exponent (in form pow/root):
	double dx = 0.0001;
	int numerator;
	int denominator;
	for (numerator = 1; numerator < denominator; numerator++)
		for (; (numerator/denominator) - root_power <= dx; denominator++)
		{}

	return result * (base * numerator) * root(base, denominator);
}
double root(double m_nValue, double index, double guess, double pc)
{
   double result = guess;
   double result_next;
   do
   {
       result_next = (1.0/index)*((index-1.0)*result+(m_nValue)/(pow(result,(index-1.0))));
       result = result_next;
       pc--;
   }while(pc>1);
   return result;
}
double abs(double num)
{
	if (num >= 0)
		return num;
	else
		return -num;
}
uint8_t sign(double num)
{
	if (num > 0)
		return 1;
	if (num < 0)
		return -1;
	else
		return 0;
}
// Two method function. Returns the modulus or the decimal of a double.
int mod(double num1, double num2)
{
	double remainder = num1;

	if (num2 = 1)
	{
		while (remainder > 1)
			remainder -= 1;
		return remainder;
	}

	while (remainder >= num2)
		remainder /= num2;
	return remainder;
}