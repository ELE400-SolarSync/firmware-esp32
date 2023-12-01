#ifndef API_HPP
#define API_HPP

#include <HTTPClient.h>

class api_lib {
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