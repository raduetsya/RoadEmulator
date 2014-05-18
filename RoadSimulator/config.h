#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>
#include <sstream>

#include <exception>

template<class T>
void setInRange(T& val, T min, T max) {
    if (val < min) val = min;
    if (val > max) val = max;
}

class Config
{
public:
    struct {
        int SCREEN_WIDTH;
        int SCREEN_HEIGHT;
        float SCREEN_MAX_FRAMERATE;
        float CAR_TURN_SPEED;
        float CAR_ACCELERATE_SPEED;
        float CAR_MAX_SPEED;
        float SAFE_TURN_MANEUR_ZONE;
        int MESSAGES_HISTORY_SIZE;
        int MESSAGES_FONT_SIZE;
        float CAR_START_X;
        float CAR_START_Y;
        float CAR_START_ANGLE;


        void initialize() {
            SCREEN_WIDTH = 600;
            SCREEN_HEIGHT = 600;
            SCREEN_MAX_FRAMERATE = 30;
            CAR_TURN_SPEED = 200;
            CAR_ACCELERATE_SPEED = 200;
            CAR_MAX_SPEED = 200;
            SAFE_TURN_MANEUR_ZONE = 40;
            MESSAGES_HISTORY_SIZE = 20;
            MESSAGES_FONT_SIZE = 17;
            CAR_START_X = 220;
            CAR_START_Y = 500;
            CAR_START_ANGLE = 0;
        }

        void setKey(std::string& key, std::string& value)
        {
            std::stringstream ss;
            ss << value;

            if (key == "SCREEN_WIDTH")
                ss >> SCREEN_WIDTH;
            if (key == "SCREEN_HEIGHT")
                ss >> SCREEN_HEIGHT;
            if (key == "CAR_TURN_SPEED")
                ss >> CAR_TURN_SPEED;
            if (key == "CAR_ACCELERATE_SPEED")
                ss >> CAR_ACCELERATE_SPEED;
            if (key == "CAR_MAX_SPEED")
                ss >> CAR_MAX_SPEED;
            if (key == "SCREEN_MAX_FRAMERATE")
                ss >> SCREEN_MAX_FRAMERATE;
            if (key == "SAFE_TURN_MANEUR_ZONE")
                ss >> SAFE_TURN_MANEUR_ZONE;
            if (key == "MESSAGES_HISTORY_SIZE")
                ss >> MESSAGES_HISTORY_SIZE;
            if (key == "MESSAGES_FONT_SIZE")
                ss >> MESSAGES_FONT_SIZE;
            if (key == "CAR_START_X")
                ss >> CAR_START_X;
            if (key == "CAR_START_Y")
                ss >> CAR_START_Y;
            if (key == "CAR_START_ANGLE")
                ss >> CAR_START_ANGLE;
        }

        void validate() {
            if (SCREEN_WIDTH < 50) SCREEN_WIDTH = 50;
            if (SCREEN_HEIGHT < 50) SCREEN_HEIGHT = 50;
            if (CAR_TURN_SPEED < 0) CAR_TURN_SPEED = 0;
            if (CAR_ACCELERATE_SPEED < 0) CAR_ACCELERATE_SPEED = 0;
            if (CAR_MAX_SPEED < 0) CAR_MAX_SPEED = 0;
            setInRange(SCREEN_MAX_FRAMERATE, 0.01f, 60.0f);
            if (SAFE_TURN_MANEUR_ZONE < 0) SAFE_TURN_MANEUR_ZONE = 0;
            if (MESSAGES_HISTORY_SIZE < 1) MESSAGES_HISTORY_SIZE = 1;
            setInRange(CAR_START_X, 0.0f, (float)SCREEN_WIDTH);
            setInRange(CAR_START_Y, 0.0f, (float)SCREEN_HEIGHT);
        }

    private:
    } data;


public:
    void loadConf(std::string filename) {
        using namespace std;
        ifstream infile(filename);
        if (!infile.is_open()) {
            throw FileNotFound();
        }
        string key;
        string value;

        data.initialize();
        while(infile >> key >> value) {
            data.setKey(key,value);
        }
        data.validate();
    }


    class FileNotFound : public std::exception { };


// Singleton pattern
public:
    static Config& get();

private:
    Config() { }
    Config(Config const&);
    Config& operator=(Config const&);

    static Config* _instance;
};

#endif // CONFIG_H
