#ifndef __error_data__
#define __error_data__

/*------------------------------------------------------------------------------
// error.h 

Хранит: тексты ошибок

Что внутри:
- ermes[] — массив строк с сообщениями об ошибках
  * "Incorrect lexema!"
  * "String of characters is not closed!" и т.д.

МОЖНО добавить новые ошибки (по желанию)

*/

static char *ermes[] = {
  /* 00 */ "Incorrect lexema!",
  /* 01 */ "Wated \'!=\'",
  /* 02 */ "Last comment is not closed!",
  /* 03 */ "Letter concatenated with number!",
  /* 04 */ "Incorrect integer foundation!",
  /* 05 */ "One point is incorrect!",
  /* 06 */ "String of characters is not closed!",
};

#endif