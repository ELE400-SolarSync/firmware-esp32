#ifndef API_HPP
#define API_HPP

#include <HTTPClient.h>

class api_lib {
// private:
//     // Azure IoT Hub connection parameters
//     const char* host = "solarsync.azure-devices.net";
//     const char* device_id = "esp32hub";
//     const char* sas_token = "SharedAccessSignature sr=SolarSync.azure-devices.net%2Fdevices%2Fesp32hub&sig=qTW1FcZWh8M1kzhDTHFCAmqY3SPP%2FD0reP6eHn0dyhE%3D&se=1701753692";

//     // Azure IoT Hub endpoint for posting messages
//     String url = String("https://") + host + "/devices/" + device_id + "/messages/events?api-version=2018-06-30";
//     StaticJsonDocument<256> jsonDoc;

// public:
// /**
//  * @brief Set the Host object (part of the resquestt with the data)
//  * 
//  * @param host 
//  */
//     void setHost(String host) {
//         this->host = host;
//     };

// /**
//  * @brief Send data to the host + url
//  * 
//  * @param url string to complete the host
//  * @return String string return of the api call
//  */
//     String getResponse() {
//         String jsonBuffer;
//         serializeJson(jsonDoc, jsonBuffer);

//         HTTPClient httpClient;

//         String server_call = host + url;
//         httpClient.begin(url)
//         httpClient.addHeader("Content-Type", "application/json");
//         httpClient.addHeader("Authorization", sas_token);

//         int httpCode = httpClient.POST(jsonBuffer);
//         if (httpCode > 0) {
//             if (httpCode == HTTP_CODE_OK) {
//                 return http.getString();
//             }
//         }
//         return httpClient.errorToString(httpResponseCode).c_str();
//     }

//     bool createJson(float temp, float puissance, float niveauBatterie, bool niveauBatterieAlert) {
//         try:
//             jsonDoc["Temperature"] = temp;
//             jsonDoc["Puissance"] = puissance;
//             jsonDoc["Niveau batterie"] = niveauBatterie;
//             jsonDoc["niveauBatterieAlert"] = niveauBatterieAlert;

//             return true;
//         catch (const std::exception& e) {
//             return false;
//         }
//     }

//     bool clearJson() {
//         try:
//             jsonDoc.clear();
//             return true;
//         catch (const std::exception& e) {
//             return false;
//         }
//     }

// /**
//  * @brief Send data to the host + url and return the code
//  * 
//  * @param url string to complete the host
//  * @return int api code
//  */
//     int getCode(String url) {
//         HTTPClient http;

//         String server_call = host + url;
//         http.begin((server_call).c_str());

//         int httpCode = http.GET();
//         return httpCode;
//     }

//     /**
//      * @brief send multiple data according to thinspeak atemplate /!\ look at AZURE template
//      * 
//      * @param data data to send
//      * @param data_len length of the data
//      * @return String tring return of the api call
//      */
//     String sendAmp(float data[], size_t data_len){
//         String url;

//         for(int i = 0; i < data_len; i++){
//             url = url + "&field" + i + "=" + String(data[i]);
//         }
      
    private:
        String host;

    public:
        struct response {
            int code;
            String data;
        };

        void setHost(String host);
        response getResponse(String url);
        int getCode(String url);
        response sendAll(float data[], size_t data_len);
        
};

#endif