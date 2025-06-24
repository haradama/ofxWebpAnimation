#include "ofxWebpAnimation.h"
#include <fstream>
#include <iostream>
#include <webp/decode.h>
#include <webp/demux.h>

ofxWebpAnimation::ofxWebpAnimation()
	: currentFrameIdx(0)
	, lastFrameTimestamp(0)
	, looping(true)
	, width(0)
	, height(0) { }

bool ofxWebpAnimation::load(const std::filesystem::path & path) {
	// ------------------------------------------------------------
	// Read file into memory
	// ------------------------------------------------------------
	std::ifstream ifs(path, std::ios::binary | std::ios::ate);
	if (!ifs.is_open()) {
		ofLogError("ofxWebpAnimation") << "Cannot open file: " << path;
		return false;
	}
	std::streamsize size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	std::vector<uint8_t> data(size);
	if (!ifs.read(reinterpret_cast<char *>(data.data()), size)) {
		ofLogError("ofxWebpAnimation") << "File read failed: " << path;
		return false;
	}

	WebPData webpData;
	webpData.bytes = data.data();
	webpData.size = data.size();

	// ------------------------------------------------------------
	// Decoder options
	// ------------------------------------------------------------
	WebPAnimDecoderOptions decOpts;
	if (!WebPAnimDecoderOptionsInit(&decOpts)) {
		ofLogError("ofxWebpAnimation") << "WebPAnimDecoderOptionsInit failed";
		return false;
	}
	decOpts.color_mode = MODE_RGBA; // RGBA output
	decOpts.use_threads = 1;

	// ------------------------------------------------------------
	// Create decoder
	// ------------------------------------------------------------
	WebPAnimDecoder * dec = WebPAnimDecoderNew(&webpData, &decOpts);
	if (!dec) {
		ofLogError("ofxWebpAnimation") << "WebPAnimDecoderNew failed";
		return false;
	}

	// ------------------------------------------------------------
	// Retrieve animation info
	// ------------------------------------------------------------
	WebPAnimInfo info;
	if (!WebPAnimDecoderGetInfo(dec, &info)) {
		ofLogError("ofxWebpAnimation") << "WebPAnimDecoderGetInfo failed";
		WebPAnimDecoderDelete(dec);
		return false;
	}
	width = info.canvas_width;
	height = info.canvas_height;

	// ------------------------------------------------------------
	// Decode all frames
	// ------------------------------------------------------------
	uint8_t * rgba = nullptr;
	int timestampMs = 0;
	std::vector<uint32_t> timestamps;

	while (WebPAnimDecoderHasMoreFrames(dec)) {
		if (!WebPAnimDecoderGetNext(dec, &rgba, &timestampMs)) {
			ofLogError("ofxWebpAnimation") << "WebPAnimDecoderGetNext failed";
			WebPAnimDecoderDelete(dec);
			return false;
		}

		// Copy RGBA to ofPixels
		Frame frame;
		frame.durationMs = 0; // temporary
		frame.pixels.setFromPixels(rgba, width, height, OF_PIXELS_RGBA);

		// Upload to GPU
		frame.texture.loadData(frame.pixels);

		frames.push_back(std::move(frame));
		timestamps.push_back(static_cast<uint32_t>(timestampMs));
	}

	WebPAnimDecoderDelete(dec);

	if (frames.empty()) {
		ofLogError("ofxWebpAnimation") << "No frames decoded";
		return false;
	}

	// ------------------------------------------------------------
	// Calculate per-frame durations
	// ------------------------------------------------------------
	for (std::size_t i = 0; i < frames.size(); ++i) {
		uint32_t dur = 100; // fallback 100 ms
		if (i + 1 < timestamps.size()) {
			dur = timestamps[i + 1] - timestamps[i];
		} else if (i > 0) {
			dur = timestamps[i] - timestamps[i - 1];
		}
		frames[i].durationMs = dur > 0 ? dur : 100;
	}

	// Reset playback state
	currentFrameIdx = 0;
	lastFrameTimestamp = ofGetElapsedTimeMillis();
	looping = true;

	ofLogNotice("ofxWebpAnimation") << "Loaded " << frames.size()
									<< " frames (" << width << "x" << height << ")";
	return true;
}

void ofxWebpAnimation::update() {
	if (frames.empty()) return;

	uint64_t now = ofGetElapsedTimeMillis();
	Frame & cur = frames[currentFrameIdx];
	if (now - lastFrameTimestamp >= cur.durationMs) {
		currentFrameIdx++;
		if (currentFrameIdx >= frames.size()) {
			if (looping)
				currentFrameIdx = 0;
			else
				currentFrameIdx = frames.size() - 1;
		}
		lastFrameTimestamp = now;
	}
}

void ofxWebpAnimation::draw(float x, float y) const {
	if (frames.empty()) return;
	frames[currentFrameIdx].texture.draw(x, y);
}

void ofxWebpAnimation::draw(float x, float y, float w, float h) const {
	if (frames.empty()) return;
	frames[currentFrameIdx].texture.draw(x, y, w, h);
}

// ---------------------------------------------------------------------
// Getters / setters
// ---------------------------------------------------------------------
void ofxWebpAnimation::setLooping(bool loop) { looping = loop; }
bool ofxWebpAnimation::getLooping() const { return looping; }
bool ofxWebpAnimation::isLoaded() const { return !frames.empty(); }
std::size_t ofxWebpAnimation::getNumFrames() const { return frames.size(); }
int ofxWebpAnimation::getWidth() const { return width; }
int ofxWebpAnimation::getHeight() const { return height; }
