#include <QStyle>
#include <QTreeWidget>
#include <QSplitter>
#include <QTextEdit>
#include <QMenuBar>
#include <QStatusBar>
#include <QSettings>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include "dndclient.h"

MainWindow::MainWindow()
{
    QIcon folderIcon(style()->standardPixmap(QStyle::SP_DirClosedIcon));
    QIcon trashIcon(style()->standardPixmap(QStyle::SP_FileIcon));

    QStringList folderLabels;
    folderLabels << tr("Folders");

    //foldersTreeWidget = new QTreeWidget;
    foldersTreeWidget = new XbelTree;
    foldersTreeWidget->setHeaderLabels(folderLabels);
    foldersTreeWidget->setDragEnabled(true);
    foldersTreeWidget->setAcceptDrops(false);
    foldersTreeWidget->setDropIndicatorShown(true);
	
    QString filename=".\\frank.xbel";
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SAX Bookmarks"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(filename)
                             .arg(file.errorString()));
        return;
    }

    if (foldersTreeWidget->read(&file))
        statusBar()->showMessage(tr("File loaded"), 2000);
    /*
    addFolder(folderIcon, tr("Inbox"));
    addFolder(folderIcon, tr("Outbox"));
    addFolder(folderIcon, tr("Sent"));
    addFolder(trashIcon, tr("Trash"));
    */
    QStringList messageLabels;
    messageLabels << tr("Subject") << tr("Sender") << tr("Date");

    messagesTreeWidget = new QTreeWidget;
    messagesTreeWidget->setAcceptDrops(true);
    messagesTreeWidget->setHeaderLabels(messageLabels);
    addMessage(tr("Happy New Year!"),
               tr("Grace K. <grace@software-inc.com>"),
               tr("2006-12-31"));
    messagesTreeWidget->resizeColumnToContents(0);
    messagesTreeWidget->resizeColumnToContents(1);

    textEdit = new QTextEdit;
    textEdit->setReadOnly(true);
    
    statusBar()->showMessage(tr("No new messages on server"));

    rightSplitter = new QSplitter(Qt::Vertical);
    rightSplitter->addWidget(messagesTreeWidget);
    rightSplitter->addWidget(textEdit);
    rightSplitter->setStretchFactor(1, 1);

    mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->addWidget(foldersTreeWidget);
    mainSplitter->addWidget(rightSplitter);
    mainSplitter->setStretchFactor(1, 1);
    setCentralWidget(mainSplitter);

    setWindowTitle(tr("Experiment Client"));
    readSettings();

	createActions();
    createMenus();

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::addFolder(const QIcon &icon, const QString &name)
{
    QTreeWidgetItem *root;
    if (foldersTreeWidget->topLevelItemCount() == 0) {
        root = new QTreeWidgetItem(foldersTreeWidget);
        root->setText(0, tr("Mail"));
        foldersTreeWidget->setItemExpanded(root, true);
    } else {
        root = foldersTreeWidget->topLevelItem(0);
    }

    QTreeWidgetItem *newItem = new QTreeWidgetItem(root);
    newItem->setText(0, name);
    newItem->setIcon(0, icon);

    if (!foldersTreeWidget->currentItem())
        foldersTreeWidget->setCurrentItem(newItem);
}

void MainWindow::addMessage(const QString &subject, const QString &from,
                            const QString &date)
{
    QTreeWidgetItem *newItem = new QTreeWidgetItem(messagesTreeWidget);
    newItem->setText(0, subject);
    newItem->setText(1, from);
    newItem->setText(2, date);

    if (!messagesTreeWidget->currentItem())
        messagesTreeWidget->setCurrentItem(newItem);
}

void MainWindow::readSettings()
{
    QSettings settings("Software Inc.", "Mail Client");

    settings.beginGroup("mainWindow");
    resize(settings.value("size", QSize(480, 360)).toSize());
    mainSplitter->restoreState(
            settings.value("mainSplitter").toByteArray());
    rightSplitter->restoreState(
            settings.value("rightSplitter").toByteArray());
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings("Software Inc.", "Mail Client");

    settings.beginGroup("mainWindow");
    settings.setValue("size", size());
    settings.setValue("mainSplitter", mainSplitter->saveState());
    settings.setValue("rightSplitter", rightSplitter->saveState());
    settings.endGroup();
}


//menu and actions.

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAsAct = new QAction(tr("&Save"), this);
    saveAsAct->setShortcuts(QKeySequence::Save);
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(save()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::open()
{
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open Bookmark File"),
                                         QDir::currentPath(),
                                         tr("XBEL Files (*.xbel *.xml)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SAX Bookmarks"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    if (foldersTreeWidget->read(&file))
        statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::save()
{
    QString fileName =
            QFileDialog::getSaveFileName(this, tr("Save XML File"),
                                         QDir::currentPath(),
                                         tr("xml Files (*.xml)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SAX Bookmarks"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    if (foldersTreeWidget->write(&file))
        statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About DOM Bookmarks"),
                      tr("The <b>DOM Bookmarks</b> example demonstrates how to "
                         "use Qt's DOM classes to read and write XML "
                         "documents."));
}
