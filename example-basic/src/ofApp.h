#pragma once
#include "ofMain.h"
#include "ofxWebpAnimation.h"

class ofApp : public ofBaseApp {
public:
	void setup() override;
	void update() override;
	void draw() override;

private:
	ofxWebpAnimation anim;
};
