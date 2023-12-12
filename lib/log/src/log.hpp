#include "esp_log.h"
#include <HTTPClient.h>
#include "../../sd/src/sdcustom.hpp"

#ifndef LOG_H
#define LOG_H

class myLogger {
    private:
        int log_path; //< Path of the log file
        int log_file; //< Name of the log file
        int oldest_log_file = 0; //< Name of the oldest log file
        String log_fullPath; //< Full path of the log file
        bool SerialToglle = true; //< Boolean to know if we need to log in the monitor
        bool SDToglle = true; //< Boolean to know if we need to log in the SD Card
        SDCustom& sd; //< SD Card object

        enum roll_t {
            SWITCH,
            DELETE
        };

    public:
        /**
         * @brief Enumeration of the debug levels
         * 
         */
        enum level_t {
            DEBUG = 0, //< Debug level
            INFO, //< Info level
            WARN, //< Warning level
            ERROR //< Error level
        };
        level_t logLevel;

        /**
         * @brief Construct a new my Logger object
         * 
         * @param sd 
         * @param log_file, default value is 0
         * @param log_path, default value is 0
         * @param level, default value is INFO
         */
        myLogger(SDCustom& sd, int log_file = 0, int log_path = 0, level_t level = INFO) : sd(sd), log_file(log_file), log_path(log_path), logLevel(level) {
            this->log_fullPath = String(log_file) + ".txt";
        }

        /**
         * @brief Initialise the logger
         * 
         */
        void init() {
            sd.begin();

            if (!sd.isSDInserted()) {
                SDToglle = false;
                error("logger", "SD Card is not inserted");
            }

            if (!sd.fileExists(log_fullPath)) {
                sd.createFile(log_fullPath);
            }
        }

        /**
         * @brief Get the Level parameter
         * 
         * @return level_t 
         */
        level_t getLevel() const {
            return logLevel;
        }

        /**
         * @brief Get the Log Path parameter
         * 
         * @return int 
         */
        int getLogPath() const {
            return log_path;
        }

        /**
         * @brief Get the Log File parameter
         * 
         * @return int 
         */
        int getLogFile() const {
            return log_file;
        }

        /**
         * @brief Set the Old Log File parameter
         * 
         * @param old_log 
         */
        void setOldLogFile(int old_log) {
            this->oldest_log_file = old_log;
        }

        /**
         * @brief Get the Old Log File parameter
         * 
         * @return int 
         */
        int getOldLogFile() const {
            return oldest_log_file;
        }

        /**
         * @brief Set the Log Path parameter
         * 
         * @param log_path 
         */
        void setLogPath(int log_path) {
            this->log_path = log_path;
        }

        /**
         * @brief Set the Log File parameter
         * 
         * @param log_file 
         */
        void setLogFile(int log_file) {
            this->log_file = log_file;
        }

        /**
         * @brief Set the Log Full Path parameter
         * 
         */
        void setLogFullPath() {
            this->log_fullPath = String(log_path) + "/" + String(log_file) + ".txt";
        }

        /**
         * @brief Set the Level parameter
         * 
         * @param level 
         */
        void setLevel(level_t level) {
            this->logLevel = level;
        }

        /**
         * @brief Function to write error log
         * 
         * @param tag First part to identify where comes the log
         * @param message 
         */
        void error(String tag, String message) {
            log(buildlogFormat(tag, "E"), message.c_str());
        }

        /**
         * @brief Function to write warning log
         * 
         * @param tag First part to identify where comes the log
         * @param message 
         */
        void warning(String tag, String message) {
            if(logLevel <= WARN) {
                log(buildlogFormat(tag, "W"), message.c_str());
            }
        }

        /**
         * @brief Function to write info log
         * 
         * @param tag First part to identify where comes the log
         * @param message 
         */
        void info(String tag, String message) {
            if(logLevel <= INFO){
                log(buildlogFormat(tag, "I"), message.c_str());
            }
        }

        /**
         * @brief Function to write debug log
         * 
         * @param tag First part to identify where comes the log
         * @param message 
         */
        void debug(String tag, String message) {
            if(logLevel <= DEBUG){
                log(buildlogFormat(tag, "D"), message.c_str());
            }
        }

        /**
         * @brief Enable logging in the monitor
         * 
         */
        void enableLoggingInMonitor() {
            SerialToglle = true;
        }

        /**
         * @brief Disable logging in the monitor
         * 
         */
        void disableLoggingInMonitor() {
            SerialToglle = false;
        }

        /**
         * @brief Enable logging in the SD Card
         * 
         */
        void enableLoggingInSD() {
            SDToglle = true;
        }

        /**
         * @brief Disable logging in the SD Card
         * 
         */
        void disableLoggingInSD() {
            SDToglle = false;
        }

        /**
         * @brief Main function to write the log
         * 
         * @param logFormat Format of the log
         * @param message Message to log
         */
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
                    if(log_file > 30 * 1000) {
                        // if there is more than 30 000 (30Go) files in the folder (each files is 1Mo)
                        Serial.println("[LOGGER] Delete oldest file");
                        logRoll(DELETE);
                        sd.writeFile(String(log_file)+".txt", logFormat + message);                    
                    }
                    else if (sd.openFile(String(log_file)+".txt").size() > 1000000) {
                        // if file is bigger than 1Mo
                        Serial.println("[LOGGER] Switch file");
                        logRoll(SWITCH);
                        sd.writeFile(String(log_file)+".txt", logFormat + message);
                    }
                    else {
                        // Write to file
                        // debug("SD TEST", "Writing in SD");
                        Serial.println("[LOGGER] Writing in SD");
                        sd.writeFile(String(log_file)+".txt", logFormat + message);
                    }
                }
            }
        }

        /**
         * @brief Build the format of the log
         * 
         * @param logTag First part to identify where comes the log
         * @param level Level of the log
         * @return String 
         */
        String buildlogFormat(String logTag, String level) {
            return "[" + String(esp_timer_get_time() / 1000) + "] [" + level + "] [" + logTag + "] ";
        }

        /**
         * @brief Roll the log, if size is too big or if there is too much files
         * 
         * @param roll Argument to know if we need to switch file or delete the oldest one
         */
        void logRoll(roll_t roll) {
            if(roll == SWITCH) {
                createNewFile();
            }
            else if(roll == DELETE) {
                deleteOldestFile();
                createNewFile();
            }
        }

        /**
         * @brief Delete the oldest log file
         * 
         */
        void deleteOldestFile() {
            sd.deleteFile(String(log_file)+".txt");
            oldest_log_file++;
        }

        /**
         * @brief Create a New log File
         * 
         */
        void createNewFile() {
            setLogFile(log_file + 1);

            sd.createFile(String(log_file)+".txt");
        }
};

#endif