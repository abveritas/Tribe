/***************************************************************************
 *   Copyright (C) 2008 - 2009 by Dario Freddi (drf@chakra-project.org)    *
 *                 2013 Manuel Tortosa (manutortosa@chakra-project.org)    *
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

#include <KMessageBox>
#include <KLocalizedString>
#include <KPushButton>

#include <tribepartitionmanager/ops/operation.h>
#include <tribepartitionmanager/core/partition.h>
#include <tribepartitionmanager/fs/filesystem.h>

#include "../pmhandler.h"
#include "readyinstallpage.h"


ReadyInstallPage::ReadyInstallPage(QWidget *parent)
        : AbstractPage(parent),
        m_install(InstallationHandler::instance())
{
}

ReadyInstallPage::~ReadyInstallPage()
{
}

void ReadyInstallPage::createWidget()
{
    ui.setupUi(this);

    QString installStr;
    if (m_install->installationType() == InstallationHandler::NetInst) {
        // show selected groups here too
        installStr = i18n("Network install");
    } else {
        installStr = i18n("Offline install");
    }
    ui.installLabel->setText(installStr);

    if (PMHandler::instance()->operationStack().operations().isEmpty()) {
        ui.formatWidget->hide();
    }

    QString formatStr;
    foreach (const Operation* op, PMHandler::instance()->operationStack().operations()) {
        formatStr += op->description() + "\n";
    }
    ui.formatLabel->setText(formatStr);

    QString mountStr;
    QHash< QString, const Partition* > mountList = PMHandler::instance()->mountList(m_install->rootDevice());
    for (QHash< QString, const Partition* >::const_iterator i = mountList.constBegin(); i != mountList.constEnd(); ++i) {
        QString partitionPath;
        if (i.value()->number() > 0) {
            partitionPath = QString("%1%2").arg(i.value()->devicePath()).arg(i.value()->number());
        } else {
            partitionPath = i18n("New partition (%1) on %2", i.value()->fileSystem().name(), i.value()->devicePath());
        }
        mountStr += i18n("%1 will be used as %2\n", partitionPath, i.key());
    }
    ui.mountLabel->setText(mountStr);

    ui.timezoneLabel->setText(i18n("'%1' will be used as timezone\n"
                        "'%2' will be used as language and '%3' will be used as locale\n"
                        "Keyboard settings: %4  %5  %6",
                        m_install->timezone(), m_install->KDELangPack(), m_install->locale(),
                        m_install->kblayout(), m_install->kbvariant()));
}

void ReadyInstallPage::aboutToGoToNext()
{
    QStringList mnt = m_install->getMountedPartitions();

    if (!mnt.isEmpty()) {
        QString sub;

        foreach(const QString &s, mnt) {
            sub.append(s);
            sub.append(", ");
        }

        sub.chop(2);

        KDialog *dialog = new KDialog(this, Qt::FramelessWindowHint);
        dialog->setButtons(KDialog::Apply | KDialog::Cancel);
        dialog->button(KDialog::Apply)->setText(i18n("Continue"));
        dialog->setModal(true);
        bool retbool;
        if (KMessageBox::createKMessageBox(dialog, QMessageBox::Warning,
                                           i18n("The following partitions appear to be mounted: %1. "
                                                "To let the installation happen correctly, Tribe will "
                                                "now unmount them.", sub),
                                           QStringList(), QString(), &retbool, KMessageBox::Notify) == KDialog::Cancel) {
            return;
        } else {
            foreach(const QString &p, mnt) {
                m_install->unmountPartition(p);
            }
        }
    }

    emit goToNextStep();
}

void ReadyInstallPage::aboutToGoToPrevious()
{
    emit goToPreviousStep();
}

#include "readyinstallpage.moc"
