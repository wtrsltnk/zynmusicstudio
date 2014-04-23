/*
  ZynAddSubFX - a software synthesizer

  zynapplication.h - The QApplication for running zyn music studio
  Copyright (C) 2014 Wouter Saaltink
  Author: Wouter Saaltink

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/
#ifndef ZYNAPPLICATION_H
#define ZYNAPPLICATION_H

#include <QApplication>

class ZynApplication : public QApplication
{
public:
    explicit ZynApplication(int argc, char *argv[]);
    virtual ~ZynApplication();

    virtual int exec();

};

#endif // ZYNAPPLICATION_H
