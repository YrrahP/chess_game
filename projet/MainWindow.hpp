#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "Board.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private:
    QGraphicsView* view;
    QGraphicsScene* scene;
    Board board; // Votre structure de donn�es d'�chiquier

    void drawBoard();
    void drawPiece(int x, int y, const std::shared_ptr<Piece>& piece);
};

#endif // MAINWINDOW_HPP