#include "stdafx.h"
#include "Exception.h"


Exception::Exception(const std::string& text)
{
	m_text = text;
}


Exception::~Exception()
{
}
