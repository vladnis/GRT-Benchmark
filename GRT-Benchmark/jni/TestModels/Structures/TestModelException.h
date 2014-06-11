/*
 * TestModelException.h
 *
 *  Created on: Jun 10, 2014
 *      Author: Nistorica Vlad
 */

#ifndef TESTMODELEXCEPTION_H_
#define TESTMODELEXCEPTION_H_

#include <iostream>
#include <exception>

class TestModelException : public std::exception {
private:
  std::string msg;
public:
  TestModelException(std::string m= "exception!") : msg(m) {};
  ~TestModelException() throw() {};
  const char* what() const throw() { return msg.c_str(); }

};

#endif /*TESTMODELEXCEPTION_H_*/
