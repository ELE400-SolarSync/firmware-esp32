#include "api.hpp"

/**
 * @brief Set the Host object (part of the resquestt with the data)
 * 
 * @param host hostname
 */
void api_lib::setHost(String host) {
    this->host = host;
}

/**
 * @brief Send data to the host + url
 * 
 * @param url string to complete the host
 * @return String string return of the api call
 */
api_lib::response api_lib::getResponse() {
    api_lib::response res;

    String jsonBuffer;
    serializeJson(api_lib::jsonDoc, jsonBuffer);

    HTTPClient httpClient;

    httpClient.begin(api_lib::url);
    httpClient.addHeader("Content-Type", "application/json");
    httpClient.addHeader("Authorization", sas_token);

    res.code = httpClient.POST(jsonBuffer);
    if (res.code > 0) {
        if (res.code == HTTP_CODE_OK) {
            res.data = httpClient.getString();
            return res;
        }
    }
    res.data = httpClient.errorToString(res.code);
    return res;
}

bool api_lib::createJson(float temp, float puissance, float niveauBatterie, bool niveauBatterieAlert) {
    try {
        api_lib::jsonDoc["Temperature"] = temp;
        api_lib::jsonDoc["Puissance"] = puissance;
        api_lib::jsonDoc["Niveau batterie"] = niveauBatterie;
        api_lib::jsonDoc["niveauBatterieAlert"] = niveauBatterieAlert;

        return true;
    }
    catch (const std::exception& e) {
        return false;
    }
}

bool api_lib::clearJson() {
    try {
        api_lib::jsonDoc.clear();
        return true;
    }
    catch (const std::exception& e) {
        return false;
    }
}