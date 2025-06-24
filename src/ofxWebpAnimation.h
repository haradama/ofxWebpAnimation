#pragma once
#include "ofMain.h"
#include <filesystem>
#include <vector>

/**
 * ofxWebpAnimation — Animated WebP loader / player for openFrameworks
 */
class ofxWebpAnimation {
public:
	struct Frame {
		ofPixels pixels; /// Raw RGBA pixels
		ofTexture texture; /// GPU copy for fast drawing
		uint32_t durationMs; /// Display time of the frame
	};

	ofxWebpAnimation();
	~ofxWebpAnimation() = default;

	/** Load an animated WebP. Returns false on error. */
	bool load(const std::filesystem::path & path);

	/** Advance internal frame timer (call this in update()). */
	void update();

	/** Draw the animation at (x,y) with its original size. */
	void draw(float x, float y) const;
	/** Draw the animation at (x,y) scaled to w×h. */
	void draw(float x, float y, float w, float h) const;

	// Playback control -------------------------------------------------
	void setLooping(bool loop); ///< Enable / disable looping
	bool getLooping() const;

	// Info getters -----------------------------------------------------
	bool isLoaded() const;
	std::size_t getNumFrames() const;
	int getWidth() const;
	int getHeight() const;

private:
	std::vector<Frame> frames;
	std::size_t currentFrameIdx;
	uint64_t lastFrameTimestamp; // ofGetElapsedTimeMillis reference
	bool looping;
	int width;
	int height;
};
