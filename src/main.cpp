#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
	// Set up openGL version 4.3
	ofSetupOpenGL(800, 800, OF_WINDOW);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
