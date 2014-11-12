#include "ofApp.h"

// comparison routine for sort...
bool comparisonFunction(  particle a, particle b ) {
    return a.pos.x < b.pos.x;
}


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    //ofSetCircleResolution(100);
    
    soundStream.listDevices();
    ofSoundStreamSetup(0, 2, this, 44100, beat.getBufferSize(), 32);

    //gui.setup(); // most of the time you don't need a name
    //gui.add(gKick.setup("fill", true));
    
    spot.loadImage("spot.png");

    numParticles = 100;
    particleSetup(numParticles);
    
    ofBackground(0);
    
    

    
    dxDamp = 0.03f;
    
    verbs = ofSplitString(bf_verbs, ",", true, true);
    nouns = ofSplitString(bf_nouns, ",", true, true);
    
    bf_verbs.clear();
    bf_nouns.clear();
    
    cout << verbs.size() <<endl;
    cout << verbs[verbs.size()-1] <<endl;
    
    cout << nouns.size() <<endl;
    cout << nouns[0] <<endl;
    
    
    ExoTwoLight.loadFont("Exo2-Light.ttf", 64, true, true);
    ExoTwoLight.setLineHeight(18.0f);
    ExoTwoLight.setLetterSpacing(.90);
    

    
}




//--------------------------------------------------------------
void ofApp::draw(){
    
    ofColor colorOne;
    ofColor colorTwo;
    
    colorOne.set (30);
    colorTwo.set (0);
    
    ofBackgroundGradient(colorOne, colorTwo, OF_GRADIENT_CIRCULAR);
    
    // Sound Setup
    
    kick = beat.kick()*10;
    snare = beat.snare()*10;
    hihat = beat.hihat()*10;
    magnitude = beat.getMagnitude()*10;
    
    // Sound Parameterization
    
    if (kick > 9){ bRepel = !bRepel; }
    
    if (hihat){
        dxRadius = ofMap(hihat, 1, 10, 0, 150); }
        float diff = radius - dxRadius;
    
        if (abs(diff) >  0.01) {
        
            radius -= diff/10.0;
            radius = radius+1;
            //cout << "radius:" << radius << ":"<< dxRadius << ":" << diff <<endl;
 
        }

    if (snare > .5){ strength = ofMap(snare, .5, 10, .5, 7); }
    if ((magnitude > 1 ) && (int(ofRandom(0,5)) == 3)){ textDraw = verbs[ofRandom(verbs.size()-1)] + "/" + nouns[ofRandom(nouns.size()-1)];}
    textAlpha = ofMap(snare, 0, 2, 50, 255);



    // Spectrum Build & Draw
    buildSpectrum();
    if (toggleSpectrum){
        
        ofSetRectMode(OF_RECTMODE_CORNER);
        drawSpectrum();
        
    }
    
    // ParticleDrawing
    if (toggleParticles){ showParticles();}
    
    // Draw Perspective Rect
    if (togglePerspecRect){ perspecRect();}
    
    // TextStrings
    if (toggleText){ showText();}
    
    // Reporting on the interface data
    if (toggleControls){
        

        string reportString =	"(space) = reset\n(a/s) strength = " + ofToString(strength) +
        "\n(z/x) radius = " + ofToString(radius) +
        "\n(r) toggle mode = " + (bRepel ? "repelling" : "attracting") +
        "\nFramerate = " + ofToString(ofGetFrameRate()) +
        "\n#Particles = " + ofToString(particles.size());
        
        
        ofSetColor(255,255,255);
        ofDrawBitmapString(reportString, 15, 20);
        
        ofSetHexColor(0xffffff);
        
        ofEllipse(25, 115, kick, kick);
        ofDrawBitmapString("Kick: " + ofToString(kick), 50, 120);
        
        ofEllipse(25, 145, snare, snare);
        ofDrawBitmapString("Snare: " + ofToString(snare), 50, 150);
        
        ofEllipse(25, 175, hihat, hihat);
        ofDrawBitmapString("Hi-Hat: " + ofToString(hihat), 50, 180);
        
        ofEllipse(25, 205, magnitude, magnitude);
        ofDrawBitmapString("Magnitude: " + ofToString(magnitude), 50, 210);
    
    }
    


    
}


void ofApp::particleSetup(int numParticles){
    
    ofEnableAlphaBlending();
    ofSetRectMode(OF_RECTMODE_CENTER);
    
    for (int i = 0; i < numParticles; i++){
        particle myParticle;
        myParticle.setInitialCondition(ofRandom((ofGetWidth()/2)-2,(ofGetWidth()/2)+2),ofRandom((ofGetWidth()/2)-2,(ofGetWidth()/2)+2),0,0);
        myParticle.img = &spot;
        particles.push_back(myParticle);
        
    }
    
    bRepel		= true;
    radius		= 40;
    strength	= 0.5f;
    
    cout << particles.size() << ": Particle Setup" << endl;
    
    
}


void ofApp::showParticles(){


    ofEnableAlphaBlending();
    ofSetRectMode(OF_RECTMODE_CENTER);
    for (int i = 0; i < particles.size(); i++){

        ofSetColor(255,255,255,ofMap(spectHist[j][6], 0, 100, 50, 255));
        particles[i].draw(i/2);
        ofSetColor(255,255,255,ofMap(spectHist[j][4], 0, 100, 50, 255));
        particles[i].draw(25+hihat*20);
        ofSetColor(255,255,255,ofMap(spectHist[j][0], 0, 100, 50, 255));
        particles[i].draw(25+kick*30);
        
        
    }


}

void ofApp::showText(){

    ofSetColor(225,textAlpha*3);
    float strSize = ExoTwoLight.stringWidth(textDraw);
    ExoTwoLight.drawString(textDraw, ofGetWidth()/2 - strSize/2, (ofGetHeight()/2)+25);


}

void ofApp::buildSpectrum(){
    
    
    
    float selectedBand;
    
    // 31 is the number of bands supported
    for (int i=0; i < 32; i++) {
        float selectedBand = beat.getBand(i);
        selectedBand = ofMap(selectedBand, 0, 3.5, 0, 100); //adjust resolution
        spectHist[i][j] = selectedBand;
        //cout << i << ":" << selectedBand << " " << endl;
    }
    
    j++;
    if (j == histLimit ){ j=0; }
    //cout << endl;
    
    
}

void ofApp::perspecRect(){
    //textAlpha = 255;
    //cout << "inPerspecRect: "<<textAlpha<< ":"<<hihat<<":"<<kick<<":"<<snare<< endl;
    ofSetColor(225,textAlpha/3);
    ofFill();
    
    // Weird Dimensional thing
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
        ofScale(.1, .1);
        ofRotateX(ofGetFrameNum()/hihat); ofRotateY(ofGetFrameNum()/kick); ofRotateZ(ofGetFrameNum()/snare);
        ofNoFill();
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        ofFill();
        ofSetColor(255,0,0,textAlpha/3);

        ofRect(-ofGetWidth(), -ofGetHeight(),ofGetWidth(), ofGetHeight());
        
        ofScale(5, 5);
        ofSetColor(255,textAlpha/3);
        ofRotateX(ofGetFrameNum()/hihat); ofRotateY(ofGetFrameNum()/kick); ofRotateZ(ofGetFrameNum()/snare);
        ofNoFill();
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(0,255,0,textAlpha/3);
        ofFill();
        ofRect(-ofGetWidth(), -ofGetHeight(),ofGetWidth(), ofGetHeight());
    
    ofPopMatrix();
    
    
}



void ofApp::drawSpectrum(){
    
    //draw the spectrum
    ofPushMatrix();
    
    ofTranslate((ofGetWidth()/2), (ofGetHeight()/2),0);
    ofTranslate(-280,-190,0);
    
    for (int x=0; x < histLimit; x++){
        
        for (int y=0; y < 32; y++){
            
            float spectValu = ofMap(spectHist[y][x],0,100,0,20); //map for display of 10
            if (x == j){ ofSetHexColor(0xFF0000); } else if (spectValu > 8) { ofSetColor(0,255,0,30+spectValu*4);} else { ofSetColor(255,30+spectValu*4);}
            ofRect(x*9,y*13,spectValu, 3);
            
        }
        
    }
    
    ofPopMatrix();
    
    
}

void ofApp::audioReceived(float* input, int bufferSize, int nChannels) {
    beat.audioReceived(input, bufferSize, nChannels);
}


//--------------------------------------------------------------
void ofApp::update(){
    
    beat.update(ofGetElapsedTimeMillis());
    
    
    for (int i = 0; i < particles.size(); i++){
        particles[i].resetForce();
    }
    

     // sort all the particles:
     
     ofSort(particles, comparisonFunction);
     
     for (int i = 0; i < particles.size(); i++){
        particles[i].resetForce();
     }
     
     for (int i = 0; i < particles.size(); i++){

        for (int j = i-1; j >= 0; j--){

            if ( fabs(particles[j].pos.x - particles[i].pos.x) > 50) break;

            if (bRepel){
                particles[i].addRepulsionForce(particles[j], radius, strength);
            } else {
                particles[i].addAttractionForce(particles[j], radius, strength);
            }
     
     
        }
     }
     
     
 
//    
//    for (int i = 0; i < particles.size(); i++){
//        
//        
//        for (int j = 0; j < i; j++){
//            if (bRepel){
//                particles[i].addRepulsionForce(particles[j], radius, strength);
//            } else {
//                particles[i].addAttractionForce(particles[j], radius, strength);
//            }
//        }
//    }
// 

    
    for (int i = 0; i < particles.size(); i++){
        particles[i].addAttractionForce(ofGetWidth()/2, ofGetHeight()/2, 1000, 1);
        particles[i].addDampingForce();
        particles[i].update();
        particles[i].bounceOffWalls();
    }
    
    
}




//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
    switch (key){
            
        case ' ':
            // reposition everything:
            for (int i = 0; i < particles.size(); i++){
                particles[i].setInitialCondition(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()),0,0);
            }
            break;
        case 'a':
            strength += 0.02f;
            break;
        case 's':
            strength -= 0.02f;
            if (strength <= 0) strength = 0.02f;
            break;
        case 'z':
            radius += 5.0f;
            break;
        case 'x':
            radius -= 5.0f;
            if (radius <= 0) radius = 5.0f;
            break;
        case 'r':
            bRepel = !bRepel;
            break;
        case '=':
            toggleControls = !toggleControls;
            break;
         case '-':
            cout << numParticles << endl;
            particles.erase(particles.begin(),particles.begin()+numParticles);
            numParticles = numParticles+50;
            particleSetup(numParticles);
            break;
        case '0':
            cout << numParticles << endl;
            particles.erase(particles.begin(),particles.begin()+numParticles);
            numParticles = numParticles-50;
            if (numParticles < 0){numParticles=0;}
            particleSetup(numParticles);
            break;
        case '1':
            toggleSpectrum = !toggleSpectrum;
            break;
       case '2':
            toggleText = !toggleText;
            textDraw = nouns[ofRandom(nouns.size()-1)] + "/" +  verbs[ofRandom(verbs.size()-1)];
            break;
        case '3':
            togglePerspecRect = !togglePerspecRect;
            break;
        case '4':
            toggleParticles = !toggleParticles;
            break;

    }

    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    particles.erase(particles.begin());
    particle myParticle;
    myParticle.img = &spot;
    myParticle.setInitialCondition(x,y,0,0);
    particles.push_back(myParticle);

    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
