/*
Хранит: ГЛАВНЫЙ АЛГОРИТМ РАСПОЗНАВАНИЯ

Что внутри:
- nxl() — "Next Lexeme" — СЕРДЦЕ СКАНЕРА!
  * Читает символы из файла
  * Определяет, что это: число, идентификатор, оператор...
  * Возвращает lc (тип лексемы) и lv (значение)

- number() — распознаёт числа
- fltnumber() — действительные числа
- id_etc() — идентификаторы и ключевые слова
//- string_const() — строки "..."

//Что делать: ИЗМЕНЯТЬ логику распознавания*/


#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "scaner_data.h"
#include "scaner_local.h"

// Функция обработки ошибок (объявлена внешне, реализация в error.cpp)
void er(int i);

//вспомгательные функции / база
//все функции возвращают true/false - является ли символ тем или иным типом

//идентификаторы ключслова
/*
 * Проверяет, является ли символ буквой (латиница)
 * Возвращает true для: A-Z, a-z
 * Используется для: распознавания идентификаторов и ключевых слов
*/
bool inline isLetter(int ch) {
  if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
    return true;
  else
    return false;
}

//2ичная
/**
 * Проверяет, является ли символ двоичной цифрой
 * Возвращает true для: 0, 1
 * Используется для: распознавания двоичных чисел (0b1010)
 */
bool inline isBin(int ch) {
  if((ch == '0' || ch == '1'))
    return true;
  else
    return false;
}

//8ичная
/**
 * Проверяет, является ли символ восьмеричной цифрой
 * Возвращает true для: 0-7
 * Используется для: распознавания восьмеричных чисел (0755)
 */
bool inline isOctal(int ch) {
  if((ch >= '0' && ch <= '7'))
    return true;
  else
    return false;
}

//10ичная
/**
 * Проверяет, является ли символ десятичной цифрой
 * Возвращает true для: 0-9
 * Используется для: распознавания десятичных чисел (123)
 */
bool inline isDigit(int ch) {
  if((ch >= '0' && ch <= '9'))
    return true;
  else
    return false;
}

//16ричная
/**
 * Проверяет, является ли символ шестнадцатеричной цифрой
 * Возвращает true для: 0-9, A-F, a-f
 * Используется для: распознавания шестнадцатеричных чисел (0xFF, 0x1A)
 */
bool inline isHex(int ch) {
  if((ch >= '0' && ch <= '9') ||
     (ch >= 'A' && ch <= 'F') || 
     (ch >= 'a' && ch <= 'f'))
    return true;
  else
    return false;
}

//пропуск разделителей (пробле таб /n)
/**
 * Проверяет, является ли символ пропускаемым (разделителем)
 * Возвращает true для: пробел, табуляция, новая строка, перевод страницы
 * Используется для: игнорирования пробелов между лексемами
 */
bool inline isSkip(int ch) {
  if(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\f')
    return true;
  else
    return false;
}

//игнор мусора
/**
 * Проверяет, является ли символ игнорируемым управляющим символом
 * Возвращает true для: управляющие символы ASCII (1-31), кроме \t, \n, \f
 * Используется для: игнорирования "мусорных" символов
 */
bool inline isIgnore(int ch) {
  if(ch >0 && ch < ' ' && ch != '\t' && ch != '\n' && ch != '\f')
    return true;
  else
    return false;
}

//читате след символ 
/**
 * Читает следующий символ из входного файла
 * Глобальные переменные:
 *   si     - текущий считанный символ
 *   infil  - указатель на входной файл
 *   line   - текущая строка (для сообщений об ошибках)
 *   column - текущая колонка (для сообщений об ошибках)
 * 
 * Действие:
 *   1. Читает один символ из файла через getc()
 *   2. Если считан '\n' - увеличивает номер строки, сбрасывает колонку
 *   3. Иначе - увеличивает номер колонки
 * 
 * Вызывается: перед распознаванием каждой лексемы
 */
static void nxsi(void)
{
    if((si = getc(infil)) == '\n') {++line; column = 0;}
    else ++column;
}

//РАСПОЗНОВАНИЕ ЛЕКСЕМ 
static void find_idkw(void)
{
  for(int i = 0; i < rw_size; ++i)
    if(!strcmp(rwtab[i].wrd, lv)) {
      lc = rwtab[i].lc;
      return;
    }
  lc = lexId;
}

static void id_etc(void)
{
    while(isLetter(si) || isDigit(si) || si=='_') {
        lv[++i_lv] = si;
        nxsi();
    }
    lv[++i_lv] = '\0';
    find_idkw(); 
}

//------------------------------------------------------------------------------
//НОВАЯ ФУНКЦИЯ: двоичные числа (0b1010, 0B1010)
static void binary_number(void)
{
    nxsi(); // пропускаем 'b' или 'B'
    if(!isBin(si)) {
        lc = lexError; 
        er(4); 
        return;
    }
    while(isBin(si)) {
        lv[++i_lv] = si;
        nxsi();
    }
    lv[++i_lv] = '\0';
    lc = lexInt;
}

//------------------------------------------------------------------------------
//НОВАЯ ФУНКЦИЯ: шестнадцатеричные числа (0xFF, 0x1A)
static void hex_number(void)
{
    nxsi(); // пропускаем 'x' или 'X'
    if(!isHex(si)) {
        lc = lexError; 
        er(4); 
        return;
    }
    while(isHex(si)) {
        lv[++i_lv] = si;
        nxsi();
    }
    lv[++i_lv] = '\0';
    lc = lexInt;
}

//------------------------------------------------------------------------------
//НОВАЯ ФУНКЦИЯ: числа с префиксом 0
static void zero_prefix(void)
{
    lv[++i_lv] = si;  // сохраняем '0'
    nxsi();
    
    if(si == 'b' || si == 'B') {
        lv[++i_lv] = si;
        binary_number();
    }
    else if(si == 'x' || si == 'X') {
        lv[++i_lv] = si;
        hex_number();
    }
    else if(isOctal(si)) {
        // восьмеричное число
        while(isOctal(si)) {
            lv[++i_lv] = si;
            nxsi();
        }
        lv[++i_lv] = '\0';
        lc = lexInt;
    }
    else {
        // просто ноль
        lv[++i_lv] = '\0';
        lc = lexInt;
    }
}

//------------------------------------------------------------------------------
//НОВАЯ ФУНКЦИЯ: комментарии { } для Паскаля
static void comment_pascal(void)
{
    // Пропускаем символы до закрывающей '}'
    while(si != '}' && si != EOF) {
        lv[++i_lv] = si;
        nxsi();
    }
    
    if(si == '}') {
        nxsi();  // пропускаем '}'
        lv[++i_lv] = '\0';
        lc = lexCOMMENT;
    } else {
        lc = lexError;
        lv[++i_lv] = '\0';
        er(2);  // комментарий не закрыт
    }
}

static void expon(void) {
  lv[++i_lv]=si; nxsi();
  //_0:
    if(si == '+' || si == '-') {lv[++i_lv]=si; nxsi(); goto _1;}
    if(isDigit(si)) {lv[++i_lv]=si; nxsi(); goto _2;}
    lc=lexError; lv[++i_lv]='\0'; return;
  _1:
    if(isDigit(si)) {lv[++i_lv]=si; nxsi(); goto _2;}
    lc=lexError; lv[++i_lv]='\0'; er(3); return;
  _2:
    if(isDigit(si)) {lv[++i_lv]=si; nxsi(); goto _2;}
    if(isLetter(si)) {lc=lexError; lv[++i_lv]='\0'; er(3); return;}
    lc=lexFloat; lv[++i_lv]='\0'; return;
}


static void fltnumber(void) {
  lv[++i_lv]=si; nxsi();
  _0:
    if(isDigit(si)) {lv[++i_lv]=si; nxsi(); goto _0;}
    if(si == 'E' || si == 'e') {
      expon(); 
      return;
    }
    if(isLetter(si)) {lc=lexError; lv[++i_lv]='\0'; er(3); return;}
    lc=lexFloat; lv[++i_lv]='\0'; return; 
}


static void number(void) {
  do {
    lv[++i_lv] = si;
    nxsi();
  } while (isDigit(si));
  if(si == '.') {fltnumber(); return;}
  if(si=='e'||si=='E') {expon();return;}
  if(isLetter(si)) {lc=lexError; lv[++i_lv]='\0'; er(3); return;}
  lc = lexInt; lv[++i_lv] = '\0'; 
}


static void fltnumber2(void) {
  lv[++i_lv]=si; nxsi();
  //_0:
    if(isDigit(si)) {lv[++i_lv]=si; nxsi(); goto _1;}
    lc = lexError; lv[++i_lv]='\0'; er(5); return;
  _1:
    if(isDigit(si)) {lv[++i_lv]=si; nxsi(); goto _1;}
    if(si == 'E' || si == 'e') {expon(); return;}
    if(isLetter(si)) {lc=lexError; lv[++i_lv]='\0'; er(3); return;}
    lc=lexFloat; lv[++i_lv]='\0'; return; 
}




void nxl(void) {
  do {
    i_lv = -1;
    lv[0] = '\0';
    if(si == EOF) {lc = lexEof;}
    else if(isSkip(si)) {nxsi(); lc = lexSkip;}
    else if(isLetter(si) || si == '_'){
      lv[++i_lv]=si; nxsi(); id_etc();
    }
    else if(isDigit(si)) {
        if(si == '0') {
            zero_prefix();  //числа с префиксом 0
        } else {
            number();       //десятичные числа
        }
    }
    else if(isIgnore(si)) {nxsi(); lc = lexIgnore;}
    else if(si == '/') {
        nxsi(); 
        lc = lexDIV;           //просто оператор деления
        lv[++i_lv] = '/';
        lv[++i_lv] = '\0';
    }
    else if(si == ';') {nxsi(); lc = lexEOP;} 

    else if(si == ':') {
      nxsi(); 
      if(si == '=') {
        nxsi(); 
        lc = lexASS;        // := оператор присваивания
        lv[++i_lv] = ':';
        lv[++i_lv] = '=';
        lv[++i_lv] = '\0';
      } else {
        lc = lexColon;      // : метка 
        lv[++i_lv] = ':';
        lv[++i_lv] = '\0';
      }
    }

    else if(si == '*') {nxsi(); lc = lexMULT;} 

    else if(si == '+') {nxsi(); lc = lexPLUS;}   
    else if(si == '-') {
      nxsi(); 
      lc = lexMIN; 
      lv[++i_lv] = '-';
      lv[++i_lv] = '\0';
    }
    else if(si == '=') {nxsi(); lc = lexEQ;}

    else if(si == '>') {
      nxsi(); 
      if(si == '=') {nxsi(); lc = lexGE;}
      else lc = lexGT;           //ИСПРАВЛЕНО (было без else)
    }
    else if(si == '<') {
      nxsi();
      if(si == '=') {nxsi(); lc = lexLE;}
      else if(si == '>') {nxsi(); lc = lexNE;}  // ДОБАВЛЕНО <>
      else lc = lexLT;          
    }
    else if(si == '{') {
        nxsi(); 
        comment_pascal();       // комментарии { }
    }
    else if(si == '.') {lv[++i_lv]=si; nxsi(); fltnumber2();}
    else {lc = lexError; er(0); nxsi();}
  } while (lc == lexCOMMENT || lc == lexSkip || lc == lexIgnore);
}

bool scan_init(char *file_name) {
  if((infil = fopen(file_name, "r"))==0) {
    printf("\nInput file is absent!\n"); return false;
  }

  nxsi();
  nxl();

  return true;
}

void scan_destroy() {
  fclose(infil);
}
