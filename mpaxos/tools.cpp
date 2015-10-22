/*
 *
 *Author:loli
 *Date: 2014/8/25
 *Usage:C++ tools 
 *
 */
#include <fstream>
#include <iostream>
#include <sstream>

std::string int2string(int i)
{
	std::ostringstream ost;
	ost << i;
	return ost.str();
}
