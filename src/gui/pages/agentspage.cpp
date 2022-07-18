//
// Created by nils on 11/20/21.
//

#include "agentspage.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QProxyStyle>
#include <QHeaderView>
#include <QItemSelectionModel>

AgentsPage::AgentsPage(Config &config, QWidget *parent) : ConfigurationPage(config, parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(20);

    agents = new AgentsView(this);
    agents->setModel((model = new AgentsModel(config.agents, this)));

    add = new QPushButton("添加");
    remove = new QPushButton("删除");
    moveUp = new QPushButton("上移");
    moveDown = new QPushButton("下移");

    connect(agents->selectionModel(), &QItemSelectionModel::selectionChanged, this, &AgentsPage::onSelect);

    remove->setDisabled(true);
    moveUp->setDisabled(true);
    moveDown->setDisabled(true);

    connect(add, &QPushButton::clicked, [this]() {
        QStringList files = QFileDialog::getOpenFileNames(
                nullptr,
                "打开代理罐",
                {},
                "Java Agent (*.jar)"
        );

        for(const QString& str : files){
            model->addAgent(str, {});
        }

        agents->selectRow(model->rowCount(QModelIndex()) - 1);
    });

    connect(remove, &QPushButton::clicked, [this]() {
        for(const QModelIndex &item : agents->selectionModel()->selectedRows()) {
            model->removeRow(item.row());
        }
    });

    connect(moveUp, &QPushButton::clicked, [this]() {
        QModelIndexList selected = agents->selectionModel()->selectedRows();
        if (!selected.isEmpty()) {
            int currentRow = selected[0].row();
            if (currentRow > 0) {
                model->moveRow(QModelIndex(), currentRow - 1, QModelIndex(), currentRow + 1);
            }
        }
    });

    connect(moveDown, &QPushButton::clicked, [this]() {
        QModelIndexList selected = agents->selectionModel()->selectedRows();
        if (!selected.isEmpty()) {
            int currentRow = selected[0].row();
            if (currentRow < model->rowCount(QModelIndex()) - 1) {
                model->moveRow(QModelIndex(), currentRow, QModelIndex(), currentRow + 2);
            }
        }
    });

    QGridLayout *agentsContainer = new QGridLayout();
    agentsContainer->setSpacing(6);
    agentsContainer->addWidget(agents, 0, 0, 5, 1);
    agentsContainer->addWidget(add, 0, 1);
    agentsContainer->addWidget(remove, 1, 1);
    agentsContainer->addWidget(moveUp, 2, 1);
    agentsContainer->addWidget(moveDown, 3, 1);

    mainLayout->addLayout(agentsContainer);


    setLayout(mainLayout);
}


QString AgentsPage::title() {
    return "代理";
}

QIcon AgentsPage::icon() {
    return QIcon(":/res/icons/agent.svg");
}

void AgentsPage::apply() {
}

void AgentsPage::load() {
}

void AgentsPage::onSelect(const QItemSelection &selected, const QItemSelection &deselected) {
    QModelIndexList selectedRows = qobject_cast<QItemSelectionModel *>(sender())->selectedRows();

    if (selectedRows.isEmpty()) {
        remove->setDisabled(true);
        moveUp->setDisabled(true);
        moveDown->setDisabled(true);
    } else {
        remove->setEnabled(true);
        moveUp->setEnabled(true);
        moveDown->setEnabled(true);
    }
}

QString AgentsPage::description() {
    return "您要使用的代理列表。不要添加任何Forge/Fabric模组，否则您的游戏将无法启动.";
}
