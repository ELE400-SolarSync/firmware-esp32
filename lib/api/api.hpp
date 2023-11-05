#include <string>
#include <HTTPClient.h>

class api_lib {
public:
    void setHost(String host);

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

private:
    String host;
};
