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

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/Font.h"
#include "cinder/cairo/Cairo.h"

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
    
    // Common
    void update();
    void draw();
    void setPosition( cinder::Vec2f position );
    void setSize( float size );
    
protected:
    // Common
    cinder::Vec2f               mCurrPos;
    cinder::Vec2f               mLastPos;
    cinder::ColorA              mColor;
    cinder::gl::Texture         mTexture;
    cinder::PolyLine2f          mPosPath;
    
private:
    // FitzText
    cinder::Font                mFont;
    std::string                 mText;
    cinder::cairo::Context      mCairoOffsContext;
    cinder::cairo::SurfaceImage mCairoOffsSurface;
    cinder::cairo::TextExtents  mCairoExtents;
    
    // FitzImage
    std::string                 mResource;
    
    // Common
    void registerPosition( cinder::Vec2f position );

#ifdef DEBUG
    void drawDebugBoundingBox();
#endif
};

};

#endif
