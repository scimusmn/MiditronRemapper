/*
 *  keyMapper.cpp
 *  MidiRemapper
 *
 *  Created by Exhibits on 4/5/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "mapKeyboard.h"

extern ofColor white;
extern ofColor black;
extern ofColor yellow;
extern ofColor gray;
extern ofColor blue;

remapKey::remapKey(double _w, double _h, char nt):pianoKey()
{
	note=nt;
	w=_w;
	h=_h;
	o_w=_w;
	o_h=_h;
	bSharp=false;
	bSelected=false;
	for (int j=0; j<3; j++) {
		int cur=notes.size();
		notes.push_back(instrument("holder",1,60));
		notes[cur].setDefault(true);
	}
	buttons.setup(2,25,OF_VERT,500);
	clearNotes.setup("Clear assigned instrument", "fonts/Arial.ttf", 24);
}

remapOctave::remapOctave(double width,char octave_begin_note):pianoOctave()
{
	w=width,h=width/1.2;
	double wwid=w/7.;
	int sharps[5]={1,3,6,8,10};
	for (int i=0; i<12; i++) {
		char nt=octave_begin_note+i;
		keys.push_back(remapKey(wwid,h,nt));
	}
	for (unsigned int i=0; i<5; i++) {
		keys[sharps[i]].setSharp(true);
	}
}

void remapKeyboard::setup(double wid,double nOctaves, unsigned char chan)
{
	w=wid;
	channel=chan;
	int numProgs=loadProgramNames("midiPrograms.ini");
	programs.setTextSize(20);
	for (unsigned int i=0; i<getProgramNames().size(); i++) {
		programs.setValue(getProgramNames()[i]);
	}
	programs.setMode(false);
	printOut.loadFont("fonts/DinC.ttf");
	printOut.setSize(24);
	printOut.setMode(OF_FONT_LEFT);
	printOut.setMode(OF_FONT_TOP);
	octaves.clear();
	for (int i=0; i<nOctaves; i++) {
		octaves.push_back(remapOctave(wid/nOctaves,i*12));
	}
	pressKey(0);
	framePad.y=wid/32;
	framePad.x=wid/32.;;
	w=w+framePad.x*2;
	h=octaves[0].h+framePad.y;
	clearMapped.setup("Clear all accoustic instruments", "fonts/Arial.ttf", 20);
}

void remapKeyboard::draw(double _x, double _y){
	pianoKeyboard::draw(_x,_y);
}

string getNoteName(unsigned char note)
{
	string ret;
	//string octave=ofToString((MIDI_KEYBOARD_START+note)/12);
	int k=note%12;
	ret=(k<2)?"C":(k<4)?"D":(k==4)?"E":(k<7)?"F":(k<9)?"G":(k<11)?"A":"B";
	if(k==1||k==3||k==6||k==8||k==10) ret+="#";
	return	ret;//+octave;
}



void remapKeyboard::drawKeyboardControls(int _x, int _y, int _w, int _h)
{
  ofPoint margin(50,50);
  ofPoint pad(20,20);
  
  //_-_-_-_-_ draw box to hold the controls
  ofSetShadowDarkness(.4);
	ofShadowRounded(_x,_y,_w,_h, 0, 10);
  ofSetColor(black.opacity(.3));
	ofRect(_x,_y,_w,_h);
  
  ofSetColor(gray.opacity(.3));
  drawHatching(_x, _y, _w, _h, 3, 3);
  
  //_-_-_-_-_ shadow the edges of the box
  ofSetShadowDarkness(.4);
  ofShade(_x, _y+_h/2, 3, _w, OF_DOWN,false);
  ofShade(_x, _y+_h/2, 3, _w, OF_UP);
  
  ofShade(_x, _y, 10, _w, OF_DOWN);
  ofShade(_x, _y+_h, 10, _w, OF_UP);
  ofShade(_x, _y, 10, _h, OF_RIGHT);
  ofShade(_x+_w, _y, 10, _h, OF_LEFT);
  
  _y-=3*pad.y/2;
  
  ofSetColor(255, 255, 255);
  printOut.setSize(24);
	printOut.setMode(OF_FONT_LEFT);
	printOut.setMode(OF_FONT_TOP);
	printOut.drawString("Clear assigned instruments from entire keyboard", _x+pad.x, _y+_h/5);
	ofSetColor(255, 255, 255);
	printOut.drawString("Select synthesized instrument for unassigned keys", _x+pad.x, _y+3*_h/5+pad.y);
	clearMapped.draw(_x+pad.x*2,_y+2*_h/5);
	programs.draw(_x+pad.x*2,_y+4*_h/5+pad.y);
}

void remapKeyboard::drawKeyInfo(int _x, int _y, int _w, int _h)
{
  ofPoint margin(25,25);
  ofPoint pad(20,15);
  
  ofRectangle box(_x+margin.x+pad.x, _y+margin.y, _w-margin.x*2-pad.x*2, _h-margin.y*2*2);
  
	pianoKey * key=0;
	for (unsigned int i=0; i<size(); i++) {
		if ((*this)[i].isSelected()) {
			key=&((*this)[i]);
		}
	}
	if(key){
		pianoKey & k=*key;
    
    printOut.setMode(OF_FONT_CENTER);
    printOut.setMode(OF_FONT_TOP);
    printOut.setSize(30);
    string line="Press this key to play ";
    if(!key->notes[0].isDefault()){
			line+=key->notes[0].title;
      //key->clearNotes.draw(box.x+(box.width-k.clearNotes.w)/2,box.y+30+printOut.stringHeight(line)+10);
		}
    else {
      line+=getNoteName(key->notes[0].base.note%12)+" on the ";
      line+=programs.getString();
    }
    
    //_-_-_-_-_ draw the shap of the info box
    ofFill();
    ofSetLineWidth(1);
		if(!k.notes[0].isDefault()){
      if(k.isSharp()) ofSetColor(k.notes[0].base.color-.2*255.);
      else ofSetColor(k.notes[0].base.color);
    }
    else if(!k.isSharp()) ofSetColor(white);
    else ofSetColor(black);
		ofBeginShape();
    ofVertex(key->x, key->y+key->h);
    ofSetCurveResolution(30);
    ofBezierVertex(k.x, k.y+k.h, k.x+(k.x-box.x)/20,box.y+(k.x-box.x)/20,box.x,box.y);
		ofVertex(box.x+box.width, box.y);
    ofBezierVertex(box.x+box.width,box.y, (k.x+k.w)+((k.x+k.w)-(box.x+box.width))/20,box.y-((k.x+k.w)-(box.x+box.width))/20,k.x+k.w, k.y+k.h);
		ofEndShape(true);
    ofFlat();
		ofRoundedRect(box.x-pad.x, box.y, box.width+pad.x*2, box.height, pad.x);
    /*ofNoFill();
    ofSetColor(yellow);
    ofSetLineWidth(4);
    ofRoundedRect(box.x-pad.x, box.y, box.width+pad.x*2, box.height, pad.x);
    ofFill();*/
    

    if(!k.notes[0].isDefault()||k.isSharp()) ofSetColor(white);
    else if(!k.isSharp()) ofSetColor(black);
    printOut.drawString(line, box.x+box.width/2, box.y+box.height/6);
    
    if(!key->notes[0].isDefault()){
      key->clearNotes.draw(box.x+(box.width-k.clearNotes.w)/2,box.y+box.height*2/6+printOut.stringHeight(line));
		}
	}
}

bool remapKeyboard::clickDown(int _x, int _y)
{
	bool ret=false;
	if(programs.clickDown(_x, _y)){
		if(programs.isSelected()){
			vector<unsigned char> msg;
			msg.push_back(MIDI_PROGRAM_CHANGE+channel);
			msg.push_back(programs.getChoiceNumber());
			ofGetAppPtr()->midiToSend(msg);
		}
	}
	else if(programs.justSelected()){
		vector<unsigned char> msg;
		msg.push_back(MIDI_PROGRAM_CHANGE+channel);
		msg.push_back(programs.getChoiceNumber());
		ofGetAppPtr()->midiToSend(msg);
	}
	else if(clearMapped.clickDown(_x, _y)){
		for (unsigned int i=0; i<(*this).size(); i++) {
			(*this)[i].resetInstruments();
			selectButton(i,0);
		}
	}
	else if(ret=pianoKeyboard::clickDown(_x,_y)){
    if(getKey().notes[0].isDefault()) getKey().notes[0].base.note=getKey().getNote()+MIDI_KEYBOARD_START;
    getKey().notes[0].play();
  }
	return ret;
}

bool remapKeyboard::clickUp()
{
	programs.clickUp();
	clearMapped.clickUp();
	if(pianoKeyboard::clickUp())
    getKey().notes[0].stop();
  
}

int remapKeyboard::getButtonChoice(int num)
{
	if(num<0) num=activeKey();
	return (*this)[num].buttons.getChoice();
}

vector<instrument> & remapKeyboard::getActiveNotes()
{
	return getKey().notes;
}

vector<instrument> & remapKeyboard::getNotes(int num)
{
	return (*this)[num].notes;
}
