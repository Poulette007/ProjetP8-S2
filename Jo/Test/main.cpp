#include "login.h"
#include "MainMenu.h"
#include <QApplication>
#include <QStackedWidget>
#include <QObject>
#include <QVBoxLayout>

void PageSuivante()
{

}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QWidget Jeu;
    QStackedWidget* AllPages = new QStackedWidget(&Jeu);

    login* LoginPage = new login();
    MainMenu* MenuPage = new MainMenu();

    AllPages->addWidget(LoginPage);
    AllPages->addWidget(MenuPage);

    QObject::connect(LoginPage->NextPage, &QPushButton::clicked, [AllPages]() {
       AllPages->setCurrentIndex(1);});


    return app.exec();
}

/*#include <QApplication>
#include <QStackedWidget>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Create a main widget
    QWidget mainWindow;

    // Create a QStackedWidget
    QStackedWidget* stackedWidget = new QStackedWidget(&mainWindow);

    // Create child widgets
    QWidget* page1 = new QWidget;
    QWidget* page2 = new QWidget;

    // Set up layouts for the pages (optional)
    QVBoxLayout* page1Layout = new QVBoxLayout(page1);
    page1Layout->addWidget(new QPushButton("Page 1"));

    QVBoxLayout* page2Layout = new QVBoxLayout(page2);
    page2Layout->addWidget(new QPushButton("Page 2"));

    // Add pages to QStackedWidget
    stackedWidget->addWidget(page1);
    stackedWidget->addWidget(page2);

    // Create buttons to switch pages
    QPushButton* switchToPage1 = new QPushButton("Switch to Page 1", &mainWindow);
    QPushButton* switchToPage2 = new QPushButton("Switch to Page 2", &mainWindow);

    // Connect buttons to switch pages
    QObject::connect(switchToPage1, &QPushButton::clicked, [stackedWidget]() {
        stackedWidget->setCurrentIndex(0); // Show page1
        });

    QObject::connect(switchToPage2, &QPushButton::clicked, [stackedWidget]() {
        stackedWidget->setCurrentIndex(1); // Show page2
        });

    // Layout for the main window
    QVBoxLayout* mainLayout = new QVBoxLayout(&mainWindow);
    mainLayout->addWidget(switchToPage1);
    mainLayout->addWidget(switchToPage2);
    mainLayout->addWidget(stackedWidget);

    mainWindow.setLayout(mainLayout);
    mainWindow.show();

    return app.exec();
}
*/