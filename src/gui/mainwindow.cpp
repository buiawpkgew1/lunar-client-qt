//
// Created by nils on 11/4/21.
//

#include "mainwindow.h"

#include <QGridLayout>
#include <QListWidgetItem>
#include <QComboBox>
#include <QStatusBar>
#include <QScrollArea>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QMessageBox>
#include <QFileSystemModel>
#include <QIODevice>

#include "pages/configurationpage.h"
#include "pages/generalpage.h"
#include "pages/minecraftpage.h"
#include "launch/launcher.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), config(Config::load()), offlineLauncher(config){
    setWindowTitle(QStringLiteral("Lunar Client Qt"));
    static QString icon = QStringLiteral("icon.ico");
    if (QFile::exists(icon))
        setWindowIcon(QIcon(icon));
    else {
        QString lcloc =
#if defined(Q_OS_WIN)
            QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + QStringLiteral("/Programs/lunarclient/Lunar Client.exe");
#elif defined(Q_OS_DARWIN)
            QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/lunarclient/Lunar Client"; //Need location
#else
            QDir::homePath() + "/lunarclient/Lunar Client"; // Need location
#endif
        QFileInfo fin(lcloc);
        QFileSystemModel* model = new QFileSystemModel;
        QIcon ic = model->fileIcon(model->index(fin.filePath()));
        setWindowIcon(ic);
        QPixmap pixmap = ic.pixmap(ic.actualSize(QSize(128, 128)));
        QFile file(icon);
        file.open(QIODevice::WriteOnly);
        pixmap.save(&file, "ICO");
    }
    

    QWidget* centralWidget = new QWidget();

    QGridLayout* mainLayout = new QGridLayout();

    pageList = new QListWidget();
    pageStack = new QStackedWidget();

    pageStack->setContentsMargins(30, 10, 30, 10);

    pageList->setIconSize(QSize(32, 32));

    pages = {
        new GeneralPage(config),
        new MinecraftPage(config),
        new AgentsPage(config),
        new HelpersPage(config)
    };

    foreach(ConfigurationPage* page, pages){
        new QListWidgetItem(page->icon(), page->title(), pageList);
        pageStack->addWidget(page);
    }

    connect(pageList, &QListWidget::currentRowChanged, pageStack, &QStackedWidget::setCurrentIndex);

    pageList->setCurrentRow(0);
    pageList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pageList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    pageList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    QFont font;
    font.setPointSize(11);
    pageList->setFont(font);

    versionSelect = new QComboBox();
    versionSelect->addItems({"1.7", "1.8", "1.12", "1.16", "1.17", "1.18"});

    launchButton = new QPushButton();
    launchButton->setMinimumHeight(80);
    connect(launchButton, &QPushButton::clicked, this, &MainWindow::launch);

    connect(&offlineLauncher, &OfflineLauncher::error, this, &MainWindow::errorCallback);

    resetLaunchButtons();

    mainLayout->addWidget(pageList);
    mainLayout->addWidget(versionSelect, 1, 0);
    mainLayout->addWidget(launchButton, 4, 0);
    mainLayout->addWidget(pageStack, 0, 3, -1, 1);

    centralWidget->setLayout(mainLayout);

    setCentralWidget(centralWidget);
    resize(800, 600);

    load();
}

void MainWindow::resetLaunchButtons() {
    launchButton->setEnabled(true);
    launchButton->setText(QStringLiteral("Launch"));
}


void MainWindow::launch(){
    apply();
    offlineLauncher.launch();
    if(config.closeOnLaunch)
        close();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    apply();
    config.save();
    event->accept();
}

void MainWindow::apply() {
    foreach(ConfigurationPage* page, pages){
        page->apply();
    }
    config.gameVersion = versionSelect->currentText();
}

void MainWindow::load() {
    foreach(ConfigurationPage* page, pages){
        page->load();
    }
    versionSelect->setCurrentText(config.gameVersion);
}


void MainWindow::errorCallback(const QString &message) {
    QMessageBox messageBox;
    messageBox.setText(message);
    messageBox.exec();
}