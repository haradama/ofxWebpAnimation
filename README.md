# ofxWebpAnimation

An openFrameworks addon for decoding and rendering animated WebP images using the [libwebp](https://chromium.googlesource.com/webm/libwebp/) image codec.  
It supports frame extraction, timing, and GPU texture upload for smooth playback in creative coding environments.

## Features

- Load animated WebP files (`*.webp`)
- Decode all frames with proper duration metadata
- Upload frames to `ofTexture` for GPU-accelerated rendering
- Play, loop, and draw animation in your OF app

## Requirements

- [libwebp](https://github.com/webmproject/libwebp)
- [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/) (for auto-discovery of headers/libs)

## Installation

### Clone into your `addons/` folder

```bash
cd openFrameworks/addons
git clone https://github.com/haradama/ofxWebpAnimation.git
````

### Install dependencies

#### macOS (Homebrew)

```bash
brew install webp pkg-config
```

#### Ubuntu / Debian

```bash
sudo apt-get install libwebp-dev pkg-config
```

#### Windows (MSYS2)

```bash
pacman -S mingw-w64-x86_64-libwebp mingw-w64-x86_64-pkg-config
```

## Usage

Place an animated WebP file (e.g., `animation.webp`) in your app's `bin/data/` folder.

### Example

```cpp
#include "ofMain.h"
#include "ofxWebpAnimation.h"

class ofApp : public ofBaseApp {
    ofxWebpAnimation anim;

    void setup() {
        anim.load(ofToDataPath("animation.webp"));
    }

    void update() {
        anim.update();
    }

    void draw() {
        anim.draw(0, 0);
    }
};
```

## License

This project is licensed under the MIT License.
See the [LICENSE](LICENSE) file for details.

## Example Project

Check out the included `example-basic/` for a minimal working demo.
