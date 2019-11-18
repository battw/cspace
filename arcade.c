#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h> //sleep, usleep
#include <string.h> //strcmp

/* A thing in the game e.g. a ship, ufo, bullet...
   Things can be chained together as a linked list. */
struct Thing {
    int x;      //position
    int y;
    int xvel;   //velocity
    int yvel;
    int len;
    int width;
    char* body;
    int ttl;    //time to live (-1 = forever)
    struct Thing* prev;
    struct Thing* next;
    bool issentinel;
};

static struct Thing* newthing(int y, int x, int yvel, int xvel,
        int len, int width, char* body, int timetolive, bool issentinel) {
    struct Thing* thing = (struct Thing*)malloc(sizeof(struct Thing));
    thing->y = y;
    thing->x = x;
    thing->yvel = yvel;
    thing->xvel = xvel;
    thing->len = len;
    thing->width = width;
    thing->body = body;
    thing->ttl = timetolive;
    thing->prev = NULL;
    thing->next = NULL;
    thing->issentinel = issentinel;

    return thing;
}

static struct Thing* newbullet(int y, int x, int yvel, int xvel) {
    /*
            *
    */
    return newthing(y, x, yvel, xvel, 1, 1, "*", 50, false);
}

static struct Thing* newship(int y, int x)  {
    /*
            |
            O
            O
           OOO
    */
    return newthing(y, x, 0, 0, 4, 3, " |  O  O OOO", -1, false);
}

/* A sentinel for marking the start of a chain of things */
static struct Thing* newsentinel() {
    return newthing(-1,-1,-1,-1,-1,-1,"SENTINEL", -1, true);
}


/*  Insert a thing after the sentinel and before the rest of the chain of
    things.
*/
static void linkthing(struct Thing* thing, struct Thing* sentinel) {
    if (thing == NULL || sentinel == NULL) {
        exit(1);
    }

    if (sentinel->next != NULL) {
        thing->next = sentinel->next;
        sentinel->next->prev = thing;
    }
    sentinel->next = thing;
    thing->prev = sentinel;
}

/*  Writes the thing to the curses window data structure (stdscr).
    Checks for collisions between things using the current contents of stcscr.
*/
static void placething(struct Thing* thing) {
    for (int y = 0; y < thing->len; ++y) {
        for (int x = 0; x < thing->width; ++x) {
            int i = y * thing->width + x;
            int ch = mvinch(thing->y + y, thing->x + x);
            //Check for collisions between things
            //TODO ch is a chtype (int) and ' ' is a char. Does this matter?
            if (ch != ERR && ch != ' ') {
                thing->ttl = 5;
                thing->body = " # #########";
            }
            mvaddch(thing->y + y, thing->x + x, thing->body[i]);
        }
    }
}

static void removething(struct Thing* thing) {
    if (thing->issentinel || thing->prev == NULL || thing == NULL) {
        //TODO what should it do if reaching an inconsistent state like this
        exit(0);
    }

    thing->prev->next = thing->next;

    if (thing->next != NULL) {
       thing->next->prev = thing->prev;
    }

    free(thing);
}


/*  Iterates over the chain of things starting at "thing",
    updates their positions,
    removes them if they are dead,
    draws them to the curses window data structure (stdscr).
*/

static void update(struct Thing* sentinel) {
    struct Thing* thing = sentinel->next;
    while (thing != NULL) {
        if (thing->ttl >= 0 && --(thing->ttl) == 0) {
            struct Thing* tmpthing = thing;
            thing = thing->next;
            removething(tmpthing);
        } else {
            thing->y += thing->yvel;
            thing->x += thing->xvel;
            placething(thing);
            thing = thing->next;
        }
    }
}

/* Get any keyboard input and decode it:
   'q' to quit.  (sets *running to false)
   Arrow keys to control the ship.
   The use of getch in this procedure causes a screen refresh.
*/
static void handleinput(struct Thing* ship, bool* running, bool* paused) {

    int ch = getch();

    if (ch == ERR) return;

    switch(ch) {
        case 'q':
            *running = false; break;
        case 'p':
            *paused = !*paused;
    }

    if (ship == NULL || *paused) return;

    switch(ch) {
        case KEY_LEFT:
            ship->xvel -= 1; break;
        case KEY_RIGHT:
            ship->xvel += 1; break;
        case KEY_UP:
            ship->yvel -= 1; break;
        case KEY_DOWN:
            ship->yvel += 1; break;
    }
}

static void run() {
    int ymax, xmax;
    getmaxyx(stdscr, ymax, xmax);

    struct Thing* shipsentinel = newsentinel();
    linkthing(newship(ymax - 5, xmax / 2), shipsentinel);
    struct Thing* bulletsentinel = newsentinel();
    bool running = true, paused = false;
    int bulletprob = 20;
    while (running) {
        handleinput(shipsentinel->next, &running, &paused);
        if (paused) {
            continue;
        }

        erase();// clears the stuff that has been drawn to the screen by curses
        update(bulletsentinel);
        update(shipsentinel);
        if (shipsentinel->next  == NULL) {
            running = false;
        }
        refresh();

        if (rand() % 100 < bulletprob) {
            linkthing(newbullet(0, rand() % xmax, 1, 0), bulletsentinel);
        }
    }
    mvprintw(ymax/2, xmax/2 - 5, "GAME OVER");
    refresh();
    sleep(2);
}

int main() {
    initscr();              // curses initialisation
    noecho();               // don't echo input to the terminal
    keypad(stdscr, true);   // allow function, arrow... keys
    curs_set(0);
    raw();		            // read character data without requiring enter key
    //nodelay(stdscr, TRUE);  // prevent getch() from blocking
    halfdelay(2);

    run();                  // enter game loop

    endwin();               // cleanup
    return 0;
}





