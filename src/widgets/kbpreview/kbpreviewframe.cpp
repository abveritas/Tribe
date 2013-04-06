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


#include <QtCore/QFile>
#include <QtGui/QFont>
#include <QtGui/QPen>
#include <QDebug>

#include <KApplication>
#include <KLocale>

#include "kbpreviewframe.h"

static const int sz = 20, kszx = 48, kszy = 48, bsz = 10, xso = 7, xsp = 4, levels = 4;
static const int strtx = 0, strty = 0, endx = 767, endy = (kszy * 5) + (xso * 4) + (bsz * 2);
static const int xOffset[] = { 10, 10, 28, 28 };
static const int yOffset[] = { 28, 8, 28, 8 };
static const QColor keyBorderColor("#d4d4d4");
static const QColor lev12color("#d4d4d4");
static const QColor lev34color("#32adf1");
static const QColor color[] = { lev12color, lev12color, lev34color, lev34color };
static const QFont upperfont("Helvetica", 8);
static const QFont lowerfont("Helvetica", 10, QFont::DemiBold);
static const QFont signfont[] = { lowerfont, upperfont, lowerfont, upperfont };
static const QFont keyfont[] = { upperfont, lowerfont, upperfont, lowerfont };
static const QPen kpen(lev12color, 2);

// TODO: parse geometry files and display keyboard according to current keyboard model
//       also the layout should be resizable, some progress is already done.

KbPreviewFrame::KbPreviewFrame(QWidget *parent) :
    QFrame(parent)
{
    setFrameStyle( QFrame::NoFrame );
}

void KbPreviewFrame::paintKey(QPainter &painter, int x, int y, int x2, int y2)
{
    const int rx = 6;
    
    QRect rr(x, y, x2, y2);
    
    painter.setPen(keyBorderColor);
    painter.drawRoundedRect(rr, rx, rx);
}

void KbPreviewFrame::paintLabeledKey(QPainter& painter, int x, int y, int x2, int y2, QColor c, QString t)
{
    paintKey(painter, x, y, x2, y2);
    
    painter.setPen(c);
    QRectF cnt(x, y, x2, y2);
    painter.drawText(cnt, Qt::AlignCenter, t);
}

void KbPreviewFrame::paintBkspc(QPainter &painter,int x,int y, int x2, int y2)
{
    const int bksz = 6, iko = 3;
    const int ikx = x + 15;
    const int iky = y + (y2 / 2) + (bksz - (bksz / 4)) - iko;
    
    paintKey(painter, x, y, x2, y2);
  
    QPainterPath ik;
        ik.moveTo(ikx, iky);
        ik.lineTo(ikx + bksz, iky - iko);
        ik.lineTo(ikx + bksz, iky + iko);
        ik.lineTo(ikx, iky);
        ik.lineTo(ikx + (bksz * 4), iky);

    painter.setPen(kpen);	
    painter.drawPath(ik);
}

void KbPreviewFrame::paintTab(QPainter& painter, int x, int y, int x2, int y2)
{
    const int tbsz = 6, tbo = 3;
    const int tbx = x + 15;
    const int tby = y + 17, tby2 = y + 32;
    
    paintKey(painter, x, y, x2, y2);
    
    QPainterPath a1;
        a1.moveTo(tbx - 1, tby - tbo - 1);
        a1.lineTo(tbx - 1, tby + tbo + 1);
        a1.moveTo(tbx, tby);
        a1.lineTo(tbx + tbsz, tby - tbo);
        a1.lineTo(tbx + tbsz, tby + tbo);
        a1.lineTo(tbx, tby);
        a1.lineTo(tbx + (tbsz * 3), tby);
    
    QPainterPath a2;
        a2.moveTo(tbx - 1, tby2);
        a2.lineTo(tbx - 1 + (tbsz * 3), tby2);
        a2.lineTo(tbx + (tbsz * 2), tby2 - tbo);
	a2.lineTo(tbx + (tbsz * 2), tby2 + tbo);
	a2.lineTo(tbx + (tbsz * 3), tby2);
        a2.moveTo(tbx + (tbsz * 3), tby2 - tbo - 1);
        a2.lineTo(tbx + (tbsz * 3), tby2 + tbo + 1);

    painter.setPen(kpen);
    painter.drawPath(a1);
    painter.drawPath(a2);
}

void KbPreviewFrame::paintIntro(QPainter &painter,int x,int y, int x2, int y2)
{
    const int iksz = 6, sko = 3;
    const int ikx = x + 15;
    const int iky = y + (y2 / 2) + (iksz - (iksz / 4)) - sko;
    
    paintKey(painter, x, y, x2, y2);
    
    QPainterPath ik;
        ik.moveTo(ikx, iky);
        ik.lineTo(ikx + iksz, iky - sko);
        ik.lineTo(ikx + iksz, iky + sko);
        ik.lineTo(ikx, iky);
        ik.lineTo(ikx + (iksz * 4), iky);
        ik.lineTo(ikx + (iksz * 4), iky - iksz);

    painter.setPen(kpen);
    painter.drawPath(ik);
}

void KbPreviewFrame::paintCapsLck(QPainter& painter, int x, int y, int x2, int y2)
{
    const int clsz = 8;
    const int clx = ((x2 - x) / 2) + (clsz + (clsz / 2));
    const int cly = ((y + y2) - (y2 / 2)) + clsz;
    const int startangle = (30 * 16), spawnangle = (120 * 16);
    
    QRect clsq(clx - (clsz / 2), cly-clsz - (clsz / 2), (clsz * 2), (clsz * 2)); 
    QRectF clsqa(clx - (clsz / 2), (cly - (clsz * 2)), (clsz * 2), (clsz * 2)); 
	
    paintKey(painter, x, y, x2, y2);
    
    painter.setPen(kpen);
    painter.drawRect(clsq);
    painter.drawArc(clsqa, startangle, spawnangle);
    painter.drawText(clx, cly, "A");
}

void KbPreviewFrame::paintShift(QPainter &painter,int x,int y, int x2, int y2)
{
    const int sksz = 8;
    const int skx = x + 30;
    const int sky = y + (y2 / 2) + (sksz - (sksz / 4));
    
    paintKey(painter, x, y, x2, y2);
    
    QPainterPath sk;
        sk.moveTo(skx + (sksz / 2), sky);
        sk.lineTo(skx + (sksz / 2) + sksz, sky);
        sk.lineTo(skx + (sksz / 2) + sksz, sky - (sksz / 2));
        sk.lineTo(skx + (sksz * 2), sky - (sksz / 2));
        sk.lineTo(skx + sksz, sky - sksz - (sksz / 2));
        sk.lineTo(skx, sky - (sksz / 2));
        sk.lineTo(skx + (sksz / 2), sky - (sksz / 2));
        sk.closeSubpath();

    painter.setPen(kpen);
    painter.drawPath(sk);
}

void KbPreviewFrame::paintTLDE(QPainter &painter,int &x,int &y)
{
    const QList <QString> symbols = keyboardLayout.TLDE.symbols;

    paintKey(painter, x, y ,kszx, kszy);

    for(int level = 0; level<symbols.size(); level++) {
        if (level < levels) {
            painter.setPen(color[level]);
            painter.setFont(keyfont[level]);
            painter.drawText(x+xOffset[level], y+yOffset[level], sz, sz, Qt::AlignTop, symbol.getKeySymbol(symbols.at(level)));
        }
    }
}

void KbPreviewFrame::paintAERow(QPainter &painter,int &x,int &y)
{
    const int noAEk = 12;
    const int bkspszx = 70;
   
    paintTLDE(painter, x, y);
    
    for(int i=0; i<noAEk; i++){
        x += kszx + xsp;

        paintKey(painter, x, y, kszx, kszy);

        QList<QString> symbols = keyboardLayout.AE[i].symbols;

        for(int level = 0; level<symbols.size(); level++) {
            if (level < levels) {
                painter.setPen(color[level]);
                painter.setFont(signfont[level]);
                painter.drawText(x+xOffset[level], y+yOffset[level], sz, sz, Qt::AlignTop, symbol.getKeySymbol(symbols.at(level)));
        
            }
        }
    }

    x += kszx + xsp;
    
    // Backspace
    paintBkspc(painter, x, y, bkspszx, kszy);
}

void KbPreviewFrame::paintADRow(QPainter &painter,int &x,int&y)
{
    const int noADk = 12;
    const int tabszx = 70;

    // Tab
    paintTab(painter, x, y, tabszx, kszy);

    x += tabszx + xsp;


    for(int i=0; i<noADk; i++){
        QList<QString> symbols = keyboardLayout.AD[i].symbols;

        paintKey(painter, x, y, kszx, kszy);

        for(int level = 0; level<symbols.size(); level++) {
            if (level < levels) {
                painter.setPen(color[level]);
                painter.setFont(keyfont[level]);
                painter.drawText(x+xOffset[level], y+yOffset[level], sz, sz, Qt::AlignTop, symbol.getKeySymbol(symbols.at(level)));
            }
        }

        x += kszx + xsp;
    }

    paintKey(painter, x, y, kszx, kszy);

    QList<QString> symbols = keyboardLayout.BKSL.symbols;
    
    for(int level = 0; level<symbols.size(); level++) {
        if (level < levels) {
            painter.setPen(color[level]);
            painter.setFont(keyfont[level]);
            painter.drawText(x+xOffset[level], y+yOffset[level], sz, sz, Qt::AlignTop, symbol.getKeySymbol(symbols.at(level)));
        }
    }
}

void KbPreviewFrame::paintACRow(QPainter &painter,int &x,int &y)
{
    const int capszx = 90, retsz = 80;
    const int noACk = 11;

    // Caps lock
    paintCapsLck(painter, x, y, capszx, kszy);

    x += capszx + xsp;

    for(int i=0; i<noACk; i++){
        paintKey(painter, x, y, kszx, kszy);

        QList<QString> symbols = keyboardLayout.AC[i].symbols;

        for(int level = 0; level<symbols.size(); level++) {
            if (level < levels) {
                painter.setPen(color[level]);
                painter.setFont(keyfont[level]);
                painter.drawText(x+xOffset[level], y+yOffset[level], sz, sz, Qt::AlignTop, symbol.getKeySymbol(symbols.at(level)));
            }
        }

        x += kszx + xsp;
    }

    // Enter
    paintIntro(painter, x ,y, retsz, kszy);
}

void KbPreviewFrame::paintABRow(QPainter &painter,int &x,int &y)
{
    const int noABk = 10;
    const int shiftsz = 120;
    
    // Left shift
    paintShift(painter, x, y, shiftsz, kszy);

    x += shiftsz + xsp;
    
    for(int i=0; i<noABk; i++) {
        paintKey(painter, x, y, kszx, kszy);

        QList<QString> symbols = keyboardLayout.AB[i].symbols;

        for(int level = 0; level<symbols.size(); level++) {
            if (level < levels) {
                painter.setPen(color[level]);
                painter.setFont(keyfont[level]);
                painter.drawText(x+xOffset[level], y+yOffset[level], sz, sz, Qt::AlignTop, symbol.getKeySymbol(symbols.at(level)));
            }
        }

        x += kszx + xsp;
    }
    
    // Right Shift
    paintShift(painter, x, y, shiftsz-18, kszy);
}

void KbPreviewFrame::paintBottomRow(QPainter &painter,int &x,int &y)
{
    const int ctrlsz = 88, altsz = 88, spsz = 378;

    // Left Ctrl
    paintLabeledKey(painter, x, y, ctrlsz, kszy, lev12color, i18n("Ctrl"));

    x += ctrlsz + xsp;

    // Alt
    paintLabeledKey(painter, x, y, altsz, kszy, lev12color, i18n("Alt"));

    x += altsz + xsp;

    // Space
    paintKey(painter, x, y, spsz, kszy);

    x += spsz + xsp;

    // AltGr
    paintLabeledKey(painter, x, y, altsz, kszy, lev34color, i18n("AltGr"));

    x += altsz + xsp;

    // Right Ctrl
    paintLabeledKey(painter, x, y, ctrlsz, kszy, lev12color, i18n("Ctrl"));
}

void KbPreviewFrame::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(Qt::darkGray));
    
    painter.setPen(keyBorderColor);
    painter.drawRect(strtx, strty, endx, endy);

    painter.setBrush(QBrush(Qt::black));

    int x, y;
    
    x = bsz;
    y = bsz;
    paintAERow(painter, x, y);

    x = bsz;
    y = bsz + kszy + xso;
    paintADRow(painter, x, y);

    x = bsz;
    y = bsz + (kszy * 2) + (xso * 2);
    paintACRow(painter, x, y);

    x = bsz;
    y = bsz + (kszy * 3) + (xso * 3);
    paintABRow(painter, x, y);
    
    x = bsz;
    y = bsz + (kszy * 4) + (xso * 4);
    paintBottomRow(painter, x ,y);

    if (symbol.isFailed()) {
        painter.setPen(keyBorderColor);
        painter.drawRect(strtx, strty, endx, endy);

        painter.setPen(lev12color);
        QRectF ct(strtx, strty, endx, endy);
        painter.drawText(ct, Qt::AlignCenter, i18n("No preview found"));
    }
}


void KbPreviewFrame::generateKeyboardLayout(const QString& layout, const QString& layoutVariant)
{
    QString filename = keyboardLayout.findSymbolBaseDir();
    filename.append(layout);

    QFile file(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = file.readAll();
    file.close();

    QList<QString> symstr = content.split("xkb_symbols ");

    if( layoutVariant.isEmpty() ) {
        keyboardLayout.generateLayout(symstr.at(1), layout);
    } else {
        for(int i = 1; i<symstr.size(); i++) {
            QString h = symstr.at(i);
            int k = h.indexOf("\"");
            h = h.mid(k);
            k = h.indexOf("{");
            h = h.left(k);
            h = h.remove(" ");
            QString f = "\"";
            f.append(layoutVariant);
            f.append("\"");
            f = f.remove(" ");

            if (h == f) { 
                keyboardLayout.generateLayout(symstr.at(i), layout);
                break;
            }
        }
    }
}
