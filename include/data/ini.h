//
// ini.h -> In-house implementation of INI files for PlatinumSrc
//
// An ini file is a basic, text-based configuration file that's
// existed for a long time, mostly on Windows 9X platforms. Since
// the ini format is basic, PlatinumSrc implements this without
// external libs, just libc.
//
// We recommend you use ini files as they're simple and this
// implementation exists, but if you want to use something like
// yaml, xml, or toml from an external dependency, that's cool!
//

#ifndef _INI_H
#define _INI_H

#define INI_MAX_LINE_LEN 128
#define INI_MAX_NAME_LEN 128
#define INI_MAX_PAIRS 128
#define INI_MAX_SECTIONS 32
#define INI_MAX_KEY_LEN 128
#define INI_MAX_VAL_LEN 256

static const char comment[2]    = { '#', ';' };
static const char sect_start    = '[';
static const char sect_end      = ']';

typedef struct {
    char key[INI_MAX_KEY_LEN];
    char val[INI_MAX_VAL_LEN];
} ini_pair;

typedef struct {
    char name[INI_MAX_NAME_LEN];
    ini_pair pairs[INI_MAX_PAIRS];
    int pair_count;
} ini_sect;

typedef struct {
    ini_sect sections[INI_MAX_SECTIONS];
    int sect_count;
} ini;

ini parse_ini_file(const char *path, int *err);

ini_sect get_ini_section(const ini *ini_file, const char *sect_name);

ini_pair get_ini_pair(const ini_sect *section, const char *key);

ini_pair set_ini_pair(const char *key, const char *value);

#endif