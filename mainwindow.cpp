#include "mainwindow.h"
#include <QGridLayout>
#include <QPushButton>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentInput(""), currentOperation("")
{
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Left layout for calculator
    QVBoxLayout *leftLayout = new QVBoxLayout();

    display = new QLineEdit(this);
    display->setReadOnly(true);
    leftLayout->addWidget(display);

    QGridLayout *gridLayout = new QGridLayout();
    QStringList buttons = {"7", "8", "9", "/",
                           "4", "5", "6", "*",
                           "1", "2", "3", "-",
                           "C", "0", "=", "+"};

    for (int i = 0; i < buttons.size(); ++i) {
        QPushButton *button = new QPushButton(buttons[i], this);
        gridLayout->addWidget(button, i / 4, i % 4);
        connect(button, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    }

    leftLayout->addLayout(gridLayout);

    // Right layout for history
    QVBoxLayout *rightLayout = new QVBoxLayout();
    historyDisplay = new QTextEdit(this);
    historyDisplay->setReadOnly(true);
    rightLayout->addWidget(historyDisplay);

    // Add left and right layouts to the main layout
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    setCentralWidget(centralWidget);

    // Set focus policy to accept focus
    setFocusPolicy(Qt::StrongFocus);
    centralWidget->setFocusPolicy(Qt::StrongFocus);

    // Load history from file
    historyFile = new QFile("history.txt", this);
    loadHistory();
}

MainWindow::~MainWindow()
{
    delete historyFile;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    QString keyText = event->text();
    bool isOperation = false;
    if (currentInput.contains("+") || currentInput.right(currentInput.length() - 1).contains("-") || currentInput.contains("*") || currentInput.contains("/"))
        isOperation = true;

    if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        currentInput += keyText;
    } else if (key == Qt::Key_Plus) {
        if (isOperation) {
            // currentInput = currentInput.left(currentInput.length() - 1);
            return;
        }
        currentOperation = "+";
        currentInput += "+";
    } else if (key == Qt::Key_Minus) {
        if (isOperation) {
            // currentInput = currentInput.left(currentInput.length() - 1);
            return;
        }
        currentOperation = "-";
        currentInput += "-";

    } else if (key == Qt::Key_Asterisk) {
        if (isOperation) {
            // currentInput = currentInput.left(currentInput.length() - 1);
            return;
        }
        currentOperation = "*";
        currentInput += "*";

    } else if (key == Qt::Key_Slash) {
        if (isOperation) {
            // currentInput = currentInput.left(currentInput.length() - 1);
            return;
        }
        currentOperation = "/";
        currentInput += "/";

    } else if (key == Qt::Key_Enter || key == Qt::Key_Return) {
        calculate();
        return;
    } else if (key == Qt::Key_Backspace) {
        currentInput = currentInput.left(currentInput.length() - 1);
    }else if (key == Qt::Key_C) {
        currentInput.clear();
        currentOperation.clear();
    } else {
        return;
    }
    display->setText(currentInput);
}

void MainWindow::buttonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button)
        return;

    QString text = button->text();
    if (text == "C") {
        currentInput.clear();
        currentOperation.clear();
    } else if (text == "=") {
        calculate();
    } else {
        if (text == "+" || text == "-" || text == "*" || text == "/") {

            if (currentInput.contains("+") || currentInput.right(currentInput.length() - 1).contains("-") || currentInput.contains("*") || currentInput.contains("/"))
                currentInput = currentInput.left(currentInput.length() - 1);
            currentOperation = text;
        }
            currentInput += text;

    }
    display->setText(currentInput);
}

void MainWindow::calculate()
{
    if (currentOperation.isEmpty() || currentInput.isEmpty())
        return;

    double result = 0.0;
    QStringList parts;
    if (currentInput.startsWith("-")){
        parts = currentInput.right(currentInput.length() - 1).split(currentOperation);
        parts[0] = "-" + parts[0];
        }
    else{
        parts = currentInput.split(currentOperation);
        }
    if (parts.size() != 2)
        return;

    bool ok1, ok2;
    double num1 = parts[0].toDouble(&ok1);
    double num2 = parts[1].toDouble(&ok2);

    if (ok1 && ok2) {
        if (currentOperation == "+") {
            result = num1 + num2;
        } else if (currentOperation == "-") {
            result = num1 - num2;
        } else if (currentOperation == "*") {
            result = num1 * num2;
        } else if (currentOperation == "/") {
            if (num2 != 0) {
                result = num1 / num2;
            } else {
                display->setText("Error");
                return;
            }
        }
    } else {
        display->setText("Error");
        return;
    }
    QString sResult = QString::number(result);
    QString calculation = QString::number(num1) + currentOperation + QString::number(num2) + "=" + sResult;
    display->setText(sResult);
    currentInput = sResult;
    currentOperation.clear();

    saveHistory(calculation);
    historyDisplay->append(calculation);
}

void MainWindow::loadHistory()
{
    if (!historyFile->open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(historyFile);
    while (!in.atEnd()) {
        QString line = in.readLine();
        historyDisplay->append(line);
    }

    historyFile->close();
}

void MainWindow::saveHistory(const QString &calculation)
{
    if (!historyFile->open(QIODevice::Append | QIODevice::Text))
        return;

    QTextStream out(historyFile);
    out << calculation << "\n";

    historyFile->close();
}
