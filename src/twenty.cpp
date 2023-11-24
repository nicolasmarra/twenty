#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

#define numBlocksPerRow 7
#define BLOCK_SIZE 0.18
#define MAX_BLOCK 56
#define N_BLOCK_INITIAL 21

// On définit un espacement entre les blocs
#define SPACING (1.6 / (numBlocksPerRow + 1))
// On définit également un espacement sur la vertical et l'horizontal (à gauche
// et en bas)

#define BACKGROUND_LEFT \
    (-0.8 + (1.6 - numBlocksPerRow * SPACING) / 2 + SPACING / 2)
#define BACKGROUND_BOTTOM (-0.8 + SPACING / 2)

struct Block {
    float x, y;
    int value;
    bool isVisible;
};

struct Color {
    float r, g, b;
    Color(float red, float green, float blue) : r(red), g(green), b(blue) {}
};

int nBlocks = N_BLOCK_INITIAL;
Block blocks[MAX_BLOCK];
int selectedBlockIndex = -1;
int score = 0, moves = 0;
int elapsedTime = 0;
bool stop_Time = false;

Color getColor(int valeur) {
    switch (valeur) {
    case 1:
        return Color(0.3, 0.8, 0.7); // bleu clair 1
    case 2:
        return Color(0.4, 0.3, 0.5); // violet foncé
    case 3:
        return Color(0.0, 0.0, 1.0); // bleu
    case 4:
        return Color(0.0, 0.5, 0.0); // vert
    case 5:
        return Color(1.0, 0.0, 0.0); // rouge
    case 6:
        return Color(0.0, 0.5, 0.5); // cyan
    case 7:
        return Color(0.5, 0.0, 0.5); // magenta
    case 8:
        return Color(0.5, 0.5, 0.0); // jaune
    case 9:
        return Color(0.5, 0.5, 0.5); // gris
    case 10:
        return Color(0.0, 1.0, 0.0); // vert vif
    case 11:
        return Color(1.0, 0.0, 1.0); // rose
    case 12:
        return Color(0.0, 1.0, 1.0); // turquoise
    case 13:
        return Color(1.0, 0.5, 0.0); // orange
    case 14:
        return Color(0.5, 1.0, 0.0); // vert clair
    case 15:
        return Color(0.0, 0.0, 0.5); // bleu foncé
    case 16:
        return Color(0.0, 0.5, 1.0); // bleu clair 2
    case 17:
        return Color(0.5, 0.0, 1.0); // violet
    case 18:
        return Color(1.0, 0.0, 0.5); // rouge clair
    case 19:
        return Color(0.5, 1.0, 0.5); // vert pâle
    case 20:
        return Color(0.5, 0.5, 1.0); // bleu pâle
    default:
        return Color(0.8, 0.0, 0.0); // rouge
    }
}

void isVictory() {
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(-0.2, 0.0);
    std::string victoryStr = "Vous avez gagné!";
    for (char c : victoryStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    stop_Time = true;

    // On désactive la souris et le déplacement des blocs
    glutMouseFunc(NULL);
    glutMotionFunc(NULL);
}

void new_score(int valeur) {
    score = valeur > score ? valeur : score;
    if (score == 20)
        isVictory();
}


void moveBlocks() {
    for (int i = 0; i < nBlocks; i++) {
        if (blocks[i].isVisible) {

            for (int j = 0; j < nBlocks; j++) {

            	Block tempBlock;
            	tempBlock.x = blocks[i].x;
            	tempBlock.y = blocks[i].y - SPACING;

            	if (!blocks[j].isVisible && (blocks[j].x == tempBlock.x && blocks[j].y == tempBlock.y))
            	{
            		float temp_x = blocks[i].x;
            		blocks[i].x = blocks[j].x;
            		float temp_y = blocks[i].y;
            		blocks[i].y = blocks[j].y;
            		blocks[j].x = temp_x;
            		blocks[j].y = temp_y;

            		Block tempBlock1 = blocks[i];
            		blocks[i] = blocks[j];
            		blocks[j] = tempBlock1;
            		break;
                }
            }
    }
    }
}



void initBlocks() {

    // Ensuite, on initialise le tableau de blocs
    for (int i = 0; i < N_BLOCK_INITIAL; ++i) {
        int row = i / numBlocksPerRow; // On récupère la ligne actuelle
        int col = i % numBlocksPerRow; // On récupère la colonne actuelle
        blocks[i].x = BACKGROUND_LEFT + SPACING * col; // On répartit les blocs sur l'écran horizontalement
        blocks[i].y = BACKGROUND_BOTTOM + SPACING * row; // On répartit les blocs sur l'écran verticalement en bas
        blocks[i].value = rand() % 5 + 1; // Initialement les blocs ont de valeurs allant de 1 à 5
        blocks[i].isVisible = true;
        new_score(blocks[i].value);
    }
}

int addBlocks() {

    if (nBlocks < MAX_BLOCK) {
        int row_nouveaux_blocs = 1;
        int compteur_ligne = 0;
        for (int i = N_BLOCK_INITIAL; i < nBlocks; i++) {

            blocks[i].y =
                BACKGROUND_BOTTOM +
                SPACING * row_nouveaux_blocs; // On répartit les blocs sur
                                              // l'écran verticalement en bas
            compteur_ligne++;

            if (compteur_ligne == numBlocksPerRow) {
                compteur_ligne = 0;
                row_nouveaux_blocs++;
            }
        }

        for (int i = 0; i < N_BLOCK_INITIAL; ++i) {
            // std::cout << "position en x: "<<blocks[i].x<<" position en y:
            // "<<blocks[i].y<<std::endl;
            int row = i / numBlocksPerRow;
            row += row_nouveaux_blocs;
            blocks[i].y = BACKGROUND_BOTTOM +
                          SPACING * row; // On répartit les blocs sur l'écran
                                         // verticalement en bas
        }

        for (int i = nBlocks; i < nBlocks + numBlocksPerRow; i++) {
            int row = 0;                   // On récupère la ligne actuelle
            int col = i % numBlocksPerRow; // On récupère la colonne actuelle
            blocks[i].x =
                BACKGROUND_LEFT +
                SPACING *
                    col; // On répartit les blocs sur l'écran horizontalement
            blocks[i].y = BACKGROUND_BOTTOM +
                          SPACING * row; // On répartit les blocs sur l'écran
                                         // verticalement en bas
            blocks[i].value =
                rand() % 5 +
                1; // Initialement les blocs ont de valeurs allant de 1 à 5
            blocks[i].isVisible = true;
            new_score(blocks[i].value);
        }
        nBlocks += numBlocksPerRow;
    } else {
        for (int i = 0; i < nBlocks; i++) {
            if (blocks[i].y == 0.5 && blocks[i].isVisible)
            {
            	std::cout << "Défaite " << std::endl;
            	return 0;
            }

        }
    }

    moveBlocks();
    return 1;
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // On convertit les coordonnées de la souris en coordonnées OpenGL
        float mouseX = (x / static_cast<float>(600)) * 2.0 - 1.0;
        float mouseY = 1.0 - (y / static_cast<float>(600)) * 2.0;

        // On cherche le bloc le plus proche du clic
        int closestBlockIndex = -1;
        float closestDistance = std::numeric_limits<float>::max();

        for (int i = 0; i < nBlocks; ++i) {
            if (blocks[i].isVisible) {
                float distance = sqrt(pow(mouseX - blocks[i].x, 2) +
                                      pow(mouseY - blocks[i].y, 2));
                if (distance < closestDistance) {
                    closestBlockIndex = i;
                    closestDistance = distance;
                }
            }
        }

        selectedBlockIndex = closestBlockIndex;
    }
}

void motion(int x, int y) {
    if (selectedBlockIndex != -1) {
        // std::cout << "Déplacement de la souris" << std::endl;
        // On Convertit les coordonnées de la souris en coordonnées OpenGL

        float mouseX = (x / static_cast<float>(600)) * 2.0 - 1.0;
        float mouseY = 1.0 - (y / static_cast<float>(600)) * 2.0;

        for (int i = 0; i < nBlocks; ++i) {
            if (i != selectedBlockIndex) {
                float distance = sqrt(pow(blocks[i].x - mouseX, 2) +
                                      pow(blocks[i].y - mouseY, 2));
                if (distance < 0.18) {
                    // On fusionne les blocs en ajoutant 1 à la valeur du bloc
                    // déplacé
                    if ((blocks[i].value == blocks[selectedBlockIndex].value) &&
                        blocks[i].isVisible) {
                        blocks[i].value += 1;
                        blocks[selectedBlockIndex].isVisible = false;
                        new_score(blocks[i].value);
                        moves++;
                    } else if (!blocks[i].isVisible) {
                        // On déplace le bloc
                        float temp_x = blocks[selectedBlockIndex].x;
                        float temp_y = blocks[selectedBlockIndex].y;
                        blocks[selectedBlockIndex].x = blocks[i].x;
                        blocks[selectedBlockIndex].y = blocks[i].y;
                        blocks[i].x = temp_x;
                        blocks[i].y = temp_y;
                        moves++;

                        // Mise à jour des pointeurs
                        Block tempBlock = blocks[selectedBlockIndex];
                        blocks[selectedBlockIndex] = blocks[i];
                        blocks[i] = tempBlock;

                    }
                    moveBlocks();
                    break;
                }
            }
        }

        glutSwapBuffers();
        glutPostRedisplay();

        // On réinitialiser l'index du bloc sélectionné
        selectedBlockIndex = -1;
    }
}

void displayInfo() {
    glColor3f(0.0, 0.0, 0.0);

    glRasterPos2f(-0.7, 0.82);
    std::string scoreStr = "Score: " + std::to_string(score);
    for (char c : scoreStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glRasterPos2f(0.4, 0.82);
    std::string movesStr = "Moves: " + std::to_string(moves);
    for (char c : movesStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glRasterPos2f(-0.7, 0.9);
    std::string tempsStr = "Temps: " + std::to_string(elapsedTime);
    for (char c : tempsStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // On applique un fond en blanc!
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(-1.0, -1.0);
    glVertex2f(1.0, -1.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(-1.0, 1.0);
    glEnd();

    // On applique ensuite un fond plus foncé pour notre champ de jeu
    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_QUADS);
    glVertex2f(-0.7, -0.8);
    glVertex2f(0.7, -0.8);
    glVertex2f(0.7, 0.8);
    glVertex2f(-0.7, 0.8);
    glEnd();

    for (int i = 0; i < nBlocks; ++i) {
        const Block &block = blocks[i];
        // On vérifie si le bloc est toujours visible
        if (block.isVisible) {
            // On récupère la couleur associée à la valeur du bloc
            Color blockColor = getColor(block.value);

            glBegin(GL_QUADS);
            glColor3f(blockColor.r, blockColor.g, blockColor.b);

            glVertex2f(block.x - BLOCK_SIZE / 2, block.y - BLOCK_SIZE / 2);
            glVertex2f(block.x + BLOCK_SIZE / 2, block.y - BLOCK_SIZE / 2);
            glVertex2f(block.x + BLOCK_SIZE / 2, block.y + BLOCK_SIZE / 2);
            glVertex2f(block.x - BLOCK_SIZE / 2, block.y + BLOCK_SIZE / 2);
            glEnd();

            // On applique une petite bordure noire en bas du bloc pour le
            // rendre plus beau
            glColor3f(0.0, 0.0, 0.0);
            glBegin(GL_LINES);
            glVertex2f(block.x - BLOCK_SIZE / 2, block.y - BLOCK_SIZE / 2);
            glVertex2f(block.x + BLOCK_SIZE / 2, block.y - BLOCK_SIZE / 2);
            glEnd();

            // Pour finir, on affiche la valeur associée au bloc
            glColor3f(1.0, 1.0, 1.0);
            glRasterPos2f(block.x - 0.02,
                          block.y -
                              0.02); // La position ou* on affichera la valeur
            std::string valueStr = std::to_string(block.value);
            for (char c : valueStr) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }
        }
    }

    displayInfo();

    glFlush();
}

void timer(int value) {

    if (!stop_Time) {
        elapsedTime++;

        if (elapsedTime == 15) {
            // addBlocks(nouvelle_ligne);
            addBlocks();
            elapsedTime = 0;
        }
    }
    // On configure le prochain appel à la fonction timer
    glutPostRedisplay();
    glutTimerFunc(1000, timer, 0);
}

int main(int argc, char **argv) {

    // on initialise FreeGLUT
    glutInit(&argc, argv);

    // on configure le display mode
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    // on configure la position de la fenêtre
    glutInitWindowPosition(200, 50);

    // on configure configure la taille de la fenêtre
    glutInitWindowSize(600, 600);

    // et on crée la fenêtre
    glutCreateWindow("TWENTY");

    // on définit la fonction display
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutTimerFunc(1000, timer, 0);

    // on initialise le générateur de la valeur aléatoire avec l'heure courante
    srand(time(nullptr));

    initBlocks(); // on initialise les blocs

    glutMainLoop();
    return 0;
}
