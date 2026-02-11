#ifndef __types__
#define __types__


typedef enum {
    tltLetter, tltDigit, tltSkip
} sic_type;

typedef enum {
  //лексемы по варианту 1 
  lexPLUS,     // +
  lexMIN,      // -
  lexMULT,     // *
  lexDIV,      // /
  lexMOD,      // mod
  lexEQ,       // =  есть
  lexNE,       // <> есть
  lexLT,       // <  есть
  lexGT,       // >  есть
  lexLE,       // <= есть
  lexGE,       // >= есть
  lexASS,      // := лексассигн есть
  lexCOMMENT,  // { }
  lexBST,      // begin
  lexEST,      // end
  lexEOP,      // ;

  kwVECTOR,    // vector
  kwOF,        // of
  kwINTEGER,   // integer
  kwREAL,      // real
  kwGOTO,      // goto
  kwREAD,      // read
  kwWRITE,     // write
  kwSKIP,      // skip
  kwSPACE,     // space
  kwTAB,       // tab
  kwIF,        // if
  kwTHEN,      // then
  kwELSE,      // else
  kwWHILE,     // while
  kwDO        // do


} lc_type;


#endif