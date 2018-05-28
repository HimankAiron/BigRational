#ifndef RATIONAL_H_
#define RATIONAL_H_
#include "Integer.h"

#include <iostream>
#include <string>
using namespace std;
class Rational
{
public:
	Rational();
	Rational(std::string num, std::string den);
	Rational(Integer& a);
	Rational(Integer& a,Integer& b);
	Rational(Integer& a,Integer& b,Integer& c);
	Rational(const Rational& obj);
	Rational(string& value);
	//Rational(Rational&& obj);
	Rational operator=(const Rational& obj);
	Rational operator + ();
	Rational operator - ();
	//Rational operator=(Rational&& obj);
	~Rational();
	/* mathematical operations */
	Rational operator + (const Rational& operand);	
	Rational operator - (const Rational& operand);
	Rational operator * (const Rational& operand);
	Rational operator / (const Rational& operand);
	

	/*Compound assignment operators*/
	Rational operator += (const Rational& operand);
	Rational operator -= (const Rational& operand);
	Rational operator *= (const Rational& operand);
	Rational operator /= (const Rational& operand);
	/* logical operators */
	bool operator < (const Rational& operand);
	bool operator > (const Rational& operand);
	bool operator <= (const Rational& operand);
	bool operator >= (const Rational& operand);
	bool operator == (const Rational& operand);
	bool operator != (const Rational& operand);

	/* output */
	friend std::ostream&operator<<(std::ostream& os, const Rational& output);
	friend std::istream&operator>>(std::istream& is, Rational& input);

	//internal function

	std::string toDecimalString(int numberOfDecimals);
	std::string toFractionString();

private:
	struct PrivateRec;
	PrivateRec * p;
};

#endif