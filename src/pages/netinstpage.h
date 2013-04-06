#ifndef NETINST_H
#define NETINST_H

#include "../abstractpage.h"

#include "ui_netinst.h"

class NetInstPage : public AbstractPage
{
    Q_OBJECT

    struct Group {
        Group()
            : Group("","",false) { }
        Group(QString name, QString description, bool selected)
            : name(name), description(description), selected(selected) { }
        Group(QString name, QString description)
            : Group(name, description, false) { }
        QString name;
        QString description;
        QStringList packages;
        bool selected = false;
    };

public:
    NetInstPage(QWidget *parent = 0);

public slots:
    void toggleGroup(QString name);

private slots:
    virtual void createWidget();
    virtual void aboutToGoToNext();
    virtual void aboutToGoToPrevious();

    bool validate();
    void intallTypeToggled(QAbstractButton *button);

private:
    bool loadGroupList();

    Ui::NetInst ui;
    InstallationHandler *m_handler;

    QHash<QString, Group> m_groups;
    QList<QString> m_groupOrder;
};

#endif // NETINST_H
