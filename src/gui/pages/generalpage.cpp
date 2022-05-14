//
// Created by nils on 11/5/21.
//

#include "generalpage.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QFileDialog>
#include <QGroupBox>
#include <QRadioButton>

#include "gui/widgets/filechooser.h"
#include "gui/widgets/widgetutils.h"

#ifndef _WIN32
#include <unistd.h>
unsigned long long getSystemMemory() {
	long pages = sysconf(_SC_PHYS_PAGES);
	long pageSize = sysconf(_SC_PAGE_SIZE);
	return pages * pageSize;
}
#else
#include <windows.h>
unsigned long long getSystemMemory() {
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	return status.ullTotalPhys;
}
#endif

GeneralPage::GeneralPage(Config& config, QWidget *parent) : ConfigurationPage(config, parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(40);

    keepMemorySame = new QCheckBox(QStringLiteral("保持初始和最大内存分配相同"));

    unsigned long long systemMemory = getSystemMemory();
    size_t mibMemory = (size_t)(systemMemory / 1024 / 1024);
    size_t pageStep = (size_t)(mibMemory / 16);

    QLabel* initialMemoryLabel = new QLabel();
    initialMemory = new QSlider(Qt::Horizontal);
    initialMemory->setMinimum(1024);
    initialMemory->setMaximum(mibMemory);
    initialMemory->setPageStep(pageStep);

    QLabel* maxMemoryLabel = new QLabel();
    maxMemory = new QSlider(Qt::Horizontal);
    maxMemory->setMinimum(1024);
    maxMemory->setMaximum(mibMemory);
    maxMemory->setPageStep(pageStep);

    //Memory slider functionality
    connect(initialMemory, &QSlider::valueChanged, [initialMemoryLabel](int val){initialMemoryLabel->setText("Initial Memory:  " + QString::number(val) + " MiB");});
    connect(maxMemory, &QSlider::valueChanged, [maxMemoryLabel](int val){maxMemoryLabel->setText("Maximum Memory:  " + QString::number(val) + " MiB");});

    connect(keepMemorySame, &QCheckBox::toggled, this, &GeneralPage::keepMinMaxSameChanged);

    //Memory slider group
    QVBoxLayout* memorySliderContainer = new QVBoxLayout();
    memorySliderContainer->setSpacing(6);

    memorySliderContainer->addWidget(initialMemoryLabel, 0, Qt::AlignHCenter);
    memorySliderContainer->addWidget(initialMemory);
    memorySliderContainer->addSpacing(30);
    memorySliderContainer->addWidget(maxMemoryLabel, 0, Qt::AlignHCenter);
    memorySliderContainer->addWidget(maxMemory);

    //Custom jre checkbox lineedit and button
    useCustomJre = new QCheckBox(QStringLiteral("使用自定义 JRE"));
    jrePath = new FileChooser(QFileDialog::ExistingFile);

    //Jvm arguments
    QVBoxLayout* jvmArgsGroup = new QVBoxLayout();
    jvmArgsGroup->setSpacing(6);

    jvmArgs = new QPlainTextEdit();

    jvmArgsGroup->addWidget(new QLabel(QStringLiteral("JVM 参数")), 0, Qt::AlignHCenter);
    jvmArgsGroup->addWidget(jvmArgs);

    //Checkboxes
    QGroupBox* groupBox = new QGroupBox(QStringLiteral("启动后"));

    QRadioButton* stayOpen = new QRadioButton(QStringLiteral("保持启动器打开"));
    closeOnLaunch = new QRadioButton(QStringLiteral("关闭启动器"));
    stayOpen->setChecked(true);

    QVBoxLayout* radioLayout = new QVBoxLayout();
    radioLayout->setSpacing(6);
    radioLayout->addWidget(stayOpen);
    radioLayout->addWidget(closeOnLaunch);
    groupBox->setLayout(radioLayout);


    mainLayout->addWidget(keepMemorySame, 0, Qt::AlignHCenter);
    mainLayout->addLayout(memorySliderContainer);
    mainLayout->addLayout(WidgetUtils::createOptional(useCustomJre, jrePath));
    mainLayout->addLayout(jvmArgsGroup, 1);
    mainLayout->addWidget(groupBox);


    setLayout(mainLayout);
}

QString GeneralPage::title() {
    return QStringLiteral("设置");
}

QIcon GeneralPage::icon() {
    return QIcon(":/res/icons/cog.svg");
}

void GeneralPage::apply() {
    config.keepMemorySame = keepMemorySame->isChecked();
    config.initialMemory = initialMemory->value();
    config.maximumMemory = maxMemory->value();

    config.useCustomJre = useCustomJre->isChecked();
    config.customJrePath = jrePath->getPath();

    config.jvmArgs = jvmArgs->toPlainText();

    config.closeOnLaunch = closeOnLaunch->isChecked();
}

void GeneralPage::load() {
    keepMemorySame->setChecked(config.keepMemorySame);

    initialMemory->setValue(config.initialMemory);
    maxMemory->setValue(config.maximumMemory);

    useCustomJre->setChecked(config.useCustomJre);
    jrePath->setPath(config.customJrePath);

    jvmArgs->setPlainText(config.jvmArgs);

    closeOnLaunch->setChecked(config.closeOnLaunch);
}


void GeneralPage::keepMinMaxSameChanged(bool checked) {
    if(checked){
        initialMemory->setValue(maxMemory->value());
        maxMemory->setDisabled(true);
        connect(initialMemory, &QSlider::valueChanged, maxMemory, &QSlider::setValue, Qt::UniqueConnection);
    }else{
        maxMemory->setEnabled(true);
        disconnect(initialMemory, &QSlider::valueChanged, maxMemory, &QSlider::setValue);
    }
}

QString GeneralPage::description() {
    return "Java和启动器设置.";
}
