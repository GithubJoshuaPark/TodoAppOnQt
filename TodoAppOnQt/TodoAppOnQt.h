#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TodoAppOnQt.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TodoAppOnQtClass; };
QT_END_NAMESPACE

class TodoAppOnQt : public QMainWindow
{
    Q_OBJECT

public:
    TodoAppOnQt(QWidget *parent = nullptr);
    ~TodoAppOnQt();

private slots:
    void on_addButton_clicked();
    void on_deleteButton_clicked();

private:
    Ui::TodoAppOnQtClass *ui;

    // --- ���⿡ �Լ� ������ �߰��ؾ� �մϴ� ---
    void saveTasksToFile();
    void loadTasksFromFile();
    QString getTodoFilePath() const;
    // ------------------------------------------
};

