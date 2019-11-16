#include "Cap.h"

Cap::Cap() {
    setup(640, 480, 30, 0);
}

Cap::Cap(int w, int h, int _fps, int _device) {
    setup(w, h, _fps, _device);
}

void Cap::setup(int w, int h, int _fps, int _device) {
    width = w;
    height = h;
    fps = _fps;
    device = _device;
    
    // get back a list of devices.
    vector<ofVideoDevice> devices = grabber.listDevices();
    
    for(size_t i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            // log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            // log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    
    grabber.setDeviceID(device);
    grabber.setDesiredFrameRate(fps);
    grabber.initGrabber(w, h);
    ofSetVerticalSync(true);
}

void Cap::update() {
    grabber.update();
    
    if(grabber.isFrameNew()) {
        newFrame = true;
    } else {
        newFrame = false;
    }
}

void Cap::draw(int w, int h) {
    grabber.draw(w, h);
}

ofPixels& Cap::getPixels() {
    return grabber.getPixels();
}
