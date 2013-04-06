/***************************************************************************
 *   Copyright (C) 2013 by Manuel Tortosa                                  *
 *   manutortosa@chakra-project.org                                        *
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

#include <QDebug>
#include <QList>
#include <QMap>
#include <QMultiMap>
#include <QProcess>

#include "keyboardpage.h"
#include "../widgets/kbpreview/x11kbparser.h"
#include "../widgets/kbpreview/kbpreviewframe.h"
#include "../installationhandler.h"

#include <KLocale>

KeyboardPage::KeyboardPage(QWidget *parent)
        : AbstractPage(parent),
        kbframe(new KbPreviewFrame(parent)),
        m_install(InstallationHandler::instance())
{
}

KeyboardPage::~KeyboardPage()
{
}

void KeyboardPage::createWidget()
{
    ui.setupUi(this);


    // get the basic definitions / parse the file for the appropriate information
    X11KbParser parser;

    // set default filepath if none given as parameter
    if (m_xorgFilePath.isEmpty())
        m_xorgFilePath = "/usr/share/X11/xkb/rules/xorg.lst";

    m_layout = parser.parseKeyboardLayouts(m_xorgFilePath);
    m_variants = parser.parseKeyboardVariants(m_xorgFilePath);

    // query stexkbmap to know which layout is currently set
    QProcess proc;
    proc.start("setxkbmap -query");
    proc.waitForFinished();

    // parse the ouput of setxkbmap
    while (!proc.atEnd()) {
        QByteArray line = proc.readLine();

        // set the appropriate pattern (<label>: <value>)
        QRegExp rx;
        rx.setPattern("^(\\S+):\\s+(\\S+)");

        // if the regexp does not match it returns -1
        if (rx.indexIn(line) != -1) {
            if (rx.cap(1).startsWith("layout")) {
                m_origLayout = rx.cap(2);
            } else if (rx.cap(1).startsWith("variant")) {
                m_origVariant = rx.cap(2);
            }
        }
    }
    
    // use only the primary keyboard layout
    m_origLayout = m_origLayout.split(",").first();

    // use m_orig only if keyboard is not set in m_install
    m_selectedLayout = m_install->kblayout().isEmpty() ? m_origLayout : m_install->kblayout();
    m_selectedVariant = m_install->kbvariant().isEmpty() ? m_origVariant : m_install->kbvariant();

    ui.cbLayout->addItems(m_layout.keys());
    QMap<QString, QString> variants = m_variants.value(m_selectedLayout);
    ui.cbVariant->addItems(variants.keys());

    ui.cbLayout->setCurrentIndex(ui.cbLayout->findText(m_layout.key(m_selectedLayout)));
    QString variantKey = variants.key(m_selectedVariant);
    if (variantKey.isEmpty()) {
       ui.cbVariant->setCurrentIndex(ui.cbVariant->findText("Default"));
    } else {
        ui.cbVariant->setCurrentIndex(ui.cbVariant->findText(variants.key(m_selectedVariant)));
    }

    // ComboBoxes
    connect(ui.cbLayout, SIGNAL(currentIndexChanged(QString)), this, SLOT(selectedLayoutChanged(QString)));
    connect(ui.cbVariant, SIGNAL(currentIndexChanged(QString)), this, SLOT(selectedVariantChanged(QString)));
    
    // make sure we fill the install variables even if the user does not change nothing
    m_install->setKbLayout(m_selectedLayout);
    m_install->setKbVariant(m_selectedVariant);

    kbframe->setFixedSize( 767, 288 );
    kbframe->generateKeyboardLayout(m_selectedLayout, m_selectedVariant);
    ui.previewLayout->addWidget(kbframe);
}

void KeyboardPage::aboutToGoToPrevious()
{
    emit goToPreviousStep();
}

void KeyboardPage::aboutToGoToNext()
{
    // qDebug() << " :: Layout " << m_install->kblayout() << " :: Variant " << m_install->kbvariant(); 
    emit goToNextStep();
}

void KeyboardPage::selectedLayoutChanged(QString text)
{
    // set the selected layout to the value for later comparisons without stepping through
    m_selectedLayout = m_layout.find(text).value();

    // clearing and adding items will emit curentIndexChanged(), we don't wan't to react to it though
    disconnect(ui.cbVariant, SIGNAL(currentIndexChanged(QString)), this, SLOT(selectedVariantChanged(QString)));
    // clear the variants to have an empty compbo box
    ui.cbVariant->clear();
    // populate combobox
    ui.cbVariant->addItems(m_variants.value(m_selectedLayout).keys());

    // make sure that the next setCurrentIndex() triggers variantChanged()
    ui.cbVariant->setCurrentIndex(-1);

    connect(ui.cbVariant, SIGNAL(currentIndexChanged(QString)), this, SLOT(selectedVariantChanged(QString)));

    m_install->setKbLayout(m_selectedLayout);
    // set the variants combo box to the "proper" value
    int idx = ui.cbVariant->findText(m_install->kbvariant());
    if (idx != -1) {
        ui.cbVariant->setCurrentIndex(idx);
    } else {
        ui.cbVariant->setCurrentIndex(ui.cbVariant->findText("Default"));
    }
}

void KeyboardPage::selectedVariantChanged(QString text)
{
    // if the value is "Default" there should not be any variant passed to the command
    // otherwhise, find the variant we have been changed to, and set the selected value accordingly
    if (text == "Default")
        m_selectedVariant = "";
    else
        m_selectedVariant = m_variants.find(m_selectedLayout).value().value(text);

    m_install->setKbVariant(m_selectedVariant);
    applyKbLayout();
}

void KeyboardPage::applyKbLayout()
{
    ui.testLayoutInput->clear();
    kbframe->generateKeyboardLayout(m_selectedLayout, m_selectedVariant);
    kbframe->update();

    // command string setup, model and layout
    QString cmd("setxkbmap -layout " + m_selectedLayout);

    // if the selected variant is empty ("Default") - omit, otherwhise add the command parameter
    if (!m_selectedVariant.isEmpty())
        cmd += " -variant " + m_selectedVariant;

    QProcess proc;
    proc.execute(cmd);
    proc.waitForFinished();
}

void KeyboardPage::generateKeyboardLayout(const QString& layout, const QString& variant)
{
    kbframe->generateKeyboardLayout(layout, variant);
}

#include "keyboardpage.moc"
