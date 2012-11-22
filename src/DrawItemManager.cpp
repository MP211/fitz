//
//  DrawItemManager.cpp
//  Fitz
//
//  Created by Mp on 11/21/12.
//  Copyright (c) 2012 Mp. All rights reserved.
//

#include "DrawItemManager.h"

#include "cinder/app/AppBasic.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace fitz {

DrawItemManager::DrawItemManager()
{
};

DrawItemManager::DrawItemManager( cinder::Rectf drawBounds ) :
    mDrawBounds( drawBounds )
{
    mDrawBounds.scaleCentered( 0.75f );
    
    mFonts.push_back( Font( loadResource("PUSAB___.otf"), 12.0f ));
};

void DrawItemManager::setup()
{
    list<DrawItemType>::iterator it = mDrawItems.begin();
    
    while( it != mDrawItems.end() ) {
        ++it;
    }
};

void DrawItemManager::update()
{
    list<DrawItemType>::iterator it = mDrawItems.begin();
    
    while( it != mDrawItems.end() ) {
        ++it;
    }
};

void DrawItemManager::draw()
{
    list<DrawItemType>::iterator it = mDrawItems.begin();
    
    while( it != mDrawItems.end() ) {
        if ( DrawItemT<FitzText> *p = boost::get<DrawItemT<FitzText>>( &(*it) )) {
            p->draw();
        }
        else if ( DrawItemT<FitzImage> *p = boost::get<DrawItemT<FitzImage>>( &(*it) )) {
            p->draw();
        }
        else {
        // Type not implemented.
        }
        
        ++it;
    }
};

void DrawItemManager::addTextItem( const string text )
{
    mDrawItems.push_back( DrawItemT<FitzText>( text, mFonts[0] ));
};

void DrawItemManager::addImageItem( const string resource )
{
    mDrawItems.push_back( DrawItemT<FitzImage>( resource ));
};

};