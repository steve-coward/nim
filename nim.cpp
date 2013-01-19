// nim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <FL/Fl_Button.H>
#include <FL/glut.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <time.h>

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
	bool m_bGLInit;
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
		m_bGLInit = true;
	}

	void draw() {
		//std::cout << "gl draw\n" << std::flush;
		
		if (!m_bGLInit) {
			initGL();
		}

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
		m_bGLInit = false;
		m_x = l;
		m_y = t;
		m_w = w;
		m_h = h;
		m_color[0] = color[0];
		m_color[1] = color[1];
		m_color[2] = color[2];
		m_color[3] = color[3];
	}

	void setSlot(int slot) { m_slot = slot; }
	int getSlot() { return(m_slot); }
	void setPile(int pile) { m_pile = pile; }
	int getPile() { return(m_pile); }
};

// OPENGL WINDOW CLASS
class MyGlWindow : public Fl_Gl_Window {
public:
	int m_startX;
	int m_startY;
	int m_width;
	int m_height;
    int m_numPiles;
	int m_maxMatchSticksPerPile;
	typedef std::vector<CBox*> Pile1d;
	typedef std::vector<Pile1d> Pile2d;
	Pile2d m_unusedPiles;
	Pile2d m_piles;
	Pile2d m_removedPiles;
	int* m_startPileCount;
	bool m_bComputerWinning;

public:
	void output(int x, int y, float r, float g, float b, char *string) {
		glColor3f( r, g, b );
		glRasterPos2i(x, y);
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
				if ((*itr2d).size() < (std::vector<Pile1d>::size_type)minNonEmptyPile) {
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
							pileTakenFrom = pile;
							numTaken = (*itr2d).size();
							removeSticks(pile, 0, false);
							break;
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
						if (!(*itr2d).empty() && ((std::vector<Pile1d>::size_type)minNonEmptyPile < (*itr2d).size())) {
							pileTakenFrom = pile;
							numTaken = (*itr2d).size()-minNonEmptyPile;
							removeSticks(pile, minNonEmptyPile, false);
							break;
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
							if ((std::vector<Pile1d>::size_type)nimPileValue < (*itr2d).size()) {
								pileTakenFrom = pile;
								numTaken = (*itr2d).size()-nimPileValue;
								removeSticks(pile, nimPileValue, false);
								break;
							}
						}
						++itr2d;
						pile++;
					}
					break;
				}
			case 3:
				{
					break;
				}
			default:
				{
					std::cout << "Error\n" << std::flush; 
					break;
				}
			}
		}

		bool bGameOver = isGameOver();
		if (bGameOver) {
			if (numTaken > 0) {
				std::cout << "Computer wins.\n" << std::flush;
			}
			else {
				std::cout << "Computer loses.\n" << std::flush;
			}
		}
		else {
			std::cout << "Computer removed " << numTaken << " matchsticks from pile " << pileTakenFrom << ".\n" << std::flush;
		}

		return bGameOver;
	}
	
	void initSticks() {
		int x, y;
		x = m_startX;
		y = m_startY;
		int numSticks[3];
		for (int i = 0; i < m_numPiles; i++) {
			numSticks[i] = m_maxMatchSticksPerPile;
		}
		for (int i = 0; i < m_numPiles; i++) {
			m_unusedPiles.push_back( Pile1d() );
			m_piles.push_back( Pile1d() );
			m_removedPiles.push_back( Pile1d() );
			for (int j = 0; j < numSticks[i]; j++) {
				CBox *imageBox = new CBox(x,y,m_width,m_height, unusedColor);
				imageBox->setSlot(j);
				imageBox->setPile(i);
				m_unusedPiles[i].push_back( imageBox );
				x += m_width;
			}
			x = 40;
			y += m_height;
		}
		redraw();
	}

	void reinitSticks(bool bNewGame) {
		int i;
		int numSticks[3];

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

		// To initialize game to known state uncomment lines below.
		//numSticks[0] = 1;
		//numSticks[1] = 2;
		//numSticks[2] = 3;
		//m_startPileCount[0] = 1;
		//m_startPileCount[1] = 2;
		//m_startPileCount[2] = 3;

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
		
		
		// Human plays first, if current nimValue is 0 human will lose.
		m_bComputerWinning = (calcNimValue() == 0);

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
	
	// removes entries until the number of remaining entries = slot
	int removeSticks(int pile, int slot, bool bPlayersTurn) {
		int numRemoved = 0;
		if ((pile >= 0) && ((std::vector<Pile1d>::size_type)pile < m_piles.size())) {
			if (slot >= 0) {
				Pile2d::iterator itr;
				itr = m_piles.begin();
				for (int i = 0; i < pile; i++) {
					++itr;
				}
				while ((std::vector<Pile1d>::size_type)slot < (*itr).size()) {
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
				}
				redraw();
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

	void draw() {
        // if first time, init
		if (!valid()) {
			valid(1);
			FixViewport(w(), h());
		}

        // Clear screen to bg color
        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0, 0.0, 0.0);

		Pile2d::iterator itr2d;
		itr2d = m_piles.begin();
		while ( itr2d != m_piles.end()) {
			std::for_each(itr2d->begin(),itr2d->end(), std::mem_fun(&CBox::draw));
			++itr2d;
		}

		itr2d = m_removedPiles.begin();
		while ( itr2d != m_removedPiles.end()) {
			std::for_each(itr2d->begin(),itr2d->end(), std::mem_fun(&CBox::draw));
			++itr2d;
		}

		itr2d = m_unusedPiles.begin();
		while ( itr2d != m_unusedPiles.end()) {
			std::for_each(itr2d->begin(),itr2d->end(), std::mem_fun(&CBox::draw));
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

		m_width = 32;
		m_height = 32;

		m_startX = 40;
		m_startY = 10;
		
		srand( (unsigned int)time(NULL) );

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
	MyGlWindow *m_mygl;
	CButton* m_buttonNewGame;
	CButton* m_buttonQuit;
	CButton* m_buttonRestart;
    int m_numPiles;
	int m_maxMatchSticksPerPile;
	bool m_bPlayerTurn;
	bool m_bInitOK;

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
		m_bPlayerTurn = true;
		m_mygl->removeAllSticks();
		m_mygl->reinitSticks(true);
	}
	void restart () {
		std::cout << "Restart game.\n\n" << std::flush;
		m_bPlayerTurn = true;
		m_mygl->removeAllSticks();
		m_mygl->reinitSticks(false);
	}

public:
    // APP WINDOW DESTRUCTOR
	~MyAppWindow() {
	}

	// APP WINDOW CONSTRUCTOR
    MyAppWindow(int W,int H,const char*L=0) : Fl_Window(W,H,L) {
		m_bInitOK = true;
		m_bPlayerTurn = true;

		// OpenGL window
        m_mygl = new MyGlWindow(0, 0, 500, 242);
		if (m_mygl == NULL) {
			m_bInitOK = false;
		}

		m_buttonNewGame = new CButton(160, 260, 80, 25, "New Game", CButton::NewGame);
		if (m_buttonNewGame == NULL) {
			m_bInitOK = false;
		}
		else {
			m_buttonNewGame->callback(&MyAppWindow::newGame_cb, this);
			// The callback performed after user clicks button, or when shortcut is typed.
			m_buttonNewGame->when(FL_WHEN_RELEASE);
		}
		
		
		m_buttonQuit = new CButton(260, 260, 80, 25, "Quit", CButton::Quit);
		if (m_buttonQuit == NULL) {
			m_bInitOK = false;
		}
		else {
			m_buttonQuit->callback(&MyAppWindow::closeWindow_cb, this);
			// The callback performed after user clicks button, or when shortcut is typed.
			m_buttonQuit->when(FL_WHEN_RELEASE);
		}
		
		m_buttonRestart = new CButton(360, 260, 80, 25, "Restart", CButton::Restart);
		if (m_buttonRestart == NULL) {
			m_bInitOK = false;
		}
		else {
			m_buttonRestart->callback(&MyAppWindow::restart_cb, this);
			// The callback performed after user clicks button, or when shortcut is typed.
			m_buttonRestart->when(FL_WHEN_RELEASE);
		}

        end();

		if (m_bInitOK) {
			std::cout << "Window created.\n" << std::flush;
		}
    }

	bool initOK() {
		return(m_bInitOK);
	}

	int handle(int e) {
		static int offset[2] = { 0, 0 };
        int ret = Fl_Window::handle(e);
        switch ( e ) {
		case FL_PUSH:
			bool bGameOver = m_mygl->isGameOver();
			if (m_bPlayerTurn && !bGameOver) {
				offset[0] = x() - Fl::event_x();    // save where user clicked for dragging
				offset[1] = y() - Fl::event_y();
				int x = Fl::event_x();
				int y = Fl::event_y();
				CBox* box = m_mygl->findHit(x, y);
				if (box != NULL) {
					int rem;
					int pile = box->getPile();
					int slot = box->getSlot();
					if ((rem = m_mygl->removeSticks(pile, slot, m_bPlayerTurn)) > 0) {
						std::cout << "Player removed " << rem << " matchsticks from pile " << pile << ".\n" << std::flush;
						m_bPlayerTurn = false;

						// now do computer's turn
						m_bPlayerTurn =! m_mygl->computerMove();

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
	MyAppWindow win(500, 300, "Nim Master");
	if (win.initOK()) {
		win.resizable(win);
	 win.show();
		return(Fl::run());
	}
}
