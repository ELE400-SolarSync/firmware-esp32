#include <SPI.h>
#include <SD.h>

// https://github.com/arduino-libraries/SD/issues/129

#ifndef SDCUSTOM_H
#define SDCUSTOM_H
class SDCustom {
    public:
        int max_size; //< Max size of the SD Card

        /**
         * @brief Construct a new SDCustom object
         * 
         * @param size the max size of the SD Card
         */
        SDCustom(int size) {
            this->max_size = size;
        };

        /**
         * @brief Start the SD Card
         * 
         * @return bool has the SD Card started 
         */
        bool begin() {
            pinMode(SD_DET, INPUT);

            SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
            if (!SD.begin(SD_CS)) {
                return false;
            }
            return true;
        };

        bool end() {
            SD.end();
            SPI.end();
            return true;
        };

        /**
         * @brief Check if the SD Card is inserted
         * 
         * @return bool is the SD Card inserted
         */
        bool isSDInserted() {
            if(!digitalRead(SD_DET)) {
                return false;
            }
            return true;
        };


        /**
         * @brief Check if a file exists
         * 
         * @param filename the name of the file
         * @return bool does the file exist
         */
        bool fileExists(String filename) {
            if (!SD.exists(filename)) {
                return false;
            }
            return true;
        };

        /**
         * @brief Open a file
         * 
         * @param filename 
         * @return SDFile 
         */
        SDFile openFile(String filename) {
            SDLib::File myFile = SD.open(filename);
            return myFile;
        };

        /**
         * @brief Close a file
         * 
         * @param file the file to close
         */
        void closeFile(SDFile file) {
            file.close();
        };


        /**
         * @brief Create a file
         * 
         * @param filename the name of the file
         */
        void createFile(String filename) {
            SDLib::File myFile = SD.open(filename, FILE_WRITE);
            myFile.close();
        };


        /**
         * @brief Delete a file
         * 
         * @param filename the name of the file
         */
        void deleteFile(String filename) {
            SD.remove(filename);
        };

        /**
         * @brief Create a directory
         * 
         * @param path 
         */
        void mkdir(String path) {
            Serial.println("Created directory: " + path + " / " + SD.mkdir(path));
        };

        /**
         * @brief Write to a file
         * 
         * @param filename the name of the file
         * @param message the message to write
         */
        void writeFile(String filename, String message) {
            if(!fileExists(filename)) {
                createFile(filename);
            }
            SDLib::File myFile = SD.open(filename, FILE_WRITE);
            myFile.println(message);
            myFile.close();
        };


        /**
         * @brief Read a file
         * 
         * @param filename the name of the file
         * @return String the contents of the file
         */
        String readFile(String filename) {
            String content;

            if(!fileExists(filename)) {
                return "error";
            }

            SDLib::File logfile = SD.open(filename);
            if(logfile){
                while(logfile.available()) {
                content += (char)logfile.read();
            }
            }
            logfile.close();

            return content;
        };

    private:
        int SD_MISO = 5; //< MISO pin
        int SD_MOSI = 7; //< MOSI pin
        int SD_SCLK = 6; //< SCLK pin
        int SD_CS = 18; //< CS pin
        int SD_DET = 19; //< DET pin
};

#endif // SDCUSTOM_H