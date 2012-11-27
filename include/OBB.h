//
//  OBB.h
//  Fitz
//
//  Created by Mp on 11/26/12.
//  Copyright (c) 2012 Mp. All rights reserved.
//
//  Credit to Morgan McGuire morgan@cs.brown.edu via flipcode for the core of this class.

#pragma once

#ifndef Fitz_OBB_h
#define Fitz_OBB_h

#include "cinder/app/AppBasic.h"

namespace fitz {

class OBB
{
public:
    OBB() {};

    template<typename T>
    OBB( const cinder::AreaT<T> &a, float angle ) :
        mAngle( angle ), mWidth( a.getWidth() ), mHeight( a.getHeight() )
    {
        mCentroid = cinder::Vec2f( (a.x1 + a.x2) / 2, (a.y1 + a.y2) / 2 );
        
        orient( mWidth, mHeight, mAngle );
    };

    template<typename T>
    OBB( const cinder::RectT<T> &r, float angle ) :
        mAngle( angle ), mWidth( r.getWidth() ), mHeight( r.getHeight() )
    {
        mCentroid = cinder::Vec2f( (r.x1 + r.x2) / 2.0f, (r.y1 + r.y2) / 2.0f );
        
        orient( mWidth, mHeight, mAngle );
    };
    
    float getWidth() const
    {
        return mWidth;
    };
    
    float getHeight() const
    {
        return mHeight;
    };
    
    cinder::Vec2f getSize() const
    {
        return cinder::Vec2f( mWidth, mHeight );
    };
    
    cinder::Vec2f getExtents() const
    {
        return cinder::Vec2f( mHalfWidth, mHalfHeight );
    };
    
    cinder::Rectf getAAB() const
    {        
        return mBounds;
    };
    
    cinder::Vec2f getUL() const
    {
        return mCorners[0];        
    };
    
    cinder::Vec2f getLR() const
    {
        return mCorners[2];
    };
    
    cinder::Vec2f getCentroid() const
    {
        return mCentroid;
    };
    
    cinder::Vec2f getPosition() const
    {
        return getUL();
    };
    
    float getX1() const
    {
        return mCorners[0].x;
    };
    
    float getY1() const
    {
        return mCorners[0].y;
    };
    
    float getX2() const
    {
        return mCorners[2].x;
    };
    
    float getY2() const
    {
        return mCorners[2].y;
    };
    
    cinder::PolyLine2f getPath() const
    {
        cinder::PolyLine2f line;
        
        for( int c = 0; c < 5; ++c ) {
            line.push_back( mCorners[c & 3] );
        }
        line.setClosed( true );
        
        return line;
    };
    
    void moveTo( const cinder::Vec2f &position )
    {
        offset( position - mCorners[0] );
    };
    
    void scale( float dx, float dy )
    {
        mWidth *= dx;
        mHeight *= dy;
        
        orient( mWidth, mHeight, mAngle );
    };
    
    friend std::ostream& operator<<( std::ostream &o, const OBB &box )
    {
        return o << "(" << box.getX1() << ", " << box.getY1() << ")-(" << box.getX2() << ", " << box.getY2() << ")";
    };
        
private:
    void orient( float w, float h, float angle )
    {
        cinder::Vec2f X( cos(angle), sin(angle) );
        cinder::Vec2f Y( -sin(angle), cos(angle) );
        
        X *= w / 2.0f;
        Y *= h / 2.0f;
        
        mCorners[0] = mCentroid - X - Y; // ul
        mCorners[1] = mCentroid + X - Y; // ur
        mCorners[2] = mCentroid + X + Y; // lr
        mCorners[3] = mCentroid - X + Y; // ll
        
        mHalfWidth  = X.x;
        mHalfHeight = Y.y;
        
        compute();        
    };

    // Updates the axis, center and corner conveniences by position (assumes corners form a rectangle).
    // Call after corners move to calc OBB.
    //
    void compute()
    {
        x1 = mCorners[0].x;
        y1 = mCorners[0].y;
        x2 = mCorners[2].x;
        y2 = mCorners[2].y;
        
        for ( int i = 0; i < 4; ++i ) {
            ax1 = std::min( mCorners[i].x, ax1 );
            ay1 = std::min( mCorners[i].y, ay1 );
            ax2 = std::max( mCorners[i].x, ax2 );
            ay2 = std::max( mCorners[i].y, ay2 );
        }
        mBounds = cinder::Rectf( ax1, ay1, ax2, ay2 );
        
        mAxis[0] = mCorners[1] - mCorners[0]; 
        mAxis[1] = mCorners[3] - mCorners[0];
        
        // Make the length of each axis 1/edge length so we know any
        // dot product must be less than 1 to fall within the edge.        
        for ( int a = 0; a < 2; ++a ) {
            mAxis[a]    /= mAxis[a].lengthSquared();
            mOrigin[a]  =  mCorners[0].dot( mAxis[a] );
        }
        
        mCentroid = ( mCorners[0] + mCorners[1] + mCorners[2] + mCorners[3] ) / 4;
    };
    
    void offset( const cinder::Vec2f &translation )
    {
        for (int c = 0; c < 4; ++c) {
            mCorners[c] += translation;
        }
        
        compute();
    }    
    
    // Corners
    float x1, y1, x2, y2, ax1, ay1, ax2, ay2;
    
    // Axis-aligned bounding box
    cinder::Rectf mBounds;
    
    // Axis-aligned dimensions
    float mWidth, mHeight;
    
    // Oriented half dimensions
    float mHalfWidth, mHalfHeight;
    
    // Rotation
    float mAngle;
    
    // Center of the OBB
    cinder::Vec2f mCentroid;
    
    // Corners, where 0 is the lower left
    cinder::Vec2f mCorners[4];
    
    // Two edges extended away from lower left
    cinder::Vec2f mAxis[2];
        
    // mOrigin[a] = mCorners[0].dot( mAxis[a] )
    float mOrigin[2];
};

}; // fitz

#endif
