//
// Created by nils on 11/27/21.
//

#ifndef LUNAR_CLIENT_QT_LAUNCHER_H
#define LUNAR_CLIENT_QT_LAUNCHER_H

#include <QObject>
#include "config/config.h"

class Launcher : public QObject {
Q_OBJECT
public:

    explicit Launcher(const Config& config, QObject* parent);

    virtual void launch() = 0;

    static QString getAgentFlags(const QString& path, const QString& options);
protected:
    const Config& config;
};

#endif //LUNAR_CLIENT_QT_LAUNCHER_H
