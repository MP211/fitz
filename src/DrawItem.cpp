//
//  DrawItem.cpp
//  Fitz
//
//  Created by Mp on 11/21/12.
//  Copyright (c) 2012 Mp. All rights reserved.
//

#include "DrawItem.h"

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace fitz {

#ifdef DEBUG
template<>
void DrawItemT<FitzText>::drawDebugBoundingBox()
{
    gl::enableAlphaBlending();
    gl::pushMatrices();
    
    gl::color( ColorA( 0.0f, 1.0f, 0.0f, 1.0f ));
    gl::draw( mOBB.getPath() );
    
    gl::disableAlphaBlending();    
};
#endif

template<>
void DrawItemT<FitzText>::setSize( float size ) // font size
{
    mCairoOffsContext.setFontSize( size );
    
    cairo::TextExtents te = mCairoOffsContext.textExtents( mText );    
    mOBB = OBB( Rectf( 0.0, 0.0, te.width(), te.height() ), 0.0f );
};

template<>
void DrawItemT<FitzImage>::setSize( float size ) // scale factor
{
};

template<>
void DrawItemT<FitzText>::setPosition( Vec2f position )
{
    registerPosition( position );    
    
    mCairoOffsContext.moveTo( mCurrPos );
};

template<>
void DrawItemT<FitzImage>::setPosition( Vec2f position )
{
    registerPosition( position );
};

template<>
void DrawItemT<FitzText>::draw()
{
    mCairoOffsContext.textPath( mText );
    
    cairo::SurfaceImage buf( getWindowWidth(), getWindowHeight(), true );
    cairo::Context      ctx( buf );
    ctx.copySurface( mCairoOffsSurface, mCairoOffsSurface.getBounds() );
    mTexture = gl::Texture( buf.getSurface() );
    mCairoOffsContext.fill();
    
    gl::enableAlphaBlending();
    gl::pushMatrices();
    
    gl::color( mColor );
    gl::draw( mTexture );
    
    gl::disableAlphaBlending();
    
#ifdef DEBUG
    drawDebugBoundingBox();
#endif
};

template<>
void DrawItemT<FitzImage>::draw()
{
};

template<>
DrawItemT<FitzText>::DrawItemT( const string text, const Font font ) :
    mText( text ), mFont( font ), mCurrPos( Vec2f::zero() )
{
    mColor = ColorA( 1.0f, Rand::randFloat(), 0.0f, 1.0f );
    
    mCairoOffsSurface = cairo::SurfaceImage( getWindowWidth(), getWindowHeight(), true );
    mCairoOffsContext = cairo::Context( mCairoOffsSurface );
    
    mCairoOffsContext.setFont( mFont );
    mCairoOffsContext.setAntiAlias( cairo::ANTIALIAS_SUBPIXEL );
    mCairoOffsContext.setSource( mColor );
};

template<>
DrawItemT<FitzImage>::DrawItemT( const string resource ) :
    mResource( resource ), mCurrPos( Vec2f::zero() )
{
};

template<typename T>
void DrawItemT<T>::update()
{
};

template<typename T>
void DrawItemT<T>::registerPosition( Vec2f position )
{
    mLastPos = mCurrPos;
    mCurrPos = position;
    
    mOBB.moveTo( mCurrPos - Vec2f( 0.0f, mOBB.getHeight()) );
    
    mPosPath.push_back( mLastPos );
};

template class DrawItemT<FitzText>;
template class DrawItemT<FitzImage>;
    
}; // fitz

