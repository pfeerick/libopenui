/*
 * Copyright (C) OpenTX
 *
 * Source:
 *  https://github.com/opentx/libopenui
 *
 * This file is a part of libopenui library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#ifndef _BUFFERED_WINDOW_H_
#define _BUFFERED_WINDOW_H_

#include "window.h"

template <class T>
class BufferedWindow: public T
{
  public:
    using T::T;

    ~BufferedWindow() override
    {
      delete bitmap;
    }

    void forcePaint()
    {
      paintUpdateNeeded = true;
      T::invalidate();
    }

  protected:
    BitmapBuffer * bitmap = nullptr;
    bool paintUpdateNeeded = false;
    virtual void paintUpdate(BitmapBuffer * dc) = 0;
};

template <class T>
class OpaqueBufferedWindow: public BufferedWindow<T>
{
  public:
    using BufferedWindow<T>::BufferedWindow;

    void paint(BitmapBuffer * dc) override
    {
      if (!this->bitmap) {
        this->bitmap = new BitmapBuffer(BMP_RGB565, T::width(), T::height());
        this->paintUpdateNeeded = true;
      }

      if (this->paintUpdateNeeded) {
        this->paintUpdate(this->bitmap);
        this->paintUpdateNeeded = false;
      }

      dc->drawBitmap(0, 0, this->bitmap);
    }
};

template <class T>
class TransparentBufferedWindow: public BufferedWindow<T>
{
  public:
    using BufferedWindow<T>::BufferedWindow;

    void paint(BitmapBuffer * dc) override
    {
      if (!BufferedWindow<T>::bitmap) {
        this->bitmap = new BitmapBuffer(BMP_RGB565, T::width(), T::height());
        this->paintUpdateNeeded = true;
      }

      if (this->paintUpdateNeeded) {
        this->paintUpdate(dc);
        this->bitmap->drawBitmap(0, 0, dc, dc->getOffsetX(), dc->getOffsetY());
        this->paintUpdateNeeded = false;
      }
      else {
        dc->drawBitmap(0, 0, this->bitmap);
      }
    }
};

#endif // _BUFFERED_WINDOW_H_
