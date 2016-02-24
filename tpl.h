#define TPL_FILE                        (1 << 0)
#define TPL_MEM                         (1 << 1)
#define TPL_PREALLOCD                   (1 << 2)
#define TPL_EXCESS_OK                   (1 << 3)
#define TPL_FD                          (1 << 4)
#define TPL_UFREE                       (1 << 5)
#define TPL_DATAPEEK                    (1 << 6)
#define TPL_FXLENS                      (1 << 7)
#define TPL_GETSIZE                     (1 << 8)
#define TPL_GATHER_BLOCKING             1
#define TPL_GATHER_NONBLOCKING          2
#define TPL_GATHER_MEM                  3

struct tpl_node
{

    int type;
    void *addr;
    void *data;
    int num;
    size_t ser_osz;
    struct tpl_node *children;
    struct tpl_node *next,*prev;
    struct tpl_node *parent;

};

struct tpl_node *tpl_map(char *fmt, ...);
void tpl_free(struct tpl_node *r);
int tpl_pack(struct tpl_node *r, int i);
int tpl_unpack(struct tpl_node *r, int i);
int tpl_dump(struct tpl_node *r, int mode, ...);
int tpl_load(struct tpl_node *r, int mode, ...);
int tpl_Alen(struct tpl_node *r, int i);
char *tpl_peek(int mode, ...);
int tpl_gather(int mode, ...);
int tpl_jot(int mode, ...);
struct tpl_node *tpl_map_va(char *fmt, va_list ap);
