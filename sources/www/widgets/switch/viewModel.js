widgetViewModelCtor =

   /**
    * Create a Switch ViewModel
    * @constructor
    */
      function switchViewModel() {
		  
		  var self = this;
		  
          //observable data
          this.state = [];
          this.command = ko.observable(1);
          this.kind = ko.observable("simple");
          this.icon = ko.observable("");
          this.capacity = [];
		  this.accessMode = [];
          this.showDeviceName = ko.observable(true);
          this.readonly = ko.observable(true);
		  
          this.commandClick = function (newState) {

              var self = this;

              if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.devices))) {
                  var cmd = null;

                  $.each(this.widget.configuration.devices, function (index, device) {
				  	  
                      if (!isNullOrUndefined(self.capacity[index])) {
                          switch (self.capacity[index]) {
                              case "dimmable": cmd = newState == 1 ? 100 : 0; break;
                              case "event": cmd = 1; break;
                              default: cmd = newState; break;
                          }

						  // We send the command only for Set and GetSet variables
						  if ( self.accessMode[index] === "GetSet" || self.accessMode[index] === "Set" )
                             KeywordManager.sendCommand(device.content.source.keywordId, cmd.toString());
                      }
                  });
              }
          };

          /**
           * Initialization method
           * @param widget widget class object
           */
          this.initialize = function () {
              //we configure the toolbar
              this.widgetApi.toolbar({
                  activated: true,
                  displayTitle: true,
                  batteryItem: false
              });
          };

          this.configurationChanged = function () {
              var self = this;
              //we update the kind observable property

              if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
                  return;

              if ((!isNullOrUndefined(this.widget.configuration)) && (!isNullOrUndefined(this.widget.configuration.devices))) {
                  $.each(this.widget.configuration.devices, function (index, device) {
					  
                      //we register keyword new acquisition
                      self.widgetApi.registerKeywordAcquisitions(device.content.source.keywordId);
                  });
              }

              if (!isNullOrUndefined(this.widget.configuration.kind)) {
                  this.kind(this.widget.configuration.kind.activeSection);
                  if (this.widget.configuration.kind.content.pushButton.radio) {
                      this.icon("glyphicon " + this.widget.configuration.kind.content.pushButton.content.icon);
                  }
              }

              if (!isNullOrUndefined(this.widget.configuration.showDeviceName)) {
                  this.showDeviceName(parseBool(this.widget.configuration.showDeviceName));
              }

              //we ask for device information
              if ((!isNullOrUndefined(this.widget.configuration.devices))) {
				  
				  var readOnlyMode=true;
				  var arrayOfDeffered = [];

                  $.each(this.widget.configuration.devices, function (index, device) {

                      if (!isNullOrUndefined(device.content.source.deviceId)) {
						  
                          // Get the capacity of the keyword
                         var deffered = KeywordManager.get(device.content.source.keywordId)
                         .done(function (keyword) {
                           self.capacity[index]   = keyword.capacityName;
						   self.accessMode[index] = keyword.accessMode;
						   
						   if ( keyword.accessMode ==="GetSet" )
							  readOnlyMode = false;
						   
                         });
						 
						 arrayOfDeffered.push(deffered);

                         //Initialization
                         if (isNullOrUndefined(self.state[index]))
                              self.state[index] = 0;
                      }
                  });
				  
				  // This variable is used only for the display
                  $.whenAll(arrayOfDeffered).done(function () {
				     self.readonly ( readOnlyMode );
				  });				  
              }
          };

          /**
          * New acquisition handler
          * @param keywordId keywordId on which new acquisition was received
          * @param data Acquisition data
          */
          this.onNewAcquisition = function (keywordId, data) {
              var self = this;

              if ((this.widget.configuration != undefined) && (this.widget.configuration.devices != undefined)) {

                  $.each(this.widget.configuration.devices, function (index, device) {
                      if (keywordId === device.content.source.keywordId) {
                          //it is the right device
                          if (self.capacity[index] === "event")
                              self.state[index] = 0;
                          else {
                              // Adapt for dimmable or switch capacities
                              if (parseInt(data.value) !== 0)
                                  self.state[index] = 1;
                              else
                                  self.state[index] = 0;
                          }
                      }
                  });

                  self.evaluateCommand();
              }
          };

          this.evaluateCommand = function () {
              var self = this;
              var average = 0;

              $.each(this.state, function (index, state) {
                  average = average + state;
              });

              average = average / this.state.length;

              self.command(average);
          }
      };