#ifndef API_HPP
#define API_HPP

#include <HTTPClient.h>
#include <ArduinoJson.h>

class api_lib {     
    private:
        // Azure IoT Hub connection parameters
        String host = "solarsync.azure-devices.net";
        String device_id = "esp32hub";
        String sas_token = "SharedAccessSignature sr=SolarSync.azure-devices.net%2Fdevices%2Fesp32hub&sig=qTW1FcZWh8M1kzhDTHFCAmqY3SPP%2FD0reP6eHn0dyhE%3D&se=1701753692";

        // Azure IoT Hub endpoint for posting messages
        String url = String("https://") + host + "/devices/" + device_id + "/messages/events?api-version=2018-06-30";
        StaticJsonDocument<256> jsonDoc;

    public:
        struct response {
            int code;
            String data;
        };

        response getResponse();
        bool clearJson();
        bool createJson(float temp, float puissance, float niveauBatterie, bool niveauBatterieAlert);
        
};

#endif