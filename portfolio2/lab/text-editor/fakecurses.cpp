#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "fakecurses.hpp"

const static char* ANSI_PREFIX          = "\x1B[";
const static char* ANSI_CLEARSCR        = "2J";
const static char* ANSI_CLEARLINE       = "K";
const static char* ANSI_POSCURSOR       = "H";
const static char* ANSI_MOVEUP          = "A";
const static char* ANSI_MOVEDOWN        = "B";
const static char* ANSI_MOVEFOR         = "C";
const static char* ANSI_MOVEBACK        = "D";
const static char* ANSI_SETMODE         = "m";
const static char* ANSI_SHOWCURSOR      = "?25h";
const static char* ANSI_HIDECURSOR      = "?25l";

const static int ANSI_FG_BASE           = 30;
const static int ANSI_BG_BASE           = 40;

static struct termios originalTermios;

void fakecurses::setCursor( short x, short y )
{
    printf( "%s%i;%i%s", ANSI_PREFIX, y, x, ANSI_POSCURSOR );
}

void fakecurses::moveCursor( short x, short y )
{
    if ( x < 0 )
        printf( "%s%i%s", ANSI_PREFIX, x*-1, ANSI_MOVEBACK );

    if ( x > 0 )
        printf( "%s%i%s", ANSI_PREFIX, x, ANSI_MOVEFOR );

    if ( y < 0 )
        printf( "%s%i%s", ANSI_PREFIX, y*-1, ANSI_MOVEUP );

    if ( y > 0 )
        printf( "%s%i%s", ANSI_PREFIX, y, ANSI_MOVEDOWN );
}

void fakecurses::showCursor( )
{
    printf( "%s%s", ANSI_PREFIX, ANSI_SHOWCURSOR );
}

void fakecurses::hideCursor( )
{
    printf( "%s%s", ANSI_PREFIX, ANSI_HIDECURSOR );
}

void fakecurses::resetColor( )
{
    printf( "%s0%s", ANSI_PREFIX, ANSI_SETMODE );
}

void fakecurses::setColor( ANSI_COLOR fg, ANSI_COLOR bg )
{
    printf( "%s%i;%i%s", ANSI_PREFIX, ANSI_FG_BASE + fg, ANSI_BG_BASE + bg, ANSI_SETMODE );
}

void fakecurses::setTextMode( ANSI_TEXT mode )
{
    printf( "%s%i%s", ANSI_PREFIX, mode, ANSI_SETMODE );
}
void fakecurses::clearScreen( )
{
    printf( "%s%s", ANSI_PREFIX, ANSI_CLEARSCR );
    printf( "%s0;0%s", ANSI_PREFIX, ANSI_POSCURSOR );    
}

void fakecurses::clearLine( )
{
    printf( "%s%s", ANSI_PREFIX, ANSI_CLEARLINE );
}

void fakecurses::printChar( char c )
{
    printf( "%c", c );
}

void fakecurses::printString( const char* szStr )
{
    printf( "%s", szStr );
}

short fakecurses::getScrWidth( )
{
    struct winsize w;
    ioctl( 1, TIOCGWINSZ, &w );

    return static_cast<short>( w.ws_col );
}

short fakecurses::getScrHeight( )
{
    struct winsize w;
    ioctl( 1, TIOCGWINSZ, &w );

    return static_cast<short>( w.ws_row );
}

void fakecurses::getScrSize( short& w, short& h )
{
    struct winsize ws;
    ioctl( 1, TIOCGWINSZ, &ws );

    w = static_cast<short>( ws.ws_col );
    h = static_cast<short>( ws.ws_row );
}

void fakecurses::init( )
{
    struct termios newTermios;

    /* Save old terminal information */
    
    tcgetattr( 0, &originalTermios ); 
    memcpy( &newTermios, &originalTermios, sizeof( termios ) );

    /* Set the terminal to raw input mode immediately */
    
    cfmakeraw( &newTermios );
    tcsetattr( 0, TCSANOW, &newTermios );
    
    setbuf( stdout, NULL );
    
    clearScreen( );
}

void fakecurses::cleanup( )
{
    tcsetattr( 0, TCSANOW, &originalTermios );
    
    setColor( COLOR_WHITE, COLOR_BLACK );
    showCursor( );
    resetColor( );
}

bool fakecurses::isKeypress( )
{
    struct timeval sTimeout;
    fd_set fds;

    sTimeout.tv_sec = 0;
    sTimeout.tv_usec = 0;

    FD_ZERO( &fds );
    FD_SET( 0, &fds );

    return ( select( 1, &fds, NULL, NULL, &sTimeout ) != 0 );
}

char fakecurses::getKey( )
{
    char cKey;
    long nResult;

    nResult = read( 0, &cKey, sizeof( char ) );

    if ( nResult == -1 )
        return -1;

    return cKey;
}
