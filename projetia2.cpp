#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include "list.h"
#include "board.h"

enum Difficulty { EASY = 0, MEDIUM, HARD };

class Menu {
public:
    Menu(float width, float height);
    void draw(sf::RenderWindow &window);
    int handleClick(sf::Vector2i mousePos);

private:
    sf::Font font;
    sf::Text menu[3];
    sf::RectangleShape borders[3];
    const std::string labels[3] = { "Facile", "Moyen", "Difficile" };

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
};

Menu::Menu(float width, float height) {
    if (!font.loadFromFile("arial.ttf")) {
        // Handle error
        std::cerr << "Error loading font\n";
    }

    if (!backgroundTexture.loadFromFile("menufond.jpg")) {
        // Handle error
        std::cerr << "Error loading background image\n";
    }

    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        width / backgroundSprite.getGlobalBounds().width,
        height / backgroundSprite.getGlobalBounds().height
    );

    for (int i = 0; i < 3; i++) {
        menu[i].setFont(font);
        menu[i].setFillColor(sf::Color::White);
        menu[i].setString(labels[i]);
        menu[i].setPosition(sf::Vector2f(width / 2 - menu[i].getGlobalBounds().width / 2, height / (3 + 1) * (i + 1)));

        // Set up the borders
        borders[i].setSize(sf::Vector2f(menu[i].getGlobalBounds().width + 20, menu[i].getGlobalBounds().height + 20));
        borders[i].setOutlineColor(sf::Color::White);
        borders[i].setOutlineThickness(2);
        borders[i].setFillColor(sf::Color::Transparent);
        borders[i].setPosition(menu[i].getPosition().x - 10, menu[i].getPosition().y - 10);
    }
}

void Menu::draw(sf::RenderWindow &window) {
    window.draw(backgroundSprite);
    for (int i = 0; i < 3; i++) {
        window.draw(borders[i]);
        window.draw(menu[i]);
    }
}

int Menu::handleClick(sf::Vector2i mousePos) {
    for (int i = 0; i < 3; i++) {
        if (borders[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            return i;
        }
    }
    return -1; // le joueur clique pas
}

int crea_menu() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Menu SFML");
    Menu menu(window.getSize().x, window.getSize().y);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int result = menu.handleClick(sf::Mouse::getPosition(window));
                if (result != -1) {
                    std::cout << "Difficulty selected: " << result << std::endl;
                    window.close(); //une fois choisis un peut quitter le menu
                    return result;
                }
            }
        }

        window.clear();
        menu.draw(window);
        window.display();
    }

    return 0;
}


// fin menu

list_t openList_p;
list_t closedList_p;
int positions[100]; // Adjust the size as needed
int position_count = 0;

void collectSolutionPath(Item *goal) {
  while (goal) {
    // Find and record the position of '99' in the board
    for (int pos = 0; pos < MAX_BOARD; pos++) {
      if (goal->board[pos] == 99) {
        positions[position_count++] = pos;
      }
    }
    goal = goal->parent;
  }
}

void showSolution(Item *goal) {
  int i = 0;

  printf("\nSolution:");

  collectSolutionPath(goal);

  // Print boards from the start to the end of the solution
  for (i = position_count - 1; i >= 0; i--) {
    printBoard(goal);
    goal = goal->parent;
  }

  printf("\nLength of the solution = %d\n", position_count - 1);
  printf("Size of open list = %d\n", openList_p.numElements);
  printf("Size of closed list = %d\n", closedList_p.numElements);

  // Print recorded positions in correct order
  printf("Positions of 99:\n");
  for (int j = position_count - 1; j >= 0; j--) {
    printf("%d; ", positions[j]);
  }
  printf("\n");
}


void ucs(void) {
    Item *cur_node, *child_p;
    int i;

    while (listCount(&openList_p) != 0) { // Tant qu'il y a des éléments dans la liste ouverte
        // Obtenir le meilleur élément de la liste ouverte
        cur_node = popBest(&openList_p);
        // Avons-nous une solution?
        if (evaluateBoard(cur_node) == 0.0) {
            showSolution(cur_node);
            return;
        }

        // Ajouter le nœud actuel à la liste fermée
        addFirst(&closedList_p, cur_node);

        // Énumérer les états adjacents
        for (int i = 0; i < MAX_BOARD; i++) {
            child_p = getChildBoard(cur_node, i);
            if (child_p != NULL) { // C'est un enfant valide !
                // Ignorer cet enfant s'il a déjà été visité
                if (onList(&closedList_p, child_p->board)==NULL) {
                    // Ajouter le nœud enfant à la liste ouverte
                    addFirst(&openList_p, child_p);
                } else {
                    freeItem(child_p); // Libérer la mémoire si l'enfant a déjà été visité
                }
            }
        }
    }

    printf("No solution found!\n");
}
int main() {
	// init lists
  initList(&openList_p);
  initList(&closedList_p);


  printf("\nInitial:");
  Item *initial_state = initGame();


  addLast( &openList_p, initial_state );

  //bfs();
	//printf("Finished!\n");

  ucs();
  
  printf("Finished!\n");
  int difficulte = crea_menu();

    //VARIABLES DE DEPART
    int taille_init = WH_BOARD;
    int Taille_fenetre = (taille_init*taille_init);
    int pos_joueur = WH_BOARD+1;
    const int pos_joueur_init = pos_joueur;
    int pos_joueur_x=(pos_joueur % taille_init) * taille_init;
    int pos_joueur_y=(pos_joueur / taille_init) * taille_init;
    int pos_fin = (WH_BOARD-2)*WH_BOARD+ WH_BOARD-2;
    int pos_fin_x=pos_fin % taille_init;
    int pos_fin_y=pos_fin / taille_init;
    int pos_ia = pos_fin;
    int pos_ia_x=(pos_ia % taille_init) * taille_init;
    int pos_ia_y=(pos_ia / taille_init) * taille_init;
    char MUR = -1; // changer si les murs ne sont pas représentés par des 0

    // Création de la fenêtre SFML
    sf::RenderWindow window2(sf::VideoMode(390, 50), "Chronometre");
    sf::RenderWindow window(sf::VideoMode(Taille_fenetre, Taille_fenetre), "Labyrinthe"); //ne pas changer l'ordre sinon le reste des events ne marche pas trop !

    window.setSize(sf::Vector2u(Taille_fenetre*4, Taille_fenetre*4)); //pour grandir la fenètre et que tout "scale" avec comme les tailles pos etc
    window.setPosition(sf::Vector2i(10,50)); //censé mettre la fenètre a un endroit convenable mais ne marche pas sous windows d'après stackoverflow 

    sf::Clock clock;
    sf::Clock clock2;
    sf::Clock clock_ia;
    // décla des variables qui aident a ne pas cliquer 150 fois de suite sur un boutton et pour pas que l'ia trouve trop vite
    float elapsedTime = 0.0f;
    float elapsedTime_ia = 0.0f;
    const float moveInterval = 0.2f; // Interval de déplacement en secondes joueur
    float moveInterval_ia=0.0f;
    if (difficulte==0)
    {
        moveInterval_ia = 1.0f;
    }
    if (difficulte==1)
    {
        moveInterval_ia = 0.7f;
    }
    if (difficulte==2)
    {
        moveInterval_ia = 0.4f;
    }

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) { //penser a télécharger le font et a le mettre dans le projet
        return EXIT_FAILURE;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, 10);
    //fin création du chrono

    // Charger les textures
    sf::Texture texture;
    if (!texture.loadFromFile("mur_jeu.png")) { //mur
        std::cerr << "Erreur de chargement de l'image" << std::endl;
        return -1;
    }
    sf::Texture texture_joueur;
    if (!texture_joueur.loadFromFile("face.png")) { //joueur
        std::cerr << "Erreur de chargement de l'image" << std::endl;
        return -1;
    }
    sf::Texture texture_ia;
    if (!texture_ia.loadFromFile("face_2.png")) { //joueur
        std::cerr << "Erreur de chargement de l'image" << std::endl;
        return -1;
    }

    // Création d'un vecteur de points
    std::vector<sf::Vector2f> points;// (largeur, hauteur)
    // Génération des murs a partir du tableau de remplissage
    for (int i = 0; i < Taille_fenetre-1; ++i) {
        if (initial_state->board[i] == MUR || i==Taille_fenetre) //si y'a un mur
        {        
        int x = (i % taille_init) * taille_init; // Coordonnée x
        int y = (i / taille_init) * taille_init; // Coordonnée y
        points.push_back(sf::Vector2f(x, y)); // Ajout du point au vecteur
        }
    }
    points.push_back(sf::Vector2f(110, 110)); //dernier point pas fait sinon on sors du board

    //Création du joueur
    std::vector<sf::Vector2f> joueur = {
        sf::Vector2f(pos_joueur_x, pos_joueur_y)
    };

    //Création de L'IA
    std::vector<sf::Vector2f> IA = {
        sf::Vector2f(pos_ia_x, pos_ia_y)
    };
    int incre;
    if(WH_BOARD==11){
    	incre=17;}
    else
    	 incre=12; //sert a parcourir le tableau de l'IA

    //Création de la fin
    std::vector<sf::Vector2f> FIN = {
        sf::Vector2f(pos_fin_x, pos_fin_y)
    };


    // Boucle principale du jeu
    while (window.isOpen()) {
        // Gestion des événements
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
                if (pos_joueur==pos_fin || pos_ia==pos_joueur_init)
                {
                    std::cout << "fin" << std::endl;
                    window.close();
                    window2.close();
                }
                

                elapsedTime_ia += clock_ia.restart().asSeconds();
                if (elapsedTime_ia >= moveInterval_ia) //si l'ia a le droit de bouger alors elle bouge
                {
                    int parcour = 0;
                    incre--;
                    pos_ia = positions[incre];
                    std::cout << pos_ia_x << "pos ia" << "  et valeur tab pos = " << pos_ia << std::endl;
                    parcour++;

                    //on recalcule les nouvelles positions de l'IA
                    pos_ia_x=(pos_ia % taille_init) * taille_init;
                    pos_ia_y=(pos_ia / taille_init) * taille_init;
                    elapsedTime_ia= 0.0f; //on reset le temps d'attente
                }
                
                elapsedTime += clock.restart().asSeconds();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && elapsedTime >= moveInterval && initial_state->board[pos_joueur-taille_init]!=MUR)
                {
                    std::cout << "La touche Z est pressée ! " <<std::endl;
                    pos_joueur_y=pos_joueur_y-taille_init;
                    pos_joueur=pos_joueur-taille_init; //cette ligne sert pour les collisions
                    elapsedTime = 0.0f; // on reset le timer pour le prochain appuis de touche
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && elapsedTime >= moveInterval && initial_state->board[pos_joueur-1]!=MUR)
                {
                    std::cout << "La touche Q est pressée ! " << std::endl;
                    pos_joueur_x=pos_joueur_x-taille_init;
                    pos_joueur=pos_joueur-1;
                    elapsedTime = 0.0f; // on reset le timer pour le prochain appuis de touche
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && elapsedTime >= moveInterval && initial_state->board[pos_joueur+taille_init]!=MUR)
                {
                    std::cout << "La touche S est pressée !" << std::endl;
                    pos_joueur_y=pos_joueur_y+taille_init;
                    pos_joueur=pos_joueur+taille_init;
                    elapsedTime = 0.0f; // on reset le timer pour le prochain appuis de touche
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && elapsedTime >= moveInterval && initial_state->board[pos_joueur+1]!=MUR)
                {
                    std::cout << "La touche D est pressée !" << std::endl;
                    pos_joueur_x=pos_joueur_x+taille_init;
                    pos_joueur=pos_joueur+1;
                    elapsedTime = 0.0f; // on reset le timer pour le prochain appuis de touche
                   
                }
        // Effacer l'écran
        window.clear();

        // Dessiner les murs
        for (const auto& point : points) { //point sera le pointeur a un élément du vecteur points
            sf::RectangleShape Rectangle(sf::Vector2f(taille_init, taille_init));
            Rectangle.setTexture(&texture);
            Rectangle.setPosition(point);
            window.draw(Rectangle);
        }
        // Dessiner le joueur
        for (const auto& point : joueur) {
            sf::RectangleShape Rectangle(sf::Vector2f(taille_init, taille_init));
            //Rectangle.setFillColor(sf::Color::Blue);
            Rectangle.setTexture(&texture_joueur);
            Rectangle.setPosition(pos_joueur_x, pos_joueur_y);
            window.draw(Rectangle);
        }
        // Dessiner l'IA
        for (const auto& point : IA) {
            sf::RectangleShape Rectangle(sf::Vector2f(taille_init, taille_init));
            Rectangle.setTexture(&texture_ia);
            Rectangle.setPosition(pos_ia_x, pos_ia_y);
            window.draw(Rectangle);
        }

        // Dessiner le point de fin
        /*
        for (const auto& point : FIN) {
            sf::RectangleShape Rectangle(sf::Vector2f(taille_init, taille_init));
            Rectangle.setFillColor(sf::Color::White);
            Rectangle.setPosition(point);
            window.draw(Rectangle);
        }
        */

        // Chronomètre pour la deuxième fenêtre
        std::ostringstream ss;
        ss << "Temps ecoule : " << clock2.getElapsedTime().asSeconds() << " secondes";
        text.setString(ss.str());

        // Effacer l'écran de la deuxième fenêtre
        window2.clear();

        // Dessiner le texte sur la deuxième fenêtre
        window2.draw(text);

        // Afficher la deuxième fenêtre
        window2.display();
        window.display();
    }


    //partie écran de victoire ou défaite 
    sf::RenderWindow window3(sf::VideoMode( 800, 600),"Fin");
    // Création du texte "VICTOIRE"
    sf::Text text_vic("VICTOIRE", font, 100);
    text_vic.setFillColor(sf::Color::Red);
    text_vic.setStyle(sf::Text::Bold);
    // Centrer le texte
    sf::FloatRect textRect = text_vic.getLocalBounds();
    text_vic.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text_vic.setPosition(sf::Vector2f(window3.getSize().x / 2.0f, window3.getSize().y / 2.0f));


    // Création du texte "Défaite"
    sf::Text text_def("DEFAITE", font, 100);
    text_def.setFillColor(sf::Color::Red);
    text_def.setStyle(sf::Text::Bold);
    // Centrer le texte
    sf::FloatRect textRect_def = text_def.getLocalBounds();
    text_def.setOrigin(textRect_def.left + textRect_def.width / 2.0f,
                   textRect_def.top + textRect_def.height / 2.0f);
    text_def.setPosition(sf::Vector2f(window3.getSize().x / 2.0f, window3.getSize().y / 2.0f));



    while (window3.isOpen()) {
        sf::Event event;
        while (window3.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window3.close();
        }
        window3.clear();
        if (pos_joueur==pos_fin)
        {
            window3.draw(text_vic);
        }else if (pos_ia==pos_joueur_init)
        {
            window3.draw(text_def);
        }
        window3.display();
    }
    return 0;

  //ucs();
  //printf("Finished!\n");

  //dfs();
  //printf("Finished!\n");
	// clean lists
	cleanupList( &openList_p );
  cleanupList( &closedList_p );


  return 0;
}
