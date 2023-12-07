#ifndef API_HPP
#define API_HPP

#include <HTTPClient.h>
#include <ArduinoJson.h>

/**
 * @brief Class to call the api
 * 
 */
class api_lib {     
    private:
        String host = "solarsync.azure-devices.net"; //< Host of the api call
        String device_id = "esp32hub"; //< Device id of the api call
        String sas_token = "SharedAccessSignature sr=SolarSync.azure-devices.net%2Fdevices%2Fesp32hub&sig=nmp3K5oFa81pxXqNOj%2FRaYezdlrO2p6VsN7qn1Al2Ug%3D&se=1701824380"; //< SAS token of the api call

        String url = String("https://") + host + "/devices/" + device_id + "/messages/events?api-version=2018-06-30"; //< Url of the api call
        StaticJsonDocument<256> jsonDoc; //< Json payload of the api call

    public:

        /**
         * @brief Contain all the data of the api call
         */
        struct response {
            int code; ///< HTTP code of the api call
            String data; ///< Data of the api call
        };

        response getResponse();
        bool clearJson();
        bool createJson(float temp, float puissance_solaire, float puissance_battery, float puissance_5v, float puissance_12v, float niveauBatterie, bool niveauBatterieAlert);
        int getCodeTest(String url);
        
};

#endif