#include "stdafx.h"
#include "MegatecUpsConfiguration.h"

CMegatecUpsConfiguration::~CMegatecUpsConfiguration()
{
}

void CMegatecUpsConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_data.initializeWith(data);
}

std::string CMegatecUpsConfiguration::getSerialPort() const
{
   return m_data.get<std::string>("SerialPort");
}

bool CMegatecUpsConfiguration::upsBeepEnable() const
{
   return m_data.get<bool>("UpsBeep");
}

double CMegatecUpsConfiguration::outputShutdownDelay() const
{
   return m_data.get<double>("UpsShutdownTimers.content.ShutdownDelay");
}

double CMegatecUpsConfiguration::outputRestoreDelay() const
{
   return m_data.get<double>("UpsShutdownTimers.content.RestoreDelay");
}

