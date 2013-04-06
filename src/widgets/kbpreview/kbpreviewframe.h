/*
 *  Copyright (C) 2012 Shivam Makkar (amourphious1992@gmail.com)
 *                2013 Manuel Tortosa (manutortosa@chakra-project.org)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef KBPREVIEWFRAME_H
#define KBPREVIEWFRAME_H

#include "keyboardlayout.h"
#include "keysymhelper.h"
#include "keyaliases.h"

#include <QtGui/QPainter>
#include <QtGui/QFrame>

class KbPreviewFrame : public QFrame
{
    Q_OBJECT
    
private:
    void paintKey(QPainter &painter, int x, int y, int x2, int y2);
    void paintLabeledKey(QPainter &painter, int x, int y, int x2, int y2, QColor c, QString t);
    void paintBkspc(QPainter &painter, int x, int y, int x2, int y2);
    void paintTab(QPainter &painter, int x, int y, int x2, int y2);
    void paintCapsLck(QPainter &painter, int x, int y, int x2, int y2);
    void paintIntro(QPainter &painter,int x, int y, int x2, int y2);
    void paintShift(QPainter &painter,int x, int y, int x2, int y2);
    void paintTLDE(QPainter &painter, int &x, int &y);
    void paintAERow(QPainter &painter, int &x, int &y);
    void paintADRow(QPainter &painter, int &x, int &y);
    void paintACRow(QPainter &painter, int &x, int &y);
    void paintABRow(QPainter &painter, int &x, int &y);
    void paintBottomRow(QPainter &painter,int &x,int &y);

    KeySymHelper symbol;
    Aliases alias;
    KeyboardLayout keyboardLayout;

public:
    explicit KbPreviewFrame(QWidget *parent = 0);

    void paintEvent(QPaintEvent * event);
    void generateKeyboardLayout(const QString &country, const QString &layoutVariant);
    QString getLayoutName() const {
        return keyboardLayout.getLayoutName();
    }
};

#endif // KBPREVIEWFRAME_H 
