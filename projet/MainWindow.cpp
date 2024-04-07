#include "MainWindow.hpp"
#include <QGraphicsRectItem>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    setCentralWidget(view);
    drawBoard();

    // Supposons que vous avez déjà initialisé le tableau `board` avec des pièces
    // Dessiner les pièces sur l'échiquier
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            auto piece = board.getPiece(x, y);
            if (piece && *piece) {
                drawPiece(x, y, *piece);
            }
        }
    }
}

MainWindow::~MainWindow() {
    delete view;
    delete scene;
}

void MainWindow::drawBoard() {
    const int tileSize = 50; // Taille de la case en pixels

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            auto rect = scene->addRect(x * tileSize, y * tileSize, tileSize, tileSize);
            rect->setBrush((x + y) % 2 == 0 ? Qt::white : Qt::gray);
        }
    }
}

void MainWindow::drawPiece(int x, int y, const std::shared_ptr<Piece>& piece) {
    const int tileSize = 50; // Assurez-vous que cela correspond à la taille de la case utilisée dans drawBoard

    // Utilisez des images pour les pièces ou dessinez-les avec des formes/lettres
    // Remplacez le lien d'image par le chemin d'accès à vos ressources d'image
    // QGraphicsPixmapItem *item = scene->addPixmap(QPixmap("chemin/vers/limage.png"));
    // item->setPos(x * tileSize, y * tileSize);

    // Exemple simple avec des lettres pour les pièces
    auto text = scene->addText(QString::fromStdString(piece->getTitre()));
    text->setPos(x * tileSize, y * tileSize);
}