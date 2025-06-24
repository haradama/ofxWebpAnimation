#include "ofApp.h"

void ofApp::setup() {
	ofSetWindowTitle("ofxWebpAnimation - basic example");
	ofBackground(50);

	if (!anim.load(ofToDataPath("animation.webp"))) {
		ofLogError() << "Failed to load animation.webp";
	}
}

void ofApp::update() {
	anim.update();
}

void ofApp::draw() {
	if (anim.isLoaded()) {
		float scale = std::min(ofGetWidth() / float(anim.getWidth()),
			ofGetHeight() / float(anim.getHeight()));
		float w = anim.getWidth() * scale;
		float h = anim.getHeight() * scale;
		anim.draw((ofGetWidth() - w) * 0.5f,
			(ofGetHeight() - h) * 0.5f,
			w, h);
	}
}
