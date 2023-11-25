#include "esp_log.h"
#include <HTTPClient.h>
#include "../../sd/src/sdcustom.hpp"

#ifndef LOG_H
#define LOG_H

class myLogger {
    private:
        int log_path;
        int log_file;
        String log_fullPath;
        bool SerialToglle = true;
        bool SDToglle = true;
        SDCustom& sd;

        enum roll_t {
            FILE,
            FOLDER
        };

    public:
        enum level_t {
            DEBUG = 0,
            INFO,
            WARN,
            ERROR
        };
        level_t logLevel;

        myLogger(SDCustom& sd, int log_file = 0, int log_path = 0, level_t level = INFO) : sd(sd), log_file(log_file), log_path(log_path), logLevel(level) {
            this->log_fullPath = String(log_path) + "/" + String(log_file) + ".txt";
        }

        void init() {
            sd.begin();
            Serial.println(sd.fileExists(log_fullPath));

            if (!sd.isSDInserted()) {
                SDToglle = false;
                error("logger", "SD Card is not inserted");
            }

            if (!sd.fileExists(log_fullPath)) {
                Serial.println("File does not exist");
                sd.mkdir(String(log_path));
                sd.createFile(log_fullPath);
            }
        }

        int getLogPath() const {
            return log_path;
        }

        int getLogFile() const {
            return log_file;
        }

        void setLogPath(int log_path) {
            this->log_path = log_path;
        }

        void setLogFile(int log_file) {
            this->log_file = log_file;
        }

        void setLogFullPath() {
            this->log_fullPath = String(log_path) + "/" + String(log_file) + ".txt";
        }

        void setLevel(level_t level) {
            this->logLevel = level;
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

        void enableLoggingInSD() {
            SDToglle = true;
        }

        void disableLoggingInSD() {
            SDToglle = false;
        }

        void log(String logFormat, String message) {
            if(SerialToglle) {
                Serial.println(logFormat + message);
            }
            if(SDToglle) {
                if(!sd.isSDInserted()) {
                    // SD Card is not inserted
                    SerialToglle = true;
                    SDToglle = false;
                    error("logger", "SD Card is not inserted");
                }
                else {
                    if(true) {
                        // SD card is full
                        // Delete oldest file
                    }
                    else if((sd.openFile(log_fullPath).size() > 1000000) && (log_file == 9)) {
                        // Folder is full
                        logRoll(FOLDER);
                    }
                    else if (sd.openFile(log_fullPath).size() > 1000000) {
                        // File is full
                        logRoll(FILE);
                    }
                    else {
                        // Write to file
                        sd.writeFile(log_fullPath, logFormat + message);
                    }
                }
            }
        }

        String buildlogFormat(String logTag, String level) {
            return "[" + String(esp_timer_get_time() / 1000) + "] [" + level + "] [" + logTag + "] ";
        }

        /**
         * @brief Roll the log
         * 
         * @param roll what to roll File or Folder
         */
        void logRoll(roll_t roll) {
            if(roll == FILE) {
                setLogFile(log_file + 1);
                setLogFullPath();
                sd.createFile(log_fullPath);
            }
            else if(roll == FOLDER) {
                setLogPath(log_path + 1);
                setLogFile(0);
                setLogFullPath();
                sd.mkdir(String(log_path));
            }
        }
};

#endif