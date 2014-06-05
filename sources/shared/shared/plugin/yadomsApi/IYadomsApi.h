#pragma once
#include "Capacity.h"
#include <shared/event/EventHandler.hpp>
#include <shared/plugin/information/IInformation.h>
#include <shared/StringExtension.h>

/*
TODO nettoyer tous ces commentaires en fran�ais
Besoins des widgets

	1. Lister les devices qui fournissent une donn�e (temp�rature, hygro,...)
	2. Lister les devices qui supportent des commandes (potentiellement jamais �mises)
	3. Envoyer des commandes � un device
	4. Recevoir des donn�es d'un device
	
	

Fonctions des plugins

	1. Communiquent avec les hardwares (lien s�rie, r�seau, ...)
	2. Cr�er/rechercher un device
	3. Historiser des donn�es
	4. Piloter les hardwares
	5. Connaissent les fonctions des hardwares (savent d�crire leurs capacit�s : envoi de commande, sonde temperature)
	
	

	
	
Roadmap

	1. d�finir IYadomsApi
	2. d�finir une capacit� (mot cl�, read/write, ...)
	3. d�finir les interfaces de communications (conteneur de donn�es, de commandes,....)
	4. diagrammes de s�quences
	5. d�finir une liste de capacit�s avec leur d�tails implicites (unit�s par exemple, ....)
   
   
D�tails/remarques :
   
   1. Il existera une instance de l'impl�mentation de IYadomsApi par instance de plugin. Ca permet de ne pas diffuser l'instanceId au code du plugin (entre autre) et
   d'une mani�re g�n�rale que cet objet contienne des donn�es/m�thodes concernant l'instance du plugin (comme l'eventHandler) en plus des donn�es/m�thodes globales de Yadoms.
   2. L'instance de plugin re�oit tous les �v�nements (kEventUpdateConfiguration, kEventDeviceCommand) sans besoin de s'abonner explicitement. Les �v�nements ne sont de toutes
   fa�ons lev�s que si actif (kEventUpdateConfiguration lev� que si le plugin contient une configuration, kDeviceCommand que si le plugin a d�clar� un device canWrite).
	
   
Questions:

   1. gestion des exceptions ou retour par bool sur les m�thodes de l'api ? ou  alors m�me en retournant un int � la fa�on linux
   
   
Notes:
SG> Mon avis sur les questions :
   1. Je suis favorable aux exceptions, plus objet, � condition que �a ne g�n�re pas d'incompatibilit� (plugin compil� avec un autre compilo que Yadoms). A tester.
*/

// TODO faire le m�nage dans ce fichier (commentaires...)

// TODO est-il judicieux d'ajouter ici : namespace yApi = shared::plugin::yadomsApi;
namespace shared { namespace plugin { namespace yadomsApi
{
   class IYadomsApi
   {
   public:
      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~IYadomsApi() {}

      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //--
      //-- Events
      //--
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      
      //-----------------------------------------------------
      ///\brief Events Id from Yadoms received by the plugin
      //-----------------------------------------------------
      enum
      {
         kEventUpdateConfiguration = shared::event::kUserFirstId, // Yadoms notify the plugin that its configuration was changed
         kEventDeviceCommand,                                     // Yadoms send a command to a device managed by this plugin

         kPluginFirstEventId                                      // The next usable event ID for the plugin code
      };

      //-----------------------------------------------------
      ///\brief Get the event handler associated to the plugin. The event handler is used
      ///       to received, wait for, or post events from/to Yadoms
      ///\return The plugin event handler
      //-----------------------------------------------------   
      virtual shared::event::CEventHandler & getEventHandler() = 0;

      //-----------------------------------------------------
      ///\brief Container class for device commands sent by server
      //TODO not yet finalized ;-)
      //-----------------------------------------------------
      class CDeviceCommand
      {
      private:
         //TODO � commenter
         CCapacity m_capacity;
         std::string m_value;
         std::string m_targetDevice;

      public:
         CDeviceCommand():m_capacity("", false, false){}  //TODO revoir construction
         virtual ~CDeviceCommand(){}
         std::string toString() const
         {
            // Full informations = identity + author name + url
            std::ostringstream str;
            str << m_targetDevice << " = " << m_value;
            return str.str();
         }
      };   
      
      
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //--
      //-- Devices
      //--
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      
      //-----------------------------------------------------
      ///\brief Check if a device already exists for the server
      ///\param    [in]    device             The device name
      ///\return true if the device exists, false if not
      //-----------------------------------------------------
      virtual bool deviceExists(const std::string& device) const = 0;

      //-----------------------------------------------------
      ///\brief Declare a device
      ///\param    [in]    device             The device name
      ///\param    [in]    model              The device model or description (ex : "Oregon Scientific CN185")
      ///\param    [in]    details            A free string used by plugin
      ///\return true if the device has been successfully created, false if already exist
      ///\throw shared::exception::CEmptyResult if creation failed
      //-----------------------------------------------------   
	  virtual bool declareDevice(const std::string& device, const std::string& model, const std::string & details) = 0;
      
      
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //--
      //-- Keywords
      //--
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------

      //-----------------------------------------------------
      ///\brief Keyword access mode
      //-----------------------------------------------------
      enum EKeywordAccessMode//TODO remonter d'un cran ?
      {
         kReadWrite,
         kReadOnly,
         kWriteOnly
      };

      //-----------------------------------------------------
      ///\brief Check if a keyword already exists for the device
      ///\param    [in]    device             The device name owner of the keyword
      ///\param    [in]    keyword            The keyword name
      ///\return true if the keyword exists, false if not
      //-----------------------------------------------------
      virtual bool keywordExists(const std::string& device, const std::string& keyword) const = 0;

      //-----------------------------------------------------
      ///\brief Declare a keyword
      ///\param    [in]    device             The device name owner of the keyword
      ///\param    [in]    keyword            The keyword name
      ///\param    [in]    capacity           The capacity name (see yApi::CStandardCapacities for standard capacities, or use your own)
      ///\param    [in]    accessMode         The keyword access
      ///\param    [in]    details            The keyword details (JSON string, optional. Can be used to declare specific properties like min/max values)
      ///\return true if the keyword has been successfully created, false if already exist
      ///\throw shared::exception::CEmptyResult if creation failed
      //-----------------------------------------------------   
      virtual bool declareKeyword(const std::string& device, const std::string& keyword, const std::string& capacity, EKeywordAccessMode accessMode, const std::string& details = shared::CStringExtension::EmptyString) = 0;
      

      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //--
      //-- Data recording
      //--
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------

      //-----------------------------------------------------
      ///\brief Historize a new data values
      ///\param    [in]    device             The device name (must be unique)
      ///\param    [in]    keyword            The value name ("temp1", "temp2", "humidity", "batteryLevel", "rssi"...)
      ///\param    [in]    value              The capacity value
      ///\throw   shared::exception::CInvalidParameter if the device is not known
      //-----------------------------------------------------     
      virtual void historizeData(const std::string & device, const std::string & keyword, const std::string & value) = 0;
      
      
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //--
      //-- Plugin information
      //--
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------

      //-----------------------------------------------------
      ///\brief Get the current plugin information (extracted from package.json file)
      ///\return The current plugin information
      //-----------------------------------------------------      
      virtual const shared::plugin::information::IInformation& getInformation() const = 0;


      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //--
      //-- Plugin configuration
      //--
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------

      //-----------------------------------------------------
      ///\brief Get the current plugin configuration
      ///\return The current plugin configuration
      //-----------------------------------------------------      
      virtual const std::string getConfiguration() const = 0;


      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //--
      //-- Plugin event recorder
      //--
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      
      
      //-----------------------------------------------------
      ///\brief The plugin event record level
      //-----------------------------------------------------      
      typedef enum
      {
         kInfo,
         kError,
      }PluginEventSeverity;
      
      //-----------------------------------------------------
      ///\brief Record a plugin major event (recorded in Yadoms database)
      ///\param    [in]    severity           The message severity
      ///\param    [in]    message            The message
      ///\return true if the message has been successfully stored, false if not
      //-----------------------------------------------------      
      virtual bool recordPluginEvent(PluginEventSeverity severity, const std::string & message) = 0;
   };
	
} } } // namespace shared::plugin::yadomsApi	
	
	