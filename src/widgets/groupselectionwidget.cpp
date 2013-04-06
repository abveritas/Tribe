#include "groupselectionwidget.h"

#include <QtDebug>

GroupSelectionWidget::GroupSelectionWidget(QString name, QString description, QStringList packages, QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    connect(ui.group, SIGNAL(toggled(bool)), this, SIGNAL(toggled(bool)));

    ui.group->setText(name);
    ui.description->setText(description);
    const int columns = 4;
    const int rows = (packages.size() - 1) / columns + 1;
    ui.packageview->setRowCount(rows);
    ui.packageview->setColumnCount(columns);

    ui.packageview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    int r = 0, c = 0;
    for (int i = 0; i < packages.size(); ++i) {
        ui.packageview->setItem(r++,c, new QTableWidgetItem(packages.at(i)));
        if (r == ui.packageview->rowCount()) {
            ++c;
            r = 0;
        }
    }

    int rowsShown = 6;
    rowsShown = rows < rowsShown ? rows : 6;
    ui.packageview->setMinimumHeight(ui.packageview->rowHeight(0));
    ui.packageview->setMaximumHeight(rowsShown * ui.packageview->rowHeight(0));
    ui.packageview->hide();
}
