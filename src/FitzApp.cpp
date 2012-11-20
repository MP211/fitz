//
//  Fitz.cpp
//  Fitz
//
//  Created by Mp on 11/20/12.
//  Copyright (c) 2012 Mp. All rights reserved.
//

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class FitzApp : public AppBasic {
public:
    void prepareSettings( Settings* settings );
    void setup();
    void update();
    void draw();
};

void FitzApp::prepareSettings( Settings* settings )
{
}

void FitzApp::setup()
{
}

void FitzApp::update()
{
}

void FitzApp::draw()
{
}

CINDER_APP_BASIC( FitzApp, RendererGl )