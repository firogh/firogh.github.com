#include <stdio.h>

#define LOCK_PREFIX_HERE \
                ".pushsection .smp_locks,\"a\"\n"       \
                ".balign 4\n"                           \
                ".long 671f - .\n" /* offset */         \
                ".popsection\n"                         \
                "671:"

#define LOCK_PREFIX LOCK_PREFIX_HERE "\n\tlock; "
#define asm_volatile_goto(x...) do { asm goto(x); asm (""); } while (0)

#define __GEN_RMWcc(fullop, var, cc, ...)                               \
do {                                                                    \
        asm_volatile_goto (fullop "; j" cc " %l[cc_label]"              \
                        : : "m" (var), ## __VA_ARGS__                   \
                        : "memory" : cc_label);                         \
        return 0;                                                       \
cc_label:                                                               \
        return 1;                                                       \
} while (0)

#define GEN_UNARY_RMWcc(op, var, arg0, cc)                              \
        __GEN_RMWcc(op " " arg0, var, cc)

#define GEN_BINARY_RMWcc(op, var, vcon, val, arg0, cc)                  \
        __GEN_RMWcc(op " %1, " arg0, var, cc, vcon (val))

static inline int test_and_set_bit(long nr, volatile unsigned long *addr)
{
        GEN_BINARY_RMWcc(LOCK_PREFIX "bts", *addr, "Ir", nr, "%0", "c");
}

struct foo {
	int f:2;
	int o:1;
	int O:1;
	int a;
        union {
                unsigned int dead_for_kabi;
                struct {
                        unsigned int            data_seq:2;     /* data sequencing level
                                                                 * 0 => none, 1 => IP only,
                                                                 * 2 => all
                                                                 */
                        unsigned int            recv_seq:1;     /* expect receive packets with
                                                                 * sequence numbers? */
                        unsigned int            send_seq:1;     /* send packets with sequence
                                                                 * numbers? */
                        unsigned int            lns_mode:1;     /* behave as LNS? LAC enables
                                                                 * sequence numbers under
                                                                 * control of LNS. */
                        unsigned int            dead:1;
                };
        };
};
int main(void)
{
	struct foo F, *f;
	f = &F;
	int rv = 0;
	f->o = 0; f->O = 0;
	f->dead_for_kabi = 0;
	printf("before set %d %d %x\n", f->dead_for_kabi, f->dead, &(f->dead_for_kabi));
	rv = test_and_set_bit(5, (unsigned long *)&f->dead_for_kabi);
	printf("after set %d %d rv %d\n", f->dead_for_kabi, f->dead, rv);

	return 0;
}
