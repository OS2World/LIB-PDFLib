#line 219 "..\\zlib.1-1-3\\zconf.h"
typedef unsigned char  Byte;   

typedef unsigned int   uInt;   
typedef unsigned long  uLong;  
#line 228
   typedef Byte   Bytef;

typedef char   charf;
typedef int    intf;
typedef uInt   uIntf;
typedef uLong  uLongf;
#line 236
   typedef void  *voidpf;
   typedef void     *voidp;
#line 63 "..\\zlib.1-1-3\\zlib.h"
typedef voidpf (*alloc_func) (voidpf opaque, uInt items, uInt size);
typedef void   (*free_func)  (voidpf opaque, voidpf address);

struct internal_state;

typedef struct z_stream_s {
    Bytef    *next_in;   
    uInt     avail_in;   
    uLong    total_in;   

    Bytef    *next_out;  
    uInt     avail_out;  
    uLong    total_out;  

    char     *msg;       
    struct internal_state  *state;  

    alloc_func zalloc;   
    free_func  zfree;    
    voidpf     opaque;   

    int     data_type;   
    uLong   adler;       
    uLong   reserved;    
} z_stream;

typedef z_stream  *z_streamp;
#line 171
extern const char *  zlibVersion (void);
#line 201
extern int  deflate (z_streamp strm, int flush);
#line 279
extern int  deflateEnd (z_streamp strm);
#line 314
extern int  inflate (z_streamp strm, int flush);
#line 383
extern int  inflateEnd (z_streamp strm);
#line 444
extern int  deflateSetDictionary 

(z_streamp strm, const Bytef *dictionary, uInt dictLength);
#line 480
extern int  deflateCopy 
(z_streamp dest, z_streamp source);
#line 498
extern int  deflateReset (z_streamp strm);
#line 509
extern int  deflateParams 

(z_streamp strm, int level, int strategy);
#line 553
extern int  inflateSetDictionary 

(z_streamp strm, const Bytef *dictionary, uInt dictLength);
#line 572
extern int  inflateSync (z_streamp strm);
#line 587
extern int  inflateReset (z_streamp strm);
#line 608
extern int  compress 
(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
#line 623
extern int  compress2 

(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen, int level);
#line 638
extern int  uncompress 
(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
#line 657
typedef voidp gzFile;

extern gzFile  gzopen  (const char *path, const char *mode);
#line 675
extern gzFile  gzdopen  (int fd, const char *mode);
#line 688
extern int  gzsetparams (gzFile file, int level, int strategy);
#line 696
extern int     gzread  (gzFile file, voidp buf, unsigned len);
#line 704
extern int     gzwrite 
(gzFile file, const voidp buf, unsigned len);
#line 712
extern int    gzprintf (gzFile file, const char *format, ...);
#line 719
extern int  gzputs (gzFile file, const char *s);
#line 726
extern char *  gzgets (gzFile file, char *buf, int len);
#line 735
extern int     gzputc (gzFile file, int c);
#line 741
extern int     gzgetc (gzFile file);
#line 747
extern int     gzflush (gzFile file, int flush);
#line 757
extern long     gzseek 
(gzFile file, long offset, int whence);
#line 775
extern int     gzrewind (gzFile file);
#line 782
extern long     gztell (gzFile file);
#line 791
extern int  gzeof (gzFile file);
#line 797
extern int     gzclose (gzFile file);
#line 804
extern const char *  gzerror (gzFile file, int *errnum);
#line 821
extern uLong  adler32 (uLong adler, const Bytef *buf, uInt len);
#line 838
extern uLong  crc32   (uLong crc, const Bytef *buf, uInt len);
#line 860
extern int  deflateInit_ 
(z_streamp strm, int level, const char *version, int stream_size);
extern int  inflateInit_ 
(z_streamp strm, const char *version, int stream_size);
extern int  deflateInit2_ 
#line 867
(z_streamp strm, int level, int method, int windowBits, int memLevel, int strategy, const char *version, int stream_size);
extern int  inflateInit2_ 
(z_streamp strm, int windowBits, const char *version, int stream_size);
#line 882
    struct internal_state {int dummy;};  
#line 885
extern const char   *  zError           (int err);
extern int             inflateSyncPoint (z_streamp z);
extern const uLongf *  get_crc_table    (void);
#line 3 "F:\\Ibmcxxo\\INCLUDE\\stdio.h"
      #pragma info(none)

         #pragma info(none)

      #pragma info(restore)
#line 43
      typedef unsigned int size_t;
#line 46
   enum _OPERATIONS { _IOINIT, _IOREAD, _IOWRITE, _IOREPOSITION,
                      _IOFLUSH, _IOUNDEFOP };
#line 51
   #pragma pack(1)
   typedef struct __file {   
      unsigned char     *_bufPtr;
      unsigned long int  _filler1;  
      unsigned long int  _userFlags;
      unsigned long int  _bufLen;
      unsigned long int  _ungetCount;
      unsigned long int  _filler2;  
      unsigned char      _ungetBuf[2];
      char               _filler3;  
      enum _OPERATIONS   _lastOp;
      signed char        _orientation;
      unsigned char     *_buf;
      unsigned long int  _bufCount;
      unsigned long int  _count;
      } FILE;
   #pragma pack()

   typedef struct __fpos_t {   
      long int __fpos_elem[2];
      } fpos_t;
#line 108
         typedef char *__va_list;
#line 114
   extern FILE *const  stdin;
   extern FILE *const  stdout;
   extern FILE *const  stderr;
#line 123
   extern int       _Optlink fprintf(FILE *, const char *, ...);
   extern int       _Optlink fscanf(FILE *, const char *, ...);
   extern int       _Optlink printf(const char *, ...);
   extern int       _Optlink scanf(const char *, ...);
   extern int       _Optlink sprintf(char *, const char *, ...);
   extern int       _Optlink sscanf(const char *, const char *, ...);
   extern void      _Optlink clearerr(FILE *);
   extern int       _Optlink fclose(FILE *);
   extern int       _Optlink feof(FILE *);
   extern int       _Optlink ferror(FILE *);
   extern int       _Optlink fflush(FILE *);
   extern int       _Optlink fgetc(FILE *);
   extern int       _Optlink fgetpos(FILE *, fpos_t *);
   extern char    * _Optlink fgets(char *, int, FILE *);
   extern FILE    * _Optlink fopen(const char *, const char *);
   extern int       _Optlink fputc(int, FILE *);
   extern int       _Optlink fputs(const char *, FILE *);
   extern size_t    _Optlink fread(void *, size_t, size_t, FILE *);
   extern FILE    * _Optlink freopen(const char *, const char *, FILE *);
   extern int       _Optlink fseek(FILE *, long int, int);
   extern int       _Optlink fsetpos(FILE *, const fpos_t *);
   extern long int  _Optlink ftell(FILE *);
   extern size_t    _Optlink fwrite(const void *, size_t, size_t, FILE *);
   extern int              _Optlink getc(FILE *);
   extern int              _Optlink getchar(void);
   extern char    * _Optlink gets(char *);
   extern void      _Optlink perror(const char *);
   extern int              _Optlink putc(int, FILE *);
   extern int              _Optlink putchar(int);
   extern int       _Optlink puts(const char *);
   extern int       _Optlink remove(const char *);
   extern int       _Optlink rename(const char *, const char *);
   extern void      _Optlink rewind(FILE *);
   extern void      _Optlink setbuf(FILE *, char *);
   extern int       _Optlink setvbuf(FILE *, char *, int, size_t);
   extern FILE    * _Optlink tmpfile(void);
   extern char    * _Optlink tmpnam(char *);
   extern int       _Optlink ungetc(int, FILE *);
   extern int       _Optlink vfprintf(FILE *, const char *, __va_list);
   extern int       _Optlink vprintf(const char *, __va_list);
   extern int       _Optlink vsprintf(char *, const char *, __va_list);
#line 247
            #pragma map(fprintf, "_fprintfieee")
            #pragma map(printf, "_printfieee")
            #pragma map(sprintf, "_sprintfieee")
            #pragma map(fscanf, "_fscanfieee")
            #pragma map(scanf, "_scanfieee")
            #pragma map(sscanf, "_sscanfieee")
            #pragma map(vfprintf, "_vfprintfieee")
            #pragma map(vprintf, "_vprintfieee")
            #pragma map(vsprintf, "_vsprintfieee")
#line 279
      int _Optlink _fcloseall(void);
      int _Optlink _rmtmp(void);
#line 284
      extern FILE * _Optlink fdopen(int, const char *);
      extern int    _Optlink fgetchar(void);
      extern int    _Optlink fileno(FILE *);
      extern int    _Optlink flushall(void);
      extern int    _Optlink fputchar(int);
      extern char * _Optlink tempnam(char *, char *);
      extern int    _Optlink unlink(const char *);

      extern FILE * _Optlink _fdopen(int, const char *);
      extern int    _Optlink _fgetchar(void);
      extern int    _Optlink _fileno(FILE *);
      extern int    _Optlink _flushall(void);
      extern int    _Optlink _fputchar(int);
      extern char * _Optlink _tempnam(char *, char *);
      extern int    _Optlink _unlink(const char *);

      extern int    _Optlink _set_crt_msg_handle(int);
#line 320
            #pragma map(fdopen, "_fdopen")
            #pragma map(fgetchar, "_fgetchar")
            #pragma map(fileno, "_fileno")
            #pragma map(flushall, "_flushall")
            #pragma map(fputchar, "_fputchar")
            #pragma map(tempnam, "_tempnam")
#line 328
               #pragma map(unlink, "_unlink")
#line 355
      #pragma info(none)

         #pragma info(restore)

      #pragma info(restore)
#line 5 "F:\\Ibmcxxo\\INCLUDE\\sys/types.h"
      #pragma info(none)

         #pragma info(none)

      #pragma info(restore)
#line 30
               typedef signed int ssize_t;
#line 35
               typedef unsigned short ino_t;
#line 41
               typedef double time_t;
#line 45
               typedef short dev_t;
#line 50
               typedef long off_t;
#line 62
      #pragma info(none)

         #pragma info(restore)

      #pragma info(restore)
#line 3 "F:\\Ibmcxxo\\INCLUDE\\setjmp.h"
      #pragma info(none)

         #pragma info(none)

      #pragma info(restore)
#line 42
      typedef int jmp_buf[8];
#line 47
   extern int  _Optlink setjmp(jmp_buf);
   extern void  _Optlink longjmp(jmp_buf, int);
#line 61
      #pragma info(none)

         #pragma info(restore)

      #pragma info(restore)
#line 3 "F:\\Ibmcxxo\\INCLUDE\\string.h"
      #pragma info(none)

         #pragma info(none)

      #pragma info(restore)
#line 88
         char  *_Builtin __strcpy(char*, const char*);
         int    _Builtin __strcmp(const char*, const char*);
         size_t _Builtin __strlen(const char*);
         char  *_Builtin __strchr(const char*, int);
         char  *_Builtin __strrchr(const char*, int);
         char  *_Builtin __strcat(char*, const char*);
         extern char *  _Optlink strcat(char *, const char *);
         extern char *  _Optlink strchr(const char *, int);
         extern int     _Optlink strcmp(const char *, const char *);
         extern char *  _Optlink strcpy(char *, const char *);
         extern size_t  _Optlink strlen(const char *);
         extern char *  _Optlink strrchr(const char *, int);
#line 108
      #pragma isolated_call(__strcpy)
      #pragma isolated_call(__strcmp)
      #pragma isolated_call(__strlen)
      #pragma isolated_call(__strchr)
      #pragma isolated_call(__strrchr)
      #pragma isolated_call(__strcat)
#line 127
            char *_Builtin __strncat(char *, const char *, size_t);
            int   _Builtin __strncmp(const char *, const char *, size_t);
            char *_Builtin __strncpy(char *, const char *, size_t);
            extern char * _Optlink strncat(char *, const char *, size_t);
            extern int    _Optlink strncmp(const char *, const char *, size_t);
            extern char * _Optlink strncpy(char *, const char *, size_t);
#line 137
         #pragma isolated_call(__strncat)
         #pragma isolated_call(__strncmp)
         #pragma isolated_call(__strncpy)
#line 176
   extern int     _Optlink strcoll(const char *, const char *);
   extern size_t  _Optlink strcspn(const char *, const char *);
   extern char *  _Optlink strerror(int);
   extern size_t  _Optlink strspn(const char *, const char *);
   extern char  * _Optlink strtok(char*, const char *);
   extern size_t  _Optlink strxfrm(char *, const char *, size_t);
#line 186
      extern char  * _Optlink strpbrk(const char *, const char *);
      extern char  * _Optlink strstr(const char *, const char *);
#line 214
            void *_Builtin __memchr(const void *, int, size_t);
            void *_Builtin __memcpy(void *, const void *, size_t);
            int   _Builtin __memcmp(const void *, const void *, size_t);
            void *_Builtin __memmove(void *, const void *, size_t);
            void *_Builtin __memset(void *, int, size_t);
            extern void * _Optlink memchr(const void *, int, size_t);
            extern int    _Optlink memcmp(const void *, const void *, size_t);
            extern void * _Optlink memcpy(void *, const void *, size_t);
            extern void * _Optlink memmove(void *, const void *, size_t);
            extern void * _Optlink memset(void *, int, size_t);
#line 231
         #pragma isolated_call(__memchr)
         #pragma isolated_call(__memcpy)
         #pragma isolated_call(__memcmp)
         #pragma isolated_call(__memmove)
         #pragma isolated_call(__memset)
#line 247
      extern void * _Optlink memccpy(void *, const void *, int, unsigned int);
      extern int    _Optlink memicmp(const void *, const void *, unsigned int);

      extern void * _Optlink _fmemcpy(void *, const void *, size_t);
      extern void * _Optlink _fmemchr(const void *, int, size_t);
      extern int    _Optlink _fmemcmp(const void *, const void *, size_t);
      extern void * _Optlink _fmemset(void *, int, size_t);
      extern void * _Optlink _fmemccpy(void *, const void *, int, unsigned int);
      extern int    _Optlink _fmemicmp(const void *, const void *, unsigned int);
#line 281
            #pragma map(_fmemcpy, "memcpy")
            #pragma map(_fmemchr, "memchr")
            #pragma map(_fmemcmp, "memcmp")
            #pragma map(_fmemset, "memset")
            #pragma map(_fmemccpy, "memccpy")
            #pragma map(_fmemicmp, "memicmp")
#line 299
      char * _Optlink strdup(const char *);
      int    _Optlink stricmp(const char *, const char *);
      char * _Optlink strlwr(char *);
      int    _Optlink strnicmp(const char *, const char *, size_t);
      char * _Optlink strupr(char *);
      int    _Optlink strcmpi(const char *, const char *);
      char * _Optlink strrev(char *);
      char * _Optlink strset(char *, int);
      char * _Optlink strnset(char *, int, size_t);
      char * _Optlink _strerror(const char *);
#line 340
      size_t  _Optlink _fstrcspn(const char *, const char *);
      char  * _Optlink _fstrdup(const char *);
      char  * _Optlink _nstrdup(const char *);
      int     _Optlink _fstricmp(const char *, const char *);
      char  * _Optlink _fstrlwr(char *);
      int     _Optlink _fstrnicmp(const char *, const char *, size_t);
      char  * _Optlink _fstrnset(char *, int, size_t);
      char  * _Optlink _fstrpbrk(const char *, const char *);
      char  * _Optlink _fstrrev(char *);
      char  * _Optlink _fstrset(char *, int);
      size_t  _Optlink _fstrspn(const char *, const char *);
      char  * _Optlink _fstrstr(const char *, const char *);
      char  * _Optlink _fstrtok(char*, const char *);
      char  * _Optlink _fstrupr(char *);
      void  * _Optlink _fmemmove(void *, const void *, size_t);
      char  * _Optlink _fstrcat(char *, const char *);
      char  * _Optlink _fstrchr(const char *, int);
      int     _Optlink _fstrcmp(const char *, const char *);
      char  * _Optlink _fstrcpy(char *, const char *);
      size_t  _Optlink _fstrlen(const char *);
      char  * _Optlink _fstrncat(char *, const char *, size_t);
      int     _Optlink _fstrncmp(const char *, const char *, size_t);
      char  * _Optlink _fstrncpy(char *, const char *, size_t);
      char  * _Optlink _fstrrchr(const char *, int);
#line 392
            #pragma map(_fstrcspn, "strcspn")
            #pragma map(_fstrdup, "strdup")
            #pragma map(_nstrdup, "strdup")
            #pragma map(_fstricmp, "stricmp")
            #pragma map(_fstrlwr, "strlwr")
            #pragma map(_fstrnicmp, "strnicmp")
            #pragma map(_fstrnset, "strnset")
            #pragma map(_fstrpbrk, "strpbrk")
            #pragma map(_fstrrev, "strrev")
            #pragma map(_fstrset, "strset")
            #pragma map(_fstrspn, "strspn")
            #pragma map(_fstrstr, "strstr")
            #pragma map(_fstrtok, "strtok")
            #pragma map(_fstrupr, "strupr")
            #pragma map(_fmemmove, "memmove")
            #pragma map(_fstrcat, "strcat")
            #pragma map(_fstrchr, "strchr")
            #pragma map(_fstrcmp, "strcmp")
            #pragma map(_fstrcpy, "strcpy")
            #pragma map(_fstrlen, "strlen")
            #pragma map(_fstrncat, "strncat")
            #pragma map(_fstrncmp, "strncmp")
            #pragma map(_fstrncpy, "strncpy")
            #pragma map(_fstrrchr, "strrchr")
#line 497
      #pragma info(none)

         #pragma info(restore)

      #pragma info(restore)
#line 3 "F:\\Ibmcxxo\\INCLUDE\\stdlib.h"
      #pragma info(none)

         #pragma info(none)

      #pragma info(restore)
#line 61
      typedef unsigned short wchar_t;
#line 64
   typedef struct _div_t {
       int quot;         
       int rem;          
       } div_t;

   typedef struct _ldiv_t {
       long int quot;    
       long int rem;     
       } ldiv_t;
#line 75
      typedef struct _lldiv_t {
         long long int quot;    
         long long int rem;     
         } lldiv_t;
#line 102
         typedef unsigned long _System _EH_RTN(void *,void *,void *,void *);
#line 108
   extern int  _Optlink _eh_del(_EH_RTN *);
   extern int  _Optlink _eh_add(_EH_RTN *);
#line 126
         extern int  _mb_cur_max;
#line 133
   extern int       _Optlink atoi(const char *);
   extern long int  _Optlink atol(const char *);
   extern long int  _Optlink strtol(const char *, char **, int);
   extern unsigned long int  _Optlink strtoul(const char *, char **, int);
   extern int       _Optlink rand(void);
   extern void      _Optlink srand(unsigned int);
   extern void *   ( _Optlink calloc)(size_t, size_t);
   extern void     ( _Optlink free)(void *);
   extern void *   ( _Optlink malloc)(size_t);
   extern void *   ( _Optlink realloc)(void *, size_t);
   extern void      _Optlink abort(void);
   extern int              _Optlink atexit(void (*)(void));
   extern void             _Optlink exit(int);
   extern char *    _Optlink getenv(const char *);
   extern int       _Optlink system(const char *);
   extern void *    _Optlink bsearch(const void *, const void *, size_t, size_t,
                              int (*_Optlink)(const void *, const void *));
   extern void      _Optlink qsort(void *, size_t, size_t,
                            int (*_Optlink)(const void *, const void *));
   extern div_t     _Optlink div(int, int);
   extern ldiv_t    _Optlink ldiv(long int, long int);
   extern int       _Optlink mblen(const char *, size_t);
   extern int       _Optlink mbtowc(wchar_t *, const char *, size_t);
   extern int       _Optlink wctomb(char *, wchar_t);
   extern size_t    _Optlink mbstowcs(wchar_t *, const char *, size_t);
   extern size_t    _Optlink wcstombs(char *, const wchar_t *, size_t);

   extern long long int  _Optlink atoll(const char *);
   extern lldiv_t  _Optlink lldiv(long long int, long long int);
   extern long long int  _Optlink strtoll(const char *, char **, int);
   extern unsigned long long int  _Optlink strtoull(const char *, char **, int);
#line 173
      extern int       _Optlink abs (int);
      extern long int  _Optlink labs(long int);
      int           _Builtin __abs(int);
      long int      _Builtin __labs(long int);
#line 180
         long long int _Builtin __llabs(long long int);
         extern long long int  _Optlink llabs(long long int);
#line 224
            void *_Builtin __alloca(size_t);
#line 231
               unsigned char _Builtin __parmdwords(void);
#line 235
         extern long double  _Optlink _atold(const char *);
         extern char *  _Optlink  ecvt(double, int, int *, int *);
         extern char *  _Optlink _ecvt(double, int, int *, int *);
         extern char *  _Optlink  fcvt(double, int, int *, int *);
         extern char *  _Optlink _fcvt(double, int, int *, int *);
         extern int     _Optlink _freemod(unsigned long);
         extern char *  _Optlink _fullpath(char *, const char *, size_t);
         extern char *  _Optlink  gcvt(double, int, char *);
         extern char *  _Optlink _gcvt(double, int, char *);
         extern int    ( _Optlink _heapmin)(void);
         extern char *  _Optlink  itoa(int, char *, int);
         extern char *  _Optlink _itoa(int, char *, int);
         extern int     _Optlink _loadmod(char *, unsigned long *);
         extern char *  _Optlink  ltoa( long, char *, int);
         extern char *  _Optlink _ltoa( long, char *, int);
         extern size_t  _Optlink _msize(void *);
         extern int     _Optlink  putenv(const char *);
         extern int     _Optlink _putenv(const char *);
         extern long double  _Optlink  strtold(const char *, char **);
         extern long double  _Optlink _strtold(const char *, char **);
         extern char *  _Optlink  ultoa(unsigned long, char *, int);
         extern char *  _Optlink _ultoa(unsigned long, char *, int);

         extern char *  _Optlink  lltoa(long long, char *, int);
         extern char *  _Optlink _lltoa(long long, char *, int);
         extern char *  _Optlink  ulltoa(unsigned long long, char *, int);
         extern char *  _Optlink _ulltoa(unsigned long long, char *, int);
#line 301
               #pragma map(ecvt, "_ecvt")
               #pragma map(fcvt, "_fcvt")
               #pragma map(gcvt, "_gcvt")
               #pragma map(itoa, "_itoa")
               #pragma map(ltoa, "_ltoa")
               #pragma map(ultoa, "_ultoa")
               #pragma map(lltoa, "_lltoa")
               #pragma map(ulltoa, "_ulltoa")
               #pragma map(putenv, "_putenv")
               #pragma map(_strtold, "strtold")
#line 411
            extern void * _Optlink _ncalloc(size_t, size_t);
            extern void * _Optlink _fcalloc(size_t, size_t);
            extern void   _Optlink _nfree(void*);
            extern void   _Optlink _ffree(void*);
            extern void * _Optlink _nmalloc(size_t);
            extern void * _Optlink _fmalloc(size_t);
            extern void * _Optlink _nrealloc(void*, size_t);
            extern void * _Optlink _frealloc(void*, size_t);
            extern int    _Optlink _nheapmin(void);
            extern int    _Optlink _fheapmin(void);
#line 435
                  #pragma map(_ncalloc, "calloc")
                  #pragma map(_fcalloc, "calloc")
                  #pragma map(_nfree, "free" )
                  #pragma map(_ffree, "free" )
                  #pragma map(_nmalloc, "malloc")
                  #pragma map(_fmalloc, "malloc")
                  #pragma map(_nrealloc, "realloc")
                  #pragma map(_frealloc, "realloc")
                  #pragma map(_nheapmin, "_heapmin")
                  #pragma map(_fheapmin, "_heapmin")
#line 471
               #pragma map(atof, "_atofieee")
               #pragma map(strtod, "_strtodieee")
#line 481
               void  _Builtin __enable(void);
               void  _Builtin __disable(void);
#line 493
                  extern int  errno;
#line 503
                  extern int  _doserrno;
#line 508
            extern const    char ** _environ;
            extern unsigned char    _osmajor;
            extern unsigned char    _osminor;
            extern unsigned char    _osmode;
#line 515
            enum {
               DOS_MODE,                
               OS2_MODE,                
               NT_MODE,                 
               WINDOWS_MODE             
               };

            typedef int (__onexit_t)(void);
            typedef __onexit_t *onexit_t;
#line 534
            extern void  _Optlink _exit(int);
            extern onexit_t     _Optlink  onexit(onexit_t);
            extern onexit_t     _Optlink _onexit(onexit_t);

            extern void  _Optlink _makepath(char *, const char *, const char *, const char *, const char *);
            extern void  _Optlink _splitpath(const char *, char *, char *, char *, char *);
            extern void  _Optlink _searchenv(char *, char *, char *);
            extern void  _Optlink  swab(char *, char *, int);
            extern void  _Optlink _swab(char *, char *, int);

            extern int  _Optlink rpmatch(const char *);
            extern int  _Optlink csid(const char *);
            extern int  _Optlink wcsid(const wchar_t);
#line 550
                  unsigned long long _Builtin __llrotl(unsigned long long, int);
                  unsigned long long _Builtin __llrotr(unsigned long long, int);
#line 555
               unsigned long  _Builtin __lrotl(unsigned long, int);
               unsigned long  _Builtin __lrotr(unsigned long, int);
               unsigned short _Builtin __srotl(unsigned short, int);
               unsigned short _Builtin __srotr(unsigned short, int);
               unsigned char  _Builtin __crotl(unsigned char, int);
               unsigned char  _Builtin __crotr(unsigned char, int);
#line 591
                  #pragma map(rpmatch, "_rpmatch")
                  #pragma map(csid, "_csid")
                  #pragma map(wcsid, "_wcsid")
                  #pragma map(onexit, "_onexit")
                  #pragma map(swab, "_swab")
#line 635
   extern double  _Optlink atof(const char *);
   extern double  _Optlink strtod(const char *, char **);
#line 690
      #pragma info(none)

         #pragma info(restore)

      #pragma info(restore)
#line 3 "F:\\Ibmcxxo\\INCLUDE\\math.h"
      #pragma info(none)

         #pragma info(none)

      #pragma info(restore)
#line 45
   extern const double  _infinity;
#line 55
         double _Builtin __fabs(double);
         extern double  _Optlink fabs(double);
#line 59
      #pragma isolated_call(__fabs)
#line 64
   extern float  _Optlink acosf(float);
   extern float  _Optlink asinf(float);
   extern float  _Optlink atanf(float);
   extern float  _Optlink atan2f(float, float);
   extern float  _Optlink ceilf(float);
   extern float  _Optlink cosf(float);
   extern float  _Optlink coshf(float);
   extern float  _Optlink expf(float);
   extern float  _Optlink fabsf(float);
   extern float  _Optlink floorf(float);
   extern float  _Optlink fmodf(float, float);
   extern float  _Optlink frexpf(float, int *);
   extern float  _Optlink ldexpf(float, int);
   extern float  _Optlink logf(float);
   extern float  _Optlink log10f(float);
   extern float  _Optlink modff(float, float *);
   extern float  _Optlink powf(float, float);
   extern float  _Optlink sinf(float);
   extern float  _Optlink sinhf(float);
   extern float  _Optlink sqrtf(float);
   extern float  _Optlink tanf(float);
   extern float  _Optlink tanhf(float);

   extern double  _Optlink ceil(double);
   extern double  _Optlink cosh(double);
   extern double  _Optlink floor(double);
   extern double  _Optlink fmod(double, double);
   extern double  _Optlink frexp(double, int *);
   extern double  _Optlink ldexp(double, int);
   extern double  _Optlink modf(double, double *);
   extern double  _Optlink pow(double, double);
   extern double  _Optlink sinh(double);
   extern double  _Optlink tanh(double);

   extern long double  _Optlink acosl(long double);
   extern long double  _Optlink asinl(long double);
   extern long double  _Optlink atanl(long double);
   extern long double  _Optlink atan2l(long double, long double);
   extern long double  _Optlink ceill(long double);
   extern long double  _Optlink cosl(long double);
   extern long double  _Optlink coshl(long double);
   extern long double  _Optlink expl(long double);
   extern long double  _Optlink fabsl(long double);
   extern long double  _Optlink floorl(long double);
   extern long double  _Optlink fmodl(long double, long double);
   extern long double  _Optlink frexpl(long double, int *);
   extern long double  _Optlink ldexpl(long double, int);
   extern long double  _Optlink logl(long double);
   extern long double  _Optlink log10l(long double);
   extern long double  _Optlink modfl(long double, long double *);
   extern long double  _Optlink powl(long double, long double);
   extern long double  _Optlink sinl(long double);
   extern long double  _Optlink sinhl(long double);
   extern long double  _Optlink sqrtl(long double);
   extern long double  _Optlink tanl(long double);
   extern long double  _Optlink tanhl(long double);

   extern float  _Optlink __powif(float, int);
   extern double  _Optlink __powi(double, int);
   extern long double  _Optlink __powil(long double, int);
#line 159
         extern double  _Optlink sin(double);
         extern double  _Optlink cos(double);
         extern double  _Optlink tan(double);
         extern double  _Optlink acos(double);
         extern double  _Optlink asin(double);
         extern double  _Optlink atan(double);
         extern double  _Optlink sqrt(double);

      extern double  _Optlink atan2(double, double);
      extern double  _Optlink exp(double);
      extern double  _Optlink log(double);
      extern double  _Optlink log10(double);
#line 329
      extern double  _Optlink _erf(double);
      extern double  _Optlink _erfc(double);
      extern double  _Optlink _gamma(double);
      extern double  _Optlink _hypot(double, double);
      extern double  _Optlink _j0(double);
      extern double  _Optlink _j1(double);
      extern double  _Optlink _jn(int, double);
      extern double  _Optlink _y0(double);
      extern double  _Optlink _y1(double);
      extern double  _Optlink _yn(int, double);
      extern double  _Optlink erf(double);
      extern double  _Optlink erfc(double);
      extern double  _Optlink gamma(double);
      extern double  _Optlink hypot(double, double);
      extern double  _Optlink j0(double);
      extern double  _Optlink j1(double);
      extern double  _Optlink jn(int, double);
      extern double  _Optlink y0(double);
      extern double  _Optlink y1(double);
      extern double  _Optlink yn(int, double);
#line 391
            #pragma map(erf, "_erf")
            #pragma map(erfc, "_erfc")
            #pragma map(gamma, "_gamma")
            #pragma map(hypot, "_hypot")
            #pragma map(j0, "_j0")
            #pragma map(j1, "_j1")
            #pragma map(jn, "_jn")
            #pragma map(y0, "_y0")
            #pragma map(y1, "_y1")
            #pragma map(yn, "_yn")
#line 416
         extern const long double  _LHUGE_VAL;
#line 434
            struct complex {
               double x, y;              
               };

            double  _Optlink _cabs(struct complex);
#line 444
         struct _exception {
            int type;                
            char *name;              
            double arg1;             
            double arg2;             
            double retval;           
            };
#line 454
         int _matherr(struct _exception*);
         int  matherr(struct _exception*);
#line 461
               #pragma map(matherr, "_matherr")
#line 528
      #pragma info(none)

         #pragma info(restore)

      #pragma info(restore)
#line 3 "F:\\Ibmcxxo\\INCLUDE\\time.h"
      #pragma info(none)

         #pragma info(none)

      #pragma info(restore)
#line 56
   typedef unsigned long clock_t;
#line 65
      struct tm {
         int tm_sec;       
         int tm_min;       
         int tm_hour;      
         int tm_mday;      
         int tm_mon;       
         int tm_year;      
         int tm_wday;      
         int tm_yday;      
         int tm_isdst;     
      };
#line 78
   extern clock_t     _Optlink clock(void);
   extern double      _Optlink difftime(time_t, time_t);
   extern time_t      _Optlink mktime(struct tm *);
   extern time_t      _Optlink time(time_t *);
   extern char *      _Optlink asctime(const struct tm *);
   extern char *      _Optlink ctime(const time_t *);
   extern struct tm * _Optlink gmtime(const time_t *);
   extern struct tm * _Optlink localtime(const time_t *);
   extern size_t      _Optlink strftime(char *, size_t, const char *, const struct tm *);
#line 115
         #pragma map(difftime, "__difftime")
         #pragma map(mktime, "__mktime")
         #pragma map(time, "__time")
         #pragma map(ctime, "__ctime")
         #pragma map(gmtime, "__gmtime")
         #pragma map(localtime, "__localtime")
#line 125
      extern char * _Optlink strptime(const char *, const char *, struct tm *);
#line 133
      extern int    _daylight;  
      extern int    daylight;
      extern long   _timezone;  
      extern long   timezone;
      extern char * _tzname[2];  
      extern char * tzname[2];
      extern void   _Optlink tzset(void);
      extern void   _Optlink _tzset(void);
      extern char * _Optlink _strdate(char *);
      extern char * _Optlink _strtime(char *);
#line 149
            #pragma map(daylight, "_daylight")
            #pragma map(tzname, "_tzname")
            #pragma map(tzset, "_tzset")
#line 161
      #pragma info(none)

         #pragma info(restore)

      #pragma info(restore)
#line 886 "pngconf.h"
typedef unsigned long png_uint_32;
typedef long png_int_32;
typedef unsigned short png_uint_16;
typedef short png_int_16;
typedef unsigned char png_byte;
#line 894
typedef size_t png_size_t;
#line 959
typedef png_int_32 png_fixed_point;
#line 962
typedef void             * png_voidp;
typedef png_byte         * png_bytep;
typedef png_uint_32      * png_uint_32p;
typedef png_int_32       * png_int_32p;
typedef png_uint_16      * png_uint_16p;
typedef png_int_16       * png_int_16p;
typedef const char   * png_const_charp;
typedef char             * png_charp;
typedef png_fixed_point  * png_fixed_point_p;
#line 974
typedef FILE                * png_FILE_p;
#line 977
typedef double           * png_doublep;
#line 981
typedef png_byte         *  * png_bytepp;
typedef png_uint_32      *  * png_uint_32pp;
typedef png_int_32       *  * png_int_32pp;
typedef png_uint_16      *  * png_uint_16pp;
typedef png_int_16       *  * png_int_16pp;
typedef const char   *  * png_const_charpp;
typedef char             *  * png_charpp;
typedef png_fixed_point  *  * png_fixed_point_pp;

typedef double           *  * png_doublepp;
#line 994
typedef char             *  *  * png_charppp;
#line 1000
typedef charf *         png_zcharp;
typedef charf *  *   png_zcharpp;
typedef z_stream  *  png_zstreamp;
#line 305 "png.h"
extern   char png_libpng_ver[12];  
#line 314
extern   int png_pass_start[7];
extern   int png_pass_inc[7];
extern   int png_pass_ystart[7];
extern   int png_pass_yinc[7];
extern   int png_pass_mask[7];
extern   int png_pass_dsp_mask[7];
#line 334
typedef struct png_color_struct
{
   png_byte red;
   png_byte green;
   png_byte blue;
} png_color;
typedef png_color  * png_colorp;
typedef png_color  *  * png_colorpp;

typedef struct png_color_16_struct
{
   png_byte index;     
   png_uint_16 red;    
   png_uint_16 green;
   png_uint_16 blue;
   png_uint_16 gray;   
} png_color_16;
typedef png_color_16  * png_color_16p;
typedef png_color_16  *  * png_color_16pp;

typedef struct png_color_8_struct
{
   png_byte red;    
   png_byte green;
   png_byte blue;
   png_byte gray;   
   png_byte alpha;  
} png_color_8;
typedef png_color_8  * png_color_8p;
typedef png_color_8  *  * png_color_8pp;
#line 369
typedef struct png_sPLT_entry_struct
{
   png_uint_16 red;
   png_uint_16 green;
   png_uint_16 blue;
   png_uint_16 alpha;
   png_uint_16 frequency;
} png_sPLT_entry;
typedef png_sPLT_entry  * png_sPLT_entryp;
typedef png_sPLT_entry  *  * png_sPLT_entrypp;
#line 385
typedef struct png_sPLT_struct
{
   png_charp name;            
   png_byte depth;            
   png_sPLT_entryp entries;   
   png_int_32 nentries;       
} png_sPLT_t;
typedef png_sPLT_t  * png_sPLT_tp;
typedef png_sPLT_t  *  * png_sPLT_tpp;
#line 404
typedef struct png_text_struct
{
   int  compression;       
#line 411
   png_charp key;           
   png_charp text;         

   png_size_t text_length;  
#line 422
} png_text;
typedef png_text  * png_textp;
typedef png_text  *  * png_textpp;
#line 443
typedef struct png_time_struct
{
   png_uint_16 year;  
   png_byte month;    
   png_byte day;      
   png_byte hour;     
   png_byte minute;   
   png_byte second;   
} png_time;
typedef png_time  * png_timep;
typedef png_time  *  * png_timepp;
#line 461
typedef struct png_unknown_chunk_t
{
    png_byte name[5];
    png_byte *data;
    png_size_t size;
#line 468
    png_byte location;  
}
png_unknown_chunk;
typedef png_unknown_chunk  * png_unknown_chunkp;
typedef png_unknown_chunk  *  * png_unknown_chunkpp;
#line 514
typedef struct png_info_struct
{

   png_uint_32 width;        
   png_uint_32 height;       
   png_uint_32 valid;        
   png_uint_32 rowbytes;     
   png_colorp palette;       
   png_uint_16 num_palette;  
   png_uint_16 num_trans;    
   png_byte bit_depth;       
   png_byte color_type;      
   png_byte compression_type;  
   png_byte filter_type;     
   png_byte interlace_type;  
#line 531
   png_byte channels;        
   png_byte pixel_depth;     
   png_byte spare_byte;      
   png_byte signature[8];    
#line 547
   float gamma;  
#line 553
   png_byte srgb_intent;  
#line 565
   int num_text;  
   int max_text;  
   png_textp text;  
#line 574
   png_time mod_time;
#line 584
   png_color_8 sig_bit;  
#line 598
   png_bytep trans;  
   png_color_16 trans_values;  
#line 609
   png_color_16 background;
#line 618
   png_int_32 x_offset;  
   png_int_32 y_offset;  
   png_byte offset_unit_type;  
#line 628
   png_uint_32 x_pixels_per_unit;  
   png_uint_32 y_pixels_per_unit;  
   png_byte phys_unit_type;  
#line 640
   png_uint_16p hist;
#line 651
   float x_white;
   float y_white;
   float x_red;
   float y_red;
   float x_green;
   float y_green;
   float x_blue;
   float y_blue;
#line 674
   png_charp pcal_purpose;   
   png_int_32 pcal_X0;       
   png_int_32 pcal_X1;       
   png_charp pcal_units;     
   png_charpp pcal_params;   
   png_byte pcal_type;       
   png_byte pcal_nparams;    
#line 684
   png_uint_32 free_me;      
#line 689
   png_unknown_chunkp unknown_chunks;
   png_size_t unknown_chunks_num;
#line 695
   png_charp iccp_name;      
   png_charp iccp_profile;   
   png_uint_32 iccp_proflen;   
   png_byte iccp_compression;  
#line 703
   png_sPLT_tp splt_palettes;
   png_uint_32 splt_palettes_num;
#line 715
   png_byte scal_unit;          

   double scal_pixel_width;     
   double scal_pixel_height;    
#line 721
   png_charp scal_s_width;      
   png_charp scal_s_height;     
#line 729
   png_bytepp row_pointers;         
#line 733
   png_fixed_point int_gamma;  
#line 737
   png_fixed_point int_x_white;
   png_fixed_point int_y_white;
   png_fixed_point int_x_red;
   png_fixed_point int_y_red;
   png_fixed_point int_x_green;
   png_fixed_point int_y_green;
   png_fixed_point int_x_blue;
   png_fixed_point int_y_blue;
#line 747
} png_info;

typedef png_info  * png_infop;
typedef png_info  *  * png_infopp;
#line 841
typedef struct png_row_info_struct
{
   png_uint_32 width;  
   png_uint_32 rowbytes;  
   png_byte color_type;  
   png_byte bit_depth;  
   png_byte channels;  
   png_byte pixel_depth;  
} png_row_info;

typedef png_row_info  * png_row_infop;
typedef png_row_info  *  * png_row_infopp;
#line 860
typedef struct png_struct_def png_struct;
typedef png_struct  * png_structp;

typedef void (_cdecl *png_error_ptr) (png_structp, png_const_charp);
typedef void (_cdecl *png_rw_ptr) (png_structp, png_bytep, png_size_t);
typedef void (_cdecl *png_flush_ptr) (png_structp);
typedef void (_cdecl *png_read_status_ptr) 
(png_structp, png_uint_32, int);
typedef void (_cdecl *png_write_status_ptr) 
(png_structp, png_uint_32, int);
#line 872
typedef void (_cdecl *png_progressive_info_ptr) (png_structp, png_infop);
typedef void (_cdecl *png_progressive_end_ptr) (png_structp, png_infop);
typedef void (_cdecl *png_progressive_row_ptr) 
(png_structp, png_bytep, png_uint_32, int);
#line 881
typedef void (_cdecl *png_user_transform_ptr) 
(png_structp, png_row_infop, png_bytep);
#line 886
typedef int (_cdecl *png_user_chunk_ptr) (png_structp, png_unknown_chunkp);
#line 889
typedef void (_cdecl *png_unknown_chunk_ptr) (png_structp);
#line 907
typedef png_voidp (*png_malloc_ptr) (png_structp, png_size_t);
typedef void (*png_free_ptr) (png_structp, png_voidp);
#line 917
struct png_struct_def
{

   jmp_buf jmpbuf;             

   png_error_ptr error_fn;     
   png_error_ptr warning_fn;   
   png_voidp error_ptr;        
   png_rw_ptr write_data_fn;   
   png_rw_ptr read_data_fn;    
   png_voidp io_ptr;           
#line 930
   png_user_transform_ptr read_user_transform_fn;  
#line 934
   png_user_transform_ptr write_user_transform_fn;  
#line 941
   png_voidp user_transform_ptr;  
   png_byte user_transform_depth;     
   png_byte user_transform_channels;  
#line 947
   png_uint_32 mode;           
   png_uint_32 flags;          
   png_uint_32 transformations;  

   z_stream zstream;           
   png_bytep zbuf;             
   png_size_t zbuf_size;       
   int zlib_level;             
   int zlib_method;            
   int zlib_window_bits;       
   int zlib_mem_level;         
   int zlib_strategy;          

   png_uint_32 width;          
   png_uint_32 height;         
   png_uint_32 num_rows;       
   png_uint_32 usr_width;      
   png_uint_32 rowbytes;       
   png_uint_32 irowbytes;      
   png_uint_32 iwidth;         
   png_uint_32 row_number;     
   png_bytep prev_row;         
   png_bytep row_buf;          
   png_bytep sub_row;          
   png_bytep up_row;           
   png_bytep avg_row;          
   png_bytep paeth_row;        
   png_row_info row_info;      

   png_uint_32 idat_size;      
   png_uint_32 crc;            
   png_colorp palette;         
   png_uint_16 num_palette;    
   png_uint_16 num_trans;      
   png_byte chunk_name[5];     
   png_byte compression;       
   png_byte filter;            
   png_byte interlaced;        
   png_byte pass;              
   png_byte do_filter;         
   png_byte color_type;        
   png_byte bit_depth;         
   png_byte usr_bit_depth;     
   png_byte pixel_depth;       
   png_byte channels;          
   png_byte usr_channels;      
   png_byte sig_bytes;         
#line 999
   png_uint_16 filler;            
#line 1004
   png_byte background_gamma_type;

   float background_gamma;

   png_color_16 background;    

     png_color_16 background_1;  
#line 1015
   png_flush_ptr output_flush_fn; 
   png_uint_32 flush_dist;     
   png_uint_32 flush_rows;     
#line 1021
   int gamma_shift;       

   float gamma;           
   float screen_gamma;    
#line 1029
   png_bytep gamma_table;      
   png_bytep gamma_from_1;     
   png_bytep gamma_to_1;       
   png_uint_16pp gamma_16_table;  
   png_uint_16pp gamma_16_from_1;  
   png_uint_16pp gamma_16_to_1;  
#line 1038
   png_color_8 sig_bit;        
#line 1042
   png_color_8 shift;          
#line 1047
   png_bytep trans;            
   png_color_16 trans_values;  
#line 1051
   png_read_status_ptr read_row_fn;    
   png_write_status_ptr write_row_fn;  

   png_progressive_info_ptr info_fn;  
   png_progressive_row_ptr row_fn;    
   png_progressive_end_ptr end_fn;    
   png_bytep save_buffer_ptr;         
   png_bytep save_buffer;             
   png_bytep current_buffer_ptr;      
   png_bytep current_buffer;          
   png_uint_32 push_length;           
   png_uint_32 skip_length;           
   png_size_t save_buffer_size;       
   png_size_t save_buffer_max;        
   png_size_t buffer_size;            
   png_size_t current_buffer_size;    
   int process_mode;                  
   int cur_palette;                   
#line 1071
     png_size_t current_text_size;    
     png_size_t current_text_left;    
     png_charp current_text;          
     png_charp current_text_ptr;      
#line 1089
   png_bytep palette_lookup;          
   png_bytep dither_index;            
#line 1094
   png_uint_16p hist;                 
#line 1098
   png_byte heuristic_method;         
   png_byte num_prev_filters;         
   png_bytep prev_filters;            
   png_uint_16p filter_weights;       
   png_uint_16p inv_filter_weights;   
   png_uint_16p filter_costs;         
   png_uint_16p inv_filter_costs;     
#line 1108
   png_charp time_buffer;             
#line 1120
   png_uint_32 free_me;        
#line 1124
   png_voidp user_chunk_ptr;
   png_user_chunk_ptr read_user_chunk_fn;  
#line 1129
   int num_chunk_list;
   png_bytep chunk_list;
#line 1134
   png_byte rgb_to_gray_status;
   png_uint_16 rgb_to_gray_red_coeff;
   png_uint_16 rgb_to_gray_green_coeff;
   png_uint_16 rgb_to_gray_blue_coeff;
#line 1142
   png_byte empty_plte_permitted;
#line 1146
   png_fixed_point int_gamma;
#line 1149
};
#line 1154
typedef png_structp version_1_0_8;

typedef png_struct  *  * png_structpp;
#line 1165
extern  png_uint_32 _cdecl png_access_version_number (void);
#line 1170
extern  void _cdecl png_set_sig_bytes 
(png_structp png_ptr, int num_bytes);
#line 1178
extern  int _cdecl png_sig_cmp 
(png_bytep sig, png_size_t start, png_size_t num_to_check);
#line 1184
extern  int _cdecl png_check_sig (png_bytep sig, int num);
#line 1187
extern  png_structp _cdecl png_create_read_struct

(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn);
#line 1192
extern  png_structp _cdecl png_create_write_struct

(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn);

extern  png_uint_32 _cdecl png_get_compression_buffer_size
   (png_structp png_ptr);

extern  void _cdecl png_set_compression_buffer_size
   (png_structp png_ptr, png_uint_32 size);
#line 1203
extern  int _cdecl png_reset_zstream (png_structp png_ptr);
#line 1217
extern  void _cdecl png_write_chunk 
(png_structp png_ptr, png_bytep chunk_name, png_bytep data, png_size_t length);
#line 1221
extern  void _cdecl png_write_chunk_start 
(png_structp png_ptr, png_bytep chunk_name, png_uint_32 length);
#line 1225
extern  void _cdecl png_write_chunk_data 
(png_structp png_ptr, png_bytep data, png_size_t length);
#line 1229
extern  void _cdecl png_write_chunk_end (png_structp png_ptr);
#line 1232
extern  png_infop _cdecl png_create_info_struct
   (png_structp png_ptr);
#line 1236
extern void png_info_init (png_infop info_ptr);
#line 1239
extern  void _cdecl png_write_info_before_PLTE 
(png_structp png_ptr, png_infop info_ptr);
extern  void _cdecl png_write_info 
(png_structp png_ptr, png_infop info_ptr);
#line 1245
extern  void _cdecl png_read_info 
(png_structp png_ptr, png_infop info_ptr);
#line 1249
extern  png_charp _cdecl png_convert_to_rfc1123
   (png_structp png_ptr, png_timep ptime);
#line 1257
extern  void _cdecl png_convert_from_struct_tm 
(png_timep ptime, struct tm * ttime);
#line 1261
extern  void _cdecl png_convert_from_time_t 
(png_timep ptime, time_t ttime);
#line 1268
extern  void _cdecl png_set_expand (png_structp png_ptr);
extern  void _cdecl png_set_gray_1_2_4_to_8 (png_structp png_ptr);
extern  void _cdecl png_set_palette_to_rgb (png_structp png_ptr);
extern  void _cdecl png_set_tRNS_to_alpha (png_structp png_ptr);
#line 1276
extern  void _cdecl png_set_bgr (png_structp png_ptr);
#line 1281
extern  void _cdecl png_set_gray_to_rgb (png_structp png_ptr);
#line 1287
extern  void _cdecl png_set_rgb_to_gray 
(png_structp png_ptr, int error_action, double red, double green );

extern  void _cdecl png_set_rgb_to_gray_fixed 
(png_structp png_ptr, int error_action, png_fixed_point red, png_fixed_point green );
extern  png_byte _cdecl png_get_rgb_to_gray_status 
(png_structp png_ptr);
#line 1296
extern  void _cdecl png_build_grayscale_palette 
(int bit_depth, png_colorp palette);
#line 1300
extern  void _cdecl png_set_strip_alpha (png_structp png_ptr);
#line 1305
extern  void _cdecl png_set_swap_alpha (png_structp png_ptr);
#line 1310
extern  void _cdecl png_set_invert_alpha (png_structp png_ptr);
#line 1315
extern  void _cdecl png_set_filler 
(png_structp png_ptr, png_uint_32 filler, int flags);
#line 1324
extern  void _cdecl png_set_swap (png_structp png_ptr);
#line 1329
extern  void _cdecl png_set_packing (png_structp png_ptr);
#line 1334
extern  void _cdecl png_set_packswap (png_structp png_ptr);
#line 1339
extern  void _cdecl png_set_shift 
(png_structp png_ptr, png_color_8p true_bits);
#line 1346
extern  int _cdecl png_set_interlace_handling (png_structp png_ptr);
#line 1351
extern  void _cdecl png_set_invert_mono (png_structp png_ptr);
#line 1357
extern  void _cdecl png_set_background 

(png_structp png_ptr, png_color_16p background_color, int background_gamma_code, int need_expand, double background_gamma);
#line 1369
extern  void _cdecl png_set_strip_16 (png_structp png_ptr);
#line 1374
extern  void _cdecl png_set_dither 

(png_structp png_ptr, png_colorp palette, int num_palette, int maximum_colors, png_uint_16p histogram, int full_dither);
#line 1382
extern  void _cdecl png_set_gamma 
(png_structp png_ptr, double screen_gamma, double default_file_gamma);
#line 1390
extern  void _cdecl png_permit_empty_plte 
(png_structp png_ptr, int empty_plte_permitted);
#line 1396
extern  void _cdecl png_set_flush (png_structp png_ptr, int nrows);

extern  void _cdecl png_write_flush (png_structp png_ptr);
#line 1402
extern  void _cdecl png_start_read_image (png_structp png_ptr);
#line 1405
extern  void _cdecl png_read_update_info 
(png_structp png_ptr, png_infop info_ptr);
#line 1409
extern  void _cdecl png_read_rows 
(png_structp png_ptr, png_bytepp row, png_bytepp display_row, png_uint_32 num_rows);
#line 1413
extern  void _cdecl png_read_row 

(png_structp png_ptr, png_bytep row, png_bytep display_row);
#line 1418
extern  void _cdecl png_read_image 
(png_structp png_ptr, png_bytepp image);
#line 1422
extern  void _cdecl png_write_row 
(png_structp png_ptr, png_bytep row);
#line 1426
extern  void _cdecl png_write_rows 
(png_structp png_ptr, png_bytepp row, png_uint_32 num_rows);
#line 1430
extern  void _cdecl png_write_image 
(png_structp png_ptr, png_bytepp image);
#line 1434
extern  void _cdecl png_write_end 
(png_structp png_ptr, png_infop info_ptr);
#line 1438
extern  void _cdecl png_read_end 
(png_structp png_ptr, png_infop info_ptr);
#line 1442
extern  void _cdecl png_destroy_info_struct 
(png_structp png_ptr, png_infopp info_ptr_ptr);
#line 1446
extern  void _cdecl png_destroy_read_struct 
(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr, png_infopp end_info_ptr_ptr);
#line 1450
extern void png_read_destroy 
(png_structp png_ptr, png_infop info_ptr, png_infop end_info_ptr);
#line 1454
extern  void _cdecl png_destroy_write_struct
   (png_structpp png_ptr_ptr, png_infopp info_ptr_ptr);
#line 1458
extern void png_write_destroy_info (png_infop info_ptr);
#line 1461
extern void png_write_destroy (png_structp png_ptr);
#line 1464
extern  void _cdecl png_set_crc_action 
(png_structp png_ptr, int crit_action, int ancil_action);
#line 1494
extern  void _cdecl png_set_filter 
(png_structp png_ptr, int method, int filters);
#line 1551
extern  void _cdecl png_set_filter_heuristics 

(png_structp png_ptr, int heuristic_method, int num_weights, png_doublep filter_weights, png_doublep filter_costs);
#line 1572
extern  void _cdecl png_set_compression_level 
(png_structp png_ptr, int level);

extern  void _cdecl png_set_compression_mem_level
   (png_structp png_ptr, int mem_level);

extern  void _cdecl png_set_compression_strategy
   (png_structp png_ptr, int strategy);

extern  void _cdecl png_set_compression_window_bits
   (png_structp png_ptr, int window_bits);

extern  void _cdecl png_set_compression_method 
(png_structp png_ptr, int method);
#line 1598
extern  void _cdecl png_init_io (png_structp png_ptr, png_FILE_p fp);
#line 1609
extern  void _cdecl png_set_error_fn 
(png_structp png_ptr, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warning_fn);
#line 1613
extern  png_voidp _cdecl png_get_error_ptr (png_structp png_ptr);
#line 1620
extern  void _cdecl png_set_write_fn 
(png_structp png_ptr, png_voidp io_ptr, png_rw_ptr write_data_fn, png_flush_ptr output_flush_fn);
#line 1624
extern  void _cdecl png_set_read_fn 
(png_structp png_ptr, png_voidp io_ptr, png_rw_ptr read_data_fn);
#line 1628
extern  png_voidp _cdecl png_get_io_ptr (png_structp png_ptr);

extern  void _cdecl png_set_read_status_fn 
(png_structp png_ptr, png_read_status_ptr read_row_fn);

extern  void _cdecl png_set_write_status_fn 
(png_structp png_ptr, png_write_status_ptr write_row_fn);
#line 1646
extern  void _cdecl png_set_read_user_transform_fn 
(png_structp png_ptr, png_user_transform_ptr read_user_transform_fn);
#line 1652
extern  void _cdecl png_set_write_user_transform_fn 
(png_structp png_ptr, png_user_transform_ptr write_user_transform_fn);
#line 1659
extern  void _cdecl png_set_user_transform_info 

(png_structp png_ptr, png_voidp user_transform_ptr, int user_transform_depth, int user_transform_channels);

extern  png_voidp _cdecl png_get_user_transform_ptr
   (png_structp png_ptr);
#line 1668
extern  void _cdecl png_set_read_user_chunk_fn 
(png_structp png_ptr, png_voidp user_chunk_ptr, png_user_chunk_ptr read_user_chunk_fn);
extern  png_voidp _cdecl png_get_user_chunk_ptr 
(png_structp png_ptr);
#line 1678
extern  void _cdecl png_set_progressive_read_fn 
#line 1681
(png_structp png_ptr, png_voidp progressive_ptr, png_progressive_info_ptr info_fn, png_progressive_row_ptr row_fn, png_progressive_end_ptr end_fn);
#line 1684
extern  png_voidp _cdecl png_get_progressive_ptr
   (png_structp png_ptr);
#line 1688
extern  void _cdecl png_process_data 
(png_structp png_ptr, png_infop info_ptr, png_bytep buffer, png_size_t buffer_size);
#line 1694
extern  void _cdecl png_progressive_combine_row 
(png_structp png_ptr, png_bytep old_row, png_bytep new_row);
#line 1698
extern  png_voidp _cdecl png_malloc 
(png_structp png_ptr, png_uint_32 size);
#line 1702
extern  void _cdecl png_free (png_structp png_ptr, png_voidp ptr);
#line 1705
extern  void _cdecl png_free_data 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 free_me, int num);
#line 1710
extern  void _cdecl png_data_freer 
(png_structp png_ptr, png_infop info_ptr, int freer, png_uint_32 mask);
#line 1739
extern  png_voidp _cdecl png_memcpy_check 
(png_structp png_ptr, png_voidp s1, png_voidp s2, png_uint_32 size);

extern  png_voidp _cdecl png_memset_check 
(png_structp png_ptr, png_voidp s1, int value, png_uint_32 size);
#line 1751
extern  void _cdecl png_error 
(png_structp png_ptr, png_const_charp error);
#line 1755
extern  void _cdecl png_chunk_error 
(png_structp png_ptr, png_const_charp error);
#line 1759
extern  void _cdecl png_warning 
(png_structp png_ptr, png_const_charp message);
#line 1763
extern  void _cdecl png_chunk_warning 
(png_structp png_ptr, png_const_charp message);
#line 1779
extern  png_uint_32 _cdecl png_get_valid 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 flag);
#line 1783
extern  png_uint_32 _cdecl png_get_rowbytes 
(png_structp png_ptr, png_infop info_ptr);
#line 1789
extern  png_bytepp _cdecl png_get_rows 
(png_structp png_ptr, png_infop info_ptr);
#line 1793
extern  void _cdecl png_set_rows 
(png_structp png_ptr, png_infop info_ptr, png_bytepp row_pointers);
#line 1798
extern  png_byte _cdecl png_get_channels 
(png_structp png_ptr, png_infop info_ptr);
#line 1803
extern  png_uint_32 _cdecl png_get_image_width 
(png_structp png_ptr, png_infop info_ptr);
#line 1807
extern  png_uint_32 _cdecl png_get_image_height 
(png_structp png_ptr, png_infop info_ptr);
#line 1811
extern  png_byte _cdecl png_get_bit_depth 
(png_structp png_ptr, png_infop info_ptr);
#line 1815
extern  png_byte _cdecl png_get_color_type 
(png_structp png_ptr, png_infop info_ptr);
#line 1819
extern  png_byte _cdecl png_get_filter_type 
(png_structp png_ptr, png_infop info_ptr);
#line 1823
extern  png_byte _cdecl png_get_interlace_type 
(png_structp png_ptr, png_infop info_ptr);
#line 1827
extern  png_byte _cdecl png_get_compression_type 
(png_structp png_ptr, png_infop info_ptr);
#line 1831
extern  png_uint_32 _cdecl png_get_pixels_per_meter 
(png_structp png_ptr, png_infop info_ptr);
extern  png_uint_32 _cdecl png_get_x_pixels_per_meter 
(png_structp png_ptr, png_infop info_ptr);
extern  png_uint_32 _cdecl png_get_y_pixels_per_meter 
(png_structp png_ptr, png_infop info_ptr);
#line 1840
extern  float _cdecl png_get_pixel_aspect_ratio 
(png_structp png_ptr, png_infop info_ptr);
#line 1845
extern  png_int_32 _cdecl png_get_x_offset_pixels 
(png_structp png_ptr, png_infop info_ptr);
extern  png_int_32 _cdecl png_get_y_offset_pixels 
(png_structp png_ptr, png_infop info_ptr);
extern  png_int_32 _cdecl png_get_x_offset_microns 
(png_structp png_ptr, png_infop info_ptr);
extern  png_int_32 _cdecl png_get_y_offset_microns 
(png_structp png_ptr, png_infop info_ptr);
#line 1857
extern  png_bytep _cdecl png_get_signature 
(png_structp png_ptr, png_infop info_ptr);
#line 1861
extern  png_uint_32 _cdecl png_get_bKGD 
(png_structp png_ptr, png_infop info_ptr, png_color_16p *background);
#line 1866
extern  void _cdecl png_set_bKGD 
(png_structp png_ptr, png_infop info_ptr, png_color_16p background);
#line 1872
extern  png_uint_32 _cdecl png_get_cHRM 
#line 1875
(png_structp png_ptr, png_infop info_ptr, double *white_x, double *white_y, double *red_x, double *red_y, double *green_x, double *green_y, double *blue_x, double *blue_y);
#line 1878
extern  png_uint_32 _cdecl png_get_cHRM_fixed 
#line 1882
(png_structp png_ptr, png_infop info_ptr, png_fixed_point *int_white_x, png_fixed_point *int_white_y, png_fixed_point *int_red_x, png_fixed_point *int_red_y, png_fixed_point *int_green_x, png_fixed_point *int_green_y, png_fixed_point *int_blue_x, png_fixed_point *int_blue_y);
#line 1888
extern  void _cdecl png_set_cHRM 

(png_structp png_ptr, png_infop info_ptr, double white_x, double white_y, double red_x, double red_y, double green_x, double green_y, double blue_x, double blue_y);
#line 1893
extern  void _cdecl png_set_cHRM_fixed 
#line 1897
(png_structp png_ptr, png_infop info_ptr, png_fixed_point int_white_x, png_fixed_point int_white_y, png_fixed_point int_red_x, png_fixed_point int_red_y, png_fixed_point int_green_x, png_fixed_point int_green_y, png_fixed_point int_blue_x, png_fixed_point int_blue_y);
#line 1903
extern  png_uint_32 _cdecl png_get_gAMA 
(png_structp png_ptr, png_infop info_ptr, double *file_gamma);

extern  png_uint_32 _cdecl png_get_gAMA_fixed 
(png_structp png_ptr, png_infop info_ptr, png_fixed_point *int_file_gamma);
#line 1912
extern  void _cdecl png_set_gAMA 
(png_structp png_ptr, png_infop info_ptr, double file_gamma);

extern  void _cdecl png_set_gAMA_fixed 
(png_structp png_ptr, png_infop info_ptr, png_fixed_point int_file_gamma);
#line 1920
extern  png_uint_32 _cdecl png_get_hIST 
(png_structp png_ptr, png_infop info_ptr, png_uint_16p *hist);
#line 1925
extern  void _cdecl png_set_hIST 
(png_structp png_ptr, png_infop info_ptr, png_uint_16p hist);
#line 1929
extern  png_uint_32 _cdecl png_get_IHDR 
#line 1932
(png_structp png_ptr, png_infop info_ptr, png_uint_32 *width, png_uint_32 *height, int *bit_depth, int *color_type, int *interlace_type, int *compression_type, int *filter_type);

extern  void _cdecl png_set_IHDR 

(png_structp png_ptr, png_infop info_ptr, png_uint_32 width, png_uint_32 height, int bit_depth, int color_type, int interlace_type, int compression_type, int filter_type);
#line 1939
extern  png_uint_32 _cdecl png_get_oFFs 

(png_structp png_ptr, png_infop info_ptr, png_int_32 *offset_x, png_int_32 *offset_y, int *unit_type);
#line 1945
extern  void _cdecl png_set_oFFs 

(png_structp png_ptr, png_infop info_ptr, png_int_32 offset_x, png_int_32 offset_y, int unit_type);
#line 1951
extern  png_uint_32 _cdecl png_get_pCAL 

(png_structp png_ptr, png_infop info_ptr, png_charp *purpose, png_int_32 *X0, png_int_32 *X1, int *type, int *nparams, png_charp *units, png_charpp *params);
#line 1957
extern  void _cdecl png_set_pCAL 

(png_structp png_ptr, png_infop info_ptr, png_charp purpose, png_int_32 X0, png_int_32 X1, int type, int nparams, png_charp units, png_charpp params);
#line 1963
extern  png_uint_32 _cdecl png_get_pHYs 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 *res_x, png_uint_32 *res_y, int *unit_type);
#line 1968
extern  void _cdecl png_set_pHYs 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 res_x, png_uint_32 res_y, int unit_type);
#line 1972
extern  png_uint_32 _cdecl png_get_PLTE 
(png_structp png_ptr, png_infop info_ptr, png_colorp *palette, int *num_palette);

extern  void _cdecl png_set_PLTE 
(png_structp png_ptr, png_infop info_ptr, png_colorp palette, int num_palette);
#line 1979
extern  png_uint_32 _cdecl png_get_sBIT 
(png_structp png_ptr, png_infop info_ptr, png_color_8p *sig_bit);
#line 1984
extern  void _cdecl png_set_sBIT 
(png_structp png_ptr, png_infop info_ptr, png_color_8p sig_bit);
#line 1989
extern  png_uint_32 _cdecl png_get_sRGB 
(png_structp png_ptr, png_infop info_ptr, int *intent);
#line 1994
extern  void _cdecl png_set_sRGB 
(png_structp png_ptr, png_infop info_ptr, int intent);
extern  void _cdecl png_set_sRGB_gAMA_and_cHRM 
(png_structp png_ptr, png_infop info_ptr, int intent);
#line 2001
extern  png_uint_32 _cdecl png_get_iCCP 

(png_structp png_ptr, png_infop info_ptr, png_charpp name, int *compression_type, png_charpp profile, png_uint_32 *proflen);
#line 2007
extern  void _cdecl png_set_iCCP 

(png_structp png_ptr, png_infop info_ptr, png_charp name, int compression_type, png_charp profile, png_uint_32 proflen);
#line 2013
extern  png_uint_32 _cdecl png_get_sPLT 
(png_structp png_ptr, png_infop info_ptr, png_sPLT_tpp entries);
#line 2018
extern  void _cdecl png_set_sPLT 
(png_structp png_ptr, png_infop info_ptr, png_sPLT_tp entries, int nentries);
#line 2024
extern  png_uint_32 _cdecl png_get_text 
(png_structp png_ptr, png_infop info_ptr, png_textp *text_ptr, int *num_text);
#line 2037
extern  void _cdecl png_set_text 
(png_structp png_ptr, png_infop info_ptr, png_textp text_ptr, int num_text);
#line 2042
extern  png_uint_32 _cdecl png_get_tIME 
(png_structp png_ptr, png_infop info_ptr, png_timep *mod_time);
#line 2047
extern  void _cdecl png_set_tIME 
(png_structp png_ptr, png_infop info_ptr, png_timep mod_time);
#line 2052
extern  png_uint_32 _cdecl png_get_tRNS 

(png_structp png_ptr, png_infop info_ptr, png_bytep *trans, int *num_trans, png_color_16p *trans_values);
#line 2058
extern  void _cdecl png_set_tRNS 

(png_structp png_ptr, png_infop info_ptr, png_bytep trans, int num_trans, png_color_16p trans_values);
#line 2068
extern  png_uint_32 _cdecl png_get_sCAL 
(png_structp png_ptr, png_infop info_ptr, int *unit, double *width, double *height);
#line 2080
extern  void _cdecl png_set_sCAL 
(png_structp png_ptr, png_infop info_ptr, int unit, double width, double height);
#line 2084
extern  void _cdecl png_set_sCAL_s 
(png_structp png_ptr, png_infop info_ptr, int unit, png_charp swidth, png_charp sheight);
#line 2099
extern  void _cdecl png_set_keep_unknown_chunks 
(png_structp png_ptr, int keep, png_bytep chunk_list, int num_chunks);
extern  void _cdecl png_set_unknown_chunks 
(png_structp png_ptr, png_infop info_ptr, png_unknown_chunkp unknowns, int num_unknowns);
extern  void _cdecl png_set_unknown_chunk_location
   (png_structp png_ptr, png_infop info_ptr, int chunk, int location);
extern  png_uint_32 _cdecl png_get_unknown_chunks 
(png_structp png_ptr, png_infop info_ptr, png_unknown_chunkpp entries);
#line 2112
extern  void _cdecl png_set_invalid 
(png_structp png_ptr, png_infop info_ptr, int mask);
#line 2117
extern  void _cdecl png_read_png 
#line 2120
(png_structp png_ptr, png_infop info_ptr, int transforms, voidp params);
extern  void _cdecl png_write_png 
#line 2124
(png_structp png_ptr, png_infop info_ptr, int transforms, voidp params);
#line 2178
extern  png_bytep _cdecl png_sig_bytes (void);

extern  png_charp _cdecl png_get_copyright (png_structp png_ptr);
extern  png_charp _cdecl png_get_header_ver (png_structp png_ptr);
extern  png_charp _cdecl png_get_header_version (png_structp png_ptr);
extern  png_charp _cdecl png_get_libpng_ver (png_structp png_ptr);
#line 2333
   extern   png_byte png_sig[8];
#line 2366
extern   const png_byte png_IHDR[5];
extern   const png_byte png_IDAT[5];
extern   const png_byte png_IEND[5];
extern   const png_byte png_PLTE[5];
extern   const png_byte png_bKGD[5];
extern   const png_byte png_cHRM[5];
extern   const png_byte png_gAMA[5];
extern   const png_byte png_hIST[5];
extern   const png_byte png_iCCP[5];
extern   const png_byte png_iTXt[5];
extern   const png_byte png_oFFs[5];
extern   const png_byte png_pCAL[5];
extern   const png_byte png_sCAL[5];
extern   const png_byte png_pHYs[5];
extern   const png_byte png_sBIT[5];
extern   const png_byte png_sPLT[5];
extern   const png_byte png_sRGB[5];
extern   const png_byte png_tEXt[5];
extern   const png_byte png_tIME[5];
extern   const png_byte png_tRNS[5];
extern   const png_byte png_zTXt[5];
#line 2406
 png_int_32 png_get_int_32 (png_bytep buf);

 png_uint_32 png_get_uint_32 (png_bytep buf);
 png_uint_16 png_get_uint_16 (png_bytep buf);
#line 2415
extern  void _cdecl png_read_init (png_structp png_ptr);
#line 2418
extern  void _cdecl png_read_init_2 

(png_structp png_ptr, png_const_charp user_png_ver, png_size_t png_struct_size, png_size_t png_info_size);
#line 2425
extern  void _cdecl png_write_init (png_structp png_ptr);
#line 2428
extern  void _cdecl png_write_init_2 

(png_structp png_ptr, png_const_charp user_png_ver, png_size_t png_struct_size, png_size_t png_info_size);
#line 2433
 png_voidp png_create_struct (int type);
#line 2436
 void png_destroy_struct (png_voidp struct_ptr);

 png_voidp png_create_struct_2 
(int type, png_malloc_ptr malloc_fn);
 void png_destroy_struct_2 
(png_voidp struct_ptr, png_free_ptr free_fn);
#line 2444
 void png_info_destroy 
(png_structp png_ptr, png_infop info_ptr);
#line 2448
 voidpf png_zalloc (voidpf png_ptr, uInt items, uInt size);
#line 2451
 void png_zfree (voidpf png_ptr, voidpf ptr);
#line 2454
 void png_reset_crc (png_structp png_ptr);
#line 2457
 void png_write_data 
(png_structp png_ptr, png_bytep data, png_size_t length);
#line 2461
 void png_read_data 
(png_structp png_ptr, png_bytep data, png_size_t length);
#line 2465
 void png_crc_read 
(png_structp png_ptr, png_bytep buf, png_size_t length);
#line 2471
 png_charp png_decompress_chunk 

(png_structp png_ptr, int comp_type, png_charp chunkdata, png_size_t chunklength, png_size_t prefix_length, png_size_t *data_length);
#line 2477
 int png_crc_finish (png_structp png_ptr, png_uint_32 skip);
#line 2480
 int png_crc_error (png_structp png_ptr);
#line 2486
 void png_calculate_crc 
(png_structp png_ptr, png_bytep ptr, png_size_t length);
#line 2490
 void png_flush (png_structp png_ptr);
#line 2497
 void png_save_uint_32 (png_bytep buf, png_uint_32 i);
#line 2500
 void png_save_int_32 (png_bytep buf, png_int_32 i);
#line 2507
 void png_save_uint_16 (png_bytep buf, unsigned int i);
#line 2510
 void png_write_sig (png_structp png_ptr);
#line 2517
 void png_write_IHDR 
#line 2520
(png_structp png_ptr, png_uint_32 width, png_uint_32 height, int bit_depth, int color_type, int compression_type, int filter_type, int interlace_type);

 void png_write_PLTE 
(png_structp png_ptr, png_colorp palette, png_uint_32 num_pal);

 void png_write_IDAT 
(png_structp png_ptr, png_bytep data, png_size_t length);

 void png_write_IEND (png_structp png_ptr);
#line 2532
 void png_write_gAMA (png_structp png_ptr, double file_gamma);
#line 2535
 void png_write_gAMA_fixed 
(png_structp png_ptr, png_fixed_point file_gamma);
#line 2541
 void png_write_sBIT 
(png_structp png_ptr, png_color_8p sbit, int color_type);
#line 2547
 void png_write_cHRM 
#line 2550
(png_structp png_ptr, double white_x, double white_y, double red_x, double red_y, double green_x, double green_y, double blue_x, double blue_y);
#line 2553
 void png_write_cHRM_fixed 
#line 2557
(png_structp png_ptr, png_fixed_point int_white_x, png_fixed_point int_white_y, png_fixed_point int_red_x, png_fixed_point int_red_y, png_fixed_point int_green_x, png_fixed_point int_green_y, png_fixed_point int_blue_x, png_fixed_point int_blue_y);
#line 2562
 void png_write_sRGB 
(png_structp png_ptr, int intent);
#line 2567
 void png_write_iCCP 

(png_structp png_ptr, png_charp name, int compression_type, png_charp profile, int proflen);
#line 2573
 void png_write_sPLT 
(png_structp png_ptr, png_sPLT_tp palette);
#line 2578
 void png_write_tRNS 
(png_structp png_ptr, png_bytep trans, png_color_16p values, int number, int color_type);
#line 2583
 void png_write_bKGD 
(png_structp png_ptr, png_color_16p values, int color_type);
#line 2588
 void png_write_hIST 
(png_structp png_ptr, png_uint_16p hist, int num_hist);
#line 2594
 png_size_t png_check_keyword 
(png_structp png_ptr, png_charp key, png_charpp new_key);
#line 2599
 void png_write_tEXt 
(png_structp png_ptr, png_charp key, png_charp text, png_size_t text_len);
#line 2604
 void png_write_zTXt 
(png_structp png_ptr, png_charp key, png_charp text, png_size_t text_len, int compression);
#line 2615
 void png_write_oFFs 
(png_structp png_ptr, png_uint_32 x_offset, png_uint_32 y_offset, int unit_type);
#line 2620
 void png_write_pCAL 

(png_structp png_ptr, png_charp purpose, png_int_32 X0, png_int_32 X1, int type, int nparams, png_charp units, png_charpp params);
#line 2626
 void png_write_pHYs 

(png_structp png_ptr, png_uint_32 x_pixels_per_unit, png_uint_32 y_pixels_per_unit, int unit_type);
#line 2632
 void png_write_tIME 
(png_structp png_ptr, png_timep mod_time);
#line 2638
 void png_write_sCAL 
(png_structp png_ptr, int unit, double width, double height);
#line 2649
 void png_write_finish_row (png_structp png_ptr);
#line 2652
 void png_write_start_row (png_structp png_ptr);
#line 2655
 void png_build_gamma_table (png_structp png_ptr);
#line 2659
 void png_combine_row 
(png_structp png_ptr, png_bytep row, int mask);
#line 2664
 void png_do_read_interlace 
(png_row_infop row_info, png_bytep row, int pass, png_uint_32 transformations);
#line 2670
 void png_do_write_interlace 
(png_row_infop row_info, png_bytep row, int pass);
#line 2675
 void png_read_filter_row 
(png_structp png_ptr, png_row_infop row_info, png_bytep row, png_bytep prev_row, int filter);
#line 2679
 void png_write_find_filter 
(png_structp png_ptr, png_row_infop row_info);
#line 2683
 void png_write_filtered_row 
(png_structp png_ptr, png_bytep filtered_row);

 void png_read_finish_row (png_structp png_ptr);
#line 2689
 void png_read_start_row (png_structp png_ptr);

 void png_read_transform_info 
(png_structp png_ptr, png_infop info_ptr);
#line 2696
 void png_do_read_filler 
(png_row_infop row_info, png_bytep row, png_uint_32 filler, png_uint_32 flags);
#line 2701
 void png_do_read_swap_alpha 
(png_row_infop row_info, png_bytep row);
#line 2706
 void png_do_write_swap_alpha 
(png_row_infop row_info, png_bytep row);
#line 2711
 void png_do_read_invert_alpha 
(png_row_infop row_info, png_bytep row);
#line 2716
 void png_do_write_invert_alpha 
(png_row_infop row_info, png_bytep row);
#line 2722
 void png_do_strip_filler 
(png_row_infop row_info, png_bytep row, png_uint_32 flags);
#line 2727
 void png_do_swap (png_row_infop row_info, png_bytep row);
#line 2731
 void png_do_packswap (png_row_infop row_info, png_bytep row);
#line 2735
 int png_do_rgb_to_gray 
(png_structp png_ptr, png_row_infop row_info, png_bytep row);
#line 2740
 void png_do_gray_to_rgb 
(png_row_infop row_info, png_bytep row);
#line 2745
 void png_do_unpack (png_row_infop row_info, png_bytep row);
#line 2749
 void png_do_unshift 
(png_row_infop row_info, png_bytep row, png_color_8p sig_bits);
#line 2754
 void png_do_invert (png_row_infop row_info, png_bytep row);
#line 2758
 void png_do_chop (png_row_infop row_info, png_bytep row);
#line 2762
 void png_do_dither 
(png_row_infop row_info, png_bytep row, png_bytep palette_lookup, png_bytep dither_lookup);
#line 2772
 void png_do_bgr (png_row_infop row_info, png_bytep row);
#line 2776
 void png_do_pack 
(png_row_infop row_info, png_bytep row, png_uint_32 bit_depth);
#line 2781
 void png_do_shift 
(png_row_infop row_info, png_bytep row, png_color_8p bit_depth);
#line 2786
 void png_do_background 
#line 2791
(png_row_infop row_info, png_bytep row, png_color_16p trans_values, png_color_16p background, png_color_16p background_1, png_bytep gamma_table, png_bytep gamma_from_1, png_bytep gamma_to_1, png_uint_16pp gamma_16, png_uint_16pp gamma_16_from_1, png_uint_16pp gamma_16_to_1, int gamma_shift);
#line 2795
 void png_do_gamma 

(png_row_infop row_info, png_bytep row, png_bytep gamma_table, png_uint_16pp gamma_16_table, int gamma_shift);
#line 2801
 void png_do_expand_palette 
(png_row_infop row_info, png_bytep row, png_colorp palette, png_bytep trans, int num_trans);
 void png_do_expand 
(png_row_infop row_info, png_bytep row, png_color_16p trans_value);
#line 2812
 void png_handle_IHDR 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
 void png_handle_PLTE 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
 void png_handle_IEND 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2820
 void png_handle_bKGD 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2825
 void png_handle_cHRM 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2830
 void png_handle_gAMA 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2835
 void png_handle_hIST 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2840
extern void png_handle_iCCP 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2850
 void png_handle_oFFs 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2855
 void png_handle_pCAL 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2860
 void png_handle_pHYs 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2865
 void png_handle_sBIT 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2870
 void png_handle_sCAL 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2875
extern void png_handle_sPLT 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2880
 void png_handle_sRGB 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2885
 void png_handle_tEXt 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2890
 void png_handle_tIME 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2895
 void png_handle_tRNS 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2900
 void png_handle_zTXt 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
#line 2905
 int png_handle_as_unknown 
(png_structp png_ptr, png_bytep chunk_name);
#line 2909
 void png_handle_unknown 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);

 void png_check_chunk_name 
(png_structp png_ptr, png_bytep chunk_name);
#line 2916
 void png_do_read_transformations (png_structp png_ptr);
 void png_do_write_transformations (png_structp png_ptr);

 void png_init_read_transformations (png_structp png_ptr);
#line 2922
 void png_push_read_chunk 
(png_structp png_ptr, png_infop info_ptr);
 void png_push_read_sig 
(png_structp png_ptr, png_infop info_ptr);
 void png_push_check_crc (png_structp png_ptr);
 void png_push_crc_skip 
(png_structp png_ptr, png_uint_32 length);
 void png_push_crc_finish (png_structp png_ptr);
 void png_push_fill_buffer 
(png_structp png_ptr, png_bytep buffer, png_size_t length);
 void png_push_save_buffer (png_structp png_ptr);
 void png_push_restore_buffer 
(png_structp png_ptr, png_bytep buffer, png_size_t buffer_length);
 void png_push_read_IDAT (png_structp png_ptr);
 void png_process_IDAT_data 
(png_structp png_ptr, png_bytep buffer, png_size_t buffer_length);
 void png_push_process_row (png_structp png_ptr);
 void png_push_handle_unknown 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
 void png_push_have_info 
(png_structp png_ptr, png_infop info_ptr);
 void png_push_have_end 
(png_structp png_ptr, png_infop info_ptr);
 void png_push_have_row (png_structp png_ptr, png_bytep row);
 void png_push_read_end 
(png_structp png_ptr, png_infop info_ptr);
 void png_process_some_data 
(png_structp png_ptr, png_infop info_ptr);
 void png_read_push_finish_row (png_structp png_ptr);

 void png_push_handle_tEXt 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
 void png_push_read_tEXt 
(png_structp png_ptr, png_infop info_ptr);
#line 2958
 void png_push_handle_zTXt 
(png_structp png_ptr, png_infop info_ptr, png_uint_32 length);
 void png_push_read_zTXt 
(png_structp png_ptr, png_infop info_ptr);
#line 27 "pngpread.c"
void _cdecl
png_process_data(png_structp png_ptr, png_infop info_ptr,
   png_bytep buffer, png_size_t buffer_size)
{
   png_push_restore_buffer(png_ptr, buffer, buffer_size);

   while (png_ptr->buffer_size)
   {
      png_process_some_data(png_ptr, info_ptr);
   }
}
#line 42
void  
png_process_some_data(png_structp png_ptr, png_infop info_ptr)
{
   switch (png_ptr->process_mode)
   {
      case 0:
      {
         png_push_read_sig(png_ptr, info_ptr);
         break;
      }
      case 1:
      {
         png_push_read_chunk(png_ptr, info_ptr);
         break;
      }
      case 2:
      {
         png_push_read_IDAT(png_ptr);
         break;
      }

      case 4:
      {
         png_push_read_tEXt(png_ptr, info_ptr);
         break;
      }
#line 70
      case 5:
      {
         png_push_read_zTXt(png_ptr, info_ptr);
         break;
      }
#line 83
      case 3:
      {
         png_push_crc_finish(png_ptr);
         break;
      }
      default:
      {
         png_ptr->buffer_size = 0;
         break;
      }
   }
}
#line 102
void  
png_push_read_sig(png_structp png_ptr, png_infop info_ptr)
{
   png_size_t num_checked = png_ptr->sig_bytes,
             num_to_check = 8 - num_checked;

   if (png_ptr->buffer_size < num_to_check)
   {
      num_to_check = png_ptr->buffer_size;
   }

   png_push_fill_buffer(png_ptr, &(info_ptr->signature[num_checked]),
      num_to_check);
   png_ptr->sig_bytes = (png_byte)(png_ptr->sig_bytes+num_to_check);

   if (png_sig_cmp(info_ptr->signature, num_checked, num_to_check))
   {
      if (num_checked < 4 &&
          png_sig_cmp(info_ptr->signature, num_checked, num_to_check - 4))
         png_error(png_ptr, "Not a PNG file");
      else
         png_error(png_ptr, "PNG file corrupted by ASCII conversion");
   }
   else
   {
      if (png_ptr->sig_bytes >= 8)
      {
         png_ptr->process_mode = 1;
      }
   }
}

void  
png_push_read_chunk(png_structp png_ptr, png_infop info_ptr)
{
#line 200
   if (!(png_ptr->mode & 0x100))
   {
      png_byte chunk_length[4];

      if (png_ptr->buffer_size < 8)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_push_fill_buffer(png_ptr, chunk_length, 4);
      png_ptr->push_length = png_get_uint_32(chunk_length);
      png_reset_crc(png_ptr);
      png_crc_read(png_ptr, png_ptr->chunk_name, 4);
      png_ptr->mode |= 0x100;
   }

   if (!__memcmp(png_ptr->chunk_name, png_IHDR, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_IHDR(png_ptr, info_ptr, png_ptr->push_length);
   }
   else if (!__memcmp(png_ptr->chunk_name, png_PLTE, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_PLTE(png_ptr, info_ptr, png_ptr->push_length);
   }
   else if (!__memcmp(png_ptr->chunk_name, (png_bytep)png_IDAT, 4))
   {
#line 243
      if (png_ptr->mode & 0x04)
      {
         if (png_ptr->push_length == 0)
            return;

         if (png_ptr->mode & 0x08)
            png_error(png_ptr, "Too many IDAT's found");
      }

      png_ptr->idat_size = png_ptr->push_length;
      png_ptr->mode |= 0x04;
      png_ptr->process_mode = 2;
      png_push_have_info(png_ptr, info_ptr);
      png_ptr->zstream.avail_out = (uInt)png_ptr->irowbytes;
      png_ptr->zstream.next_out = png_ptr->row_buf;
      return;
   }
   else if (!__memcmp(png_ptr->chunk_name, png_IEND, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_IEND(png_ptr, info_ptr, png_ptr->push_length);
      png_ptr->process_mode = 6;
      png_push_have_end(png_ptr, info_ptr);
   }

   else if (!__memcmp(png_ptr->chunk_name, png_gAMA, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_gAMA(png_ptr, info_ptr, png_ptr->push_length);
   }
#line 285
   else if (!__memcmp(png_ptr->chunk_name, png_sBIT, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_sBIT(png_ptr, info_ptr, png_ptr->push_length);
   }
#line 297
   else if (!__memcmp(png_ptr->chunk_name, png_cHRM, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_cHRM(png_ptr, info_ptr, png_ptr->push_length);
   }
#line 309
   else if (!__memcmp(png_ptr->chunk_name, png_sRGB, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_sRGB(png_ptr, info_ptr, png_ptr->push_length);
   }
#line 321
   else if (!__memcmp(png_ptr->chunk_name, png_iCCP, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_iCCP(png_ptr, info_ptr, png_ptr->push_length);
   }
#line 333
   else if (!__memcmp(png_ptr->chunk_name, png_sPLT, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_sPLT(png_ptr, info_ptr, png_ptr->push_length);
   }
#line 345
   else if (!__memcmp(png_ptr->chunk_name, png_tRNS, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_tRNS(png_ptr, info_ptr, png_ptr->push_length);
   }
#line 357
   else if (!__memcmp(png_ptr->chunk_name, png_bKGD, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_bKGD(png_ptr, info_ptr, png_ptr->push_length);
   }
#line 369
   else if (!__memcmp(png_ptr->chunk_name, png_hIST, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_hIST(png_ptr, info_ptr, png_ptr->push_length);
   }
#line 381
   else if (!__memcmp(png_ptr->chunk_name, png_pHYs, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_pHYs(png_ptr, info_ptr, png_ptr->push_length);
   }
#line 393
   else if (!__memcmp(png_ptr->chunk_name, png_oFFs, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_oFFs(png_ptr, info_ptr, png_ptr->push_length);
   }
#line 405
   else if (!__memcmp(png_ptr->chunk_name, png_pCAL, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_pCAL(png_ptr, info_ptr, png_ptr->push_length);
   }
#line 417
   else if (!__memcmp(png_ptr->chunk_name, png_sCAL, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_sCAL(png_ptr, info_ptr, png_ptr->push_length);
   }
#line 429
   else if (!__memcmp(png_ptr->chunk_name, png_tIME, 4))
   {
      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_handle_tIME(png_ptr, info_ptr, png_ptr->push_length);
   }
#line 441
   else if (!__memcmp(png_ptr->chunk_name, png_tEXt, 4))
   {
      png_push_handle_tEXt(png_ptr, info_ptr, png_ptr->push_length);
   }
#line 447
   else if (!__memcmp(png_ptr->chunk_name, png_zTXt, 4))
   {
      png_push_handle_zTXt(png_ptr, info_ptr, png_ptr->push_length);
   }
#line 458
   else
   {
      png_push_handle_unknown(png_ptr, info_ptr, png_ptr->push_length);
   }

   png_ptr->mode &= ~0x100;
}

void  
png_push_crc_skip(png_structp png_ptr, png_uint_32 skip)
{
   png_ptr->process_mode = 3;
   png_ptr->skip_length = skip;
}

void  
png_push_crc_finish(png_structp png_ptr)
{
   if (png_ptr->skip_length && png_ptr->save_buffer_size)
   {
      png_size_t save_size;

      if (png_ptr->skip_length < (png_uint_32)png_ptr->save_buffer_size)
         save_size = (png_size_t)png_ptr->skip_length;
      else
         save_size = png_ptr->save_buffer_size;

      png_calculate_crc(png_ptr, png_ptr->save_buffer_ptr, save_size);

      png_ptr->skip_length -= save_size;
      png_ptr->buffer_size -= save_size;
      png_ptr->save_buffer_size -= save_size;
      png_ptr->save_buffer_ptr += save_size;
   }
   if (png_ptr->skip_length && png_ptr->current_buffer_size)
   {
      png_size_t save_size;

      if (png_ptr->skip_length < (png_uint_32)png_ptr->current_buffer_size)
         save_size = (png_size_t)png_ptr->skip_length;
      else
         save_size = png_ptr->current_buffer_size;

      png_calculate_crc(png_ptr, png_ptr->current_buffer_ptr, save_size);

      png_ptr->skip_length -= save_size;
      png_ptr->buffer_size -= save_size;
      png_ptr->current_buffer_size -= save_size;
      png_ptr->current_buffer_ptr += save_size;
   }
   if (!png_ptr->skip_length)
   {
      if (png_ptr->buffer_size < 4)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_crc_finish(png_ptr, 0);
      png_ptr->process_mode = 1;
   }
}

void  
png_push_fill_buffer(png_structp png_ptr, png_bytep buffer, png_size_t length)
{
   png_bytep ptr;

   ptr = buffer;
   if (png_ptr->save_buffer_size)
   {
      png_size_t save_size;

      if (length < png_ptr->save_buffer_size)
         save_size = length;
      else
         save_size = png_ptr->save_buffer_size;

      __memcpy(ptr, png_ptr->save_buffer_ptr, save_size);
      length -= save_size;
      ptr += save_size;
      png_ptr->buffer_size -= save_size;
      png_ptr->save_buffer_size -= save_size;
      png_ptr->save_buffer_ptr += save_size;
   }
   if (length && png_ptr->current_buffer_size)
   {
      png_size_t save_size;

      if (length < png_ptr->current_buffer_size)
         save_size = length;
      else
         save_size = png_ptr->current_buffer_size;

      __memcpy(ptr, png_ptr->current_buffer_ptr, save_size);
      png_ptr->buffer_size -= save_size;
      png_ptr->current_buffer_size -= save_size;
      png_ptr->current_buffer_ptr += save_size;
   }
}

void  
png_push_save_buffer(png_structp png_ptr)
{
   if (png_ptr->save_buffer_size)
   {
      if (png_ptr->save_buffer_ptr != png_ptr->save_buffer)
      {
         png_size_t i,istop;
         png_bytep sp;
         png_bytep dp;

         istop = png_ptr->save_buffer_size;
         for (i = 0, sp = png_ptr->save_buffer_ptr, dp = png_ptr->save_buffer;
            i < istop; i++, sp++, dp++)
         {
            *dp = *sp;
         }
      }
   }
   if (png_ptr->save_buffer_size + png_ptr->current_buffer_size >
      png_ptr->save_buffer_max)
   {
      png_size_t new_max;
      png_bytep old_buffer;

      new_max = png_ptr->save_buffer_size + png_ptr->current_buffer_size + 256;
      old_buffer = png_ptr->save_buffer;
      png_ptr->save_buffer = (png_bytep)png_malloc(png_ptr,
         (png_uint_32)new_max);
      __memcpy(png_ptr->save_buffer, old_buffer, png_ptr->save_buffer_size);
      png_free(png_ptr, old_buffer);
      png_ptr->save_buffer_max = new_max;
   }
   if (png_ptr->current_buffer_size)
   {

__memcpy(png_ptr->save_buffer + png_ptr->save_buffer_size, png_ptr->current_buffer_ptr, png_ptr->current_buffer_size);
      png_ptr->save_buffer_size += png_ptr->current_buffer_size;
      png_ptr->current_buffer_size = 0;
   }
   png_ptr->save_buffer_ptr = png_ptr->save_buffer;
   png_ptr->buffer_size = 0;
}

void  
png_push_restore_buffer(png_structp png_ptr, png_bytep buffer,
   png_size_t buffer_length)
{
   png_ptr->current_buffer = buffer;
   png_ptr->current_buffer_size = buffer_length;
   png_ptr->buffer_size = buffer_length + png_ptr->save_buffer_size;
   png_ptr->current_buffer_ptr = png_ptr->current_buffer;
}

void  
png_push_read_IDAT(png_structp png_ptr)
{
#line 619
   if (!(png_ptr->mode & 0x100))
   {
      png_byte chunk_length[4];

      if (png_ptr->buffer_size < 8)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_push_fill_buffer(png_ptr, chunk_length, 4);
      png_ptr->push_length = png_get_uint_32(chunk_length);

      png_reset_crc(png_ptr);
      png_crc_read(png_ptr, png_ptr->chunk_name, 4);
      png_ptr->mode |= 0x100;

      if (__memcmp(png_ptr->chunk_name, (png_bytep)png_IDAT, 4))
      {
         png_ptr->process_mode = 1;
         if (!(png_ptr->flags & 0x0020))
            png_error(png_ptr, "Not enough compressed data");
         return;
      }

      png_ptr->idat_size = png_ptr->push_length;
   }
   if (png_ptr->idat_size && png_ptr->save_buffer_size)
   {
      png_size_t save_size;

      if (png_ptr->idat_size < (png_uint_32)png_ptr->save_buffer_size)
      {
         save_size = (png_size_t)png_ptr->idat_size;

         if((png_uint_32)save_size != png_ptr->idat_size)
            png_error(png_ptr, "save_size overflowed in pngpread");
      }
      else
         save_size = png_ptr->save_buffer_size;

      png_calculate_crc(png_ptr, png_ptr->save_buffer_ptr, save_size);
      png_process_IDAT_data(png_ptr, png_ptr->save_buffer_ptr, save_size);

      png_ptr->idat_size -= save_size;
      png_ptr->buffer_size -= save_size;
      png_ptr->save_buffer_size -= save_size;
      png_ptr->save_buffer_ptr += save_size;
   }
   if (png_ptr->idat_size && png_ptr->current_buffer_size)
   {
      png_size_t save_size;

      if (png_ptr->idat_size < (png_uint_32)png_ptr->current_buffer_size)
      {
         save_size = (png_size_t)png_ptr->idat_size;

         if((png_uint_32)save_size != png_ptr->idat_size)
            png_error(png_ptr, "save_size overflowed in pngpread");
      }
      else
         save_size = png_ptr->current_buffer_size;

      png_calculate_crc(png_ptr, png_ptr->current_buffer_ptr, save_size);
      png_process_IDAT_data(png_ptr, png_ptr->current_buffer_ptr, save_size);

      png_ptr->idat_size -= save_size;
      png_ptr->buffer_size -= save_size;
      png_ptr->current_buffer_size -= save_size;
      png_ptr->current_buffer_ptr += save_size;
   }
   if (!png_ptr->idat_size)
   {
      if (png_ptr->buffer_size < 4)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_crc_finish(png_ptr, 0);
      png_ptr->mode &= ~0x100;
   }
}

void  
png_process_IDAT_data(png_structp png_ptr, png_bytep buffer,
   png_size_t buffer_length)
{
   int ret;

   if ((png_ptr->flags & 0x0020) && buffer_length)
      png_error(png_ptr, "Extra compression data");

   png_ptr->zstream.next_in = buffer;
   png_ptr->zstream.avail_in = (uInt)buffer_length;
   for(;;)
   {
      ret = inflate(&png_ptr->zstream, 1);
      if (ret != 0)
      {
         if (ret == 1)
         {
            if (png_ptr->zstream.avail_in)
               png_error(png_ptr, "Extra compressed data");
            if (!(png_ptr->zstream.avail_out))
            {
               png_push_process_row(png_ptr);
            }

            png_ptr->mode |= 0x08;
            png_ptr->flags |= 0x0020;
            break;
         }
         else if (ret == (-5))
            break;
         else
            png_error(png_ptr, "Decompression Error");
      }
      if (!(png_ptr->zstream.avail_out))
      {
         png_push_process_row(png_ptr);
         png_ptr->zstream.avail_out = (uInt)png_ptr->irowbytes;
         png_ptr->zstream.next_out = png_ptr->row_buf;
      }
      else
         break;
   }
}

void  
png_push_process_row(png_structp png_ptr)
{
   png_ptr->row_info.color_type = png_ptr->color_type;
   png_ptr->row_info.width = png_ptr->iwidth;
   png_ptr->row_info.channels = png_ptr->channels;
   png_ptr->row_info.bit_depth = png_ptr->bit_depth;
   png_ptr->row_info.pixel_depth = png_ptr->pixel_depth;

   png_ptr->row_info.rowbytes = ((png_ptr->row_info.width *
      (png_uint_32)png_ptr->row_info.pixel_depth + 7) >> 3);

   png_read_filter_row(png_ptr, &(png_ptr->row_info),
      png_ptr->row_buf + 1, png_ptr->prev_row + 1,
      (int)(png_ptr->row_buf[0]));

   png_memcpy_check(png_ptr, png_ptr->prev_row, png_ptr->row_buf,
      png_ptr->rowbytes + 1);

   if (png_ptr->transformations)
      png_do_read_transformations(png_ptr);
#line 772
   if (png_ptr->interlaced && (png_ptr->transformations & 0x0002))
   {
      if (png_ptr->pass < 6)
         png_do_read_interlace(&(png_ptr->row_info),
            png_ptr->row_buf + 1, png_ptr->pass, png_ptr->transformations);

      switch (png_ptr->pass)
      {
         case 0:
         {
            int i;
            for (i = 0; i < 8 && png_ptr->pass == 0; i++)
            {
               png_push_have_row(png_ptr, png_ptr->row_buf + 1);
               png_read_push_finish_row(png_ptr);
            }
            break;
         }
         case 1:
         {
            int i;
            for (i = 0; i < 8 && png_ptr->pass == 1; i++)
            {
               png_push_have_row(png_ptr, png_ptr->row_buf + 1);
               png_read_push_finish_row(png_ptr);
            }
            if (png_ptr->pass == 2)
            {
               for (i = 0; i < 4 && png_ptr->pass == 2; i++)
               {
                  png_push_have_row(png_ptr, 0);
                  png_read_push_finish_row(png_ptr);
               }
            }
            break;
         }
         case 2:
         {
            int i;
            for (i = 0; i < 4 && png_ptr->pass == 2; i++)
            {
               png_push_have_row(png_ptr, png_ptr->row_buf + 1);
               png_read_push_finish_row(png_ptr);
            }
            for (i = 0; i < 4 && png_ptr->pass == 2; i++)
            {
               png_push_have_row(png_ptr, 0);
               png_read_push_finish_row(png_ptr);
            }
            break;
         }
         case 3:
         {
            int i;
            for (i = 0; i < 4 && png_ptr->pass == 3; i++)
            {
               png_push_have_row(png_ptr, png_ptr->row_buf + 1);
               png_read_push_finish_row(png_ptr);
            }
            if (png_ptr->pass == 4)
            {
               for (i = 0; i < 2 && png_ptr->pass == 4; i++)
               {
                  png_push_have_row(png_ptr, 0);
                  png_read_push_finish_row(png_ptr);
               }
            }
            break;
         }
         case 4:
         {
            int i;
            for (i = 0; i < 2 && png_ptr->pass == 4; i++)
            {
               png_push_have_row(png_ptr, png_ptr->row_buf + 1);
               png_read_push_finish_row(png_ptr);
            }
            for (i = 0; i < 2 && png_ptr->pass == 4; i++)
            {
               png_push_have_row(png_ptr, 0);
               png_read_push_finish_row(png_ptr);
            }
            break;
         }
         case 5:
         {
            int i;
            for (i = 0; i < 2 && png_ptr->pass == 5; i++)
            {
               png_push_have_row(png_ptr, png_ptr->row_buf + 1);
               png_read_push_finish_row(png_ptr);
            }
            if (png_ptr->pass == 6)
            {
               png_push_have_row(png_ptr, 0);
               png_read_push_finish_row(png_ptr);
            }
            break;
         }
         case 6:
         {
            png_push_have_row(png_ptr, png_ptr->row_buf + 1);
            png_read_push_finish_row(png_ptr);
            if (png_ptr->pass != 6)
               break;
            png_push_have_row(png_ptr, 0);
            png_read_push_finish_row(png_ptr);
         }
      }
   }
   else

   {
      png_push_have_row(png_ptr, png_ptr->row_buf + 1);
      png_read_push_finish_row(png_ptr);
   }
}

void  
png_read_push_finish_row(png_structp png_ptr)
{
#line 919
   png_ptr->row_number++;
   if (png_ptr->row_number < png_ptr->num_rows)
      return;

   if (png_ptr->interlaced)
   {
      png_ptr->row_number = 0;
      png_memset_check(png_ptr, png_ptr->prev_row, 0,
         png_ptr->rowbytes + 1);
      do
      {
         png_ptr->pass++;
         if (png_ptr->pass >= 7)
            break;

         png_ptr->iwidth = (png_ptr->width +
            png_pass_inc[png_ptr->pass] - 1 -
            png_pass_start[png_ptr->pass]) /
            png_pass_inc[png_ptr->pass];

         png_ptr->irowbytes = ((png_ptr->iwidth *
            png_ptr->pixel_depth + 7) >> 3) + 1;

         if (png_ptr->transformations & 0x0002)
            break;

         png_ptr->num_rows = (png_ptr->height +
            png_pass_yinc[png_ptr->pass] - 1 -
            png_pass_ystart[png_ptr->pass]) /
            png_pass_yinc[png_ptr->pass];

      } while (png_ptr->iwidth == 0 || png_ptr->num_rows == 0);
   }
}
#line 955
void  
png_push_handle_tEXt(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
{
   if (!(png_ptr->mode & 0x01) || (png_ptr->mode & 0x10))
      {
         png_error(png_ptr, "Out of place tEXt");

         if(info_ptr == 0) return;
      }
#line 976
   png_ptr->current_text = (png_charp)png_malloc(png_ptr,
         (png_uint_32)(length+1));
   png_ptr->current_text[length] = '\0';
   png_ptr->current_text_ptr = png_ptr->current_text;
   png_ptr->current_text_size = (png_size_t)length;
   png_ptr->current_text_left = (png_size_t)length;
   png_ptr->process_mode = 4;
}

void  
png_push_read_tEXt(png_structp png_ptr, png_infop info_ptr)
{
   if (png_ptr->buffer_size && png_ptr->current_text_left)
   {
      png_size_t text_size;

      if (png_ptr->buffer_size < png_ptr->current_text_left)
         text_size = png_ptr->buffer_size;
      else
         text_size = png_ptr->current_text_left;
      png_crc_read(png_ptr, (png_bytep)png_ptr->current_text_ptr, text_size);
      png_ptr->current_text_left -= text_size;
      png_ptr->current_text_ptr += text_size;
   }
   if (!(png_ptr->current_text_left))
   {
      png_textp text_ptr;
      png_charp text;
      png_charp key;

      if (png_ptr->buffer_size < 4)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_push_crc_finish(png_ptr);
#line 1019
      key = png_ptr->current_text;
      png_ptr->current_text = 0;

      for (text = key; *text; text++)
           ;

      if (text != key + png_ptr->current_text_size)
         text++;

      text_ptr = (png_textp)png_malloc(png_ptr, (png_uint_32)sizeof(png_text));
      text_ptr->compression = -1;
      text_ptr->key = key;
#line 1035
      text_ptr->text = text;

      png_set_text(png_ptr, info_ptr, text_ptr, 1);

      png_free(png_ptr, key);
      png_free(png_ptr, text_ptr);
   }
}
#line 1046
void  
png_push_handle_zTXt(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
{
   if (!(png_ptr->mode & 0x01) || (png_ptr->mode & 0x10))
      {
         png_error(png_ptr, "Out of place zTXt");

         if(info_ptr == 0) return;
      }
#line 1069
   png_ptr->current_text = (png_charp)png_malloc(png_ptr,
       (png_uint_32)(length+1));
   png_ptr->current_text[length] = '\0';
   png_ptr->current_text_ptr = png_ptr->current_text;
   png_ptr->current_text_size = (png_size_t)length;
   png_ptr->current_text_left = (png_size_t)length;
   png_ptr->process_mode = 5;
}

void  
png_push_read_zTXt(png_structp png_ptr, png_infop info_ptr)
{
   if (png_ptr->buffer_size && png_ptr->current_text_left)
   {
      png_size_t text_size;

      if (png_ptr->buffer_size < (png_uint_32)png_ptr->current_text_left)
         text_size = png_ptr->buffer_size;
      else
         text_size = png_ptr->current_text_left;
      png_crc_read(png_ptr, (png_bytep)png_ptr->current_text_ptr, text_size);
      png_ptr->current_text_left -= text_size;
      png_ptr->current_text_ptr += text_size;
   }
   if (!(png_ptr->current_text_left))
   {
      png_textp text_ptr;
      png_charp text;
      png_charp key;
      int ret;
      png_size_t text_size, key_size;

      if (png_ptr->buffer_size < 4)
      {
         png_push_save_buffer(png_ptr);
         return;
      }

      png_push_crc_finish(png_ptr);

      key = png_ptr->current_text;
      png_ptr->current_text = 0;

      for (text = key; *text; text++)
           ;
#line 1116
      if (text == key + png_ptr->current_text_size)
      {
         png_free(png_ptr, key);
         return;
      }

      text++;

      if (*text != 0)  
      {
         png_free(png_ptr, key);
         return;
      }

      text++;

      png_ptr->zstream.next_in = (png_bytep )text;
      png_ptr->zstream.avail_in = (uInt)(png_ptr->current_text_size -
         (text - key));
      png_ptr->zstream.next_out = png_ptr->zbuf;
      png_ptr->zstream.avail_out = (uInt)png_ptr->zbuf_size;

      key_size = text - key;
      text_size = 0;
      text = 0;
      ret = 1;

      while (png_ptr->zstream.avail_in)
      {
         ret = inflate(&png_ptr->zstream, 1);
         if (ret != 0 && ret != 1)
         {
            inflateReset(&png_ptr->zstream);
            png_ptr->zstream.avail_in = 0;
            png_free(png_ptr, key);
            png_free(png_ptr, text);
            return;
         }
         if (!(png_ptr->zstream.avail_out) || ret == 1)
         {
            if (text == 0)
            {
               text = (png_charp)png_malloc(png_ptr,
                  (png_uint_32)(png_ptr->zbuf_size - png_ptr->zstream.avail_out +
                     key_size + 1));

__memcpy(text + key_size, png_ptr->zbuf, png_ptr->zbuf_size - png_ptr->zstream.avail_out);
               __memcpy(text, key, key_size);
               text_size = key_size + png_ptr->zbuf_size -
                  png_ptr->zstream.avail_out;
               *(text + text_size) = '\0';
            }
            else
            {
               png_charp tmp;

               tmp = text;
               text = (png_charp)png_malloc(png_ptr, text_size +
                  (png_uint_32)(png_ptr->zbuf_size - png_ptr->zstream.avail_out
                   + 1));
               __memcpy(text, tmp, text_size);
               png_free(png_ptr, tmp);

__memcpy(text + text_size, png_ptr->zbuf, png_ptr->zbuf_size - png_ptr->zstream.avail_out);
               text_size += png_ptr->zbuf_size - png_ptr->zstream.avail_out;
               *(text + text_size) = '\0';
            }
            if (ret != 1)
            {
               png_ptr->zstream.next_out = png_ptr->zbuf;
               png_ptr->zstream.avail_out = (uInt)png_ptr->zbuf_size;
            }
         }
         else
         {
            break;
         }

         if (ret == 1)
            break;
      }

      inflateReset(&png_ptr->zstream);
      png_ptr->zstream.avail_in = 0;

      if (ret != 1)
      {
         png_free(png_ptr, key);
         png_free(png_ptr, text);
         return;
      }

      png_free(png_ptr, key);
      key = text;
      text += key_size;

      text_ptr = (png_textp)png_malloc(png_ptr, (png_uint_32)sizeof(png_text));
      text_ptr->compression = 0;
      text_ptr->key = key;
#line 1219
      text_ptr->text = text;

      png_set_text(png_ptr, info_ptr, text_ptr, 1);

      png_free(png_ptr, key);
      png_free(png_ptr, text_ptr);
   }
}
#line 1340
void  
png_push_handle_unknown(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
{
   png_uint_32 skip=0;
   png_check_chunk_name(png_ptr, png_ptr->chunk_name);

   if (!(png_ptr->chunk_name[0] & 0x20))
   {

      if(png_handle_as_unknown(png_ptr, png_ptr->chunk_name) !=
           3

           && png_ptr->read_user_chunk_fn == (png_user_chunk_ptr)0

         )

         png_chunk_error(png_ptr, "unknown critical chunk");
#line 1359
      if (info_ptr == 0)
         return;
   }
#line 1364
   if (png_ptr->flags & 0x8000L)
   {
       png_unknown_chunk chunk;
#line 1377
       __strcpy((png_charp)chunk.name, (png_charp)png_ptr->chunk_name);
       chunk.data = (png_bytep)png_malloc(png_ptr, length);
       png_crc_read(png_ptr, chunk.data, length);
       chunk.size = length;

       if(png_ptr->read_user_chunk_fn != (png_user_chunk_ptr)0)
       {

          if ((*(png_ptr->read_user_chunk_fn)) (png_ptr, &chunk) <= 0)
          {
             if (!(png_ptr->chunk_name[0] & 0x20))
                if(png_handle_as_unknown(png_ptr, png_ptr->chunk_name) !=
                     3)
                   png_chunk_error(png_ptr, "unknown critical chunk");
          }
             png_set_unknown_chunks(png_ptr, info_ptr, &chunk, 1);
       }
       else

          png_set_unknown_chunks(png_ptr, info_ptr, &chunk, 1);
       png_free(png_ptr, chunk.data);
   }
   else

      skip=length;
   png_push_crc_skip(png_ptr, skip);
}

void  
png_push_have_info(png_structp png_ptr, png_infop info_ptr)
{
   if (png_ptr->info_fn != 0)
      (*(png_ptr->info_fn))(png_ptr, info_ptr);
}

void  
png_push_have_end(png_structp png_ptr, png_infop info_ptr)
{
   if (png_ptr->end_fn != 0)
      (*(png_ptr->end_fn))(png_ptr, info_ptr);
}

void  
png_push_have_row(png_structp png_ptr, png_bytep row)
{
   if (png_ptr->row_fn != 0)
      (*(png_ptr->row_fn))(png_ptr, row, png_ptr->row_number,
         (int)png_ptr->pass);
}

void _cdecl
png_progressive_combine_row (png_structp png_ptr,
   png_bytep old_row, png_bytep new_row)
{
#line 1434
   if (new_row != 0)     
      png_combine_row(png_ptr, old_row, png_pass_dsp_mask[png_ptr->pass]);
}

void _cdecl
png_set_progressive_read_fn(png_structp png_ptr, png_voidp progressive_ptr,
   png_progressive_info_ptr info_fn, png_progressive_row_ptr row_fn,
   png_progressive_end_ptr end_fn)
{
   png_ptr->info_fn = info_fn;
   png_ptr->row_fn = row_fn;
   png_ptr->end_fn = end_fn;

   png_set_read_fn(png_ptr, progressive_ptr, png_push_fill_buffer);
}

png_voidp _cdecl
png_get_progressive_ptr(png_structp png_ptr)
{
   return png_ptr->io_ptr;
}
