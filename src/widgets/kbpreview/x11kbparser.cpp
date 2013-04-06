/***************************************************************************
 *   Copyright (C) 2011 by Georg Grabler                                   *
 *   ggrabler@gmail.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "x11kbparser.h"

#include <QFile>
#include <QDebug>
#include <QRegExp>

X11KbParser::X11KbParser()
{
}

QMap<QString, QString> X11KbParser::parseKeyboardModels(QString filepath)
{
    QMap<QString, QString> models;

    QFile fh(filepath);
    fh.open(QIODevice::ReadOnly);

    if (!fh.isOpen()) {
        qDebug() << "X11 Keyboard model definitions not found!";
        return models;
    }

    bool modelsFound = false;
    // read the file until the end or until we break the loop
    while (!fh.atEnd()) {
        QByteArray line = fh.readLine();

        // check if we start with the model section in the file
        if (!modelsFound && line.startsWith("! model"))
            modelsFound = true;
        else if (modelsFound && line.startsWith ("!"))
            break;
        else if (!modelsFound)
            continue;

        // here we are in the model section, otherwhise we would continue or break
        QRegExp rx;
        rx.setPattern("^\\s+(\\S+)\\s+(\\w.*)\n$");

        // insert into the model map
        if (rx.indexIn(line) != -1)
            models.insert(rx.cap(2), rx.cap(1));
    }

    return models;
}

QMap<QString, QString> X11KbParser::parseKeyboardLayouts(QString filepath)
{
    QMap<QString, QString> layouts;

    QFile fh(filepath);
    fh.open(QIODevice::ReadOnly);

    if (!fh.isOpen()) {
        qDebug() << "X11 Keyboard layout definitions not found!";
        return layouts;
    }

    bool layoutsFound = false;
    // read the file until the end or we break the loop
    while (!fh.atEnd()) {
        QByteArray line = fh.readLine();

        // find the layout section otherwhise continue. If the layout section is at it's end, break the loop
        if (!layoutsFound && line.startsWith("! layout"))
            layoutsFound = true;
        else if (layoutsFound && line.startsWith ("!"))
            break;
        else if (!layoutsFound)
            continue;

        QRegExp rx;
        rx.setPattern("^\\s+(\\S+)\\s+(\\w.*)\n$");

        // insert into the layout map
        if (rx.indexIn(line) != -1)
            layouts.insert(rx.cap(2), rx.cap(1));
    }

    return layouts;
}

/**
 * Returns a QMap of all keyboard variants, the map looks like this:
 * QMap(
 *  ( "<layout1>", QMap( ("<description1>", "<variant1>")("<description1>", "<variant1>")("<description1>", "<variant1>") ) )
 *  ( "<layout2>", QMap( ("<description1>", "<variant1>")("<description1>", "<variant1>")("<description1>", "<variant1>") ) )
 * )
 */
QMap<QString, QMap<QString, QString> > X11KbParser::parseKeyboardVariants(QString filepath)
{
    QMap<QString, QMap<QString, QString> > variants;

    QFile fh(filepath);
    fh.open(QIODevice::ReadOnly);

    if (!fh.isOpen()) {
        qDebug() << "X11 Keyboard variant definitions not found!";
        return variants;
    }

    bool variantsFound = false;
    // read the file until the end or until we break
    while (!fh.atEnd()) {
        QByteArray line = fh.readLine();

        // continue until we found the variant section. If found, read until the next section is found
        if (!variantsFound && line.startsWith("! variant")) {
            variantsFound = true;
            continue;
        } else if (variantsFound && line.startsWith ("!"))
            break;
        else if (!variantsFound)
            continue;

        QRegExp rx;
        rx.setPattern("^\\s+(\\S+)\\s+(\\S+): (\\w.*)\n$");

        // insert into the variants multimap, if the pattern matches
        if (rx.indexIn(line) != -1) {
            if (variants.find(rx.cap(2)) != variants.end()) {
                // in this case we found an entry in the multimap, and add the values to the multimap
                variants.find(rx.cap(2)).value().insert(rx.cap(3), rx.cap(1));
            } else {
                // create a new map in the multimap - the value was not found.
                QMap<QString, QString> m;
                m.insert("Default", "");
                m.insert(rx.cap(3), rx.cap(1));
                variants.insert(rx.cap(2), m);
            }
        }
    }

    // cross-checking, not all layouts do have a variant
    QMap<QString, QString> layouts = this->parseKeyboardLayouts(filepath);

    QMapIterator<QString, QString> li(layouts);

    while (li.hasNext()) {
        li.next();
        if (variants.find(li.value()) == variants.end()) {
            QMap<QString, QString> m;
            m.insert("Default", "");
            variants.insert(li.value(), m);
        }
    }

    return variants;
}
