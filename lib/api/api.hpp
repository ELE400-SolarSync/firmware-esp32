#include <string>
#include <HTTPClient.h>

class api_lib {
private:
    String host;

public:
    void setHost(String host) {
        this->host = host;
    };

    String get(String url) {
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

    String sendAmp(int amp){
        String url = "&field1=" + String(amp);
        return get(url);
    }
};
