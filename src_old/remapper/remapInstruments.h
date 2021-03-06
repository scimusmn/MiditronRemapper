/*
 *  remapInstruments.h
 *  MidiRemapper
 *
 *  Created by Exhibits on 4/5/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "instrument.h"
#include "band.h"

#define as_remapInst(x) static_cast<remapInst *>(x)

class remapInst : public instrument{
protected:
public:
  int point;
	remapInst():instrument(){}
	remapInst(string title, unsigned char channel,unsigned char note);
	void operator=(const instrument & t);
	void mouseMotion(double _x, double _y);
	void draw(double _x, double _y);
	bool clickDown(double _x, double _y);
	//bool clickUp(vector<instrument> & compare);  //deprecated
	bool clickUp();
	void drawBackground();
	//bool over(instrument & compare, int i);
};

class remapBand : public bandBar{
protected:
	//vector<remapInst> rInstruments;
  ofPoint margin;
  ofPoint binPad;
public:
	remapBand():bandBar(){}
  void configureSize();
	void setup(xmlParse * config, double hgt=0);
	void draw(double _x, double _y);
	void drawDraggedBlocks();
	//bool clickUp(vector<instrument> & comp);
	bool clickUp();
	bool clickDown(int _x, int _y);
	bool mouseMotion(int _x, int _y);
	remapInst & lastInstrument()
	{
		return *(as_remapInst(instruments[lastInst]));
	}
};