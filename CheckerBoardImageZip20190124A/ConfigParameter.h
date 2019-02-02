#pragma once
#include <afx.h>
#include <iostream>

class ConfigParameter :
	public CObject
{
public:
	ConfigParameter(int argId, std::string str );
	ConfigParameter(int argId, int value);
	ConfigParameter(const ConfigParameter& cp);
	ConfigParameter();
	~ConfigParameter();
//private:
	int m_paramId;
	boolean m_isString;	// false imples number
	std::string m_strValue;
	int m_numValue;
};

