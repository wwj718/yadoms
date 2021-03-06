#include "stdafx.h"
#include "ZWaveConfiguration.h"


CZWaveConfiguration::~CZWaveConfiguration()
{
}

void CZWaveConfiguration::initializeWith(shared::CDataContainer container)
{
   m_container.initializeWith(container);
}


std::string CZWaveConfiguration::getSerialPort() const
{
   return m_container.get<std::string>("Serial Port");
}

bool CZWaveConfiguration::getIncludeSystemKeywords() const
{
   return m_container.getWithDefault<bool>("IncludeSystemKeywords", false);
}

std::string CZWaveConfiguration::getPath() const
{
   return m_container.get<std::string>("libraryPath");
}

void CZWaveConfiguration::setPath(const std::string & path)
{
   m_container.set("libraryPath", path);
}


