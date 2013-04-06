#ifndef GROUPSELECTIONWIDGET_H
#define GROUPSELECTIONWIDGET_H

#include "ui_groupselectionwidget.h"

#include <QSignalMapper>
#include <QWidget>

class GroupSelectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupSelectionWidget(QString name, QString description, QStringList packages, QWidget *parent = 0);
    
signals:
    void toggled(bool);
    
public slots:
//    void toggleGroup(bool toggled) const { emit groupToggled(m_name, toggled); }

private:
    Ui::GroupSelectionWidget ui;
    static QSignalMapper *m_mapper;
};

#endif // GROUPSELECTIONWIDGET_H
