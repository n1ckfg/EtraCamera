#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofBackground(34, 34, 34);
    
    cap = Cap(1280, 720, 30, 0);

    int bufferSize = 512;
    sampleRate = 44100;
    phase = 0;
    phaseAdder = 0.0f;
    phaseAdderTarget = 0.0f;
    volume = 0.1f;
    pan = 0.5f;

    lAudio.assign(bufferSize, 0.0);
    rAudio.assign(bufferSize, 0.0);
    
    soundStream.printDeviceList();
    
    ofSoundStreamSettings settings;
    
    // if you want to set the device id to be different than the default:
    //
    //    auto devices = soundStream.getDeviceList();
    //    settings.setOutDevice(devices[3]);
    
    // you can also get devices for an specific api:
    //
    //    auto devices = soundStream.getDeviceList(ofSoundDevice::Api::PULSE);
    //    settings.setOutDevice(devices[0]);
    
    // or get the default device for an specific api:
    //
    // settings.api = ofSoundDevice::Api::PULSE;
    
    // or by name:
    //
    //    auto devices = soundStream.getMatchingDevices("default");
    //    if(!devices.empty()) {
    //        settings.setOutDevice(devices[0]);
    //    }
    
#ifdef TARGET_LINUX
    // Latest linux versions default to the HDMI output
    // this usually fixes that. Also check the list of available
    // devices if sound doesn't work
    auto devices = soundStream.getMatchingDevices("default");
    if(!devices.empty()) {
        settings.setOutDevice(devices[0]);
    }
#endif
    
    settings.setOutListener(this);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);
    
    // on OSX: if you want to use ofSoundPlayer together with ofSoundStream you need to synchronize buffersizes.
    // use ofFmodSetBuffersize(bufferSize) to set the buffersize in fmodx prior to loading a file.
}


//--------------------------------------------------------------
void ofApp::update() {
    cap.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & buffer) {
    float time = sin((float) ofGetElapsedTimeMillis() / 100.0);
    time = ofMap(time, -1.0, 1.0, 0.6, 0.8);
    
    float leftScale = 1 - pan;
    float rightScale = pan;
    
    // sin (n) seems to have trouble when n is very large, so we
    // keep phase in the range of 0-TWO_PI like this:
    while (phase > TWO_PI) {
        phase -= TWO_PI;
    }
    
    phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
    for (size_t i = 0; i < buffer.getNumFrames(); i++) {
        phase += phaseAdder;
        float sample = sin(phase);
        lAudio[i] = buffer[i*buffer.getNumChannels()    ] = sample * time * volume * leftScale;
        rAudio[i] = buffer[i*buffer.getNumChannels() + 1] = sample * time * volume * rightScale;
    }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
    
}
