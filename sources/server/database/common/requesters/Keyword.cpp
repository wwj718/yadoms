#include "stdafx.h"
#include "Keyword.h"
#include <shared/exception/NotImplemented.hpp>
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "database/common/adapters/DatabaseAdapters.h"
#include <shared/exception/EmptyResult.hpp>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/OutOfRange.hpp>
#include "database/common/DatabaseTables.h"
#include "database/common/Query.h"
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include "Acquisition.h"

namespace database { namespace common { namespace requesters { 

	CKeyword::CKeyword(boost::shared_ptr<IDatabaseRequester> databaseRequester, boost::shared_ptr<CAcquisition> acquisitionRequester)
      :m_acquisitionRequester(acquisitionRequester), m_databaseRequester(databaseRequester)
   {
   }

   CKeyword::~CKeyword()
   {
   }

   bool CKeyword::keywordExists(int deviceId, const std::string& keywordName) const
   {
      try
      {
         getKeyword(deviceId, keywordName);
      }
      catch (shared::exception::CEmptyResult&)
      {
         return false;
      }
      return true;
   }

   bool CKeyword::keywordExists(int keywordId) const
   {
      try
      {
         getKeyword(keywordId);
      }
      catch (shared::exception::CEmptyResult&)
      {
         return false;
      }
      return true;
   }

   // IKeywordRequester implementation
   void CKeyword::addKeyword(const entities::CKeyword& newKeyword)
   {
      //validate keyword and capacity names; they must match URI pattern
      try
      {
         Poco::URI checkCapacity(newKeyword.CapacityName());
      }
      catch (Poco::SyntaxException & ex)
      {
         throw shared::exception::COutOfRange("The capacity name do not match naming rules : " + ex.displayText());
      }

      try
      {
         Poco::URI checkKeyword(newKeyword.Name());
      }
      catch (Poco::SyntaxException & ex)
      {
         throw shared::exception::COutOfRange("The keyword name do not match naming rules : " + ex.displayText());
      }



      CQuery qSelect = m_databaseRequester->newQuery();

      qSelect. SelectCount().
         From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
         And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

      int count = m_databaseRequester->queryCount(qSelect);
      if(count == 0)
      {
         //create the database entry with needed fields
         CQuery qInsert = m_databaseRequester->newQuery();
         qInsert.InsertInto(CKeywordTable::getTableName(), CKeywordTable::getDeviceIdColumnName(), CKeywordTable::getCapacityNameColumnName(), CKeywordTable::getAccessModeColumnName(), CKeywordTable::getNameColumnName(), CKeywordTable::getTypeColumnName(), CKeywordTable::getMeasureColumnName()).
            Values(newKeyword.DeviceId(), newKeyword.CapacityName(), newKeyword.AccessMode(), newKeyword.Name(), newKeyword.Type(), newKeyword.Measure());

         if(m_databaseRequester->queryStatement(qInsert) <= 0)
            throw shared::exception::CEmptyResult("Fail to insert keyword into table");

         //update fields
         std::string friendlyName = newKeyword.Name();
         if(newKeyword.FriendlyName.isDefined())
            friendlyName =newKeyword.FriendlyName();
         
         CQuery update = m_databaseRequester->newQuery();
         update.Update(CKeywordTable::getTableName()).Set(CKeywordTable::getFriendlyNameColumnName(), friendlyName).
            Where(CKeywordTable::getDeviceIdColumnName(),  CQUERY_OP_EQUAL, newKeyword.DeviceId()).
            And(CKeywordTable::getNameColumnName(),  CQUERY_OP_EQUAL, newKeyword.Name());

         if(m_databaseRequester->queryStatement(update) <= 0)
            throw shared::exception::CEmptyResult("Fail to update FriendlyName field");
         

         if(newKeyword.Details.isDefined())
         {
            CQuery updateDetails = m_databaseRequester->newQuery();
            updateDetails.Update(CKeywordTable::getTableName()).Set(CKeywordTable::getDetailsColumnName(), newKeyword.Details()).
               Where(CKeywordTable::getDeviceIdColumnName(),  CQUERY_OP_EQUAL, newKeyword.DeviceId()).
               And(CKeywordTable::getNameColumnName(),  CQUERY_OP_EQUAL, newKeyword.Name());

            if(m_databaseRequester->queryStatement(updateDetails) <= 0)
               throw shared::exception::CEmptyResult("Fail to update Details field");
         } 
         
         if(newKeyword.Units.isDefined())
         {
            CQuery updateUnits = m_databaseRequester->newQuery();
            updateUnits.Update(CKeywordTable::getTableName()).Set(CKeywordTable::getUnitsColumnName(), newKeyword.Units()).
               Where(CKeywordTable::getDeviceIdColumnName(),  CQUERY_OP_EQUAL, newKeyword.DeviceId()).
               And(CKeywordTable::getNameColumnName(),  CQUERY_OP_EQUAL, newKeyword.Name());

            if(m_databaseRequester->queryStatement(updateUnits) <= 0)
               throw shared::exception::CEmptyResult("Fail to update Units field");
         }    
         
		 if (newKeyword.TypeInfo.isDefined())
         {
            CQuery updateValues = m_databaseRequester->newQuery();
            updateValues.Update(CKeywordTable::getTableName()).Set(CKeywordTable::getTypeInfoColumnName(), newKeyword.TypeInfo()).
               Where(CKeywordTable::getDeviceIdColumnName(),  CQUERY_OP_EQUAL, newKeyword.DeviceId()).
               And(CKeywordTable::getNameColumnName(),  CQUERY_OP_EQUAL, newKeyword.Name());

            if(m_databaseRequester->queryStatement(updateValues) <= 0)
               throw shared::exception::CEmptyResult("Fail to update Values field");
         }
      }
      else
      {
         //already exist, do nothing
      }
   }

   void CKeyword::addKeyword(int deviceId,
                             const shared::plugin::yPluginApi::historization::IHistorizable& keyword,
                             const shared::CDataContainer& details)
   {
      entities::CKeyword keywordEntity;
      keywordEntity.DeviceId = deviceId;
      keywordEntity.CapacityName = keyword.getCapacity().getName();
      keywordEntity.AccessMode = keyword.getAccessMode();
      keywordEntity.Type = keyword.getCapacity().getType();
      keywordEntity.Units = keyword.getCapacity().getUnit();
      keywordEntity.Name = keyword.getKeyword();
      keywordEntity.FriendlyName = keyword.getKeyword();
      keywordEntity.Measure = keyword.getMeasureType();
      keywordEntity.TypeInfo = keyword.getTypeInfo();
      keywordEntity.Details = details;

      addKeyword(keywordEntity);
   }

   void CKeyword::addKeywords(int deviceId,
                              const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> >& keywords)
	{
      //TODO : mettre les m�mes contr�les que addKeyword

      // TODO : utiliser les transactions pour de meilleurs perfs (tester en utilisant fakePlugin, dont le d�marrage met plus de 5 secondes � cause de la d�claration
      // des KWs.
      // Pour �a, il faudra cr�er le dataAccessLayer pour les KWs

      for (auto kw = keywords.begin(); kw != keywords.end(); ++kw)
         addKeyword(deviceId, *(kw->get()));
   }

   boost::shared_ptr<entities::CKeyword> CKeyword::getKeyword(int deviceId, const std::string & keyword) const
   {
      adapters::CKeywordAdapter adapter;

      CQuery qSelect = m_databaseRequester->newQuery();

      qSelect. Select().
         From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId).
         And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, keyword);

      m_databaseRequester->queryEntities(&adapter, qSelect);
      if (adapter.getResults().empty())
         throw shared::exception::CEmptyResult((boost::format("Keyword name %1% for device %2% not found in database") % keyword % deviceId).str());

      return adapter.getResults().at(0);
   }

   boost::shared_ptr<entities::CKeyword> CKeyword::getKeyword(int keywordId) const
   {
      adapters::CKeywordAdapter adapter;

      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect. Select().
         From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

      m_databaseRequester->queryEntities(&adapter, qSelect);
      if (adapter.getResults().empty())
         throw shared::exception::CEmptyResult((boost::format("Keyword id %1% not found in database") % keywordId).str());

      return adapter.getResults().at(0);
   }

   std::vector<boost::shared_ptr<entities::CKeyword> > CKeyword::getKeywordIdFromFriendlyName(int deviceId, const std::string & friendlyName) const
   {
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect.Select().
         From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId).
         And(CKeywordTable::getFriendlyNameColumnName(), CQUERY_OP_EQUAL, friendlyName);

      adapters::CKeywordAdapter adapter;
      m_databaseRequester->queryEntities(&adapter, qSelect);
      return adapter.getResults();
   }

   std::vector<boost::shared_ptr<entities::CKeyword> > CKeyword::getKeywords(int deviceId) const
   {
      adapters::CKeywordAdapter adapter;
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect. Select().
         From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId);
      m_databaseRequester->queryEntities(&adapter, qSelect);
      return adapter.getResults();
   }

   std::vector<boost::shared_ptr<entities::CKeyword> > CKeyword::getKeywordsMatchingCapacity(const std::string & capacity) const
   {
      adapters::CKeywordAdapter adapter;
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect.Select().
         From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL, capacity);
      m_databaseRequester->queryEntities(&adapter, qSelect);
      return adapter.getResults();
   }

   std::vector<boost::shared_ptr<entities::CKeyword> > CKeyword::getAllKeywords() const
	{
      adapters::CKeywordAdapter adapter;
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect.Select().From(CKeywordTable::getTableName());
      m_databaseRequester->queryEntities(&adapter, qSelect);
      return adapter.getResults();
	}

   std::vector<boost::shared_ptr<entities::CKeyword> > CKeyword::getDeviceKeywordsWithCapacity(int deviceId, const std::string & capacityName, const shared::plugin::yPluginApi::EKeywordAccessMode & accessMode) const
   {
      adapters::CKeywordAdapter adapter;
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect. Select().
         From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId).
         And(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL, capacityName).
         And(CKeywordTable::getAccessModeColumnName(), CQUERY_OP_EQUAL, accessMode);

      m_databaseRequester->queryEntities(&adapter, qSelect);
      return adapter.getResults();
   }

   void CKeyword::removeKeyword(const int deviceId, const std::string & keyword)
   {
      boost::shared_ptr<entities::CKeyword> keywordToDelete = getKeyword(deviceId, keyword);
      if(keywordToDelete)
         removeKeyword(keywordToDelete->Id());
      else
         throw shared::exception::CEmptyResult("Can not find keyword");
   }

   void CKeyword::removeKeyword(const int keywordId)
   {
      //delete acquisition data
      m_acquisitionRequester->removeKeywordData(keywordId);

      //delete keyword
      CQuery qDelete = m_databaseRequester->newQuery();
      qDelete. DeleteFrom(CKeywordTable::getTableName()).
         Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);
      if(m_databaseRequester->queryStatement(qDelete) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CKeyword::updateKeywordFriendlyName(int keywordId, const std::string & newFriendlyName)
   {
      //get a good name
      if(newFriendlyName != shared::CStringExtension::EmptyString)
      {
         boost::shared_ptr<entities::CKeyword> keywordToUpdate = getKeyword(keywordId);
         if(keywordToUpdate)
         {
            //insert in db
            CQuery qUpdate = m_databaseRequester->newQuery();
            qUpdate. Update(CKeywordTable::getTableName()).
               Set(CKeywordTable::getFriendlyNameColumnName(), newFriendlyName).
               Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            if(m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw shared::exception::CEmptyResult("Fail to update keyword friendlyName");
         }
         else
         {
            throw shared::exception::CEmptyResult("Can not find keyword"); 
         }
      }
   }

   void CKeyword::updateKeywordFriendlyName(int deviceId, const std::string & keyword, const std::string & newFriendlyName)
   {
      boost::shared_ptr<entities::CKeyword> keywordToUpdate = getKeyword(deviceId, keyword);
      if(keywordToUpdate)
         updateKeywordFriendlyName(keywordToUpdate->Id(), keyword, newFriendlyName);
      else
         throw shared::exception::CEmptyResult("Can not find keyword");
   }
   // [END] IKeywordRequester implementation

} //namespace requesters
} //namespace common
} //namespace database 
