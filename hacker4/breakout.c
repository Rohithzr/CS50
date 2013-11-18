/*
* breakout.c
*
* Ankit Gupta
* ankitgupta@college.harvard.edu
*
*
* Implements breakout, with a few "hacker" features including:
*     -- GOD mode: Passing command line argument "GOD:
*     -- velocity increases as bricks are broken  
*     -- BONUS: each broken brick is worth one more than the previous
*/

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"
#include "ginteractors.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// width of paddle in pixels
#define PWIDTH 80

// height of paddle in pixels
#define PHEIGHT 10

// Distance of paddle from bottom (three times the height)
#define PDBOTTOM 3*PHEIGHT

// Height of each brick
#define BHEIGHT 10

// horizontal gap between bricks
#define HGAP 5

// vertical gap between bricks
#define VGAP 4

// Width of each brick, calculated from givens above.
#define BWIDTH (WIDTH-HGAP*COLS)/COLS


// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
GLabel initLives(GWindow window);

void updateScoreboard(GWindow window, GLabel label, int points);
void updateLives(GWindow window, GLabel livecounter, int lives);
GObject detectCollision(GWindow window, GOval ball);

int main(int argc, char* argv[])
{
    // Declares a boolean value for whether godmode is activated
    bool godmode = false;
    
    // Checks to make sure the number of command line arguments is valid.
    if (argc > 2)
    {
        return 1;
    }
    
    // If there are two arguments "GOD" is passed in, turn on god mode.
    if ((argc == 2) && (strcmp(argv[1], "GOD") == 0))
    {
        godmode = true;
    }

    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);
    

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);
   
    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);
    
    // instantiate counter for the number of lives in top right of screen
    GLabel livecounter = initLives(window);
    

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // counts the number of bricks broken
    int bbroken = 0;
    
    // initialize the x and y velocities. xvelocity is randomly generated.
    double xvelocity = 2 * drand48();
    double yvelocity = 2.0;

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // update the scoreboard and number of lives.
        updateScoreboard(window, label, points);
        updateLives(window, livecounter, lives);
       
        // declare a new GEvent that stores the next mouse movement.
        GEvent event = getNextEvent(MOUSE_EVENT);
       
        if (event != NULL)
        {
            // If the mouse moved, line up the paddle with the cursor.
            if (getEventType(event) == MOUSE_MOVED)
            {
                double x = getX(event) - PWIDTH / 2;
                setLocation(paddle, x, HEIGHT - 3 * PDBOTTOM);  
            }
       }
       // Move the ball as per the calculated velocities.
        move(ball, xvelocity, yvelocity);
      
       // If godmode is activated, line up the paddle with the ball.
        if (godmode == true)
        {
            setLocation(paddle, getX(ball) + RADIUS - PWIDTH / 2, 
            HEIGHT - 3 * PDBOTTOM);
        }
       
       // Have a little lingering so we can follow the ball.
        pause(10);
       
       // if the ball hits the right wall, rebound and speed it up.
       // Absolute value was used to fix an error where
       // the ball got stuck in the corner
        if (getX(ball) + 2 * RADIUS >= WIDTH)
        {
            xvelocity = -abs(xvelocity) - .1;
       }
       // if the ball hits the left wall, rebound and speed it up.
       // Absolute value was used to fix an error where
       // the ball got stuck in the corner
       else if(getX(ball) <= 0)
        {
            xvelocity = abs(xvelocity) + .1;
       }
       // If the ball hits the top, rebound.
        if (getY(ball) <= 0)
        {
            yvelocity = -yvelocity;
       }
       
       // Check if there is a collision - if so, store in "object".
        GObject object = detectCollision(window, ball);
       
       // If there was a collision, then check what it was.
        if (object != NULL)
        {
            // If it is the paddle, then flip the y velocity.
            if (object == paddle)
            {
                yvelocity = -yvelocity;
                
                /*These next two loops make it so a ball moving right
                that hits the left half of the paddle switches to moving left,
                and a ball moving left that hits the right half of the paddle
                switches to moving right. These is similar to the brickbreaker
                I am used to.
                */
                
                // This first if-statement makes the x-velocity more if in the
                // left 1/4th or right 1/4th of the paddle.
                if ((getX(ball) + RADIUS < getX(paddle) + PWIDTH / 4) || 
                   (getX(ball) + RADIUS > getX(paddle) + 3 * PWIDTH / 4))
                {   
                    xvelocity = xvelocity + .3;
                }
                
                // This second if-statement makes the x-velocity more if in the
                // left 1/8th or right 1/8th of the paddle.
                if ((getX(ball) + RADIUS < getX(paddle) + PWIDTH / 8) || 
                   (getX(ball) + RADIUS > getX(paddle) + 7 * PWIDTH / 8))
                {   
                    xvelocity = xvelocity + .3;
                }
                
                // These next two if-statements make balls that hit the left  
                // side of the  paddle rebound left, and those that hit the 
                // right side of the paddle rebound right.
                if ((getX(ball) + RADIUS > getX(paddle) + PWIDTH / 2) && 
                   (xvelocity < 0))
                {
                    xvelocity = -xvelocity;
                }
                if ((getX(ball) + RADIUS < getX(paddle) + PWIDTH / 2) && 
                   (xvelocity > 0))
                {
                    xvelocity = -xvelocity;
                }
                
            }
            
            // If it hits a rectangle that is not the paddle, 
            // flip the y velocity, remove the block, add points along 
            // with the bonus and set the new values for number of broken bricks
            // and bricks remaining.
            else if (strcmp(getType(object), "GRect") == 0)
            {
                yvelocity = -yvelocity;
                removeGWindow(window, object);
                points = points + 10 + bbroken;
                bricks--;
                bbroken++;
                
                // The next two if statements make the x velocity 
                // more positive or negative by .4 every time a block is hit,
                // making the ball move faster.
                if (xvelocity >= 0)
                {
                    xvelocity = xvelocity + .4;
                }    
                if (xvelocity < 0)
                {
                    xvelocity = xvelocity - .4;
                }
            }
           
       }
       // If the ball hits the bottom, move the ball back to the middle,
       // deduct a life, and continue when the user clicks the mouse.
        if (getY(ball) + 2 * RADIUS >= HEIGHT)
        {
            setLocation(ball, WIDTH / 2 - RADIUS, HEIGHT / 2 - RADIUS);
            lives--;
            updateLives(window, livecounter, lives);
            waitForClick();
        }
    }
    // Make a label for the "Game Over" at the end.
    GLabel gameover = newGLabel("Game Over");
    setFont(gameover, "SansSerif-36");
    add(window, gameover);
    
    // Set the "Game Over" tag to the right above the paddle.
    setLocation(gameover, 100, 500);
    
    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // Create an array of rectangles.
    GRect rect[COLS][ROWS];
    
    // Iterate over the colums and rows.
    for (int col = 0; col < COLS; col++)
    {
        for (int row = 0; row < ROWS; row++)
        {
            // Set the rectangle to a certain point, accounting for its width
            // and the space (HGAP) between adjacent rectangles.
            // Do the same vertically with a space of VGAP
            rect[col][row] = newGRect(BWIDTH * col + HGAP * col + 1, 
            BHEIGHT * row + VGAP * row + 6 * BHEIGHT, BWIDTH, BHEIGHT);
            
            // Set the color row by row.
            if (row == 0)
            {
                setColor(rect[col][row], "BLACK");
            }
            if (row == 1)
            {
                setColor(rect[col][row], "BLUE");
            }
            if (row == 2)
            {
                setColor(rect[col][row], "GREEN");
            }
            if (row == 3)
            {
                setColor(rect[col][row], "ORANGE");
            }
            if (row == 4)
            {
                setColor(rect[col][row], "RED");
            }
            // Fill the color and add it to the window.
            setFilled(rect[col][row], true);
            add(window, rect[col][row]); 
        }
    
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // Instantiate a ball at the center by accounting for radius.
    GOval ball = newGOval(WIDTH / 2 - RADIUS, HEIGHT / 2 - RADIUS, 
    2 * RADIUS, 2 * RADIUS);
    
    add(window, ball);
    
    // Return the ball to main
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // Instantiate a paddle, centered and a distance of 
    // PDBOTTOM from the bottom.
    GRect paddle = newGRect(WIDTH / 2 - PWIDTH / 2, 
    HEIGHT - PDBOTTOM, PWIDTH, PHEIGHT);
    
    add(window, paddle);
    
    // Make the paddle blue.
    setColor(paddle, "BLUE");
    setFilled(paddle, true);
    
    // Return the paddle to main.
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // Make a new label for the scoreboard, set its font, add it to the
    // window and return it to main.
    
    GLabel label = newGLabel("");
    setFont(label, "SansSerif-36");
    add(window, label);
    return label;
}

GLabel initLives(GWindow window)
{
    // make a new label for the number of lives, set its font, add it to the
    // window and return it to main.
    GLabel livecounter = newGLabel("");
    setFont(livecounter, "SansSerif-36");
    add(window, livecounter);
    return livecounter;
}


/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

void updateLives(GWindow window, GLabel livecounter, int lives)
{
    // update label
    char s[12];
    sprintf(s, "%i", lives);
    setLabel(livecounter, s);

    // Place in top right of window
    double x = (WIDTH - 30);
    double y = 30;
    setLocation(livecounter, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
