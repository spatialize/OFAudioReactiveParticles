#pragma once

#include "ofMain.h"
#include "ofxBeat.h"
#include "ofxGui.h"
#include "particle.h"

#include <string>
#include <iostream>


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofSoundStream soundStream;
    ofxBeat beat;
    void audioReceived(float*, int, int);
    
    void drawSpectrum();
    void buildSpectrum();
    void perspecRect();
    void showText();
    void showParticles();
    void particleSetup(int numParticles);
    
    ofxFloatSlider gKick;
    ofxFloatSlider gSnare;
    ofxFloatSlider gHighhat;
    ofxFloatSlider gMagnitude;
    
    ofxPanel gui;
    
    ofTrueTypeFont myfont;
    ofTrueTypeFont ExoTwoLight;
    
    int histLimit = 60;
    float spectHist[32][60] = {};
    int j = 0;
    
    // let's make a vector of them
    vector <particle> particles;

    
    bool	bRepel;
    float	radius;
    float	strength;
    float   dxDamp;
    float   dxRadius;
    
    float textAlpha;
    
    int numParticles;
    
    ofImage spot;
    
    bool toggleSpectrum;
    bool togglePerspecRect;
    bool toggleText;
    bool toggleParticles;
    
    bool toggleControls;
    ofBuffer bf_verbs = ofBufferFromFile("verbs.txt"); // reading into the buffer
    ofBuffer bf_nouns = ofBufferFromFile("nouns.txt"); // reading into the buffer
    
    vector <string> verbs;    
    vector <string> nouns;
    
    string textDraw;
    
    float kick;
    float snare;
    float hihat;
    float magnitude;
    
    
};
