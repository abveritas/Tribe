/***************************************************************************
 *   Copyright (C) 2013 by Manuel Tortosa (manutortosa@chakra-project.org) *
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

#ifndef KEYBOARDPAGE_H
#define KEYBOARDPAGE_H

#include "../abstractpage.h"
#include "../installationhandler.h"
#include "../widgets/kbpreview/kbpreviewframe.h"
#include "ui_keyboard.h"

class InstallationHandler;

class KeyboardPage : public AbstractPage
{
    Q_OBJECT

public:
    KeyboardPage(QWidget *parent = 0);
    virtual ~KeyboardPage();

private:
    void generateKeyboardLayout(const QString& layout, const QString& variant);

private slots:
    void createWidget();
    void aboutToGoToNext();
    void aboutToGoToPrevious();
    void applyKbLayout();
    void selectedLayoutChanged(QString text);
    void selectedVariantChanged(QString text);

private:
    Ui::Keyboard ui;

    KbPreviewFrame *kbframe;
    InstallationHandler *m_install;

    QMap<QString, QString> m_layout;
    QMap<QString, QMap<QString, QString> > m_variants;

    QString m_selectedLayout;
    QString m_selectedVariant;
    QString m_origLayout;
    QString m_origVariant;
    QString m_xorgFilePath;
};

#endif /*KEYBOARDPAGE_H*/
