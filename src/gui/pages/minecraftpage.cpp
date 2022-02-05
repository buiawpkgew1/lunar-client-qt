//
// Created by nils on 12/2/21.
//

#include "minecraftpage.h"

#include <QHBoxLayout>
#include <QLabel>
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

<<<<<<< HEAD
    useNickHiderName = new QCheckBox(QStringLiteral("NickHider 名称"));
    nickHiderName = new QLineEdit();
    nickHiderName->setPlaceholderText(QStringLiteral("You"));

    useAutoggMessage = new QCheckBox(QStringLiteral("AutoGG 消息"));
=======
    useLevelHeadPrefix = new QCheckBox(QStringLiteral("LevelHead Prefix"));
    levelHeadPrefix = new QLineEdit();
    levelHeadPrefix->setPlaceholderText(QStringLiteral("Level: "));

    useAutoggMessage = new QCheckBox(QStringLiteral("AutoGG Message"));
>>>>>>> upstream/master
    autoggMessage = new QLineEdit();
    autoggMessage->setPlaceholderText(QStringLiteral("gg"));

    useLevelHeadPrefix = new QCheckBox(QStringLiteral("水平头前缀"));
    levelHeadPrefix = new QLineEdit();
    levelHeadPrefix->setPlaceholderText(QStringLiteral("等级: "));

    useLevelHeadNick = new QCheckBox(QStringLiteral("LevelHead 尼克级别"));
    levelHeadNickLevel = new QLineEdit();
    levelHeadNickLevel->setPlaceholderText(QString::number(-1));


    useCosmetics = new QCheckBox(QStringLiteral("启用化妆品"));
    unlockCosmetics = new QCheckBox(QStringLiteral("解锁所有化妆品"));

    connect(useCosmetics, &QCheckBox::toggled, unlockCosmetics, &QCheckBox::setEnabled);

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
<<<<<<< HEAD
    hLayout->addLayout(WidgetUtils::createOptional(useNickHiderName, nickHiderName));
    hLayout->addLayout(WidgetUtils::createOptional(useAutoggMessage, autoggMessage));
    mainLayout->addLayout(hLayout);
    QHBoxLayout* h2Layout = new QHBoxLayout;
    h2Layout->addLayout(WidgetUtils::createOptional(useLevelHeadPrefix, levelHeadPrefix));
    h2Layout->addLayout(WidgetUtils::createOptional(useLevelHeadNick, levelHeadNickLevel));
    mainLayout->addLayout(h2Layout);
    mainLayout->addWidget(useCosmetics, 1, Qt::AlignHCenter);
    mainLayout->addWidget(unlockCosmetics, 0, Qt::AlignCenter);
=======
    hLayout->addLayout(WidgetUtils::createOptional(useLevelHeadPrefix, levelHeadPrefix));
    hLayout->addLayout(WidgetUtils::createOptional(useAutoggMessage, autoggMessage));

    mainLayout->addLayout(hLayout);
    mainLayout->addLayout(WidgetUtils::createOptional(useNickLevel, nickLevel));

>>>>>>> upstream/master
    mainLayout->addStretch(1);

    setLayout(mainLayout);
}

QString MinecraftPage::title() {
    return QStringLiteral("Minecraft");
}

QIcon MinecraftPage::icon() {
    return QIcon(":/res/icons/minecraft.svg");
}

void MinecraftPage::apply() {
    config.useCustomMinecraftDir = customMinecraftDir->isChecked();
    config.customMinecraftDir = minecraftPathChooser->getPath();

    config.joinServerOnLaunch = joinServerOnLaunch->isChecked();
    config.serverIp = serverToJoin->text();

    config.useLevelHeadPrefix = useLevelHeadPrefix->isChecked();
    config.levelHeadPrefix = levelHeadPrefix->text();

    config.useLevelHeadNick = useLevelHeadNick->isChecked();
    config.levelHeadNickLevel = levelHeadNickLevel->text().toInt();

    config.useAutoggMessage = useAutoggMessage->isChecked();
    config.autoggMessage = autoggMessage->text();

    config.useCosmetics = useCosmetics->isChecked();
    config.unlockCosmetics = unlockCosmetics->isChecked();

    config.windowWidth = windowWidth->value();
    config.windowHeight = windowHeight->value();
}

void MinecraftPage::load() {
    customMinecraftDir->setChecked(config.useCustomMinecraftDir);
    minecraftPathChooser->setPath(config.customMinecraftDir);

    joinServerOnLaunch->setChecked(config.joinServerOnLaunch);
    serverToJoin->setText(config.serverIp);

    useLevelHeadPrefix->setChecked(config.useLevelHeadPrefix);
    levelHeadPrefix->setText(config.levelHeadPrefix);

    useLevelHeadNick->setChecked(config.useLevelHeadNick);
    levelHeadNickLevel->setText(QString::number(config.levelHeadNickLevel));

    useAutoggMessage->setChecked(config.useAutoggMessage);
    autoggMessage->setText(config.autoggMessage);

    useCosmetics->setChecked(config.useCosmetics);
    unlockCosmetics->setChecked(config.unlockCosmetics);

    windowWidth->setValue(config.windowWidth);
    windowHeight->setValue(config.windowHeight);
}