

#include "Arduino.h"

#include "credentials.h"
#include "controller.h"
#include "messages.h"
#include "mqtt.h"
#include "power.h"
#include "provisioner.h"
#include "settings.h"





// 1A) REGULAR SUBSCRIPTIONS AND MESSAGE HANDLING ----------------------------------

static const char * subscription_list[] = {
	"~/~/whatever",
	nullptr				// keep this
};


void messageHandler(char * topic, char * payload) {
	// DO SOMETHING
}




// 2A) STRUCTURED COMMANDS ---------------------------------------------------------

static const char * command_namespaces[] = {
	"namespace1",
	"namespace2",
	nullptr			// keep this
};


bool commandHandler(const char * sub_topic, const char * payload, char * error) {
	if (strcmp(sub_topic, "namespace1") == 0) {		
		// do something
		return true;
	}
	if (strcmp(sub_topic, "namespace2") == 0) {		
		// do something else
		return true;
	}
	return false;
}



//	3A) SETTINGS -------------------------------------------------------------------

char _threshold[SETTINGS_BUF_SIZE]  = "2.5";
char _sleep_secs[SETTINGS_BUF_SIZE] = "30";
char _mode[SETTINGS_BUF_SIZE]	   = "auto";

float threshold;
int   sleep_secs;

SettingRow settings_table[] = {
	{ "threshold",  _threshold  },
	{ "sleep_secs", _sleep_secs },
	{ "mode",       _mode       },
	{ nullptr,      nullptr     },
};

void settingsChanged() {
	threshold  = settings.convert_to_float(_threshold, 2.5);
	sleep_secs = settings.convert_to_int(_sleep_secs, 30);
	// _mode needs no conversion — use the buffer directly wherever you need it
}



void setup() {

	Serial.begin(115200);
	Serial.println("\n\n\n+++++++++++  DEVICE BOOT  ++++++++++++++++++++++++++++++++++\n");

	controller.setup(WIFI_SSID, WIFI_PASS, MQTT_USER, MQTT_PASS);
	// controller.setup(MQTT_USER, MQTT_PASS);		// auto-launches the provisioner

	
	// 1B) REGULAR SUBSCRIPTIONS AND MESSAGE HANDLING -----------------------------------
	
	mqtt.set_subscriptions(subscription_list);
	mqtt.set_callback(messageHandler);

	// 2B) STRUCTURED COMMANDS ----------------------------------------------------------

	mqtt.set_command_namespaces(command_namespaces);
	messages.set_command_handler(commandHandler);
	//	3A) SETTINGS --------------------------------------------------------------------
	settings.begin(settings_table, settingsChanged);

	// provisioner.set_pin(X);		// un-comment if you want to assign a pin for provisioning (delete this line if not)

}




void loop() {

	controller.loop();

	if (mqtt.is_connected) {

		// do something
		
	}

	// power.sleep();

}

