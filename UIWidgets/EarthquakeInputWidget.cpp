#include "EarthquakeInputWidget.h"
#include "VisualizationWidget.h"
#include "UserInputGMWidget.h"
#include "GMWidget.h"
#include "ShakeMapWidget.h"
#include "WorkflowAppRDT.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QStackedWidget>

EarthquakeInputWidget::EarthquakeInputWidget(QWidget *parent, VisualizationWidget* visWidget, RandomVariablesContainer * RVContainer) : SimCenterAppWidget(parent), theRandomVariablesContainer(RVContainer), theVisualizationWidget(visWidget)
{
    theEQWidget = nullptr;
    theRootStackedWidget = nullptr;
    theShakeMapWidget = nullptr;
    theEQSSWidget = nullptr;
    theUserInputGMWidget = nullptr;
}


EarthquakeInputWidget::~EarthquakeInputWidget()
{

}


QWidget* EarthquakeInputWidget::getEarthquakesWidget(void)
{
    if(theEQWidget == nullptr)
        this->createEarthquakesWidget();

    return theEQWidget;
}



void EarthquakeInputWidget::createEarthquakesWidget(void)
{
    theEQWidget = new QWidget(this);

    QGridLayout* gridLayout = new QGridLayout();
    theEQWidget->setLayout(gridLayout);

    auto smallVSpacer = new QSpacerItem(0,10);

    QLabel* selectionText = new QLabel();
    selectionText->setText("Earthquake Hazard Type:");

    auto earthquakeSelectionCombo = new QComboBox();
    earthquakeSelectionCombo->addItem("Earthquake Scenario Simulation");
    earthquakeSelectionCombo->addItem("ShakeMap Input");
    earthquakeSelectionCombo->addItem("User Specified Ground Motions");
    earthquakeSelectionCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    connect(earthquakeSelectionCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(handleEQTypeSelection(QString)));

    theRootStackedWidget = new QStackedWidget();

    // Add a vertical spacer at the bottom to push everything up
    auto vspacer = new QSpacerItem(0,0,QSizePolicy::Minimum, QSizePolicy::Expanding);
    auto hspacer = new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(smallVSpacer,0,0);
    gridLayout->addWidget(selectionText,1,0);
    gridLayout->addWidget(earthquakeSelectionCombo,1,1);
    gridLayout->addItem(hspacer,1,2);
    gridLayout->addWidget(theRootStackedWidget,2,0,1,3);
    gridLayout->addItem(vspacer, 3, 0,1,3);

    theEQSSWidget = new GMWidget(this, theVisualizationWidget);
    theShakeMapWidget = new ShakeMapWidget(theVisualizationWidget);
    theUserInputGMWidget = new UserInputGMWidget(theVisualizationWidget);

    connect(theShakeMapWidget, &ShakeMapWidget::loadingComplete, this, &EarthquakeInputWidget::shakeMapLoadingFinished);

    theRootStackedWidget->addWidget(theEQSSWidget);
    theRootStackedWidget->addWidget(theShakeMapWidget->getShakeMapWidget());
    theRootStackedWidget->addWidget(theUserInputGMWidget->getUserInputGMWidget());

    theRootStackedWidget->setCurrentWidget(theEQSSWidget);
}


void EarthquakeInputWidget::shakeMapLoadingFinished(const bool value)
{
    if(!value)
        return;

    // Shift the focus to the visualization widget
    auto mainWindowWidget = qobject_cast<WorkflowAppRDT*>(this->parent());

    if(!mainWindowWidget)
        return;

    mainWindowWidget->setActiveWidget(theVisualizationWidget);

}


void EarthquakeInputWidget::handleEQTypeSelection(const QString& selection)
{
    if(selection == "Earthquake Scenario Simulation")
        theRootStackedWidget->setCurrentWidget(theEQSSWidget);
    else if(selection == "ShakeMap Input")
        theRootStackedWidget->setCurrentWidget(theShakeMapWidget->getShakeMapWidget());
    else if(selection == "User Specified Ground Motions")
        theRootStackedWidget->setCurrentWidget(theUserInputGMWidget->getUserInputGMWidget());


}
