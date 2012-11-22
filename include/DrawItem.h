//
//  DrawItem.h
//  Fitz
//
//  Created by Mp on 11/21/12.
//  Copyright (c) 2012 Mp. All rights reserved.
//

#pragma once

#ifndef Fitz_DrawItem_h
#define Fitz_DrawItem_h

#include "cinder/Font.h"

#include "boost/variant.hpp"

namespace fitz {

struct FitzText;
struct FitzImage;

template<typename T>
class DrawItemT
{
public:
    DrawItemT();
    
    // FitzText
    DrawItemT( const std::string text, const cinder::Font font );
    
    // FitzImage
    DrawItemT( const std::string resource );
    
    void update();
    void draw();
    
private:
    // FitzText
    cinder::Font    mFont;
    std::string     mText;
    
    // FitzImage
    std::string     mResource;
};

};

#endif
