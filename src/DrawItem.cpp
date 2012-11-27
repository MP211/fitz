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
    cairo::TextExtents te1 = mCairoContext.textExtents( mText );
    
    mCairoContext.setFontSize( size );
    
    cairo::TextExtents te2 = mCairoContext.textExtents( mText );
    
    float dx = te2.width()/te1.width(), dy = te2.height()/te1.height();    
    mOBB.scale( dx, dy );
};

template<>
void DrawItemT<FitzImage>::setSize( float size )
{
    float dx = size/mImageWidth, dy = size/mImageHeight;

    mCairoContext.scale( dx, dy );
    
    mImageWidth *= dx;
    mImageHeight *= dy;

    mOBB.scale( dx, dy );
};

template<>
void DrawItemT<FitzText>::setPosition( Vec2f position )
{
    // Offset for upper-left positioning for parity with images
    mCairoContext.moveTo( position + Vec2f( 0.0f, mOBB.getHeight()) );
    
    mOBB.moveTo( position );
    
    registerPosition( mCurrPos );
};

template<>
void DrawItemT<FitzImage>::setPosition( Vec2f position )
{
    mCairoContext.translate( position - mCurrPos );
    
    mOBB.moveTo( position );
    
    registerPosition( mCurrPos );
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
    cairo::SurfaceImage buf = cairo::SurfaceImage( mImageSource );
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
    
    cairo::TextExtents te = mCairoContext.textExtents( mText );    
    mOBB = OBB( Rectf( 0.0, 0.0, te.width(), te.height() ), 0.0f );
};

template<>
DrawItemT<FitzImage>::DrawItemT( const string resource ) :
    mResource( resource ), mCurrPos( Vec2f::zero() )
{
    initCairoBuffer();
    
    mImageSource    = loadImage( loadAsset(mResource) );
    mImageWidth     = mImageSource->getWidth();
    mImageHeight    = mImageSource->getHeight();
    
    mOBB = OBB( Rectf( 0.0f, 0.0f, mImageWidth, mImageHeight ), 0.0f );
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
    gl::drawSolidCircle( mOBB.getCentroid(), 4.0f );
    gl::drawSolidCircle( mOBB.getUL(), 2.0f );
    
    gl::disableAlphaBlending();    
};
#endif

template class DrawItemT<FitzText>;
template class DrawItemT<FitzImage>;
    
}; // fitz

