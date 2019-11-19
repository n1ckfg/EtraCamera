#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

void ofApp::setup() {
    settings.loadFile("settings.xml");

    ofSetVerticalSync(false);    
    framerate = settings.getValue("settings:framerate", 60); 
    width = settings.getValue("settings:width", 160); 
    height = settings.getValue("settings:height", 120); 
    ofSetFrameRate(framerate);

    host = settings.getValue("settings:host", "127.0.0.1"); 
    port = settings.getValue("settings:port", 7110);
    
    debug = (bool) settings.getValue("settings:debug", 1);

    sender.setup(host, port);

    // ~ ~ ~   get a persistent name for this computer   ~ ~ ~
    compname = "RPi";
    file.open(ofToDataPath("compname.txt"), ofFile::ReadWrite, false);
    ofBuffer buff;
    if (file) { // use existing file if it's there
        buff = file.readToBuffer();
        compname = buff.getText();
    } else { // otherwise make a new one
        compname += "_" + ofGetTimestampString("%y%m%d%H%M%S%i");
        ofStringReplace(compname, "\n", "");
        ofStringReplace(compname, "\r", "");
        buff.set(compname.c_str(), compname.size());
        ofBufferToFile("compname.txt", buff);
    }
    std::cout << compname << endl;

    cam.setup(width, height, false); // color/gray;

    triggerThreshold = settings.getValue("settings:trigger_threshold", 0.5);
    flowResetThreshold = settings.getValue("settings:flow_reset_threshold", 1.0);
    counterMax = settings.getValue("settings:trigger_frames", 3);
    timeDelay = settings.getValue("settings:time_delay", 5000);
    counterDelay = settings.getValue("settings:counter_reset", 1000);

    // ~ ~ ~   cam settings   ~ ~ ~
    camSharpness = settings.getValue("settings:sharpness", 0); 
    camContrast = settings.getValue("settings:contrast", 0); 
    camBrightness = settings.getValue("settings:brightness", 50); 
    camIso = settings.getValue("settings:iso", 300); 
    camExposureMode = settings.getValue("settings:exposure_mode", 0); 
    camExposureCompensation = settings.getValue("settings:exposure_compensation", 0); 
    camShutterSpeed = settings.getValue("settings:shutter_speed", 0);

    cam.setSharpness(camSharpness);
    cam.setContrast(camContrast);
    cam.setBrightness(camBrightness);
    cam.setISO(camIso);
    cam.setExposureMode((MMAL_PARAM_EXPOSUREMODE_T) camExposureMode);
    cam.setExposureCompensation(camExposureCompensation);
    cam.setShutterSpeed(camShutterSpeed);
    //cam.setFrameRate // not implemented in ofxCvPiCam

    // ~ ~ ~   optical flow settings   ~ ~ ~
    useFarneback = (bool) settings.getValue("settings:dense_flow", 1);
    sendMotionInfo = (bool) settings.getValue("settings:send_motion_info", 1);
    pyrScale = 0.5;   // 0 to 1, default 0.5
    levels = 4;   // 1 to 8, default 4
    winsize = 8;   // 4 to 64, default 8
    iterations = 2;   // 1 to 8, default 2
    polyN = 7;   // 5 to 10, default 7
    polySigma = 1.5;   // 1.1 to 2, default 
    OPTFLOW_FARNEBACK_GAUSSIAN = false; // default false
    winSize = 32;   // 4 to 64, default 32
    maxLevel = 3;   // 0 to 8, default 3
    maxFeatures = 200;   // 1 to 1000, default 200
    qualityLevel = 0.01;   // 0.001 to 0.02, default 0.01
    minDistance = 4;   // 1 to 16, default 4

    motionVal = 0;
    counterOn = 0;
    markTriggerTime = 0;
    trigger = false;
    isMoving = false;

    //------------------- for the simple sine wave synthesis
    int bufferSize = 512;
    sampleRate = 44100;
    phase = 0;
    phaseAdder = 0.0;
    volume = 2.0;
    pan = 0.5;
    targetFrequency = 590.0;
    phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;

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
    soundStream.stop();
    
    // on OSX: if you want to use ofSoundPlayer together with ofSoundStream you need to synchronize buffersizes.
    // use ofFmodSetBuffersize(bufferSize) to set the buffersize in fmodx prior to loading a file.
}

void ofApp::update() {
    frame = cam.grab();

    if (!frame.empty()) {
        if (useFarneback) {
			curFlow = &farneback;
	        farneback.setPyramidScale( pyrScale );
	        farneback.setNumLevels( levels );
	        farneback.setWindowSize( winsize );
	        farneback.setNumIterations( iterations );
	        farneback.setPolyN( polyN );
	        farneback.setPolySigma( polySigma );
	        farneback.setUseGaussian( OPTFLOW_FARNEBACK_GAUSSIAN );
		} else {
			curFlow = &pyrLk;
            pyrLk.setMaxFeatures( maxFeatures );
            pyrLk.setQualityLevel( qualityLevel );
            pyrLk.setMinDistance( minDistance );
            pyrLk.setWindowSize( winSize );
            pyrLk.setMaxLevel( maxLevel );
		}

        // you use Flow polymorphically
        curFlow->calcOpticalFlow(frame);

        if (useFarneback) {
        	motionValRaw = farneback.getAverageFlow();
    	} else {
    		motionValRaw = glm::vec2(0,0);
    		auto points = pyrLk.getMotion();

    		for (int i=0; i<points.size(); i++) {
    			motionValRaw.x += points[i].x;
    			motionValRaw.y += points[i].y;
    		}  
    		motionValRaw.x /= (float) points.size();
    		motionValRaw.y /= (float) points.size();
    	}

        motionVal = (abs(motionValRaw.x) + abs(motionValRaw.y)) / 2.0;
        isMoving = motionVal > triggerThreshold;
        std::cout << "val: " << motionVal << " motion: " << isMoving << endl;

        int t = ofGetElapsedTimeMillis();
        
        // reset count if too much time has elapsed since the last change
        if (t > markCounterTime + counterDelay) counterOn = 0;

        // motion detection logic
        // 1. motion detected, but not triggered yet
    	if (!trigger && isMoving) {
        	if (counterOn < counterMax) { // start counting the ON frames
        		counterOn++;
                markCounterTime = t;
        	} else { // trigger is ON
                markTriggerTime = t;
	        	trigger = true;
                soundStream.start();
	        }  
        // 2. motion is triggered
        } else if (trigger && isMoving) { // keep resetting timer as long as motion is detected
            markTriggerTime = t;
        // 3. motion no longer detected
    	} else if (trigger && !isMoving && t > markTriggerTime + timeDelay) {
            trigger = false;
            soundStream.stop();
        }

        sendOsc();
    }

    // optical flow can get stuck in feedback loops
    if (motionVal > flowResetThreshold) {
        curFlow->resetFlow();
        trigger = false;
        soundStream.stop();
    }
}

void ofApp::draw() {   
    if (debug) {
    	ofSetColor(255);
    	ofBackground(0);

        if(!frame.empty()) {
    	    drawMat(frame,0, 0);
    	    curFlow->draw(0, 0);
    	}

        stringstream info;
        info << "FPS: " << ofGetFrameRate() << endl;
        ofDrawBitmapStringHighlight(info.str(), 10, 10, ofColor::black, ofColor::yellow);
    }
}


void ofApp::sendOsc() {
	ofxOscMessage msg;

    msg.setAddress("/pilencer");
    msg.addStringArg(compname);
    msg.addIntArg((int) trigger);

    // if you're only detecting motion, leave this off to save bandwidth
    if (sendMotionInfo) {
        msg.addFloatArg(motionVal); // total motion, always positive
        msg.addFloatArg(motionValRaw.x); // x change
        msg.addFloatArg(motionValRaw.y); // y change
    }  

    sender.sendMessage(msg);
    std:cout << "*** SENT: " << trigger << " ***\n";
}

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
