// nim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/gl.h>
#include <FL/glu.h>
#include <FL/glut.h>
#include <iostream>
#include <vector>
#include <time.h>
#include "TGALoad.h"

static const int MAX_SLOTS = 12;
static const GLfloat startColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f }; // yellow
static const GLfloat playerColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // blue
static const GLfloat computerColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f }; // red
static const GLfloat unusedColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // black

class CButton : public Fl_Button
{
public:
	enum ButtonType {
		Quit,
		NewGame,
		Restart
	};

private:
	ButtonType m_type;
public:
	void setType(ButtonType m_type) { m_type = m_type; }
	ButtonType getType() { return(m_type); }

	void draw() {
		//std::cout << "butt draw\n" << std::flush;
		Fl_Button::draw();
	}

public:
	CButton(int l, int t, int w, int h, const char* label, ButtonType type) : Fl_Button(l, t, w, h, label) {
		setType(type);
	}
};

class CBox
{
private:
	int m_w;
	int m_h;
	int m_x;
	int m_y;
	int m_slot;
	int m_pile;
	Fl_JPEG_Image* m_jpg;
	GLuint m_texName;
	bool bGLInit;
public:
	GLfloat m_color[4];
	void setColor(const GLfloat color[4]) { m_color[3] = color[3];
									  m_color[2] = color[2];
									  m_color[1] = color[1];
									  m_color[0] = color[0];
	}
	void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
			m_color[3] = a;
			m_color[2] = b;
			m_color[1] = g;
			m_color[0] = r;
	}
public:
	void initGL() {
/*		GLenum errCode;
		const GLubyte *errString;
		
		//glEnable( GL_TEXTURE_2D );
		if ((errCode = glGetError()) != GL_NO_ERROR) {
			errString = gluErrorString(errCode);
			std::cout <<  "OpenGL Error: (glEnable) " << errString << "\n" << std::flush;
		}
		else {
			std::cout <<  "OpenGL OK: glEnable\n" << std::flush;
		}
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if ((errCode = glGetError()) != GL_NO_ERROR) {
			errString = gluErrorString(errCode);
			std::cout <<  "OpenGL Error: (glTexParameterf) " << errString << "\n" << std::flush;
		}
		else {
			std::cout <<  "OpenGL OK: glTexParameterf\n" << std::flush;
		}
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		if ((errCode = glGetError()) != GL_NO_ERROR) {
			errString = gluErrorString(errCode);
			std::cout <<  "OpenGL Error: (glTexParameterf) " << errString << "\n" << std::flush;
		}
		else {
			std::cout <<  "OpenGL OK: glTexParameterf\n" << std::flush;
		}

		glGenTextures(1, &m_texName);
		if ((errCode = glGetError()) != GL_NO_ERROR) {
			errString = gluErrorString(errCode);
			std::cout <<  "OpenGL Error: (glGenTextures) " << errString << "\n" << std::flush;
		}
		else {
			std::cout <<  "OpenGL OK: glGenTextures\n" << std::flush;
		}
		if (m_jpg != NULL) {
			glTexImage2D(GL_TEXTURE_2D, 0, 4, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_jpg->data());
		}
		
		if ((errCode = glGetError()) != GL_NO_ERROR) {
			errString = gluErrorString(errCode);
			std::cout <<  "OpenGL Error: (glTexImage2D) " << errString << "\n" << std::flush;
		}
		else {
			std::cout <<  "OpenGL OK: glTexImage2D\n" << std::flush;
		}
*/
		bGLInit = true;
	}

	void draw() {
		GLenum errCode;
		const GLubyte *errString;

		//std::cout << "gl draw\n" << std::flush;
		
		if (!bGLInit) {
			initGL();
		}

		//Fl_Box::draw();
		// widget that will contain image
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/*if ((errCode = glGetError()) != GL_NO_ERROR) {
			errString = gluErrorString(errCode);
			std::cout <<  "OpenGL Error: (glClear) " << errString << "\n" << std::flush;
		}
		else {
			std::cout <<  "OpenGL OK: glClear\n" << std::flush;
		}*/

		//glEnable( GL_TEXTURE_2D );
		/*if ((errCode = glGetError()) != GL_NO_ERROR) {
			errString = gluErrorString(errCode);
			std::cout <<  "OpenGL Error: (glEnable) " << errString << "\n" << std::flush;
		}
		else {
			std::cout <<  "OpenGL OK: glEnable\n" << std::flush;
		}*/

		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
		/*if ((errCode = glGetError()) != GL_NO_ERROR) {
			errString = gluErrorString(errCode);
			std::cout <<  "OpenGL Error: (glTexEnvf) " << errString << "\n" << std::flush;
		}
		else {
			std::cout <<  "OpenGL OK: glTexEnvf\n" << std::flush;
		}*/

		//glBindTexture(GL_TEXTURE_2D, m_texName);
		/*if ((errCode = glGetError()) != GL_NO_ERROR) {
			errString = gluErrorString(errCode);
			std::cout <<  "OpenGL Error: (glBindTexture) " << errString << "\n" << std::flush;
		}
		else {
			std::cout <<  "OpenGL OK: glBindTexture\n" << std::flush;
		}*/

		glBegin(GL_POLYGON);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glTexCoord2d(0.0,0.0);glVertex3i( m_x, m_y, 0);
		glTexCoord2d(0.0,1.0);glVertex3i( m_x+m_w, m_y, 0);
		glTexCoord2d(1.0,1.0);glVertex3i( m_x+m_w, m_y+m_h, 0);
		glTexCoord2d(1.0,0.0);glVertex3i( m_x, m_y+m_h, 0);
		glEnd();
		glBegin(GL_POLYGON);
		glColor4fv(m_color);
		glTexCoord2d(0.0,0.0);glVertex3f( m_x+1.0f, m_y+1.0f, 0.0f);
		glTexCoord2d(0.0,1.0);glVertex3f( m_x+m_w-1.0f, m_y+1.0f, 0.0f);
		glTexCoord2d(1.0,1.0);glVertex3f( m_x+m_w-1.0f, m_y+m_h-1.0f, 0.0f);
		glTexCoord2d(1.0,0.0);glVertex3f( m_x+1.0f, m_y+m_h-1.0f, 0.0f);
		glEnd();

		glFlush();
		//glDisable(GL_TEXTURE_2D);

		//std::cout << "out gl draw\n" << std::flush;
	}

	bool HitTest(int x, int y) {
		return ((x >= m_x) && (x <= m_x+m_w) && (y >= m_y) && (y <= m_y+m_h)); 
	}

public:
	CBox(int l, int t, int w, int h, const GLfloat color[4]) {
		GLenum errCode;
		const GLubyte *errString;

		bGLInit = false;
		m_x = l;
		m_y = t;
		m_w = w;
		m_h = h;
		m_color[0] = color[0];
		m_color[1] = color[1];
		m_color[2] = color[2];
		m_color[3] = color[3];
		//m_jpg = new Fl_JPEG_Image("matchstick.jpg");
	}

	void setSlot(int slot) { m_slot = slot; }
	int getSlot() { return(m_slot); }
	void setPile(int pile) { m_pile = pile; }
	int getPile() { return(m_pile); }
};

// OPENGL WINDOW CLASS
class MyGlWindow : public Fl_Gl_Window {
public:
	int startX;
	int startY;
	int width;
	int height;
    int m_numPiles;
	int m_maxMatchSticksPerPile;
	typedef std::vector<CBox*> Pile1d;
	typedef std::vector<Pile1d> Pile2d;
	Pile2d m_unusedPiles;
	Pile2d m_piles;
	Pile2d m_removedPiles;
	int* m_startPileCount;
	bool m_bComputerWinning;
	Texture texture;

public:
	void output(int x, int y, float r, float g, float b, char *string) {
		glColor3f( r, g, b );
		glRasterPos2f(x, y);
		int len, i;
		len = (int)strlen(string);
		for (i = 0; i < len; i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
		}
	}

	CBox* findHit(int x, int y) {
		Pile1d::iterator itr1d;
		Pile2d::iterator itr2d;
		itr2d = m_piles.begin();
		while ( itr2d != m_piles.end()) {
			itr1d = itr2d->begin();
			while ( itr1d != itr2d->end()) {
				if ((*itr1d)->HitTest(x, y)) {
					return(*itr1d);
				}
				++itr1d;
			}
			++itr2d;
		}
		return(NULL);
	}

	int calcNimValue() {
		int nimValue = 0;
		Pile2d::iterator itr2d;
		
		itr2d = m_piles.begin();
		while ( itr2d != m_piles.end()) {
			if (!(*itr2d).empty()) {
				nimValue ^= (*itr2d).size();
			}
			++itr2d;
		}

		return nimValue;
	}
	int calcNumEmptyPiles() {
		int numEmptyPiles = 0;
		Pile2d::iterator itr2d;
		
		itr2d = m_piles.begin();
		while ( itr2d != m_piles.end()) {
			if ((*itr2d).empty()) {
				numEmptyPiles++;
			}
			++itr2d;
		}

		return numEmptyPiles;
	}
	int calcSmallestNonEmptyPile() {
		int minNonEmptyPile = MAX_SLOTS;
		Pile2d::iterator itr2d;
		
		itr2d = m_piles.begin();
		while ( itr2d != m_piles.end()) {
			if (!(*itr2d).empty()) {
				if ((*itr2d).size() < minNonEmptyPile) {
					minNonEmptyPile = (*itr2d).size();
				}
			}
			++itr2d;
		}

		return minNonEmptyPile;
	}
	bool isGameOver() {
		return(calcNumEmptyPiles() == m_numPiles);
	}
	
	// Winner is player to take last turn
	// Return true if game is over
	bool computerMove() {
		int pileTakenFrom = -1;
		int numTaken = 0;
		int pile;
		int minNonEmptyPile;
		int numEmptyPiles;
		int nimValue;
		Pile2d::iterator itr2d;

		// Calculate nim value
		nimValue = calcNimValue();
		numEmptyPiles = calcNumEmptyPiles();
		minNonEmptyPile = calcSmallestNonEmptyPile();

		std::cout << "nimValue = " << nimValue << ".\n" << std::flush;

		if (nimValue == 0) {
			// Computer loses if opponent plays perfectly
			std::cout << "Computer is losing.\n" << std::flush;
			m_bComputerWinning = false;
			// Make a random move
			if (numEmptyPiles < m_numPiles) {
				pileTakenFrom = rand() % m_numPiles;
				while (m_piles[pileTakenFrom].size() == 0) {
					(pileTakenFrom)++;
					if (pileTakenFrom >= m_numPiles) {
						pileTakenFrom = 0;
					}
				}
				// Must take at least one
				numTaken = (rand() % m_piles[pileTakenFrom].size()) + 1;
				removeSticks(pileTakenFrom, m_piles[pileTakenFrom].size()-numTaken, false);
				return false;
			}
			else {
				numTaken = 0;
				pileTakenFrom = -1;
				return true;
			}
			
		}
		else { 
			// else Computer wins!
			std::cout << "Computer will win.\n" << std::flush;
			m_bComputerWinning = true;
			// Issue: This switch has m_numPiles value hard coded
			switch (numEmptyPiles) {
			case 2:
				{
					// Take all remaining from non empty pile.
					// Game Over.
					pile = 0;
					itr2d = m_piles.begin();
					while ( itr2d != m_piles.end()) {
						if (!(*itr2d).empty()) {
							removeSticks(pile, 0, false);
							pileTakenFrom = pile;
							numTaken = (*itr2d).size();
							return true;
						}
						pile++;
						++itr2d;
					}
					break;
				}
			case 1:
				{
					// Even out non empty piles
					pile = 0;
					itr2d = m_piles.begin();
					while ( itr2d != m_piles.end()) {
						if (!(*itr2d).empty() && (minNonEmptyPile < (*itr2d).size())) {
							removeSticks(pile, minNonEmptyPile, false);
							pileTakenFrom = pile;
							numTaken = (*itr2d).size()-minNonEmptyPile;
							return false;
						}
						pile++;
						++itr2d;
					}
					break;
				}
			case 0:
				{
					// Produce zero nim value
					int nimPileValue;
					pile = 0;
					itr2d = m_piles.begin();
					while ( itr2d != m_piles.end()) {
						if (!(*itr2d).empty()) {
							nimPileValue = nimValue ^ (*itr2d).size();
							if (nimPileValue < (*itr2d).size()) {
								numTaken = (*itr2d).size()-nimPileValue;
								removeSticks(pile, nimPileValue, false);
								pileTakenFrom = pile;
								return false;
							}
						}
						++itr2d;
						pile++;
					}
					break;
				}
			case 3:
				{
					numTaken = 0;
					pileTakenFrom = -1;
					return true;
				}
			default:
				{
					std::cout << "Error\n" << std::flush; 
					return true;
					break;
				}
			}
		}

		bool bGameOver = isGameOver();
		if (bGameOver) {
			std::cout << "Computer loses.\n" << std::flush;
		}
		else {
			std::cout << "Computer removed " << numTaken << " matchsticks from pile " << pileTakenFrom << ".\n" << std::flush;
		}

		return bGameOver;
	}
	
	void initSticks() {
		int x, y;
		x = startX;
		y = startY;
		int numSticks[3];
		for (int i = 0; i < m_numPiles; i++) {
			numSticks[i] = m_maxMatchSticksPerPile;
		}
		for (int i = 0; i < m_numPiles; i++) {
			m_unusedPiles.push_back( Pile1d() );
			m_piles.push_back( Pile1d() );
			m_removedPiles.push_back( Pile1d() );
			for (int j = 0; j < numSticks[i]; j++) {
				CBox *imageBox = new CBox(x,y,width,height, unusedColor);
				imageBox->setSlot(j);
				imageBox->setPile(i);
				m_unusedPiles[i].push_back( imageBox );
				x += width;
			}
			x = 40;
			y += height;
		}
		redraw();
	}

	void reinitSticks(bool bNewGame) {
		int i;
		int numSticks[3];

		m_bComputerWinning = false;

		if (bNewGame) {
			for (i = 0; i < m_numPiles; i++) {
				numSticks[i] = (rand() % m_maxMatchSticksPerPile) + 1;
				m_startPileCount[i] = numSticks[i];
			}
		}
		else {
			for (i = 0; i < m_numPiles; i++) {
				numSticks[i] = m_startPileCount[i];
			}
		}
		numSticks[0] = 1;
		numSticks[1] = 2;
		numSticks[2] = 4;
		m_startPileCount[0] = 1;
		m_startPileCount[1] = 2;
		m_startPileCount[2] = 4;

		Pile1d::iterator itr1d;
		Pile2d::iterator itr2d;
		Pile2d::iterator itr2di;
		itr2d = m_unusedPiles.begin();
		itr2di = m_piles.begin();
		i = 0;
		while (itr2d != m_unusedPiles.end()) {
			std::cout << "Pile " << i << " will start with " << numSticks[i] << " match sticks.\n" << std::flush;
			for (int j = 0; j < numSticks[i]; j++) {
				itr1d = itr2d->begin();
				(*itr1d)->setColor(startColor);
				m_piles[i].push_back(*itr1d);
				m_unusedPiles[i].erase(itr1d);
			}
			++itr2d;
			++itr2di;
			i++;
		}
		redraw();
	}

	void removeAllSticks() {
		Pile1d::iterator itr1d;
		Pile2d::iterator itr2d;

		Pile2d::iterator itr2di = m_removedPiles.begin();
		itr2d  = m_unusedPiles.begin();
		while (itr2di != m_removedPiles.end()) {
			while (!itr2di->empty()) {
				itr1d = itr2di->end();
				itr1d--;
				(*itr1d)->setColor(unusedColor);
				itr2d->insert(itr2d->begin(), *itr1d);
				itr2di->erase(itr1d);
			}
			++itr2d;
			++itr2di;
		}
		Pile2d::iterator itr2dj = m_piles.begin();
		itr2d  = m_unusedPiles.begin();
		while (itr2dj != m_piles.end()) {
			while (!itr2dj->empty()) {
				itr1d = itr2dj->end();
				itr1d--;
				(*itr1d)->setColor(unusedColor);
				itr2d->insert(itr2d->begin(), *itr1d);
				itr2dj->erase(itr1d);
			}
			++itr2d;
			++itr2dj;
		}
	}
	
	int removeSticks(int pile, int slot, bool bPlayersTurn) {
		int numRemoved = 0;
		if ((pile >= 0) && (pile < m_piles.size())) {
			if (slot >= 0) {
				Pile2d::iterator itr;
				itr = m_piles.begin();
				for (int i = 0; i < pile; i++) {
					++itr;
				}
				while (slot < (*itr).size()) {
					CBox* matchstick = (*itr).back();
					if (bPlayersTurn) {
						matchstick->setColor(playerColor);
					}
					else {
						matchstick->setColor(computerColor);
					}
					m_removedPiles[pile].insert(m_removedPiles[pile].begin(), matchstick);
					//delete matchstick;
					(*itr).pop_back( );
					numRemoved++;
					redraw();
				}
			}
		}
		return(numRemoved);
	}

	// FIX OPENGL VIEWPORT
    //     Do this on init or when window's size is changed
    void FixViewport(int W,int H) {
        glLoadIdentity();
        glViewport(0,0,W,H);
        //glOrtho(-W,W,-H,H,-1,1);
        glOrtho(0,W,H,0,-1,1);
    }

   // DRAW METHOD
    void draw() {
         // first time? init
		//std::cout << "draw\n" << std::flush;
		if (!valid()) {
			valid(1);
			FixViewport(w(), h());
		}

        // Clear screen to bg color
        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0, 0.0, 0.0);

		Pile1d::iterator itr1d;
		Pile2d::iterator itr2d;
		itr2d = m_piles.begin();
		while ( itr2d != m_piles.end()) {
			itr1d = itr2d->begin();
			while ( itr1d != itr2d->end()) {
				(*itr1d)->draw();
				++itr1d;
			}
			++itr2d;
		}

		itr2d = m_removedPiles.begin();
		while ( itr2d != m_removedPiles.end()) {
			itr1d = itr2d->begin();
			while ( itr1d != itr2d->end()) {
				(*itr1d)->draw();
				++itr1d;
			}
			++itr2d;
		}

		itr2d = m_unusedPiles.begin();
		while ( itr2d != m_unusedPiles.end()) {
			itr1d = itr2d->begin();
			while ( itr1d != itr2d->end()) {
				(*itr1d)->draw();
				++itr1d;
			}
			++itr2d;
		}

		output(60,  140, 1.0, 1.0, 0.0, "Yellow:unselected.");
		output(200, 140, 0.0, 0.0, 1.0, "Blue:you.");
		output(280, 140, 0.0, 1.0, 0.0, "Red:Nim Master.");

		bool bGameOver = isGameOver();
		if (bGameOver && m_bComputerWinning) {
			output(150, 155, 0.0, 0.0, 0.0, "Computer wins.");
		}
		else if (bGameOver) {
			output(150, 155, 0.0, 0.0, 0.0, "You win.");
		}
		else if (m_bComputerWinning) {
			output(150, 155, 0.0, 0.0, 0.0, "Computer will win.");
		}
		else {
			output(150, 155, 0.0, 0.0, 0.0, "You are winning.");
		}
		
		output(80, 190, 0.0, 0.0, 0.0, "Take turn by selecting leftmost of any number");
		output(80, 205, 0.0, 0.0, 0.0, "of abutting squares from right end of one row.");
		output(80, 220, 0.0, 0.0, 0.0, "He who selects last wins.");
		output(80, 235, 0.0, 0.0, 0.0, "First player to select with nim value of board = 0 should win with perfect play.");

		glFlush();
	}

	// OPENGL WINDOW CONSTRUCTOR
	MyGlWindow(int X,int Y,int W,int H,const char*L=0) : Fl_Gl_Window(X,Y,W,H,L) {

		// widget that will contain image
		// load jpeg image into ram
		if (!valid()) {
			valid(1);
			FixViewport(w(), h());
		}

		glutInitDisplayMode(GLUT_RGBA);

		GLenum errCode;
		const GLubyte *errString;
		if (!LoadTGA(&texture, "matchstick.tga")) {
			std::cout <<  "OpenGL Error: (LoadTGA)\n" << std::flush;
		}
		glGenTextures(1, &texture.texID);				// Create The Texture ( CHANGE )
		if ((errCode = glGetError()) != GL_NO_ERROR) {
			errString = gluErrorString(errCode);
			std::cout <<  "OpenGL Error: (glGenTextures) " << errString << "\n" << std::flush;
		}
		else {
			std::cout <<  "OpenGL OK: glGenTextures\n" << std::flush;
		}
		glBindTexture(GL_TEXTURE_2D, texture.texID);
		if ((errCode = glGetError()) != GL_NO_ERROR) {
			errString = gluErrorString(errCode);
			std::cout <<  "OpenGL Error: (glBindTexture) " << errString << "\n" << std::flush;
		}
		else {
			std::cout <<  "OpenGL OK: glBindTexture\n" << std::flush;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, texture.bpp / 8, texture.width, texture.height, 0, texture.type, GL_UNSIGNED_BYTE, texture.imageData);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		width = texture.width;
		height = texture.height;

		if (texture.imageData)						// If Texture Image Exists ( CHANGE )
		{
			free(texture.imageData);					// Free The Texture Image Memory ( CHANGE )
		}

		startX = 40;
		startY = 10;
		
		srand( time(NULL) );

		m_numPiles = 3;
		m_startPileCount = new int[m_numPiles];
		m_maxMatchSticksPerPile = 12;

		m_removedPiles.reserve(m_numPiles);

		initSticks();
		reinitSticks(true);
		end();
	}
};

// APP WINDOW CLASS
class MyAppWindow : public Fl_Window {
private:
	MyGlWindow *mygl; 
	CButton* buttonNewGame;
	CButton* buttonQuit;
	CButton* buttonRestart;
	int startX;
	int startY;
    int m_numPiles;
	int m_maxMatchSticksPerPile;
	bool bPlayerTurn;

protected:
	static void closeWindow_cb (Fl_Widget* o, void* v) {
		std::cout << "Exiting app.\n" << std::flush;
		exit(0);
	}
	static void newGame_cb (Fl_Widget* o, void* v) {
		( (MyAppWindow*)v )->newGame();
	}
	static void restart_cb (Fl_Widget* o, void* v) {
		( (MyAppWindow*)v )->restart();
	}
	void newGame () {
		std::cout << "New game.\n\n" << std::flush;
		mygl->removeAllSticks();
		mygl->reinitSticks(true);
	}
	void restart () {
		std::cout << "Restart game.\n\n" << std::flush;
		mygl->removeAllSticks();
		mygl->reinitSticks(false);
	}

public:
    // APP WINDOW DESTRUCTOR
	~MyAppWindow() {
	}

	// APP WINDOW CONSTRUCTOR
    MyAppWindow(int W,int H,const char*L=0) : Fl_Window(W,H,L) {
		bPlayerTurn = true;

		// OpenGL window
		startX = 160;
		startY = 10;
        mygl = new MyGlWindow(0, 0, 500, 242);

		buttonNewGame = new CButton(startX, 260, 80, 25, "New Game", CButton::NewGame);
		buttonQuit = new CButton(startX + 100, 260, 80, 25, "Quit", CButton::Quit);
		buttonRestart = new CButton(startX + 200, 260, 80, 25, "Restart", CButton::Restart);

		buttonQuit->callback(&MyAppWindow::closeWindow_cb, this);
		buttonQuit->when(FL_WHEN_RELEASE); // The callback is done after user successfully clicks button, or when shortcut is typed.
		buttonNewGame->callback(&MyAppWindow::newGame_cb, this);
		buttonNewGame->when(FL_WHEN_RELEASE); // The callback is done after user successfully clicks button, or when shortcut is typed.
		buttonRestart->callback(&MyAppWindow::restart_cb, this);
		buttonRestart->when(FL_WHEN_RELEASE); // The callback is done after user successfully clicks button, or when shortcut is typed.

        end();

		std::cout << "Window created.\n" << std::flush;
    }

	int handle(int e) {
        //std::cout << "handle\n" << std::flush;
		static int offset[2] = { 0, 0 };
        int ret = Fl_Window::handle(e);
        switch ( e ) {
		case FL_PUSH:
			bool bGameOver = mygl->isGameOver();
			if (bPlayerTurn && !bGameOver) {
				offset[0] = x() - Fl::event_x();    // save where user clicked for dragging
				offset[1] = y() - Fl::event_y();
				int x = Fl::event_x();
				int y = Fl::event_y();
				CBox* box = mygl->findHit(x, y);
				if (box != NULL) {
					int rem;
					int pile = box->getPile();
					int slot = box->getSlot();
					if ((rem = mygl->removeSticks(pile, slot, bPlayerTurn)) > 0) {
						std::cout << "Player removed " << rem << " matchsticks from pile " << pile << ".\n" << std::flush;
						bPlayerTurn = false;

						// now do computer's turn
						bGameOver = mygl->computerMove();
						
						bPlayerTurn = true;

						redraw();
						return(1);
					}
				}
			}
        }
        return(0);
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
	//std::cout << "Creating window\n" << std::flush;
	MyAppWindow win(500, 300, "Nim Master");
    win.resizable(win);
    win.show();
    //std::cout << "Begin run.\n" << std::flush;
	return(Fl::run());
}
