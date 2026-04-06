#include "data/ini.h"
#include "log/log.h"
#include <string.h>
#include <stdio.h>

//
// This is hard to read code, sorry about that.
// But hey, that's the joy of writing good old C! :^)
//

ini parse_ini_file(const char *path, int *err) {
    ini result = {0};

    FILE *ini_fptr;
    ini_fptr = fopen(path, "r");

    if (ini_fptr == NULL) {
        LOG(LOG_ERROR, "ini file not found in path \"%s\"", path);
        if (err) *err = 1;
        return result;
    }

    ini_sect *cur_sect = NULL;

    // Naive read, just go through line-by-line
    // 257 ==> 256 chars + \n OR \0
    char buffer[257];
    while (fgets(buffer, sizeof(buffer), ini_fptr)) {
        char *p = buffer;

        while (*p == ' ' || *p == '\t') p++;

        if (*p == '\n' || *p == '\0'
            || *p == comment[0] || *p == comment[1]) continue;

        // section parsing
        if (*p == sect_start) {
            char *start = ++p;

            while (*p && *p != sect_end && *p != '\n') p++;
            *p = '\0';

            if (result.sect_count < INI_MAX_SECTIONS) {
                ini_sect *sect = &result.sections[result.sect_count++];
                strncpy(sect->name, start, INI_MAX_NAME_LEN);
                cur_sect = sect;
            }
        } else {
            // key-val parsing
            char *key = p;

            while (*p && *p != '=' && *p != '\n') p++;
            if (*p != '=') {
                LOG(LOG_WARN, "malformed line in ini");
                continue;
            }

            *p++ = '\0';
            char *val = p;

            // removing inline comments and keeping quoted vals
            int in_quotes = 0;
            for (char *c = val; *c && *c != '\n'; c++) {
                if (*c == '\\' && *(c+1) == '"') { c++; continue; }
                if (*c == '"') { in_quotes = !in_quotes; continue; }
                if ((*c == comment[0] || *c == comment[1]) && !in_quotes) {
                    *c = '\0';
                    break;
                }
            }

            if (cur_sect != NULL && cur_sect->pair_count < INI_MAX_PAIRS) {
                ini_pair *pair = &cur_sect->pairs[cur_sect->pair_count++];
                strncpy(pair->key, key, INI_MAX_KEY_LEN);
                strncpy(pair->val, val, INI_MAX_VAL_LEN);
            } else if (cur_sect == NULL) {
                LOG(LOG_WARN, "key-val pair not in section, ignoring");
            }
        }
    }

    fclose(ini_fptr);
    if (err) *err = 0;
    return result;
}