#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void buttonClicked();
    void calculate();

private:
    void loadHistory();
    void saveHistory(const QString &calculation);

    QLineEdit *display;
    QTextEdit *historyDisplay;
    QString currentInput;
    QString currentOperation;
    QFile *historyFile;
};

#endif // MAINWINDOW_H
