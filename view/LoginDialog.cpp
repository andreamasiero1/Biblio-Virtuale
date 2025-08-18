
#include "LoginDialog.h"
#include <QApplication>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), authenticator("biblioteca_virtuale/data/users.json") {
    setupUI();
    setStyleSheet();
    setWindowTitle("Biblioteca Virtuale - Login");
    setFixedSize(400, 300);
}

LoginDialog::~LoginDialog() {}

void LoginDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 30, 40, 30);

    // Logo/Title section
    titleLabel = new QLabel("Biblioteca Virtuale");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2c3e50; margin-bottom: 10px;");
    
    QLabel *subtitleLabel = new QLabel("Sistema di Gestione Media");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 14px; color: #7f8c8d; margin-bottom: 20px;");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);

    // Login form
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(15);

    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("Inserisci username");
    usernameEdit->setText("admin"); // Default username
    connect(usernameEdit, &QLineEdit::textChanged, this, &LoginDialog::onUsernameChanged);

    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("Inserisci password");
    passwordEdit->setText("admin"); // Default password
    connect(passwordEdit, &QLineEdit::textChanged, this, &LoginDialog::onPasswordChanged);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginDialog::attemptLogin);

    formLayout->addRow("Username:", usernameEdit);
    formLayout->addRow("Password:", passwordEdit);

    mainLayout->addLayout(formLayout);

    // Status label
    statusLabel = new QLabel("");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("color: #e74c3c; font-size: 12px;");
    mainLayout->addWidget(statusLabel);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    loginButton = new QPushButton("Accedi");
    cancelButton = new QPushButton("Annulla");

    loginButton->setDefault(true);
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::attemptLogin);
    connect(cancelButton, &QPushButton::clicked, this, &LoginDialog::reject);

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(loginButton);
    mainLayout->addLayout(buttonLayout);

    // Add some stretch at the bottom
    mainLayout->addStretch();
}

void LoginDialog::setStyleSheet() {
    QDialog::setStyleSheet(R"(
        QDialog {
            background-color: #ecf0f1;
        }
        QLineEdit {
            padding: 10px;
            border: 2px solid #bdc3c7;
            border-radius: 5px;
            font-size: 14px;
            background-color: white;
        }
        QLineEdit:focus {
            border-color: #3498db;
        }
        QPushButton {
            padding: 10px 20px;
            border: none;
            border-radius: 5px;
            font-size: 14px;
            font-weight: bold;
            min-width: 80px;
        }
        QPushButton#loginButton {
            background-color: #3498db;
            color: white;
        }
        QPushButton#loginButton:hover {
            background-color: #2980b9;
        }
        QPushButton#loginButton:pressed {
            background-color: #21618c;
        }
        QPushButton#cancelButton {
            background-color: #95a5a6;
            color: white;
        }
        QPushButton#cancelButton:hover {
            background-color: #7f8c8d;
        }
        QLabel {
            color: #2c3e50;
        }
    )");

    loginButton->setObjectName("loginButton");
    cancelButton->setObjectName("cancelButton");
}

void LoginDialog::attemptLogin() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        statusLabel->setText("Inserisci username e password");
        return;
    }

    if (authenticator.authenticate(username, password)) {
        statusLabel->setText("Login effettuato con successo!");
        statusLabel->setStyleSheet("color: #27ae60; font-size: 12px;");
        accept(); // Close dialog with accepted result
    } else {
        statusLabel->setText("Username o password non corretti");
        statusLabel->setStyleSheet("color: #e74c3c; font-size: 12px;");
        passwordEdit->clear();
        passwordEdit->setFocus();
    }
}

void LoginDialog::onUsernameChanged() {
    statusLabel->clear();
}

void LoginDialog::onPasswordChanged() {
    statusLabel->clear();
}

