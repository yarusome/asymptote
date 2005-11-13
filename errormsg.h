/*****
 * errormsg.h
 * Andy Hammerlindl 2002/06/17
 *
 * Used in all phases of the compiler to give error messages.
 *****/

#ifndef ERRORMSG_H
#define ERRORMSG_H

#include <list>
#include <iostream>
#include "memory.h"
#include "settings.h"

using std::ostream;
using std::endl;

struct handled_error {}; // Exception to process next file.
struct interrupted {};   // Exception to process user interrupts.

class fileinfo : public gc {
  mem::string filename;
  size_t lineNum;

public:
  fileinfo(string filename)
    : filename(filename), lineNum(1) {}

  size_t line()
  {
    return lineNum;
  }
  
  // Specifies a newline symbol at the character position given.
  void newline() {
    ++lineNum;
  }
  
  mem::string name() {
    return filename;
  }
};


class position {
  fileinfo *file;
  size_t line; // The offset in characters in the file.
  size_t column;

public:
  position() : file(NULL), line(0), column(0) {}
    
  void init(fileinfo *f, int p) {
    file = f;
    if (file) {
      line = file->line();
      column = p;
    } else {
      line = column = 0;
    }
  }

  bool operator! () const
  {
    return (file == 0);
  }
  
  friend ostream& operator << (ostream& out, const position& pos);

  static position nullPos() {
    position p;
    p.init(0,0);
    return p;
  }
};

class errorstream {
  ostream& out;
  bool anyErrors;
  bool anyWarnings;
  bool floating;	// Was a message output without a terminating newline?
  bool pending;		// Are there pending interrupts or tracing requests?
  
public:
  static bool interrupt; // Is there a pending interrupt?
  
  errorstream(ostream& out = std::cerr)
    : out(out), anyErrors(false), anyWarnings(false), floating(false),
      pending(false) {}

  void clear();

  void message(position pos, const string& s);
  
  void Interrupt(bool b) {
    interrupt=b;
    if(b) pending=true;
  }
  
  // An error is encountered, not in the user's code, but in the way the
  // compiler works!  This may be augmented in the future with a message
  // to contact the compiler writers.
  void compiler();
  void compiler(position pos);

  // An error encountered when running compiled code.  This method does
  // not stop the executable, but the executable should be stopped
  // shortly after calling this method.
  void runtime(position pos);

  // Errors encountered when compiling making it impossible to run the code.
  void error(position pos);

  // Indicate potential problems in the code, but the code is still usable.
  void warning(position pos);

  // Print out position in code to aid debugging.
  void trace(position pos);
  
  // Sends stuff to out to print.
  // NOTE: May later make it do automatic line breaking for long messages.
  template<class T>
  errorstream& operator << (const T& x) {
    out << x;
    return *this;
  }

  // Reporting errors to the stream may be incomplete.  This draws the
  // appropriate newlines or file excerpts that may be needed at the end.
  void sync();

  bool errors() const {
    return anyErrors;
  }
  
  bool warnings() const {
    return anyWarnings || errors();
  }
  
  bool Pending() {
    return pending;
  }

  void Pending(bool b) {
    pending=b;
  }

  void process(const position& pos);
  
};

extern errorstream *em;

#endif
