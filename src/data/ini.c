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

ini_sect get_ini_section(const ini *ini_file, const char *sect_name) {
    ini_sect section_result = {0};

    // nullifying the file or sections is quite bad
    if (ini_file == NULL || sect_name == NULL) {
        LOG(LOG_ERROR, "one or more arguments of get_ini_section is null!");
        return section_result;
    }

    for (int i = 0; i < ini_file->sect_count; i++) {
        if (strncmp(ini_file->sections[i].name, sect_name, INI_MAX_NAME_LEN) == 0) {
            return ini_file->sections[i];
        }
    }

    LOG(LOG_WARN, "section \"%s\" not found in ini", sect_name);
    return section_result;
}

ini_pair get_ini_pair(const ini_sect *section, const char *key) {
    ini_pair pair_result = {0};

    // A null section or null key is not something we can handle
    if (section == NULL || key == NULL) {
        LOG(LOG_ERROR, "one or more arguments of get_ini_pair is null!");
        return pair_result;
    }

    for (int i = 0; i < section->pair_count; i++) {
        if (strncmp(section->pairs[i].key, key, INI_MAX_KEY_LEN) == 0) {
            return section->pairs[i];
        }
    }

    LOG(LOG_WARN, "key \"%s\" not found in section \"%s\"", key, section->name);
    return pair_result;
}