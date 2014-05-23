/** @module Page class */

/**
 * Creates an instance of Page
 * @constructor
 */
function PluginInstance(id, name, pluginName, configuration, enabled) {
   assert(id !== undefined, "id of a pluginInstance must be defined");
   assert(name !== undefined, "name of a pluginInstance must be defined");
   assert(pluginName !== undefined, "pluginName of a pluginInstance must be defined");
   assert(configuration !== undefined, "configuration of a pluginInstance must be defined");
   assert(enabled !== undefined, "enabled of a pluginInstance must be defined");

   this.id = id;
   this.name = name;
   this.pluginName = pluginName;
   this.configuration = configuration;
   this.enabled = enabled;
   this.lastRunningStatus = null;
}

PluginInstance.prototype.getStatus = function(callback) {
   //we ask for the status of current pluginInstance
   var self = this;
   $.getJSON("/rest/plugin/" + this.id + "/status/")
      .done(function(data) {
         //we parse the json answer
         //the default answer is false
         var result = false;
         if (data.result != "true")
         {
            notifyError($.t("objects.pluginInstance.errorGettingStatus", {pluginName : self.name}), JSON.stringify(data));
            return;
         }
         if (parseBool(data.data.running))
         {
            result = true;
         }
         if (!isNullOrUndefined(callback))
            callback(result);

         //we update the lastRunningStatus
         self.lastRunningStatus = result;

         return result;
      })
      .fail(function() { notifyError($.t("objects.pluginInstance.errorGettingStatus", {pluginName : self.name})); });
};

PluginInstance.prototype.start = function(callback) {
   var self = this;
   $.ajax({
      type: "PUT",
      url: "/rest/plugin/" + this.id + "/start",
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.pluginInstance.errorStarting", {pluginName : self.name}), JSON.stringify(data));
            return;
         }
         if (!isNullOrUndefined(callback))
            callback();
      })
      .fail(function() {notifyError($.t("objects.pluginInstance.errorStarting", {pluginName : self.name}));});
};

PluginInstance.prototype.stop = function(callback) {
   var self = this;
   $.ajax({
      type: "PUT",
      url: "/rest/plugin/" + this.id + "/stop",
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.pluginInstance.errorStopping", {pluginName : self.name}), JSON.stringify(data));
            return;
         }

         if (!isNullOrUndefined(callback))
            callback();
      })
      .fail(function() {notifyError($.t("objects.pluginInstance.errorStopping", {pluginName : self.name}));});
};

PluginInstance.prototype.deleteFromServer = function(callback) {
   var self = this;
   $.ajax({
      type: "DELETE",
      url: "/rest/plugin/" + this.id,
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.pluginInstance.errorDeleting", {pluginName : self.name}), JSON.stringify(data));
            return;
         }

         if (!isNullOrUndefined(callback))
            callback();
      })
      .fail(function() {notifyError($.t("objects.pluginInstance.errorDeleting", {pluginName : self.name}));});
};

PluginInstance.prototype.createToServer = function(callback) {
   var self = this;

   $.ajax({
      type: "POST",
      url: "/rest/plugin",
      data: JSON.stringify({ name: self.name, pluginName: self.pluginName, configuration: self.configuration, enabled: self.enabled}),
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
   .done(function(data) {
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError($.t("objects.pluginInstance.errorCreating", {pluginName : self.name}), JSON.stringify(data));
         //launch callback with false as ko result
         if (!isNullOrUndefined(callback))
            callback(false);
         return;
      }

      //we update our information from the server
      self.id = data.data.id;
      self.name = data.data.name;
      self.pluginName = data.data.pluginName;
      self.configuration = data.data.configuration;
      self.enabled = parseBool(data.data.enabled);

      if (!isNullOrUndefined(callback))
         callback(true);
   })
   .fail(function() {
      notifyError($.t("objects.pluginInstance.errorCreating", {pluginName : self.name}));
      //launch callback with false as ko result
      if (!isNullOrUndefined(callback))
         callback(false);
   });
};

PluginInstance.prototype.updateToServer = function(callback) {
   var self = this;

   $.ajax({
      type: "PUT",
      url: "/rest/plugin/" + self.id,
      data: JSON.stringify(self),
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
   .done(function(data) {
      //we parse the json answer
      if (data.result != "true")
      {
         notifyError($.t("objects.pluginInstance.errorUpdating", {pluginName : self.name}), JSON.stringify(data));
         //launch callback with false as ko result
         if (!isNullOrUndefined(callback))
            callback(false);
         return;
      }
      //it's okay
      //we update our information from the server
      self.id = data.data.id;
      self.name = data.data.name;
      self.pluginName = data.data.pluginName;
      self.configuration = data.data.configuration;
      self.enabled = parseBool(data.data.enabled);

      //we call the callback with true as a ok result
      if (!isNullOrUndefined(callback))
         callback(true);
   })
   .fail(function() {
      notifyError($.t("objects.pluginInstance.errorUpdating", {pluginName : self.name}));
      //launch callback with false as ko result
      if (!isNullOrUndefined(callback))
         callback(false);
   });
};

PluginInstance.prototype.downloadPackage = function(callback) {
   var self = this;
   $.getJSON( "plugin/" + self.pluginName + "/package.json")
      .done(function (data) {
         self.package = data;

         //we manage i18n
         i18n.options.resGetPath = 'plugin/__ns__/locales/__lng__.json';
         i18n.loadNamespace(self.pluginName);

         if (!isNullOrUndefined(callback))
            callback();
      })
      .fail(function() {notifyError($.t("objects.pluginInstance.errorGettingPackage", {pluginName : self.name}));});
};

/**
 * Override JSON.stringify method in order to send only database columns
 * @returns {{id: *, name: *, pluginName: *, configuration: *, enabled: *}}
 */
PluginInstance.prototype.toJSON = function () {
   return {
      id : this.id,
      name: this.name,
      pluginName: this.pluginName,
      configuration: this.configuration,
      enabled: this.enabled
   };
};
