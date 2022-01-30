//
// Created by nils on 12/2/21.
//

#include "minecraftpage.h"

#include <QHBoxLayout>
#include <QLabel>
#include <limits>

#include <gui/widgets/widgetutils.h>

MinecraftPage::MinecraftPage(Config &config, QWidget *parent) : ConfigurationPage(config, parent) {
    customMinecraftDir = new QCheckBox(QStringLiteral("自定义 .minecraft 目录"));
    minecraftPathChooser = new FileChooser(QFileDialog::Directory);

    serverToJoin = new QLineEdit();
    serverToJoin->setDisabled(true);

    connect(serverToJoin, &QLineEdit::returnPressed, [this](){serverToJoin->clearFocus();});

    joinServerOnLaunch = new QCheckBox(QStringLiteral("启动时加入服务器"));

    connect(joinServerOnLaunch, &QCheckBox::toggled, serverToJoin, &QLineEdit::setEnabled);

    QVBoxLayout* serverContainer = new QVBoxLayout();
    serverContainer->setSpacing(6);
    serverContainer->addWidget(joinServerOnLaunch, 0, Qt::AlignHCenter);
    serverContainer->addWidget(serverToJoin);

    useNickHiderName = new QCheckBox(QStringLiteral("NickHider 名称"));
    nickHiderName = new QLineEdit();
    nickHiderName->setPlaceholderText(QStringLiteral("You"));

    useLevelHeadPrefix = new QCheckBox(QStringLiteral("水平头前缀"));
    levelHeadPrefix = new QLineEdit();
    levelHeadPrefix->setPlaceholderText(QStringLiteral("等级: "));

    useAutoggMessage = new QCheckBox(QStringLiteral("AutoGG 消息"));
    autoggMessage = new QLineEdit();
    autoggMessage->setPlaceholderText(QStringLiteral("gg"));

    useNickLevel = new QCheckBox(QStringLiteral("为刻痕显示的 LevelHead 级别(-1 表示不会显示任何内容)"));
    nickLevel = new QSpinBox;
    nickLevel->setMinimum(INT_MIN);
    nickLevel->setMaximum(INT_MAX);

    windowWidth = new QSpinBox();
    windowHeight = new QSpinBox();

    windowWidth->setMinimum(0);
    windowWidth->setMaximum(99999);

    windowHeight->setMinimum(0);
    windowHeight->setMaximum(99999);

    QHBoxLayout* windowResContainer = new QHBoxLayout();
    windowResContainer->setSpacing(30);
    windowResContainer->addWidget(new QLabel(QStringLiteral("窗口宽度")));
    windowResContainer->addWidget(windowWidth, 1);
    windowResContainer->addWidget(new QLabel(QStringLiteral("窗口高度")));
    windowResContainer->addWidget(windowHeight, 1);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(40);
    mainLayout->addLayout(WidgetUtils::createOptional(customMinecraftDir, minecraftPathChooser));
    mainLayout->addLayout(serverContainer);
    mainLayout->addLayout(windowResContainer);
    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addLayout(WidgetUtils::createOptional(useNickHiderName, nickHiderName));
    hLayout->addLayout(WidgetUtils::createOptional(useLevelHeadPrefix, levelHeadPrefix));
    mainLayout->addLayout(hLayout);
    mainLayout->addLayout(WidgetUtils::createOptional(useAutoggMessage, autoggMessage));
    mainLayout->addLayout(WidgetUtils::createOptional(useNickLevel, nickLevel));

    mainLayout->addStretch(1);

    setLayout(mainLayout);
}

QString MinecraftPage::title() {
    return QStringLiteral("我的世界");
}

QIcon MinecraftPage::icon() {
    return QIcon(":/res/icons/minecraft.svg");
}

void MinecraftPage::apply() {
    config.useCustomMinecraftDir = customMinecraftDir->isChecked();
    config.customMinecraftDir = minecraftPathChooser->getPath();

    config.joinServerOnLaunch = joinServerOnLaunch->isChecked();
    config.serverIp = serverToJoin->text();

    config.useNickHiderName = useNickHiderName->isChecked();
    config.nickHiderName = nickHiderName->text();

    config.useLevelHeadPrefix = useLevelHeadPrefix->isChecked();
    config.levelHeadPrefix = levelHeadPrefix->text();

    config.useAutoggMessage = useAutoggMessage->isChecked();
    config.autoggMessage = autoggMessage->text();

    config.useNickLevel = useNickLevel->isChecked();
    config.nickLevel = nickLevel->value();

    config.windowWidth = windowWidth->value();
    config.windowHeight = windowHeight->value();
}

void MinecraftPage::load() {
    customMinecraftDir->setChecked(config.useCustomMinecraftDir);
    minecraftPathChooser->setPath(config.customMinecraftDir);

    joinServerOnLaunch->setChecked(config.joinServerOnLaunch);
    serverToJoin->setText(config.serverIp);

    useNickHiderName->setChecked(config.useNickHiderName);
    nickHiderName->setText(config.nickHiderName);

    useLevelHeadPrefix->setChecked(config.useLevelHeadPrefix);
    levelHeadPrefix->setText(config.levelHeadPrefix);

    useAutoggMessage->setChecked(config.useAutoggMessage);
    autoggMessage->setText(config.autoggMessage);

    useNickLevel->setChecked(config.useNickLevel);
    nickLevel->setValue(config.nickLevel);

    windowWidth->setValue(config.windowWidth);
    windowHeight->setValue(config.windowHeight);
}







