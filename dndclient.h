#ifndef dndclient_H
#define dndclient_H

#include <QtWidgets/QMainWindow>
#include <QStyle>
class QIcon;
class QSplitter;
class QTextEdit;
class QTreeWidget;
#include "xbeltree.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void open();
    void save();
    void about();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void addFolder(const QIcon &icon, const QString &name);
    void addMessage(const QString &subject, const QString &from,
                    const QString &date);
    void readSettings();
    void writeSettings();

    QSplitter *mainSplitter;
    QSplitter *rightSplitter;
    XbelTree  *foldersTreeWidget;
    QTreeWidget *messagesTreeWidget;
    QTextEdit *textEdit;
//Menu And Action
private:
    void createActions();
    void createMenus();

	QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *openAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

};

#endif
