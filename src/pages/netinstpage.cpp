#include "../installationhandler.h"
#include "netinstpage.h"

#include "../widgets/groupselectionwidget.h"

#include <config-tribe.h>

#include <QFile>
#include <QMap>
#include <QTextStream>

#include <KIO/Job>
#include <KIO/NetAccess>

#include <QtDebug>

NetInstPage::NetInstPage(QWidget *parent)
    : AbstractPage(parent),
      m_handler(InstallationHandler::instance())
{
    // populate m_groups with i12ized strings
    m_groups["default"]         = {i18n("Default"),         i18n("Default group"), true};
    m_groups["wireless"]        = {i18n("Wireless"),        i18n("Tools for wireless connections")};
    m_groups["ccr"]             = {i18n("CCR"),             i18n("Tools for the Chakra Community Repository")};
    m_groups["graphics"]        = {i18n("Graphics"),        i18n("Applications to work with graphics")};
    m_groups["burn"]            = {i18n("Burning"),         i18n("Set of packages for disc burning")};
    m_groups["printing"]        = {i18n("Printing"),        i18n("Print much?")};
    m_groups["multimedia"]      = {i18n("Multimedia"),      i18n("Music and video players")};
    m_groups["gtk-integration"] = {i18n("Gtk Integration"), i18n("Integrate Gtk applications with plasma")};
    m_groups["misc"]            = {i18n("Miscelaneous"),    i18n("Useful tools and apps")};
}

void NetInstPage::createWidget()
{
    ui.setupUi(this);

    if (!loadGroupList()) {
        qDebug() << "Not connected";
        ui.netinst->setEnabled(false);
        ui.netinst->setText(i18n("Network Installation. (Disabled: Unable to fetch package lists, check your network connection)"));
        return;
    }

    QSignalMapper *mapper = new QSignalMapper(this);
    foreach (QString groupKey, m_groupOrder) {
        if (groupKey == "default") {
            m_groups[groupKey].selected = true;
            continue;
        }
        Group group = m_groups[groupKey];
        GroupSelectionWidget* groupWidget = new GroupSelectionWidget(group.name, group.description, group.packages, this);
        ui.groupswidget->layout()->addWidget(groupWidget);
        mapper->setMapping(groupWidget, groupKey);
        connect(groupWidget, SIGNAL(toggled(bool)), mapper, SLOT(map()));
    }

    connect(mapper, SIGNAL(mapped(QString)), this, SLOT(toggleGroup(QString)));
}

void NetInstPage::aboutToGoToNext()
{
    if (validate())
        emit goToNextStep();
}

void NetInstPage::aboutToGoToPrevious()
{
    emit goToPreviousStep();
}

bool NetInstPage::loadGroupList()
{
    QString confUrl("https://gitorious.org/chakra/tribe/blobs/raw/netinstall/data/netinstall.conf");

    KIO::Job* getJob = KIO::get(confUrl, KIO::Reload, KIO::Overwrite | KIO::HideProgressInfo);
    QByteArray data;
    if (!KIO::NetAccess::synchronousRun(getJob, 0, &data)) {
        return false;
    }

    QString currentGroup;
    QTextStream in(&data);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#')) {
            continue;
        }
        if (line.startsWith('[')) {
            QStringList tmp = line.split(']');
            int end = line.indexOf(']');
            currentGroup = line.mid(1, end - 1);
            // show groups in order as they appear
            m_groupOrder << currentGroup;
        } else {
            QString pkg = line.left(line.indexOf(' '));
            m_groups[currentGroup].packages << pkg;
        }
    }
    return true;
}

void NetInstPage::intallTypeToggled(QAbstractButton *button)
{
    if (button == ui.netinst) {
        m_handler->setInstallationType(InstallationHandler::NetInst);
    } else if (button == ui.isoinst) {
        m_handler->setInstallationType(InstallationHandler::NetInst);
    }
}

void NetInstPage::toggleGroup(QString groupKey)
{
    m_groups[groupKey].selected = !m_groups[groupKey].selected;
}

bool NetInstPage::validate()
{
    if (ui.netinst->isChecked()) {
        m_handler->setInstallationType(InstallationHandler::NetInst);
        QStringList packages;
        foreach (Group group, m_groups) {
            if (group.selected) {
                packages += group.packages;
            }
        }
        m_handler->setPackages(packages);
        return true;
    }
    if (ui.isoinst->isChecked()) {
        m_handler->setInstallationType(InstallationHandler::Iso);
        return true;
    }

    return false;
}
