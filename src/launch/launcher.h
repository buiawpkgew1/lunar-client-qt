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

    Launcher(const Config& config, QObject* parent);

    virtual bool launch() = 0;

    static QString getLevelHeadOptions(const bool& uselevelheadprefix, const QString& levelheadprefix, const bool& uselevelheadnicklevel, const QString& levelheadnicklevel);
protected:
    const Config& config;
};

#endif //LUNAR_CLIENT_QT_LAUNCHER_H
