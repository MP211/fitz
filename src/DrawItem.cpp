//
//  DrawItem.cpp
//  Fitz
//
//  Created by Mp on 11/21/12.
//  Copyright (c) 2012 Mp. All rights reserved.
//

#include "DrawItem.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace fitz {

template<>
DrawItemT<FitzText>::DrawItemT( const string text, const Font font ) :
    mText( text ), mFont( font )
{
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
}

template<>
void DrawItemT<FitzImage>::draw()
{
}

template class DrawItemT<FitzText>;
template class DrawItemT<FitzImage>;
    
};