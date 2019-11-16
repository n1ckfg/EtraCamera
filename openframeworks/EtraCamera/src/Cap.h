#pragma once

#include "ofMain.h"

class Cap {
    
    public:
        Cap();
        Cap(int w, int h, int _fps, int _device);
    
        void setup(int w, int h, int _fps, int _device);
        void update();
        void draw(int w, int h);
        void clear();
        ofPixels& getPixels();

        int width;
        int height;
        int fps;
        int device;
    
        ofVideoGrabber grabber;
        bool newFrame;
    
};
