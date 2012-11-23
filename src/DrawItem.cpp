//
//  DrawItem.cpp
//  Fitz
//
//  Created by Mp on 11/21/12.
//  Copyright (c) 2012 Mp. All rights reserved.
//

#include "DrawItem.h"

#include "cinder/gl/gl.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace fitz {

template<>
DrawItemT<FitzText>::DrawItemT( const string text, const Font font ) :
    mText( text ), mFont( font )
{
    mColor = ColorA( 1.0f, Rand::randFloat(), 0.0f, 1.0f );
        
    int bW = getWindowWidth(), bH = getWindowHeight();
    cairo::SurfaceImage surface( bW, bH, true );
    cairo::Context ctx( surface );
    ctx.setFont( mFont );
    ctx.setFontSize( Rand::randInt( 76 ) );
    ctx.setAntiAlias( cairo::ANTIALIAS_DEFAULT );
    ctx.setSource( mColor );
    cairo::TextExtents te = ctx.textExtents( mText );
    ctx.moveTo( Rand::randFloat( bW ) - te.width(), Rand::randFloat( bH ) - te.height() );
    ctx.showText( mText );
    mTexture = gl::Texture( surface.getSurface());
}

template<>
DrawItemT<FitzImage>::DrawItemT( const string resource ) :
    mResource( resource )
{
}

template<typename T>
void DrawItemT<T>::update()
{
}

template<>
void DrawItemT<FitzText>::draw()
{
    gl::enableAlphaBlending();
    gl::pushMatrices();
  
    gl::color( mColor );
    gl::draw( mTexture );
    
    gl::disableAlphaBlending();
}

template<>
void DrawItemT<FitzImage>::draw()
{
}

template class DrawItemT<FitzText>;
template class DrawItemT<FitzImage>;
    
};