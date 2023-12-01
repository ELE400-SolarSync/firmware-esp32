#ifndef LOG_H
#define LOG_H

#include "esp_log.h"
#include <HTTPClient.h>
#include "../../sd/src/sdcustom.hpp"

class myLogger {
    private:
        int log_path;
        int log_file;
        int oldest_log_file = 0;
        String log_fullPath;
        bool SerialToglle = true;
        bool SDToglle = true;
        SDCustom& sd;

        enum roll_t {
            SWITCH,
            DELETE
        };

    public:
        enum level_t {
            DEBUG = 0,
            INFO,
            WARN,
            ERROR
        };
        level_t logLevel;

        myLogger(SDCustom& sd, int log_file = 0, int log_path = 0, level_t level = INFO);
        void init();
        level_t getLevel() const;
        int getLogPath() const;
        int getLogFile() const;
        void setOldLogFile(int old_log);
        int getOldLogFile() const;
        void setLogPath(int log_path);
        void setLogFile(int log_file);
        void setLogFullPath();
        void setLevel(level_t level);
        void error(String tag, String message);
        void warning(String tag, String message); 
        void info(String tag, String message);
        void debug(String tag, String message);
        void enableLoggingInMonitor();
        void disableLoggingInMonitor();
        void enableLoggingInSD();
        void disableLoggingInSD();
        void log(String logFormat, String message);
        String buildlogFormat(String logTag, String level);
        void logRoll(roll_t roll);
        void deleteOldestFile();
        void createNewFile();
};

#endif