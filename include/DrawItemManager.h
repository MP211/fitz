//
//  DrawItemManager.h
//  Fitz
//
//  Created by Mp on 11/21/12.
//  Copyright (c) 2012 Mp. All rights reserved.
//

#pragma once

#ifndef Fitz_DrawItemManager_h
#define Fitz_DrawItemManager_h

#include "cinder/Font.h"

#include "boost/variant.hpp"

#include "DrawItem.h"

namespace fitz {

typedef boost::variant< DrawItemT<FitzText>, DrawItemT<FitzImage> > DrawItemType;

class DrawItemManager
{
public:
    DrawItemManager();
    DrawItemManager( cinder::Rectf drawBounds );
    
    void setup();
    void update();
    void draw();
    void addTextItem( const std::string text );
    void addImageItem( const std::string resource );

private:
    cinder::Rectf               mDrawBounds;
    std::list<DrawItemType>     mDrawItems;
    std::vector<cinder::Font>   mFonts;
   
#ifdef DEBUG
    void drawDebugPre();
    void drawDebugPost();
#endif
};
    
};

#endif
