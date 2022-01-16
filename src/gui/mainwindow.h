//
// Created by nils on 11/4/21.
//

#ifndef LUNAR_CLIENT_QT_MAINWINDOW_H
#define LUNAR_CLIENT_QT_MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QComboBox>
#include <QStandardPaths>

#include "launch/offlinelauncher.h"
#include "launch/launcher.h"
#include "pages/configurationpage.h"
#include "pages/generalpage.h"
#include "pages/agentspage.h"
#include "pages/helperspage.h"
#include "config/config.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void launch();
    void closeEvent(QCloseEvent* closeEvent) override;
    void apply();
    void load();
private slots:
    void resetLaunchButtons();
    void errorCallback(const QString& message);
private:
    QListWidget* pageList;
    QStackedWidget* pageStack;
    QPushButton* launchButton;
    QComboBox* versionSelect;

    QList<ConfigurationPage*> pages;

    OfflineLauncher offlineLauncher;

    Config config;
};


#endif //LUNAR_CLIENT_QT_MAINWINDOW_H
