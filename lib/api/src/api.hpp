#include <string>
#include <HTTPClient.h>

class api_lib {
private:
    String host;

public:
/**
 * @brief Set the Host object (part of the resquestt with the data)
 * 
 * @param host 
 */
    void setHost(String host) {
        this->host = host;
    };

/**
 * @brief Send data to the host + url
 * 
 * @param url string to complete the host
 * @return String string return of the api call
 */
    String getResponse(String url) {
        HTTPClient http;

        String server_call = host + url;
        http.begin((server_call).c_str());

        int httpCode = http.GET();
        if (httpCode > 0) {
            if (httpCode == HTTP_CODE_OK) {
                return http.getString();
            }
        }
        return "";
    }

/**
 * @brief Send data to the host + url and return the code
 * 
 * @param url string to complete the host
 * @return int api code
 */
    int getCode(String url) {
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
     * @return String tring return of the api call
     */
    String sendAmp(float data[], size_t data_len){
        String url;

        for(int i = 0; i < data_len; i++){
            url = url + "&field" + i + "=" + String(data[i]);
        }
        
        return getResponse(url);
    }
};
