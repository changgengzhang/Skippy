#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

using namespace std;


class Exception : public exception
{

public:

	explicit Exception(const string & message) : exception(), message(message)
	{}


	virtual ~Exception(void) throw ()
	{}


	inline string getMessage(void) const
	{
		return this->message;
	}


protected:

	string message;

};


class InvalidDimensionsException : public Exception
{

public:

	InvalidDimensionsException(const string & message) : Exception(message)
	{}

};


class InvalidCoordinatesException : public Exception
{

public:

	InvalidCoordinatesException(const string & message) : Exception(message)
	{}

};

#endif // !EXCEPTION_H

