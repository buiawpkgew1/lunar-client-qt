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

AgentsPage::AgentsPage(Config& config, QWidget *parent) : ConfigurationPage(config, parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(20);

    agents = new QListWidget();
    QFont font;
    font.setPointSize(11);
    agents->setFont(font);
    agents->setAlternatingRowColors(true);

    QPalette palette;
    palette.setColor(QPalette::Disabled, QPalette::Text, Qt::blue);
    agents->setPalette(palette);

    QPushButton* add = new QPushButton(QStringLiteral("添加"));
    QPushButton* remove = new QPushButton(QStringLiteral("移除"));
    QPushButton* moveUp = new QPushButton(QStringLiteral("上移"));
    QPushButton* moveDown = new QPushButton(QStringLiteral("下移"));

    connect(add, &QPushButton::clicked, [this](){
        QStringList files = QFileDialog::getOpenFileNames(
                nullptr,
                QStringLiteral("Open Agent Jar"),
                {},
                QStringLiteral("Java Agent (*.jar)")
                );
        foreach(const QString& str, files){
            if(!str.isEmpty()){
                addAgent(str, true);
            }
        }
    });

    connect(remove, &QPushButton::clicked, [this](){
        foreach(QListWidgetItem* item, agents->selectedItems()){
            delete item;
        }
    });

    connect(moveUp, &QPushButton::clicked, [this](){
        int currentRow = agents->currentRow();
        if(currentRow > 0){
            auto currentItem = agents->takeItem(currentRow);
            agents->insertItem(currentRow - 1, currentItem);
            agents->setCurrentRow(currentRow - 1);
        }
    });

    connect(moveDown, &QPushButton::clicked, [this](){
        int currentRow = agents->currentRow();
        if(currentRow < agents->count()-1){
            auto currentItem = agents->takeItem(currentRow);
            agents->insertItem(currentRow + 1, currentItem);
            agents->setCurrentRow(currentRow + 1);
        }
    });

    QGridLayout* agentsContainer = new QGridLayout();
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
    return QStringLiteral("Agents");
}

QIcon AgentsPage::icon() {
    return QIcon(":/res/icons/agent.svg");
}

void AgentsPage::apply() {
    config.agents = getAgents();
}

void AgentsPage::load() {
    foreach(const QString &path, config.agents) {
        if (QFile::exists(path)) {
            addAgent(path);
        }
    }
}

void AgentsPage::addAgent(const QString& path, bool select) {
    auto item = new QListWidgetItem(QFileInfo(path).fileName(), agents);
    item->setToolTip(path);
    if(select)
        agents->setCurrentItem(item);
}

QStringList AgentsPage::getAgents() {
    QStringList list;
    for(int i = 0; i < agents->count(); ++i){
        list << agents->item(i)->toolTip();
    }
    return list;
}
