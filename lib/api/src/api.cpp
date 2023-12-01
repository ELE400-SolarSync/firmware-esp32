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
response api_lib::getResponse(String url) {
    HTTPClient http;

    response res;

    String server_call = host + url + "/";

    http.begin((server_call).c_str());

    res.code = http.GET();
    if (res.code > 0) {
        if (res.code == HTTP_CODE_OK) {
            res.data = http.getString();
            return res;
        }
    }
    res.data = "";
    return res;
}

/**
 * @brief Send data to the host + url and return the code
 * 
 * @param url string to complete the host
 * @return int api code
 */
int api_lib::getCode(String url) {
    HTTPClient http;

    String server_call = host + url;
    http.begin((server_call).c_str());

    int httpCode = http.GET();
    return httpCode;
}

/**
 * @brief send multiple data according to thinspeak atemplate /!\ look at AZURE template
 * 
 * @param data data to send
 * @param data_len length of the data
 * @return int code return by the api call
 */
response api_lib::sendAll(float data[], size_t data_len){
    String url;

    for(int i = 0; i < data_len; i++){
        url = url + "&field" + i + "=" + String(data[i]);
    }

    return getResponse(url);
}