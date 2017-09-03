#define STR_(x) #x
#define STR(x) STR_(x)

#define IDB_ALPHA      101
#define IDB_BACK       102
#define IDB_CLOSE      103
#define IDB_FOREGROUND 104
#define IDB_HALFTONE   105
#define IDB_HELP       106
#define IDB_HOLE       107
#define IDB_LOCK       108
#define IDB_MAXIMIZE   109
#define IDB_MINIMIZE   110
#define IDB_RESET      111
#define IDB_SCALE      112

#define IDI_NAGARAMI 201

#define VER_FILEDESCRIPTION "Nagaramiユーティリティ"
#define VER_LEGALCOPYRIGHT  "Copyright (c) 2017 Gonbee"
#define VER_PRODUCTNAME     "Nagarami"
#define VER_VERSION\
    VER_MAJOR,\
    VER_MINOR,\
    VER_BUILD,\
    VER_REVISION
#define VER_VERSION_STR\
    STR(VER_MAJOR) "."\
    STR(VER_MINOR) "."\
    STR(VER_BUILD) "."\
    STR(VER_REVISION)
#define VER_MAJOR    0
#define VER_MINOR    2
#define VER_BUILD    0
#define VER_REVISION 1
