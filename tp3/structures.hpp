#pragma once
#include <string>
#include <memory>
#include <functional>
#include <cassert>
#include "gsl/span"
using namespace std;
using gsl::span;

struct Film;
struct Acteur;

class ListeFilms {
public:
    ListeFilms() = default;
    ListeFilms(const string& nomFichier);
    ListeFilms(const ListeFilms& l) = delete; // Suppression explicite
    ListeFilms& operator=(const ListeFilms&) = delete; // Suppression explicite de l'op�rateur d'assignation de copie
    ~ListeFilms();
    void ajouterFilm(Film* film);
    void enleverFilm(const Film* film);
    Acteur* trouverActeur(const string& nomActeur) const;
    //span<Film*> enSpan() const;
    int size() const { return nElements; }

private:
    void changeDimension(int nouvelleCapacite);

    int capacite = 0, nElements = 0;
    Film** elements = nullptr;
    bool possedeLesFilms_ = false;
};


struct ListeActeurs {
    int capacite = 0, nElements = 0;
    unique_ptr<shared_ptr<Acteur>[]> elements;

    // Constructeur par d�faut
    ListeActeurs() : capacite(0), nElements(0), elements(nullptr) {}

    // Constructeur avec un argument pour la capacit�
    ListeActeurs(int capacite_) : capacite(capacite_), nElements(0), elements(make_unique<shared_ptr<Acteur>[]>(capacite_)) {}
};

struct Film {
    string titre = "";
    string realisateur = "";
    int anneeSortie = 0;
    int recette = 0;
    ListeActeurs acteurs;

    Film() = default;
    Film(const Film&) = delete;
    Film& operator=(const Film&) = delete;
};

struct Acteur {
    string nom = "";
    int anneeNaissance = 0;
    char sexe = 'U';
//    ListeFilms joueDans;
};

