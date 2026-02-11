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
- divcom() — комментарии /* */
//- string_const() — строки "..."

//Что делать: ИЗМЕНЯТЬ логику распознавания


#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "scaner_data.h"
#include "scaner_local.h"

//------------------------------------------------------------------------------

void er(int i);

bool inline isLetter(int ch) {
  if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
    return true;
  else
    return false;
}

//------------------------------------------------------------------------------

bool inline isBin(int ch) {
  if((ch == '0' || ch == '1'))
    return true;
  else
    return false;
}

//------------------------------------------------------------------------------

bool inline isOctal(int ch) {
  if((ch >= '0' && ch <= '7'))
    return true;
  else
    return false;
}

//------------------------------------------------------------------------------

bool inline isDigit(int ch) {
  if((ch >= '0' && ch <= '9'))
    return true;
  else
    return false;
}

//------------------------------------------------------------------------------

bool inline isHex(int ch) {
  if((ch >= '0' && ch <= '9') ||
     (ch >= 'A' && ch <= 'F') || 
     (ch >= 'a' && ch <= 'f'))
    return true;
  else
    return false;
}

//------------------------------------------------------------------------------

bool inline isSkip(int ch) {
  if(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\f')
    return true;
  else
    return false;
}

//------------------------------------------------------------------------------

// ���������� �������������� ������� � ������ ������������
bool inline isIgnore(int ch) {
  if(ch >0 && ch < ' ' && ch != '\t' && ch != '\n' && ch != '\f')
    return true;
  else
    return false;
}

//------------------------------------------------------------------------------

// ������ ��������� ������ �� �������� ������
static void nxsi(void)
{
    if((si = getc(infil)) == '\n') {++line; column = 0;}
    else ++column;
}

//------------------------------------------------------------------------------

// ���������� �������������� � ��������� �����
static void find_idkw(void)
{
  for(int i = 0; i < rw_size; ++i)
    if(!strcmp(rwtab[i].wrd, lv)) {
      lc = rwtab[i].lc;
      return;
    }
  lc = lexId;
}

//------------------------------------------------------------------------------

// �������� �� ������������� ��� �������� �����
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

// ���� ������� ��� �����������
static void divcom(void)
{
    if(si != '*') {lc = lexSlash; return;}
    nxsi();
loop: 
    if(si == EOF) { 
      lc = lexError; lv[++i_lv]='\0'; er(2); return; 
    } 
    while(si != '*') {
        lv[++i_lv] = si;
        if(si == EOF) {lc = lexError; lv[++i_lv]='\0'; er(2); return;} 
        nxsi();
    }
    nxsi(); if(si != '/') {lv[++i_lv] = si; goto loop;}
    lv[++i_lv] = '\0';
    lc = lexComment;
    nxsi();
}

//------------------------------------------------------------------------------

// �������� ������� �����
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

//------------------------------------------------------------------------------

// ����� � ��������� ������
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

//------------------------------------------------------------------------------

// ����� ��� ��������������
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

//------------------------------------------------------------------------------

// ����� � ���������
static void prenumber(void) {
    //_0:
        if(isDigit(si)) {
          lv[++i_lv] = si; nxsi(); goto _1;
        }
        lc = lexError; lv[++i_lv] = '\0'; er(3); return;
    _1:
        if(isDigit(si)) {lv[++i_lv] = si; nxsi(); goto _1;}
        if(si=='}') {
          lv[++i_lv] = '\0'; nxsi();
          if(!strcmp(lv, "2")) {lv[i_lv]='#'; goto _2;}
          if(!strcmp(lv, "8")) {lv[i_lv]='#'; goto _3;}
          if(!strcmp(lv, "10")) {lv[i_lv]='#'; goto _4;}
          if(!strcmp(lv, "16")) {lv[i_lv]='#'; goto _5;}
          lc = lexError; lv[++i_lv] = '\0'; er(4); return;
        }
        lc = lexError; lv[++i_lv] = '\0'; er(3); return;
    _2:
        if(isBin(si)) {lv[++i_lv]=si; nxsi(); goto _2;}
        if(isLetter(si) || isDigit(si)) 
        {
          lc = lexError; lv[++i_lv]='\0'; er(3); return;
        }
        lc = lexInt; lv[++i_lv] ='\0'; return;
    _3:
        if(isOctal(si)) {lv[++i_lv]=si; nxsi(); goto _3;}
        if(isLetter(si) || isDigit(si))
        {
          lc = lexError; lv[++i_lv]='\0'; er(3); return;
        }
        lc = lexInt; lv[++i_lv] ='\0'; return;
    _4:
        if(isDigit(si)) {lv[++i_lv]=si; nxsi(); goto _4;}
        if(isLetter(si)) {lc = lexError; lv[++i_lv]='\0'; er(3); return;}
        lc = lexInt; lv[++i_lv] ='\0'; return;
    _5:
        if(isHex(si)) 
        {
          lv[++i_lv]=si; nxsi(); goto _5;
        }
        if(isLetter(si)) {lc = lexError; lv[++i_lv]='\0'; er(3); return;}
        lc = lexInt; lv[++i_lv]='\0'; return;
}

//------------------------------------------------------------------------------

// ��������������, ������������ � �����
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

//------------------------------------------------------------------------------

// ������ ��������
static void string_const(void)
{
_1:
  if(si == '\"') { // ������� ������ ��� �������� � ������
    nxsi(); goto _2;
  }
  if(si == EOF) { // ����� �����, � ������ �� �����������
    lc = lexError; lv[++i_lv]='\0'; er(6); return; 
  } 
  lv[++i_lv]=si; 
  nxsi();
  goto _1; // ����� ������ ������ ������ ������
_2:
  if(si == '\"') { // ����� ��� ���������
    lv[++i_lv]=si; // ��������� ��� � ������ ��� ����     
    nxsi(); 
    goto _1;
  }
  lc = lexString; lv[++i_lv] = '\0'; return; // ����������� ������
}

//------------------------------------------------------------------------------
// �������, ����������� ��������� �������
// ���������� �������������� ������������
//------------------------------------------------------------------------------

void nxl(void) {
  do {
    i_lv = -1;
    lv[0] = '\0';
    if(si == EOF) {lc = lexEof;}
    else if(isSkip(si)) {nxsi(); lc = lexSkip;}
    else if(isLetter(si) || si == '_'){
      lv[++i_lv]=si; nxsi(); id_etc();
    }
    else if(isDigit(si)) {number();}
    else if(isIgnore(si)) {nxsi(); lc = lexIgnore;}
    else if(si == '/') {nxsi(); divcom();}
    else if(si == '\"') {nxsi(); string_const();}
    else if(si == ';') {nxsi(); lc = lexSemicolon;}
    else if(si == ',') {nxsi(); lc = lexComma;}
    else if(si == ':') {
      nxsi(); 
      if(si == '=') {nxsi(); lc = lexAssign;}
      else lc = lexColon;
    }
    else if(si == '(') {nxsi(); lc = lexLftRndBr;}
    else if(si == ')') {nxsi(); lc = lexRghRndBr;}
    else if(si == '[') {nxsi(); lc = lexLftSqBr;}
    else if(si == ']') {nxsi(); lc = lexRghSqBr;}
    else if(si == '*') {nxsi(); lc = lexStar;}
    else if(si == '%') {nxsi(); lc = lexPercent;}
    else if(si == '+') {nxsi(); lc = lexPlus;}
    else if(si == '-') {
      nxsi(); 
      if(si == '>') {nxsi(); lc = lexArrow;}  
      else lc = lexMinus;
    }
    else if(si == '=') {nxsi(); lc = lexEQ;}
    else if(si == '!') {
      nxsi(); 
      if(si == '=') {nxsi(); lc = lexNE;}
      else {lc = lexError; er(1);}
    }
    else if(si == '>') {
      nxsi(); 
      if(si == '=') {nxsi(); lc = lexGE;}
      lc = lexGT;
    }
    else if(si == '<') {
      nxsi();
      if(si == '=') {nxsi(); lc = lexLE;}
      lc = lexLT;
    }
    else if(si == '{') {nxsi(); prenumber();}
    else if(si == '.') {lv[++i_lv]=si; nxsi(); fltnumber2();}
    else {lc = lexError; er(0); nxsi();}
  } while (lc == lexComment || lc == lexSkip || lc == lexIgnore);
}

//------------------------------------------------------------------------------
// �������, ������������ ������ �� ��������
// ���������� ����� �������� ��������������� �����������
//------------------------------------------------------------------------------

bool scan_init(char *file_name) {
  if((infil = fopen(file_name, "r"))==0) {
    printf("\nInput file is absent!\n"); return false;
  }

  nxsi();
  nxl();

  return true;
}

//------------------------------------------------------------------------------
// �������, ����������� ������ �������
//------------------------------------------------------------------------------

void scan_destroy() {
  fclose(infil);
}
