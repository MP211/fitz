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

template<>
void DrawItemT<FitzText>::setSize( float size )
{
    mCairoContext.setFontSize( size );
    
    cairo::TextExtents te = mCairoContext.textExtents( mText );    
    mOBB = OBB( Rectf( 0.0, 0.0, te.width(), te.height() ), 0.0f );
};

template<>
void DrawItemT<FitzImage>::setSize( float size )
{
    mCairoContext.scale( size/mCairoBuffer.getWidth(), size/mCairoBuffer.getHeight() );
    
    mOBB = OBB( Rectf( 0.0, 0.0, mImage->getWidth(), mImage->getHeight() ), 0.0f );
};

template<>
void DrawItemT<FitzText>::setPosition( Vec2f position )
{
    registerPosition( position );    
    
    mCairoContext.moveTo( mCurrPos );
    
    mOBB.moveTo( mCurrPos - Vec2f( 0.0f, mOBB.getHeight()) );     
};

template<>
void DrawItemT<FitzImage>::setPosition( Vec2f position )
{
    registerPosition( position );
    
    mCairoContext.translate( position );
};

template<>
void DrawItemT<FitzText>::draw()
{
    mCairoContext.textPath( mText );
    
    copyCairoBuffer();
    
    gl::enableAlphaBlending();
    gl::pushMatrices();
    
    gl::color( mColor );
    gl::draw( mTexture );
    
    gl::disableAlphaBlending();
    
#ifdef DEBUG
    drawBoundingBox();
#endif
};

template<>
void DrawItemT<FitzImage>::draw()
{
    cairo::SurfaceImage buf = cairo::SurfaceImage( mImage );
    mCairoContext.setSourceSurface( buf, 0, 0 );    
    mCairoContext.paint();
    
    copyCairoBuffer();
    
    gl::enableAlphaBlending();
    gl::pushMatrices();
    
    gl::color( ColorA::gray(1.0f) );
    gl::draw( mTexture );
    
    gl::disableAlphaBlending();
    
#ifdef DEBUG
    drawBoundingBox();
#endif    
};

template<>
DrawItemT<FitzText>::DrawItemT( const string text, const Font font ) :
    mText( text ), mFont( font ), mCurrPos( Vec2f::zero() )
{
    mColor = ColorA( 1.0f, Rand::randFloat(), 0.0f, 1.0f );
    
    initCairoBuffer();
    
    mCairoContext.setFont( mFont );
    mCairoContext.setAntiAlias( cairo::ANTIALIAS_SUBPIXEL );
    mCairoContext.setSource( mColor );
};

template<>
DrawItemT<FitzImage>::DrawItemT( const string resource ) :
    mResource( resource ), mCurrPos( Vec2f::zero() )
{
    initCairoBuffer();
    
    mImage = loadImage( loadAsset(mResource) );
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
    
    mPosPath.push_back( mLastPos );
};

template<typename T>
void DrawItemT<T>::initCairoBuffer()
{
    mCairoBuffer    = cairo::SurfaceImage( getWindowWidth(), getWindowHeight(), true );
    mCairoContext   = cairo::Context( mCairoBuffer );
};

template<typename T>
void DrawItemT<T>::copyCairoBuffer()
{
    cairo::SurfaceImage buf( getWindowWidth(), getWindowHeight(), true );
    cairo::Context      ctx( buf );
    
    ctx.copySurface( mCairoBuffer, mCairoBuffer.getBounds() );
    mTexture = gl::Texture( buf.getSurface() );
    mCairoContext.fill();
};

#ifdef DEBUG
template<typename T>
void DrawItemT<T>::drawBoundingBox()
{
    gl::enableAlphaBlending();
    gl::pushMatrices();
    
    gl::color( ColorA( 0.0f, 1.0f, 0.0f, 1.0f ));
    gl::draw( mOBB.getPath() );
    
    gl::disableAlphaBlending();    
};
#endif

template class DrawItemT<FitzText>;
template class DrawItemT<FitzImage>;
    
}; // fitz

