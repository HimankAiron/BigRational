#include "Integer.h"
#include <vector>
#include <math.h>
#include <bits/stdc++.h>
#include <string>

using namespace std;
//A constant long long int value for the base that is used
//Since the maximum number a cell can hold is UINT_MAX, 
//The base would be UINT_MAX + 1, which is 4294967296
const unsigned long long int base = 4294967296; //--UINT_MAX + 1
string to_string (unsigned long long int num)
{
    string res;
    while (num>0)
    {
        res = res+(char)(num%10 + 48);
        num /= 10;
    }
    reverse(res.begin(), res.end());
    return res;
}
//Helper functions
vector<unsigned int> Sum(vector<unsigned int> a, vector<unsigned int> b);
vector<unsigned int> Multiply(vector<unsigned int> a, vector<unsigned int> b);
vector<unsigned int> Subtract(vector<unsigned int> a, vector<unsigned int> b);

struct Integer::PrivateRec
{
	vector<unsigned int> value;
};

//The default constructor for Integer
//It initialized a Integer with zero value
//The sign bit is zero (positive)
Integer::Integer()
{
	p = new PrivateRec;
	this->p->value.push_back(0);
	this->p->value.push_back(0);
}

Integer::~Integer()
{
	delete p;
}

//The constructor used by te users. The user initializes
//a integer as Integer("1312312"). The constructor 
//extracts each value and keeps on creating a Integer 
//The subsequent values are appended to the Integer using 
//the + and the * operators. 
Integer::Integer(const string& value)
{
	this->p = new PrivateRec;
	this->p->value.push_back(0);
	this->p->value.push_back(0);
	Integer ten(10);
	string::const_iterator charIt = value.begin();

	if (*charIt == '-') {
		++charIt;
		while (charIt != value.end()) {
			*this = *this*ten + Integer(static_cast<int>(*charIt - '0'));
			++charIt;
		}
		this->p->value.pop_back();
		this->p->value.push_back(1);
	}
	else {
		while (charIt != value.end()) {
			*this = *this*ten + Integer(static_cast<int>(*charIt - '0'));
			++charIt;
		}
	}

}


//The Integer constuctor. The constructor is responsible for creating a 
//Integer out of a respective Integer value. The constructor takes the
//value and creates a Integer with one cell value and a positive or a 
//negitive sign. The Integer will always be a one cell value because the 
//maximum value a cell can hold is greater than the max value of an int. 
Integer::Integer(const int& value)
{
	this->p = new PrivateRec;
	int temp = abs(value);
	if (temp == 0)
	{
		this->p->value.push_back(0);
	}
	else
	{
		while (temp != 0)
		{
			this->p->value.push_back(temp % base);
			temp /= base;
		}
	}
	if (value < 0)
	{
		this->p->value.push_back(1);
	}
	else
	{
		this->p->value.push_back(0);
	}
}


//Copy constructor.
Integer::Integer(const Integer& obj)
{
	this->p = new PrivateRec;
	this->p->value = obj.p->value;
}


//copy operator overloaded
Integer Integer::operator=(const Integer& obj)
{
	if (this != &obj)
	{
		this->p->value = obj.p->value;
	}
	return *this;
}



/*mathematical operations*/

//Power
//A Integer raised to the power (int value). The operation returns 
//A integer. the implementation uses a recursive approach using the * 
//operator. 
Integer Pow(Integer& num, int power)
{
	if (power <= 0)
	{
		return Integer(1);
	}
	return num * Pow(num, power - 1);
}

//Absolute
//Takes a Integer. Changes its sign value to zero (to make it positive)
//and returns a Integer. This is a friend function to the Integer class
//because it has to access the inner vector of the Integer value
Integer Abs(const Integer& operand)
{
	Integer answer = operand;
	answer.p->value[answer.p->value.size() - 1] = 0;
	return answer;
}


// Unary Operators

Integer Integer::operator +()
{
	return *this ;
}


Integer Integer::operator -()
{
	Integer one("-1");
	*this = (*this)*one;
	return *this;
}



// Arithmetic Operators

//addition
//The add operator will either add, or subtract two respective Integers based
//on the sign values. the respective cases are handled under different if clauses
Integer Integer::operator + (const Integer& operand)
{
	Integer answer;
	int sizeThis = this->p->value.size();
	int sizeOther = operand.p->value.size();
	//a+b
	if (this->p->value[sizeThis - 1] == 0 && operand.p->value[sizeOther - 1] == 0)
	{
		answer.p->value = Sum(this->p->value, operand.p->value);
		answer.p->value.push_back(0);
	}
	//-a-b = -(a+b)
	else if (this->p->value[sizeThis - 1] == 1 && operand.p->value[sizeOther - 1] == 1)
	{
		answer.p->value = Sum(this->p->value, operand.p->value);
		answer.p->value.push_back(1);
	}
	//b is negative
	else if (this->p->value[sizeThis - 1] == 0 && operand.p->value[sizeOther - 1] == 1) 
	{
		if (*this >= Abs(operand))//a-b
		{
			answer.p->value = Subtract(this->p->value, operand.p->value);
			answer.p->value.push_back(0);
		}
		else //-(b-a) Abs(b) is greater than a
		{
			answer.p->value = Subtract(operand.p->value, this->p->value);
			answer.p->value.push_back(1);
		}
		//a is negative
	}
	else if (this->p->value[sizeThis - 1] == 1 && operand.p->value[sizeOther - 1] == 0) 
	{
		if (Abs(*this) <= operand)//-a+b
		{
			answer.p->value = Subtract(operand.p->value, this->p->value);
			answer.p->value.push_back(0);
		}
		else //-a+b where Abs(a) is greater than b = -(a-b)
		{
			answer.p->value = Subtract(this->p->value, operand.p->value);
			answer.p->value.push_back(1);
		}
	}
	return answer;
}

//subtraction
//The subtract operator will either add, or subtract two respective Integers based
//on the sign values. the respective cases are handled under different if clauses
Integer Integer::operator - (const Integer& operand)
{
	Integer answer;
	int sizeThis = this->p->value.size();
	int sizeOther = operand.p->value.size();
	//a-b
	if (this->p->value[sizeThis - 1] == 0 && operand.p->value[sizeOther - 1] == 0)
	{
		//a-b
		if (*this >= operand)
		{
			answer.p->value = Subtract(this->p->value, operand.p->value);
			answer.p->value.push_back(0);
		}
		else //a-b = -(b-a)
		{
			answer.p->value = Subtract(operand.p->value, this->p->value);
			answer.p->value.push_back(1);

		}
	}
	//-a-(-b) = -a+b
	else if (this->p->value[sizeThis - 1] == 1 && operand.p->value[sizeOther - 1] == 1)
	{
		if (Abs(*this) <= Abs(operand)) //b-a
		{
			answer.p->value = Subtract(operand.p->value, this->p->value);
			answer.p->value.push_back(0);
		}
		else//-a+b = -(a-b)
		{
			answer.p->value = Subtract(this->p->value, operand.p->value);
			answer.p->value.push_back(1);
		}
	}
	//b is negative
	else if (this->p->value[sizeThis - 1] == 0 && operand.p->value[sizeOther - 1] == 1) 	
	{
		//a-(-b) = a+b
		answer.p->value = Sum(this->p->value, operand.p->value);
		answer.p->value.push_back(0);
	}
	//a is negative -a-b = -(a+b)
	else if (this->p->value[sizeThis - 1] == 1 && operand.p->value[sizeOther - 1] == 0) 	
	{
		answer.p->value = Sum(this->p->value, operand.p->value);
		answer.p->value.push_back(1);
	}
	return answer;
}

//multiplication
//Does the paper multiplication for two numbers. each value is multiplicand 
//is calculated and added to the previous value. the final answer is appended
//with a sign based on the sign of the two Integers
Integer Integer::operator * (const Integer& operand)
{
	Integer answer;
	int sizeThis = this->p->value.size();
	int sizeOther = operand.p->value.size();
	answer.p->value = Multiply(this->p->value, operand.p->value);
	if ((this->p->value[sizeThis - 1] == 0 && operand.p->value[sizeOther - 1] == 0) || (this->p->value[sizeThis - 1] == 1 && operand.p->value[sizeOther - 1] == 1))
	{
		answer.p->value.push_back(0);
	}
	else
	{
		answer.p->value.push_back(1);
	}
	return answer;
}


//division
//Does the paper division for two numbers.It uses the BaseCaseDivRem approach
//for the division. 
Integer Integer::operator / (const Integer& operand)
{
	Integer dividand(Abs(*this));
	Integer divisor(Abs(operand));

	if (dividand < divisor)
	{
		return Integer(0);
	}

	Integer two(2);
	Integer baseValue(to_string(base));
	Integer quotient(0);
	int k = 0;

	//NORMALIZATION STEP
	//if B is not normalized we compute A' = 2^k A and 
	//B' = 2^k B
	while (divisor.p->value[divisor.p->value.size() - 2] < (base / 2))
	{
		divisor = divisor * two;
		dividand = dividand * two;
		k++;
	}

	//calculating size
	int sizeThis = dividand.p->value.size();
	int sizeOther = divisor.p->value.size();


	int m = sizeThis - sizeOther;

	//quotient initialization
	unsigned int * q;
	q = new unsigned int[m + 1];


	Integer BmB = Pow(baseValue, m) * divisor;
	if (dividand >= BmB)
	{
		q[m] = 1;
		dividand = dividand - BmB;
	}
	else
	{
		q[m] = 0;
	}

	for (int i = m - 1; i >= 0; i--)
	{
		unsigned long long int valOne;
		unsigned long long int valTwo;
		if (sizeOther - 1 + i <= dividand.p->value.size() - 2)
		{
			valOne = dividand.p->value[sizeOther - 1 + i];
		}
		else
		{
			valOne = 0;
		}
		if (sizeOther - 1 + i - 1 <= dividand.p->value.size() - 2)
		{
			valTwo = dividand.p->value[sizeOther - 1 + i - 1];
		}
		else
		{
			valTwo = 0;
		}
		unsigned long long int temp = (valOne * base + valTwo) / (unsigned long long int)divisor.p->value[sizeOther - 2];

		if (temp > base - 1)
		{
			q[i] = base - 1;
		}
		else
		{
			q[i] = temp;
		}
		Integer Bj = Pow(baseValue, i);
		Integer QBjB = Integer(to_string(q[i]))*Bj*divisor;
		dividand = dividand - QBjB;
		while (dividand < Integer(0))
		{
			q[i] = q[i] - 1;
			dividand = dividand + Bj*divisor;
		}

	}

	for (int i = 0; i <= m; i++)
	{
		quotient = quotient + Integer(to_string(q[i]))* Pow(baseValue, i);
	}

	if (quotient.p->value.size() >= 2)
	{
		while (quotient.p->value[quotient.p->value.size() - 1] == 0 && quotient.p->value.size() >= 2)
		{
			quotient.p->value.pop_back();
		}
	}

	if ((this->p->value[this->p->value.size() - 1] == 0 && operand.p->value[operand.p->value.size() - 1] == 0) || (this->p->value[this->p->value.size() - 1] == 1 && operand.p->value[operand.p->value.size() - 1] == 1))
	{
		quotient.p->value.push_back(0);
	}
	else
	{
		quotient.p->value.push_back(1);
	}

	return quotient;
}


//modulus
//uses the / operator to calculate the modulus. Since the modulus can be different
//based on the sign values of the two numbers. It is handled under different if 
//conditions. The quotient calculation is done on Absolute values. 
Integer Integer::operator % (const Integer& operand)
{
	Integer dividand = Abs(*this);
	Integer divisor = Abs(operand);
	int sizeThis = this->p->value.size();
	int sizeOther = operand.p->value.size();
	if (this->p->value[sizeThis - 1] == 0 && operand.p->value[sizeOther - 1] == 1)
	{
		Integer quotient = dividand / divisor;
		Integer remainder = ((quotient + Integer(1)) * divisor) - dividand;
		if (remainder != Integer(0))
		{
			remainder.p->value.pop_back();
			remainder.p->value.push_back(1);
		}
		return remainder;
	}
	else if (this->p->value[sizeThis - 1] == 1 && operand.p->value[sizeOther - 1] == 0)
	{
		Integer quotient = dividand / divisor;
		Integer remainder = ((quotient + Integer(1)) * divisor) - dividand;
		return remainder;
	}
	else if (this->p->value[sizeThis - 1] == 1 && operand.p->value[sizeOther - 1] == 1)// both negative
	{
		Integer quotient = dividand / divisor;
		Integer remainder = dividand - (quotient * divisor);
		if (remainder != Integer(0))
		{
			remainder.p->value.pop_back();
			remainder.p->value.push_back(1);
		}
		return remainder;
	}
	else //both positive
	{
		Integer quotient = *this / operand;
		return *this - (quotient * operand);
	}
}





/* compound assignment operators */

Integer Integer::operator += (const Integer& operand)
{
	*this = *this + operand;
	return *this;
}


Integer Integer::operator -= (const Integer& operand)
{
	*this = *this - operand;
	return *this;
}

Integer Integer::operator *= (const Integer& operand)
{
	*this = *this*operand;
	return *this;
}

Integer Integer::operator /= (const Integer& operand)
{
	*this = *this/operand;
	return *this;
}

Integer Integer::operator %= (const Integer& operand)
{
	*this = *this%operand;
	return *this;
}




/* logical operations */

//<
bool Integer::operator < (const Integer&operand) //0 is positive, 1 is negative
{
	int sizeThis = this->p->value.size();
	int sizeOther = operand.p->value.size();
	if (this->p->value[sizeThis - 1] == 0 && operand.p->value[sizeOther - 1] == 1)
	{
		return false;
	}
	else if (this->p->value[sizeThis - 1] == 1 && operand.p->value[sizeOther - 1] == 0)
	{
		return true;
	}
	else if (this->p->value[sizeThis - 1] == 1 && operand.p->value[sizeOther - 1] == 1)// both negative
	{
		if (sizeThis < sizeOther)
		{
			return false;
		}
		else if (sizeThis > sizeOther)
		{
			return true;
		}
		else
		{
			//for (int i = 0; i < sizeOther - 1; i++)
			for (int i = sizeOther - 2; i >= 0; i--)
			{
				if (this->p->value[i]>operand.p->value[i])
				{
					return true;
				}
				else if (this->p->value[i]<operand.p->value[i])
				{
					return false;
				}
			}
			return false;
		}
	}
	else // both positive
	{
		if (sizeThis > sizeOther)
		{
			return false;
		}
		else if (sizeThis < sizeOther)
		{
			return true;
		}
		else
		{
			//for (int i = 0; i < sizeOther - 1; i++)
			for (int i = sizeOther - 2; i >= 0; i--)
			{
				if (this->p->value[i]<operand.p->value[i])
				{
					return true;
				}
				else if (this->p->value[i]>operand.p->value[i])
				{
					return false;
				}
			}
			return false;
		}
	}
}

//>
bool Integer::operator > (const Integer&operand)
{
	int sizeThis = this->p->value.size();
	int sizeOther = operand.p->value.size();
	if (this->p->value[sizeThis - 1] == 0 && operand.p->value[sizeOther - 1] == 1)
	{
		return true;
	}
	else if (this->p->value[sizeThis - 1] == 1 && operand.p->value[sizeOther - 1] == 0)
	{
		return false;
	}
	else if (this->p->value[sizeThis - 1] == 1 && operand.p->value[sizeOther - 1] == 1)// both negative
	{
		if (sizeThis < sizeOther)
		{
			return true;
		}
		else if (sizeThis > sizeOther)
		{
			return false;
		}
		else
		{
			//for (int i = 0; i < sizeOther - 1; i++)
			for (int i = sizeOther - 2; i >= 0; i--)
			{
				if (this->p->value[i]<operand.p->value[i])
				{
					return true;
				}
				else if (this->p->value[i]>operand.p->value[i])
				{
					return false;
				}
			}
			return false;
		}
	}
	else // both positive
	{
		if (sizeThis > sizeOther)
		{
			return true;
		}
		else if (sizeThis < sizeOther)
		{
			return false;
		}
		else
		{
			//for (int i = 0; i < sizeOther - 1; i++)
			for (int i = sizeOther - 2; i >= 0; i--)
			{
				if (this->p->value[i]>operand.p->value[i])
				{
					return true;
				}
				else if (this->p->value[i]<operand.p->value[i])
				{
					return false;
				}
			}
			return false;
		}
	}
}

//<=
bool Integer::operator <= (const Integer&operand)
{
	int sizeThis = this->p->value.size();
	int sizeOther = operand.p->value.size();
	if (this->p->value[sizeThis - 1] == 0 && operand.p->value[sizeOther - 1] == 1)
	{
		return false;
	}
	else if (this->p->value[sizeThis - 1] == 1 && operand.p->value[sizeOther - 1] == 0)
	{
		return true;
	}
	else if (this->p->value[sizeThis - 1] == 1 && operand.p->value[sizeOther - 1] == 1)// both negative
	{
		if (sizeThis < sizeOther)
		{
			return false;
		}
		else if (sizeThis > sizeOther)
		{
			return true;
		}
		else
		{
			//for (int i = 0; i < sizeOther - 1; i++)
			for (int i = sizeOther - 2; i >= 0; i--)
			{
				if (this->p->value[i] < operand.p->value[i])
				{
					return false;
				}
				else if (this->p->value[i] > operand.p->value[i])
				{
					return true;
				}
			}
			return true;
		}
	}
	else // both positive
	{
		if (sizeThis > sizeOther)
		{
			return false;
		}
		else if (sizeThis < sizeOther)
		{
			return true;
		}
		else
		{
			//for (int i = 0; i < sizeOther - 1; i++)
			for (int i = sizeOther - 2; i >= 0; i--)
			{
				if (this->p->value[i] > operand.p->value[i])
				{
					return false;
				}
				else if (this->p->value[i] < operand.p->value[i])
				{
					return true;
				}
			}
			return true;
		}
	}
}

//>=
bool Integer::operator >= (const Integer&operand)
{
	int sizeThis = this->p->value.size();
	int sizeOther = operand.p->value.size();
	if (this->p->value[sizeThis - 1] == 0 && operand.p->value[sizeOther - 1] == 1)
	{
		return true;
	}
	else if (this->p->value[sizeThis - 1] == 1 && operand.p->value[sizeOther - 1] == 0)
	{
		return false;
	}
	else if (this->p->value[sizeThis - 1] == 1 && operand.p->value[sizeOther - 1] == 1)// both negative
	{
		if (sizeThis < sizeOther)
		{
			return true;
		}
		else if (sizeThis > sizeOther)
		{
			return false;
		}
		else
		{
			//for (int i = 0; i < sizeOther - 1; i++)
			for (int i = sizeOther - 2; i >= 0; i--)
			{
				if (this->p->value[i]>operand.p->value[i])
				{
					return false;
				}
				else if (this->p->value[i]<operand.p->value[i])
				{
					return true;
				}
			}
			return true;
		}
	}
	else // both positive
	{
		if (sizeThis > sizeOther)
		{
			return true;
		}
		else if (sizeThis < sizeOther)
		{
			return false;
		}
		else
		{
			//for (int i = 0; i < sizeOther - 1; i++)
			for (int i = sizeOther - 2; i >= 0; i--)
			{
				if (this->p->value[i]<operand.p->value[i])
				{
					return false;
				}
				else if (this->p->value[i]>operand.p->value[i])
				{
					return true;
				}
			}
			return true;
		}
	}
}

//==
bool Integer::operator == (const Integer&operand)
{
	int sizeThis = this->p->value.size();
	int sizeOther = operand.p->value.size();
	if (sizeThis != sizeOther)
	{
		return false;
	}
	else
	{
		if (this->p->value[sizeThis - 1] != operand.p->value[sizeOther - 1])
		{
			return false;
		}
		else
		{
			for (int i = 0; i < sizeOther - 1; i++)
			{
				if (this->p->value[i] != operand.p->value[i])
				{
					return false;
				}
			}
			return true;
		}
	}
}

//!=
bool Integer::operator != (const Integer&operand)
{
	int sizeThis = this->p->value.size();
	int sizeOther = operand.p->value.size();
	if (sizeThis != sizeOther)
	{
		return true;
	}
	else
	{
		if (this->p->value[sizeThis - 1] != operand.p->value[sizeOther - 1])
		{
			return true;
		}
		else
		{
			for (int i = 0; i < sizeOther - 1; i++)
			{
				if (this->p->value[i] != operand.p->value[i])
				{
					return true;
				}
			}
			return false;
		}
	}
}


//calculate GCD (HCF) of two Integers. 
Integer GCD(Integer& num1, Integer& num2)
{
	if (num2 == Integer("0"))
	{
		return num1;
	}
	else
	{
		Integer temp = num1%num2;
		return GCD(num2, temp);
	}
}

//output operator. Takes a Integer and returns the respective 
//decimal value representation of it. 
ostream& operator<<(ostream& os, const Integer& output)
{
	Integer result(Abs(output));
	Integer temp;
	Integer ten("10"); //base converted back to decimal
	string out = "";
	if (result == Integer(0))
	{
		out += "0";
	}
	else
	{
		while (result != Integer(0))
		{
			temp = result%ten;
			result = result / ten;
			out += to_string(temp.p->value[0]);
		}
	}
	output.p->value[output.p->value.size() - 1] == 1 ? out += "-" : out += "";

	for (string::reverse_iterator rit = out.rbegin(); rit != out.rend(); ++rit)
		os << *rit;

	return os;
}


//the input operator. takes the input in string form and calls the
//Integer(string value) constructor.
istream&operator>>(istream& is, Integer& input)
{
	string in;
	is >> in;
	input = Integer(in);
	return is;
}

//Helper Functions

//Sum takes two positive vectors and returns a vector with sum of the
//two numbers. the algorithm followed is the basic schoolbook addition.
vector<unsigned int> Sum(vector<unsigned int> a, vector<unsigned int> b)
{
	vector<unsigned int> answer;
	unsigned long long int temp;
	unsigned long long int carry = 0;
	int sizea = a.size();
	int sizeb = b.size();
	//for (int i = 0; i < (sizea <= sizeb ? sizeb : sizea); i++)
	for (int i = 0; i < (sizea <= sizeb - 1 ? sizeb : sizea - 1); i++)
	{
		if (i > sizea - 2)
		{
			temp = (unsigned long long int)b[i] + carry;
			carry = temp / base;
			answer.push_back(temp % base);
		}
		else if (i > sizeb - 2)
		{
			temp = (unsigned long long int)a[i] + carry;
			carry = temp / base;
			answer.push_back(temp % base);
		}
		else
		{
			temp = (unsigned long long int)a[i] + (unsigned long long int)b[i] + carry;
			carry = temp / base;
			answer.push_back(temp % base);
		}
	}
	if (carry != 0)
	{
		while (carry != 0)
		{
			answer.push_back(carry % base);
			carry = carry / base;
		}
	}
	return answer;
}

//multiply a and b
//Multiply takes two positive vectors and returns a vector with multiplication
//of the two numbers. the algorithm followed is the basic schoolbook multiplication.
vector<unsigned int> Multiply(vector<unsigned int> a, vector<unsigned int> b)
{
	vector<unsigned int> answer;
	unsigned long long int temp;
	unsigned long long int carry = 0;
	int sizea = a.size();
	int sizeb = b.size();
	//for (int i = 0; i < sizea; i++)
	for (int i = 0; i < sizea - 1; i++)
	{
		//for (int j = 0; j < sizeb; j++)
		for (int j = 0; j < sizeb - 1; j++)
		{
			temp = (unsigned long long int)a[i] * (unsigned long long int)b[j] + carry;
			if (answer.size() == 0)
			{
				answer.push_back(temp % base);
				carry = temp / base;
			}
			else if (i + j > answer.size() - 1)
			{
				answer.push_back(temp % base);
				carry = temp / base;
			}
			else
			{
				unsigned long long int temp1 = (unsigned long long int)answer[i + j] + temp;
				answer[i + j] = (temp1) % base;
				carry = temp1 / base;
			}
		}
		if (carry != 0)
		{
			while (carry != 0)
			{
				answer.push_back(carry % base);
				carry = carry / base;
			}
		}
	}
	if (answer.size() >= 2)
	{
		while (answer[answer.size() - 1] == 0 && answer.size() >= 2)
		{
			answer.pop_back();
		}
	}
	return answer;
}

//subtract a and b as long as a is greater than b
//since a < b will result in b-a and appending negative sign. this function always 
//assumes that a will be a greater vector than b. the algorithm is simple schoolbook 
//subtraction
vector<unsigned int> Subtract(vector<unsigned int> a, vector<unsigned int> b)
{
	vector<unsigned int> answer;
	unsigned long long int temp;
	unsigned int carry = 0;
	int sizea = a.size();
	int sizeb = b.size();

	//for (int i = 0; i < (sizea <= sizeb ? sizeb : sizea); i++)
	for (int i = 0; i < (sizea <= sizeb - 1 ? sizeb : sizea - 1); i++)
	{
		if (i > sizea - 2)
		{
			temp = b[i];
			answer.push_back(temp);
		}
		else if (i > sizeb - 2)
		{
			temp = a[i];
			answer.push_back(temp);
		}
		else
		{
			if (a[i] >= b[i])
			{
				temp = (unsigned long long int)a[i] - (unsigned long long int)b[i];
				answer.push_back(temp);
			}
			else
			{
				//adding base to a[i]
				unsigned long long int temp1 = (unsigned long long int)a[i] + base - (unsigned long long int)b[i];
				answer.push_back(temp1);
				if (a[i + 1] > 0)
				{
					a[i + 1] -= 1;
				}
				else
				{
					int j = i + 1;
					while (a[j] == 0)
					{
						a[j] = base - 1;
						j++;
					}
					a[j] -= 1;
				}
			}
		}
	}
	if (answer.size() >= 2)
	{
		while (answer[answer.size() - 1] == 0 && answer.size() >= 2)
		{
			answer.pop_back();
		}
	}
	return answer;
}

//type casting to string
//instead of outputting the number after its respective decimal value
//representation. this returns a string with the answer. 
string Integer::toString()
{
	Integer result(Abs(*this));
	Integer temp;
	Integer ten("10"); //base converted back to decimal
	string out = "";
	if (result == Integer(0))
	{
		out += "0";
	}
	else
	{
		while (result != Integer(0))
		{
			temp = result%ten;
			result = result / ten;
			out += to_string(temp.p->value[0]);
		}
	}
	this->p->value[this->p->value.size() - 1] == 1 ? out += "-" : out += "";

	reverse(out.begin(),out.end());
	return out;
}




// int main()
// {
// 	/* Numbers */
// 	/* sample code to run functions on integers */
// 	Integer a("-422223423531234");
// 	Integer b("10111");
// 	Integer c("122313124");
// 	Integer d = Integer(b);

// 	Integer e("5655770");
// 	Integer f("1131154");
// 	-a;
// 	cout << "a = " << a<<endl;
// 	cout << "b = " << b << endl;
// 	cout << "c = " << c << endl;
// 	cout << "d = " << d << endl;

// 	//mathematical operations
// 	cout << "a + b = " << a + b << endl;
// 	cout << "a - b = " << a - b << endl;
// 	cout << "a * b = " << a*b << endl;
// 	cout << "a / b = " << a / b << endl;
// 	cout << "a % b = " << a % b << endl;

// 	//GCD
// 	cout << "GCD of a and b = " << GCD(a, b) << endl;


// 	//Logical Operations
// 	if (a < b) {
// 		cout << "a is lesser than b" << endl;
// 	}
// 	else
// 		cout << "a is greater than b" << endl;

// 	if (b > a) {
// 		cout << "b is greater than a" << endl;
// 	}
// 	else
// 		cout << "a is greater than b" << endl;

// 	if (c == d) {
// 		cout << "c is equal to d" << endl;
// 	}
// 	else
// 		cout << "c is not equal to d" << endl;

// 	if (a != b) {
// 		cout << "a is not equal to b" << endl;
// 	}
// 	else
// 		cout << "a is equal to b" << endl;

// }