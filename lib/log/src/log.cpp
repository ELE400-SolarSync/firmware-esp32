#include "log.hpp"

myLogger::myLogger(SDCustom& sd, int log_file, int log_path, myLogger::level_t level) : sd(sd), log_file(log_file), log_path(log_path), logLevel(level) {
    this->log_fullPath = String(log_file) + ".txt";
}

/**
 * @brief Initialise the logger
 * 
 */
void myLogger::init() {
    sd.begin();

    if (!sd.isSDInserted()) {
        SDToglle = false;
        error("logger", "SD Card is not inserted");
    }

    if (!sd.fileExists(log_fullPath)) {
        sd.createFile(log_fullPath);
    }
}

myLogger::level_t myLogger::getLevel() const {
    return logLevel;
}

int myLogger::getLogPath() const {
    return log_path;
}

int myLogger::getLogFile() const {
    return log_file;
}

void myLogger::setOldLogFile(int old_log) {
    this->oldest_log_file = old_log;
}

int myLogger::getOldLogFile() const {
    return oldest_log_file;
}

void myLogger::setLogPath(int log_path) {
    this->log_path = log_path;
}

void myLogger::setLogFile(int log_file) {
    this->log_file = log_file;
}

void myLogger::setLogFullPath() {
    this->log_fullPath = String(log_path) + "/" + String(log_file) + ".txt";
}

void myLogger::setLevel(level_t level) {
    this->logLevel = level;
}

/**
 * @brief Function to write error log
 * 
 * @param tag First part to identify where comes the log
 * @param message 
 */
void myLogger::error(String tag, String message) {
    log(buildlogFormat(tag, "E"), message.c_str());
}

/**
 * @brief Function to write warning log
 * 
 * @param tag First part to identify where comes the log
 * @param message 
 */
void myLogger::warning(String tag, String message) {
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
void myLogger::info(String tag, String message) {
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
void myLogger::debug(String tag, String message) {
    if(logLevel <= DEBUG){
        log(buildlogFormat(tag, "D"), message.c_str());
    }
}

void myLogger::enableLoggingInMonitor() {
    SerialToglle = true;
}

void myLogger::disableLoggingInMonitor() {
    SerialToglle = false;
}

void myLogger::enableLoggingInSD() {
    SDToglle = true;
}

void myLogger::disableLoggingInSD() {
    SDToglle = false;
}

/**
 * @brief Main function to write the log
 * 
 * @param logFormat Format of the log
 * @param message Message to log
 */
void myLogger::log(String logFormat, String message) {
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
                // if there is more than 30 0000 (30Go) files in the folder (each files is 1Mo)
                logRoll(DELETE);
                sd.writeFile(String(log_file)+".txt", logFormat + message);                    
            }
            else if (sd.openFile(String(log_file)+".txt").size() > 1000000) {
                // if file is bigger than 1Mo
                logRoll(SWITCH);
                sd.writeFile(String(log_file)+".txt", logFormat + message);
            }
            else {
                // Write to file
                sd.writeFile(String(log_file)+".txt", logFormat + message);
            }
        }
    }
}

String myLogger::buildlogFormat(String logTag, String level) {
    return "[" + String(esp_timer_get_time() / 1000) + "] [" + level + "] [" + logTag + "] ";
}

/**
 * @brief Roll the log, if size is too big or if there is too much files
 * 
 * @param roll Argument to know if we need to switch file or delete the oldest one
 */
void myLogger::logRoll(roll_t roll) {
    if(roll == SWITCH) {
        createNewFile();
    }
    else if(roll == DELETE) {
        myLogger::deleteOldestFile();
        createNewFile();
    }
}

void myLogger::deleteOldestFile() {
    sd.deleteFile(String(log_file)+".txt");
    oldest_log_file++;
}

void myLogger::createNewFile() {
    setLogFile(log_file + 1);

    sd.createFile(String(log_file)+".txt");
}