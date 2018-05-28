#ifndef INTEGER_H_
#define INTEGER_H_
#include <iostream>
#include <string>


class Integer
{
public:
	Integer();
	Integer(const std::string& value);
	Integer(const int& value);
	Integer(const Integer& obj);
	Integer operator = (const Integer& obj);
	~Integer();
	

	Integer operator +();
	Integer operator -();
	

	/* mathematical operations */

	Integer operator + (const Integer& operand);
	Integer operator - (const Integer& operand);
	Integer operator * (const Integer& operand);
	Integer operator / (const Integer& operand);
	Integer operator % (const Integer& operand);

	/* compound assignment operations */
	Integer operator += (const Integer& operand);
	Integer operator -= (const Integer& operand);
	Integer operator *= (const Integer& operand);
	Integer operator /= (const Integer& operand);
	Integer operator %= (const Integer& operand);

	/* logical operators */
	bool operator < (const Integer& operand);
	bool operator > (const Integer& operand);
	bool operator <= (const Integer& operand);
	bool operator >= (const Integer& operand);
	bool operator == (const Integer& operand);
	bool operator != (const Integer& operand);

	//type casting to string
	std::string toString();

	/* output */
	friend std::ostream&operator<<(std::ostream& os, const Integer& output);
	friend std::istream&operator>>(std::istream& is, Integer& input);

	/*friend function for absolute value*/
	friend Integer Abs(const Integer& operand);

private:
	struct PrivateRec;
	PrivateRec* p;
};
Integer GCD(Integer& num1, Integer& num2);
Integer Pow(Integer& num, int power);
Integer Abs(const Integer& operand);

#endif