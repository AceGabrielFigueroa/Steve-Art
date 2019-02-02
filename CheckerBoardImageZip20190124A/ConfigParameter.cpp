#include "stdafx.h"
#include "ConfigParameter.h"


ConfigParameter::ConfigParameter()
{
}

ConfigParameter::ConfigParameter(const ConfigParameter& cp)
{
	m_paramId = cp.m_paramId;
	m_isString = cp.m_isString;
	m_strValue = cp.m_strValue;
	m_numValue = cp.m_numValue;
}


ConfigParameter::~ConfigParameter()
{
}

ConfigParameter::ConfigParameter(int paramId, std::string str)
{
	m_paramId = paramId;
	m_isString = true;
	m_strValue = str;
	m_numValue = 0;
}

ConfigParameter::ConfigParameter(int paramId, int value)
{
	m_paramId = paramId;
	m_isString = false;
	m_strValue = "";
	m_numValue = value;
}
