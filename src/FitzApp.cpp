//
//  Fitz.cpp
//  Fitz
//
//  Created by Mp on 11/20/12.
//  Copyright (c) 2012 Mp. All rights reserved.
//

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "DrawItemManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace fitz;

class FitzApp : public AppBasic {
public:
    void prepareSettings( Settings* settings );
    void setup();
    void update();
    void draw();
    
private:
    DrawItemManager mDrawItemManager;
};

void FitzApp::prepareSettings( Settings* settings )
{
    settings->setWindowSize(1024, 768);
    //settings->setFullScreen(true);
    settings->setResizable(false);
};

void FitzApp::setup()
{
    mDrawItemManager = DrawItemManager( Rectf( getWindowBounds() ));
    
    // TMP
    const int       wordsCount          = 4;
    const string    words[wordsCount]   = { "John", "Paul", "Ringo", "George" };    
    for ( const string *word = &words[0]; word != &words[wordsCount]; ++word ) {
        mDrawItemManager.addTextItem( *word );
    }
    
    mDrawItemManager.setup();
};

void FitzApp::update()
{
    mDrawItemManager.update();
};

void FitzApp::draw()
{
    gl::setMatricesWindow( getWindowSize(), true );
    gl::clear( Color(0,0,0) );
    
    mDrawItemManager.draw();
};


CINDER_APP_BASIC( FitzApp, RendererGl )