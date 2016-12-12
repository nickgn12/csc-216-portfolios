#ifndef LIB_FAKECURSES_HPP
#define LIB_FAKECURSES_HPP

namespace fakecurses
{
    enum ANSI_TEXT
    {
        TEXT_NORMAL	= 0,
        TEXT_BOLD	= 1,
        TEXT_UNDERSCORE	= 4,
        TEXT_BLINK	= 5,
        TEXT_REVERSE	= 7,
        TEXT_CONCEALED	= 8
    };

    enum ANSI_COLOR
    {
        COLOR_BLACK = 0,
        COLOR_RED,
        COLOR_GREEN,
        COLOR_YELLOW,
        COLOR_BLUE,
        COLOR_MAGENTA,
        COLOR_CYAN,
        COLOR_LGRAY,
        COLOR_DGRAY = 60,
        COLOR_LRED,
        COLOR_LGREEN,
        COLOR_LYELLOW,
        COLOR_LBLUE,
        COLOR_LMAGENTA,
        COLOR_LCYAN,
        COLOR_WHITE
    };

    void setCursor( short x, short y );
    void moveCursor( short x, short y );
    void showCursor( );
    void hideCursor( );

    void resetColor( );
    void setColor( ANSI_COLOR fg, ANSI_COLOR bg );
    void setTextMode( ANSI_TEXT mode );

    void clearScreen( );
    void clearLine( );

    void printChar( char c );
    void printString( const char* szStr );

    short getScrWidth( );
    short getScrHeight( );
    void getScrSize( short& w, short& h );

    void init( );
    void cleanup( );

    bool isKeypress( );
    char getKey( );
}

#endif
