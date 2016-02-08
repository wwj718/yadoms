from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys
import modals


""" Operations on plugins dashboard page """

def getCreatePluginButton(browser):
   return browser.find_element_by_id("btn-add-new-plugin")
   
def waitNewPluginModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "new-plugin-modal")))
   return NewPluginModal(browser.find_element_by_id("new-plugin-modal"))
   
      
def waitPluginsTable(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "plugin-instance-list")))
   return browser.find_element_by_id("plugin-instance-list")

def waitPluginsTableHasNPlugins(browser, pluginsNumberExpected):
   pluginsTable = waitPluginsTable(browser)
   WebDriverWait(browser, 10).until(lambda driver: getPluginNumberInTable(browser, pluginsTable) == pluginsNumberExpected)
   return pluginsTable

def getPluginNumberInTable(browser, pluginsTable):
   return len(pluginsTable.find_elements_by_tag_name("tr")) - 1
   
def getPluginDatas(pluginsTable, pluginNumber):
   plugin = pluginsTable.find_elements_by_tag_name("tr")[pluginNumber + 1]
   return plugin.find_elements_by_tag_name("td")

def getPluginName(pluginsTable, pluginNumber):
   return getPluginDatas(pluginsTable, pluginNumber)[0].text

def getPluginType(pluginsTable, pluginNumber):
   return getPluginDatas(pluginsTable, pluginNumber)[1].text
   
def getPluginAutoStart(pluginsTable, pluginNumber):
   return getPluginDatas(pluginsTable, pluginNumber)[2].find_element_by_tag_name("input").is_selected()
   
def getPluginButtons(pluginsTable, pluginNumber):
   pluginsActionsButtonsCell = getPluginDatas(pluginsTable, pluginNumber)[3]
   buttonsGroup = pluginsActionsButtonsCell.find_element_by_class_name("btn-group")
   return buttonsGroup.find_elements_by_xpath("./child::*")
   
def getPluginButton(pluginsTable, pluginNumber, index):
   buttons = getPluginButtons(pluginsTable, pluginNumber)
   return buttons[index]
   
def getPluginStartStopButton(pluginsTable, pluginNumber):
   """ Start/Stop button is the first button of the buttons group """
   button = getPluginButton(pluginsTable, pluginNumber, 0)
   assert "btn-startStop" in button.get_attribute("class")
   return button
   
def getPluginEditButton(pluginsTable, pluginNumber):
   """ Configure button is the second button of the buttons group """
   button = getPluginButton(pluginsTable, pluginNumber, 1)
   assert "btn-configure" in button.get_attribute("class")
   return button
   
def getPluginRemoveButton(pluginsTable, pluginNumber):
   """ Remove button is the third button of the buttons group """
   button = getPluginButton(pluginsTable, pluginNumber, 2)
   assert "btn-delete" in button.get_attribute("class")
   return button
   

def waitConfigurePluginModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "configure-plugin-modal")))
   return ConfigurePluginModal(browser.find_element_by_id("configure-plugin-modal"))

def waitRemovePluginConfirmationModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "confirmation-modal")))
   return RemovePluginConfirmationModal(browser.find_element_by_id("confirmation-modal"))
   
   
   
class PluginState:
   Error, Stopped, Running, Custom, Unknown = range(5)
   
def getPluginState(pluginsTable, pluginNumber):
   pluginStateCell = getPluginDatas(pluginsTable, pluginNumber)[4]
   labelStatusClasses = pluginStateCell.find_element_by_class_name("label-status").get_attribute("class")
   labelStatusText = pluginStateCell.find_element_by_class_name("label-status").text
   if "label-danger" in labelStatusClasses:
      return PluginState.Error
   if "label-warning" in labelStatusClasses:
      if labelStatusText == i18n.get()["modals"]["dashboard"]["sub-windows"]["plugins"]["stopped"]:
         return PluginState.Stopped
      else:
         return PluginState.Custom
   if "label-success" in labelStatusClasses:
      return PluginState.Running
   assert False   

   
   
class NewPluginModal():
   """ Operations on new plugin modal (plugin selection) """
   
   def __init__(self, newPluginModalWebElement):
       self.__newPluginModalWebElement = newPluginModalWebElement

   def selectPlugin(self, expectedPluginName):
      select = WebDriverWait(self.__newPluginModalWebElement, 10).until(Condition.visibility_of_element_located((By.ID, "pluginTypeList")))
      for option in select.find_elements_by_tag_name('option'):
         if option.text == expectedPluginName:
            return option
      print "selectPlugin : Nothing to select, ", expectedPluginName, " not found"
      assert False
         
   def getConfirmButton(self, browser):
      return browser.find_element_by_id("btn-confirm-add-plugin")
            
            
            

class RemovePluginConfirmationModal(modals.RemoveObjectConfirmationModal):
   """ Operations on delete plugin confirmation modal """
   pass
   



class ConfigurePluginModal():
   """ Operations on plugin configuration modal """
   
   def __init__(self, configurePluginModalWebElement):
      self.__configurePluginModalWebElement = configurePluginModalWebElement

   def __getConfigurationItemByName(self, dataI18nString):
      """ Find a configuration item by its "data-i18n" field """
      controlGroups = self.__configurePluginModalWebElement.find_elements_by_class_name("control-group")
      for controlGroup in controlGroups:
         label = controlGroup.find_element_by_class_name("configuration-label")
         name = label.find_element_by_class_name("configuration-label-name")
         if (name.get_attribute("data-i18n") == dataI18nString):
            return controlGroup.find_element_by_class_name("configuration-control").find_elements_by_xpath("./child::*")[0]
               
      # Not found
      assert False      

   def getPluginName(self):
      return self.__getConfigurationItemByName("modals.configure-plugin.name-configuration.name")
      
   def replacePluginName(self, newName):
      nameField = self.getPluginName()
      nameField.send_keys(Keys.CONTROL + "a")
      nameField.send_keys(Keys.DELETE)
      nameField.send_keys(newName)
         
   def getConfirmButton(self, browser):
      return browser.find_element_by_id("btn-confirm-configure-plugin")
      