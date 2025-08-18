#include <QApplication>
#include "view/MainWindow.h"
#include "view/LoginDialog.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Show login dialog first
    LoginDialog loginDialog;
    if (loginDialog.exec() != QDialog::Accepted) {
        return 0; // User cancelled login or failed to authenticate
    }
    
    // If login successful, show main window
    MainWindow window;
    window.show();
    
    return app.exec();
}

