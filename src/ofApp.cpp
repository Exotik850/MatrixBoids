#include "ofApp.h"

// Setup constants 
#define NUM 10000 // number of particles
#define RES 5 // resolution
#define INC 0.1 // Resolution of the noise, sampling smaller values zooms into the noise
#define MAX_SPEED 2 // max speed of particles
#define FORCE_STRENGTH 0.5 // strength of the force
#define ANG_MULTIPLIER 2 // multiplier for the angle of the vector field for less or more movement
#define UPDATE_GRID true // update the grid or not
#define LOGGING true // Display the elapsed time for drawing and processing
const int COLS = 800 / RES; // number of columns in the vector 
const int ROWS = 800 / RES; // number of rows in the vector field
const ofVec2f VEL_HEADING_REF(1, 0); // Vector for coloring

//Vector Grid
ofVec2f grid[COLS][ROWS];
float t = 0; // time for 3rd dimension scrolling of the noise 

// Particle arrays for position and velocity
ofVec2f pos[NUM];
ofVec2f prev_pos[NUM];
ofVec2f vel[NUM];

// Utility functions
ofVec2f getVectorAt(ofVec2f pos) {
	// Get the vector at the given position
	int x = ofMap(pos.x, 0, ofGetWidth(), 0, COLS);
	int y = ofMap(pos.y, 0, ofGetHeight(), 0, ROWS);
	return grid[x][y];
}

void updateGrid() {
	// Update the vector field
	for (int x = 0; x < COLS; x++) {
		for (int y = 0; y < ROWS; y++) {
			double ang = ofMap(ofNoise(x * INC, y * INC, t), 0, 1, 0, TWO_PI * ANG_MULTIPLIER);
			ofVec2f vec(cos(ang), sin(ang));
			vec *= FORCE_STRENGTH;
			grid[x][y].set(cos(ang), sin(ang));
		}
	}
}

void drawGrid() {
	// Draw the vectors as a line in the grid 
	ofSetColor(255, 255);
	for (int x = 0; x < COLS; x++) {
		for (int y = 0; y < ROWS; y++) {
			ofDrawLine(x * RES, y * RES, x * RES + grid[x][y].x * RES, y * RES + grid[x][y].y * RES);
		}
	}
}


//--------------------------------------------------------------
void ofApp::setup() {
	// Seed
	ofSeedRandom();

	// Setup the vector field
	printf("Setting up the vector field...\n");
	for (int x = 0; x < COLS; x++) {
		for (int y = 0; y < ROWS; y++) {
			double ang = ofMap(ofNoise(x * INC, y * INC, t), 0, 1, 0, TWO_PI * ANG_MULTIPLIER);
			grid[x][y].set(cos(ang), sin(ang));
		}
	}
	printf("Done!\n");

	// Setup the particles
	printf("Setting up the particles...\n");
	for (int i = 0; i < NUM; i++) {
		pos[i].set(ofRandomWidth(), ofRandomHeight());
		vel[i].set(ofRandom(-1, 1), ofRandom(-1, 1));
	}
	printf("Done!\n");

	// Disable background drawing and reduce opacity
	ofSetBackgroundAuto(false);
	//ofEnableAlphaBlending();



}
//--------------------------------------------------------------
void ofApp::update() {
	float initTime = ofGetElapsedTimef();
	// Set the previous position of the particles
	for (int i = 0; i < NUM; i++) {
		prev_pos[i] = pos[i];
	}

	// For each particle, update its velocity 
	for (int i = 0; i < NUM; i++) {
		// Get the vector at the particle's position
		ofVec2f vec = getVectorAt(pos[i]);

		// Add the force to the particle's velocity
		vel[i] += vec * FORCE_STRENGTH;

		// limit the velocity
		if (vel[i].length() > MAX_SPEED) {
			vel[i].normalize();
			vel[i] *= MAX_SPEED;
		}
	}

	// For each particle, update its position
	for (int i = 0; i < NUM; i++) {
		pos[i] += vel[i];
	}


	// If a particle goes off the screen, reset it
	for (int i = 0; i < NUM; i++) {
		if (pos[i].x < 0 || pos[i].x > ofGetWidth()) {
			pos[i].x = ofRandomWidth();
		}
		if (pos[i].y < 0 || pos[i].y > ofGetHeight()) {
			pos[i].y = ofRandomHeight();
		}
	}


	// Update the grid for next frame
	t += 0.001;
	if (UPDATE_GRID)
		updateGrid();
	if (LOGGING)
		printf("Processing time: %f\r", ofGetElapsedTimef() - initTime);
}

//--------------------------------------------------------------
void ofApp::draw() {
	float initTime = ofGetElapsedTimef();
	// drawGrid();
	// Draw a translucent background to slowly fade previous lines
	ofSetColor(0, 0, 0, 10);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	// Draw line from previous position to current position if the distance is less than half the width
	for (int i = 0; i < NUM; i++) {
		// Set the HSB color based on the velocity angle
		ofColor color = ofColor::fromHsb(ofMap(vel[i].angleRad(VEL_HEADING_REF), -PI, PI, 0, 255), 255, 255);
		ofSetColor(color, 100);
		ofVec2f diff = pos[i] - prev_pos[i];
		if (diff.length() <= MAX_SPEED + 0.1f) {
			ofDrawLine(prev_pos[i], pos[i]);
		}
	}
	//if (LOGGING)
	//	printf("Drawing time: %f\n", ofGetElapsedTimef() - initTime);
}