#ifndef COLORS_h
#define COLORS_h

/* FOREGROUND */
// These codes set the actual text to the specified color
#define RESETTEXT  "\x1B[0m" // Set all colors back to normal.
#define FOREBLK  "\x1B[30m" // Black
#define FORERED  "\x1B[31m" // Red
#define FOREGRN  "\x1B[32m" // Green
#define FOREYEL  "\x1B[33m" // Yellow
#define FOREBLU  "\x1B[34m" // Blue
#define FOREMAG  "\x1B[35m" // Magenta
#define FORECYN  "\x1B[36m" // Cyan
#define FOREWHT  "\x1B[37m" // White

/* BACKGROUND */
// These codes set the background color behind the text.
#define BACKBLK "\x1B[40m"
#define BACKRED "\x1B[41m"
#define BACKGRN "\x1B[42m"
#define BACKYEL "\x1B[43m"
#define BACKBLU "\x1B[44m"
#define BACKMAG "\x1B[45m"
#define BACKCYN "\x1B[46m"
#define BACKWHT "\x1B[47m"

// These will set the text color and then set it back to normal afterwards.
#define BLK(x) FOREBLK x RESETTEXT
#define RED(x) FORERED x RESETTEXT
#define GRN(x) FOREGRN x RESETTEXT
#define YEL(x) FOREYEL x RESETTEXT
#define BLU(x) FOREBLU x RESETTEXT
#define MAG(x) FOREMAG x RESETTEXT
#define CYN(x) FORECYN x RESETTEXT
#define WHT(x) FOREWHT x RESETTEXT

// Example usage: cout << BLU("This text's color is now blue!") << endl;

// These will set the text's background color then reset it back.
#define BackBLK(x) BACKBLK x RESETTEXT
#define BackRED(x) BACKRED x RESETTEXT
#define BackGRN(x) BACKGRN x RESETTEXT
#define BackYEL(x) BACKYEL x RESETTEXT
#define BackBLU(x) BACKBLU x RESETTEXT
#define BackMAG(x) BACKMAG x RESETTEXT
#define BackCYN(x) BACKCYN x RESETTEXT
#define BackWHT(x) BACKWHT x RESETTEXT

// Example usage: cout << BACKRED(FOREBLU("I am blue text on a red background!")) << endl;

// These functions will set the background to the specified color indefinitely.
// NOTE: These do NOT call RESETTEXT afterwards. Thus, they will set the background color indefinitely until the user executes cout << RESETTEXT
// OR if a function is used that calles RESETTEXT i.e. cout << RED("Hello World!") will reset the background color since it calls RESETTEXT.
// To set text COLOR indefinitely, see SetFore functions below.
#define SetBackBLK BACKBLK
#define SetBackRED BACKRED
#define SetBackGRN BACKGRN
#define SetBackYEL BACKYEL
#define SetBackBLU BACKBLU
#define SetBackMAG BACKMAG
#define SetBackCYN BACKCYN
#define SetBackWHT BACKWHT

// Example usage: cout << SetBackRED << "This text's background and all text after it will be red until RESETTEXT is called in some way" << endl;

// These functions will set the text color until RESETTEXT is called. (See above comments)
#define SetForeBLK FOREBLK
#define SetForeRED FORERED
#define SetForeGRN FOREGRN
#define SetForeYEL FOREYEL
#define SetForeBLU FOREBLU
#define SetForeMAG FOREMAG
#define SetForeCYN FORECYN
#define SetForeWHT FOREWHT

// Example usage: cout << SetForeRED << "This text and all text after it will be red until RESETTEXT is called in some way" << endl;

#define BOLD(x) "\x1B[1m" x RESETTEXT // Embolden text then reset it.
#define BRIGHT(x) "\x1B[1m" x RESETTEXT // Brighten text then reset it. (Same as bold but is available for program clarity)
#define UNDL(x) "\x1B[4m" x RESETTEXT // Underline text then reset it.

// Example usage: cout << BOLD(BLU("I am bold blue text!")) << endl;

// These functions will embolden or underline text indefinitely until RESETTEXT is called in some way.

#define SetBOLD "\x1B[1m" // Embolden text indefinitely.
#define SetBRIGHT "\x1B[1m" // Brighten text indefinitely. (Same as bold but is available for program clarity)
#define SetUNDL "\x1B[4m" // Underline text indefinitely.

// Example usage: cout << setBOLD << "I and all text after me will be BOLD/Bright until RESETTEXT is called in some way!" << endl;

#endif /* COLORS_h */