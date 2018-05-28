#include "Rational.h"
#include "Integer.h"
//#include "Integer.cpp"
#include <vector>

using namespace std;

struct Rational::PrivateRec
{
	Integer numerator;
	Integer denominator;
};

//The default constructor intializes a rational number with 
//zero number. The denominator is 1 to handle the corner case
//for division by zero. 
Rational::Rational()
{
	p = new PrivateRec;
	p->numerator = Integer("0");
	p->denominator = Integer("1");
}

Rational::~Rational()
{
	delete p;
}

Rational::Rational(std::string num, std::string den)
{
	p = new PrivateRec;
	p->numerator = Integer(num);
	p->denominator = Integer(den);
}
//The constructor used by te users. The user initializes
//a Rational as Rational("1312312"). The constructor 
//extracts each value and keeps on creating a Rational 
//The subsequent values are appended to the Rational using 
//the + and the * operators.


// TTTTTTTTTTTTTTTTTOOOOOOOOOOOOODODDDDDDDDDDDDDDDDDDDDDDDDDDDDOooooooooooooooooooooooo
Rational::Rational( string& value)
{
	this->p = new PrivateRec;

	string::const_iterator charIt = value.begin();
	
	int locdot = -1;
	int locdiv = -1;
	while(charIt != value.end())
	{
		if(*charIt == '.')
			locdot = charIt -value.begin();
		if(*charIt == '/')
			locdiv = charIt - value.begin();
		charIt++;
	}
	if(locdot == -1 and locdiv == -1)
	{
		this->p->numerator = Integer(value);
		this->p->denominator = Integer("1");
	}
	else if(locdot == -1)
	{
		string num ,den;
		num = value.substr(0,locdiv);
		den = value.substr(locdiv+1);
		Integer num_temp = Integer(num);
		Integer den_temp = Integer(den);
		Integer gcd = GCD(num_temp,den_temp);
		this->p->numerator = (num_temp)/gcd;
		this->p->denominator = den_temp/gcd;

	}
	else if(locdiv == -1)
	{
		string num ,den;
		num = value.substr(0,locdot);
		den = value.substr(locdot+1);
		Integer num_temp = Integer(num) + Integer(den);
		Integer den_temp = Integer("1");
		this->p->numerator = (num_temp);
		this->p->denominator = den_temp;
	}
	else
	{
		string whole, num ,den;
		whole = value.substr(0,locdot);
		num = value.substr(locdot+1,locdiv-locdot-1);
		den = value.substr(locdiv+1);
		Integer num_temp = Integer(whole)*Integer(den) + Integer(num);
		Integer den_temp = Integer(den);
		Integer gcd = GCD(num_temp,den_temp);
		this->p->numerator = (num_temp)/gcd;
		this->p->denominator = den_temp/gcd;
	}
	
}


//copy constructor
Rational::Rational(const Rational& obj)
{
	this->p = new PrivateRec;
	this->p->numerator = obj.p->numerator;
	this->p->denominator = obj.p->denominator;
}


//The Rational constuctor. The constructor is responsible for creating a 
//Rational out of a respective Integer value. The constructor takes the
//Integer and creates a Rational with one numerator as that integer and with
// denominator as 1

Rational::Rational(Integer& a)
{
	this->p = new PrivateRec;
	this->p->numerator = a;
	this->p->denominator = Integer("1");
}



//The Rational constuctor. The constructor is responsible for creating a 
//Rational out of a respective two Integer values. The constructor takes the
//Integer and creates a Rational with one numerator as that integer of "a" and with
// denominator as integer "b"

Rational::Rational(Integer& a,Integer& b)
{
	this->p = new PrivateRec;

	if(b < Integer("0"))
	{	
		-a;
		-b;
	}
	Integer gcd = GCD(a,b);
	this->p->numerator = a/gcd;
	this->p->denominator = b/gcd;
}

//The Rational constuctor. The constructor is responsible for creating a 
//Rational out of a respective two Integer values. The constructor takes the
//Integer and creates a Rational as a + b/c form
Rational::Rational( Integer& a, Integer& b, Integer& c)
{
	Integer num_temp = a*c + b;
	Integer den_temp = c;

	if(den_temp < Integer("0"))
	{	
		-num_temp;
		-den_temp;
	}
	Integer gcd = GCD(num_temp,den_temp);
	num_temp = num_temp/gcd;
	den_temp = c/gcd;
	this->p = new PrivateRec;

	this->p->numerator = num_temp;
	this->p->denominator = den_temp;
}




//copy operator overloaded
Rational Rational::operator = (const Rational& obj)
{
	if (this != &obj)
	{
		this->p->numerator = obj.p->numerator;
		this->p->denominator = obj.p->denominator;
	}
	return *this;
}



//unary operators
Rational Rational::operator +()
{
	return *this;
}

Rational Rational::operator - ()
{

	this->p->numerator = Integer("-1")*this->p->numerator;

	return *this;
}





//the addition is handled by using the Integer operators. 
// x/y + z/w = (x*w + z*y)/yw
//the numerator and the denominator are then divided by their
//GCD to simplify the result
Rational Rational::operator + (const Rational& operand)
{
	Rational answer;
	answer.p->denominator = this->p->denominator * operand.p->denominator;
	answer.p->numerator = (this->p->numerator * operand.p->denominator) + (this->p->denominator * operand.p->numerator);
	Integer temp = Abs(answer.p->numerator);
	Integer gcd = GCD(answer.p->denominator, temp);
	answer.p->denominator = answer.p->denominator / gcd;
	answer.p->numerator = answer.p->numerator / gcd;
	return answer;
}

//the subtractio is handled by using the Integer operators. 
// x/y - z/w = (x*w - z*y)/yw
//the numerator and the denominator are then divided by their
//GCD to simplify the result
Rational Rational::operator - (const Rational& operand)
{
	Rational answer;
	answer.p->denominator = this->p->denominator * operand.p->denominator;
	answer.p->numerator = (this->p->numerator * operand.p->denominator) - (this->p->denominator * operand.p->numerator);
	Integer temp = Abs(answer.p->numerator);
	Integer hcf = GCD(answer.p->denominator, temp);
	answer.p->denominator = answer.p->denominator / hcf;
	answer.p->numerator = answer.p->numerator / hcf;
	return answer;
}

//the multiplication is handled by using the Integer operators. 
// x/y * z/w = xz/yw
//the numerator and the denominator are then divided by their
//GCD to simplify the result
Rational Rational::operator * (const Rational& operand)
{
	Rational answer;
	answer.p->denominator = this->p->denominator * operand.p->denominator;
	answer.p->numerator = this->p->numerator * operand.p->numerator;
	Integer temp = Abs(answer.p->numerator);
	Integer hcf = GCD(answer.p->denominator, temp);
	answer.p->denominator = answer.p->denominator / hcf;
	answer.p->numerator = answer.p->numerator / hcf;
	return answer;
}

//the division is the same as multiplication. just with the operand inverted. 
// (x/y) / (z/w) = xw/yz
//the numerator and the denominator are then divided by their
//GCD to simplify the result
Rational Rational::operator / (const Rational& operand)
{
	Rational answer;
	Integer zero(0);
	answer.p->denominator = this->p->denominator * operand.p->numerator;
	answer.p->numerator = this->p->numerator * operand.p->denominator;
	if (answer.p->numerator < zero && answer.p->denominator < zero)
	{
		answer.p->numerator = Abs(answer.p->numerator);
		answer.p->denominator = Abs(answer.p->denominator);
	}
	else if (answer.p->numerator > zero && answer.p->denominator < zero)
	{
		answer.p->numerator = answer.p->numerator * Integer(-1);
		answer.p->denominator = Abs(answer.p->denominator);
	}	
	Integer temp = Abs(answer.p->numerator);
	Integer hcf = GCD(answer.p->denominator, temp);
	answer.p->denominator = answer.p->denominator / hcf;
	answer.p->numerator = answer.p->numerator / hcf;
	return answer;
}





// Compound assignment operators


Rational Rational::operator += (const Rational& operand)
{
	*this = *this + operand;
	return *this;
}


Rational Rational::operator -= (const Rational& operand)
{
	*this = *this - operand;
	return *this;
}

Rational Rational::operator *= (const Rational& operand)
{
	*this = (*this)*operand;
	return *this;
}

Rational Rational::operator /= (const Rational& operand)
{
	*this = *this/operand;
	return *this;
}








//< operator
bool Rational::operator < (const Rational& operand)
{
	Integer thisNumber = this->p->numerator * operand.p->denominator;
	Integer compareWith = operand.p->numerator* this->p->denominator;
	if (thisNumber<compareWith)
	{
		return true;
	}
	else
	{
		return false;
	}

}

//> operator
bool Rational::operator>(const Rational& operand)
{
	Integer thisNumber = this->p->numerator * operand.p->denominator;
	Integer compareWith = operand.p->numerator* this->p->denominator;
	if (thisNumber>compareWith)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//<= operator
bool Rational::operator<=(const Rational& operand)
{
	Integer thisNumber = this->p->numerator * operand.p->denominator;
	Integer compareWith = operand.p->numerator* this->p->denominator;
	if (thisNumber <= compareWith)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//>= operator
bool Rational::operator>=(const Rational& operand)
{
	Integer thisNumber = this->p->numerator * operand.p->denominator;
	Integer compareWith = operand.p->numerator* this->p->denominator;
	if (thisNumber >= compareWith)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//== operator
bool Rational::operator==(const Rational& operand)
{
	Integer thisNumber = this->p->numerator * operand.p->denominator;
	Integer compareWith = operand.p->numerator* this->p->denominator;
	if (thisNumber == compareWith)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//!= operator
bool Rational::operator!=(const Rational& operand)
{
	Integer thisNumber = this->p->numerator * operand.p->denominator;
	Integer compareWith = operand.p->numerator* this->p->denominator;
	if (thisNumber != compareWith)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// //output. for simple fractions(i.e. with denominator = 1),we directly print the numerator
// for 0 to 1 , fraction with \ is printed
// for greater than 1 , mixed fraction in form a.b/c is printed
std::ostream&operator<<(std::ostream& os, const Rational& output)
{
	if(output.p->denominator == Integer("1"))
		os << output.p->numerator;
	else if(output.p->denominator < output.p->numerator)
	{
		Integer whole = output.p->numerator/output.p->denominator;
		Integer remaining = output.p->numerator%output.p->denominator;
		os << whole << "." << remaining << "/" << output.p->denominator;
	}
	else
		os << output.p->numerator << "/" << output.p->denominator;
	return os;
}

//input operator
std::istream&operator>>(std::istream& is, Rational& input)
{
	string value;
	is >> value;
	input = Rational(value);
	return is;
}



int main()
{
	/* Numbers */
	/* sample code to run functions */
	Integer x("2");
	Integer y("4");
	Rational a(x,y,y);
	Rational b(x,y);
	string s = "2/4";
	Rational c(s);
	Rational d = Rational(b);
	 -a;
	 a /= b;
	cout << "a = " << a<<endl;
	cout << "b = " << b << endl;
	cout << "c = " << c << endl;
	 cout << "d = " << d << endl;

	//mathematical operations
	 cout << "a + b = " << a + b << endl;
	cout << "a - b = " << a - b << endl;
	cout << "a * b = " << a*b << endl;
	cout << "a / b = " << a / b << endl;


	//Logical Operations
	if (a <= b) {
		cout << "a is lesser than b" << endl;
	}
	else
		cout << "a is greater than b" << endl;

	if (b >= a) {
		cout << "b is greater than a" << endl;
	}
	else
		cout << "a is greater than b" << endl;

	if (a == b) {
		cout << "a is equal to b" << endl;
	}
	else
		cout << "a is not equal to b" << endl;

	if (a != b) {
		cout << "a is not equal to b" << endl;
	}
	else
		cout << "a is equal to b" << endl;

	cout << "Give input:"<<endl;
	 Rational xx;
	 cin >> xx;
	
	 cout << "Valid form for above is: ";
	 cout << xx;

}