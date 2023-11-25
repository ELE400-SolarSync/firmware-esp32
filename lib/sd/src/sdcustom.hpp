#include <SPI.h>
#include <SD.h>

// https://github.com/arduino-libraries/SD/issues/129


class SDCustom {
    public:
        bool begin() {
            pinMode(SD_DET, INPUT);

            SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
            if (!SD.begin(SD_CS)) {
                return false;
            }
            return true;
        };

        bool isSDInserted() {
            if(!digitalRead(SD_DET)) {
                return false;
            }
            return true;
        };

        bool fileExists(const char *filename) {
            if (!SD.exists(filename)) {
                return false;
            }
            return true;
        };

        void createFile(const char *filename) {
            SDLib::File myFile = SD.open(filename, FILE_WRITE);
            myFile.close();
        };

        void deleteFile(const char *filename) {
            SD.remove(filename);
        };

        void writeFile(const char *filename, const char *message) {
            if(!fileExists(filename)) {
                createFile(filename);
            }
            SDLib::File myFile = SD.open(filename, FILE_WRITE);
            myFile.println(message);
            myFile.close();
        };

        String readFile(const char *filename) {
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
        int SD_MISO = 19;
        int SD_MOSI = 23;
        int SD_SCLK = 18;
        int SD_CS = 5;
        int SD_DET = 22;
};