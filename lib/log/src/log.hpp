#include "esp_log.h"

class myLogger {
    private:
        String log_path;
        String logFile;
        bool SerialToglle = true;

        uint64_t start_time;

        void getFormattedTimestamp(time_t timestamp, char* buffer, size_t buffer_size) {
            struct tm timeinfo;
            localtime_r(&timestamp, &timeinfo);
            
            strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", &timeinfo);
        }

    public:
        enum level_t {
            DEBUG = 0,
            INFO,
            WARN,
            ERROR
        };
        level_t logLevel;

        myLogger(String log_file, String log_path, level_t level = INFO) {
            this->logFile = log_file;
            this->log_path = log_path;
            this->logLevel = level;

            struct tm startDateTime;
            startDateTime.tm_year = 2023;
            startDateTime.tm_mon = 10;
            startDateTime.tm_mday = 6; 
            startDateTime.tm_hour = 12; 
            startDateTime.tm_min = 0; 
            startDateTime.tm_sec = 0; 

            start_time = mktime(&startDateTime);
        }

        String getLogPath() const {
            return log_path;
        }

        String getLogFile() const {
            return logFile;
        }

        void setLogPath(String log_path) {
            this->log_path = log_path;
        }

        void setLogFile(String log_file) {
            this->logFile = log_file;
        }

        void setLevel(level_t level) {
            this->logLevel = level;
        }

        level_t getLevel() const {
            return logLevel;
        }

        void error(String tag, String message) {
            log(buildlogFormat(tag, "E"), message.c_str());
        }

        void warning(String tag, String message) {
            if(logLevel <= WARN) {
                log(buildlogFormat(tag, "W"), message.c_str());
            }
        }

        void info(String tag, String message) {
            if(logLevel <= INFO){
                log(buildlogFormat(tag, "I"), message.c_str());
            }
        }

        void debug(String tag, String message) {
            if(logLevel <= DEBUG){
                log(buildlogFormat(tag, "D"), message.c_str());
            }
        }

        void enableLoggingInMonitor() {
            SerialToglle = true;
        }

        void disableLoggingInMonitor() {
            SerialToglle = false;
        }

        void log(String logFormat, String message) {
            if(SerialToglle) {
                Serial.println("Terminal : " +logFormat + message);
            }
            // printf("File : %s%s\n", logFormat.c_str(), message.c_str());
        }

        String buildlogFormat(String logTag, String level) {
            uint64_t elapsed_time = esp_timer_get_time() - start_time;

            char time_buffer[64];
            getFormattedTimestamp(start_time + (elapsed_time / 1000000), time_buffer, sizeof(time_buffer));

            return "[" + String(time_buffer) + "] [" + level + "] [" + logTag + "] ";
        }
};