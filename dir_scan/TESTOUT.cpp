//------------------------------------------------------------------------------
// tesout.cpp — вывод результатов лексического анализа (вариант 1)
//------------------------------------------------------------------------------

#include <stdio.h>
#include "scaner_ext.h"

void fprintlex(FILE* f) 
{
    const char* lcs;
    const char* lvs;

    switch(lc) 
    {
        case lexId:        lcs="lexId";       lvs=lv; break;
        case lexError:     lcs="lexError";    lvs=lv; break;
        case lexInt:       lcs="lexInt";      lvs=lv; break;
        case lexFloat:     lcs="lexFloat";    lvs=lv; break;
        case lexString:    lcs="lexString";   lvs=lv; break;
        case lexEof:       lcs="lexEof";      lvs="EOF"; break;
        case lexSkip:      lcs="lexSkip";     lvs=" "; break;
        case lexIgnore:    lcs="lexIgnore";   lvs=lv; break;
        case lexColon:     lcs="lexColon";    lvs=":"; break;

        case lexPLUS:      lcs="lexPLUS";     lvs="+"; break;
        case lexMIN:       lcs="lexMIN";      lvs="-"; break;
        case lexMULT:      lcs="lexMULT";     lvs="*"; break;
        case lexDIV:       lcs="lexDIV";      lvs="/"; break;
        case lexMOD:       lcs="lexMOD";      lvs="mod"; break;

        case lexEQ:        lcs="lexEQ";       lvs="="; break;
        case lexNE:        lcs="lexNE";       lvs="<>"; break;
        case lexLT:        lcs="lexLT";       lvs="<"; break;
        case lexGT:        lcs="lexGT";       lvs=">"; break;
        case lexLE:        lcs="lexLE";       lvs="<="; break;
        case lexGE:        lcs="lexGE";       lvs=">="; break;

        case lexASS:       lcs="lexASS";      lvs=":="; break;

        case lexCOMMENT:  lcs="lexCOMMENT";  lvs=lv; break;

        case lexBST:      lcs="lexBST";      lvs="begin"; break;
        case lexEST:      lcs="lexEST";      lvs="end"; break;

        case lexEOP:      lcs="lexEOP";      lvs=";"; break;

        case kwVECTOR:    lcs="kwVECTOR";    lvs=lv; break;
        case kwOF:        lcs="kwOF";        lvs=lv; break;
        case kwINTEGER:  lcs="kwINTEGER";  lvs=lv; break;
        case kwREAL:     lcs="kwREAL";     lvs=lv; break;

        case kwGOTO:     lcs="kwGOTO";     lvs=lv; break;
        case kwREAD:     lcs="kwREAD";     lvs=lv; break;
        case kwWRITE:    lcs="kwWRITE";    lvs=lv; break;

        case kwSKIP:      lcs="kwSKIP";      lvs=lv; break;
        case kwSPACE:    lcs="kwSPACE";    lvs=lv; break;
        case kwTAB:      lcs="kwTAB";      lvs=lv; break;

        case kwIF:        lcs="kwIF";        lvs=lv; break;
        case kwTHEN:      lcs="kwTHEN";      lvs=lv; break;
        case kwELSE:      lcs="kwELSE";      lvs=lv; break;
        case kwWHILE:     lcs="kwWHILE";     lvs=lv; break;
        case kwDO:        lcs="kwDO";        lvs=lv; break;

        default:
            lcs="UNKNOWN";
            lvs=lv;
    }

    fprintf(f, "[%d, %d] lc: %-10s  lv: %s\n", line, column, lcs, lvs);
}
