#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofExtended.h"
//#include "MyMidiListener.h"
#include "ofxMidiIn.h"
#include "ofxMidiOut.h"

#include "keyboard.h"
#include "remapper/mapKeyboard.h"
#include "instrument.h"
#include "band.h"
#include "remapper/keyRemapper.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	void midiReceived(double deltatime, std::vector< unsigned char > *message, int port);
	void midiToSend(vector<unsigned char> message);

	ofxMidiIn keyboard;
	ofxMidiOut midiOut;
	
	remapKeyboard kb;
	
	bandBar band;
	ofScrollBar scroll;
	ofScrollBar scrollVert;
	
	remapper rmp;
	
	
	
	int size;
	int pos;
	bool typed;
  
  titleBar title;
};

#endif
