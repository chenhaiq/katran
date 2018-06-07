/* eBPF mini library */
#include "bpf.h"
#ifndef __LIBBPF_H
#define __LIBBPF_H

#ifndef __NR_bpf
# if defined(__i386__)
#  define __NR_bpf 357
# elif defined(__x86_64__)
#  define __NR_bpf 321
# elif defined(__aarch64__)
#  define __NR_bpf 280
#  error __NR_bpf not defined. libbpf does not support your arch.
# endif
#endif

struct bpf_insn;

int ebpf_create_map_node(enum bpf_map_type map_type, const char *name,
                        int key_size, int value_size, int max_entries,
                        __u32 map_flags, int node);
int ebpf_create_map_name(enum bpf_map_type map_type, const char *name,
                        int key_size, int value_size, int max_entries,
                        __u32 map_flags);
int ebpf_create_map(enum bpf_map_type map_type, int key_size, int value_size,
                   int max_entries, __u32 map_flags);
int ebpf_create_map_in_map_node(enum bpf_map_type map_type, const char *name,
                               int key_size, int inner_map_fd, int max_entries,
                               __u32 map_flags, int node);
int ebpf_create_map_in_map(enum bpf_map_type map_type, int key_size,
                          int inner_map_fd, int max_entries, __u32 map_flags);

int ebpf_update_elem(int fd, void *key, void *value, unsigned long long flags);
int ebpf_lookup_elem(int fd, void *key, void *value);
int ebpf_delete_elem(int fd, void *key);
int ebpf_get_next_key(int fd, void *key, void *next_key);

int ebpf_prog_load(enum bpf_prog_type prog_type,
                  const struct bpf_insn *insns, int insn_len,
                  const char *license, __u32 kern_version,
                  char *buf, int buf_size);

int ebpf_prog_load_name(enum bpf_prog_type prog_type, const char *name,
                       const struct bpf_insn *insns, int insn_len,
                       const char *license, __u32 kern_version,
                       char *buf, int buf_size);

int ebpf_obj_pin(int fd, const char *pathname);
int ebpf_obj_get(const char *pathname);
int ebpf_prog_attach(int prog_fd, int target_fd, enum bpf_attach_type type,
                    unsigned int flags);
int ebpf_prog_detach(int target_fd, enum bpf_attach_type type);
int ebpf_prog_detach2(int prog_fd, int target_fd, enum bpf_attach_type type);
int ebpf_prog_test_run(int prog_fd, int repeat, void *data, __u32 size,
                      void *data_out, __u32 *size_out, __u32 *retval,
                      __u32 *duration);


/* ALU ops on registers, bpf_add|sub|...: dst_reg += src_reg */

#define BPF_ALU64_REG(OP, DST, SRC)       \
  ((struct bpf_insn) {          \
    .code  = BPF_ALU64 | BPF_OP(OP) | BPF_X,  \
    .dst_reg = DST,         \
    .src_reg = SRC,         \
    .off   = 0,         \
    .imm   = 0 })

#define BPF_ALU32_REG(OP, DST, SRC)       \
  ((struct bpf_insn) {          \
    .code  = BPF_ALU | BPF_OP(OP) | BPF_X,    \
    .dst_reg = DST,         \
    .src_reg = SRC,         \
    .off   = 0,         \
    .imm   = 0 })

/* ALU ops on immediates, bpf_add|sub|...: dst_reg += imm32 */

#define BPF_ALU64_IMM(OP, DST, IMM)       \
  ((struct bpf_insn) {          \
    .code  = BPF_ALU64 | BPF_OP(OP) | BPF_K,  \
    .dst_reg = DST,         \
    .src_reg = 0,         \
    .off   = 0,         \
    .imm   = IMM })

#define BPF_ALU32_IMM(OP, DST, IMM)       \
  ((struct bpf_insn) {          \
    .code  = BPF_ALU | BPF_OP(OP) | BPF_K,    \
    .dst_reg = DST,         \
    .src_reg = 0,         \
    .off   = 0,         \
    .imm   = IMM })

/* Short form of mov, dst_reg = src_reg */

#define BPF_MOV64_REG(DST, SRC)         \
  ((struct bpf_insn) {          \
    .code  = BPF_ALU64 | BPF_MOV | BPF_X,   \
    .dst_reg = DST,         \
    .src_reg = SRC,         \
    .off   = 0,         \
    .imm   = 0 })

#define BPF_MOV32_REG(DST, SRC)         \
  ((struct bpf_insn) {          \
    .code  = BPF_ALU | BPF_MOV | BPF_X,   \
    .dst_reg = DST,         \
    .src_reg = SRC,         \
    .off   = 0,         \
    .imm   = 0 })

/* Short form of mov, dst_reg = imm32 */

#define BPF_MOV64_IMM(DST, IMM)         \
  ((struct bpf_insn) {          \
    .code  = BPF_ALU64 | BPF_MOV | BPF_K,   \
    .dst_reg = DST,         \
    .src_reg = 0,         \
    .off   = 0,         \
    .imm   = IMM })

#define BPF_MOV32_IMM(DST, IMM)         \
  ((struct bpf_insn) {          \
    .code  = BPF_ALU | BPF_MOV | BPF_K,   \
    .dst_reg = DST,         \
    .src_reg = 0,         \
    .off   = 0,         \
    .imm   = IMM })

/* BPF_LD_IMM64 macro encodes single 'load 64-bit immediate' insn */
#define BPF_LD_IMM64(DST, IMM)          \
  BPF_LD_IMM64_RAW(DST, 0, IMM)

#define BPF_LD_IMM64_RAW(DST, SRC, IMM)       \
  ((struct bpf_insn) {          \
    .code  = BPF_LD | BPF_DW | BPF_IMM,   \
    .dst_reg = DST,         \
    .src_reg = SRC,         \
    .off   = 0,         \
    .imm   = (__u32) (IMM) }),      \
  ((struct bpf_insn) {          \
    .code  = 0, /* zero is reserved opcode */ \
    .dst_reg = 0,         \
    .src_reg = 0,         \
    .off   = 0,         \
    .imm   = ((__u64) (IMM)) >> 32 })

#ifndef BPF_PSEUDO_MAP_FD
# define BPF_PSEUDO_MAP_FD  1
#endif

/* pseudo BPF_LD_IMM64 insn used to refer to process-local map_fd */
#define BPF_LD_MAP_FD(DST, MAP_FD)        \
  BPF_LD_IMM64_RAW(DST, BPF_PSEUDO_MAP_FD, MAP_FD)


/* Direct packet access, R0 = *(uint *) (skb->data + imm32) */

#define BPF_LD_ABS(SIZE, IMM)         \
  ((struct bpf_insn) {          \
    .code  = BPF_LD | BPF_SIZE(SIZE) | BPF_ABS, \
    .dst_reg = 0,         \
    .src_reg = 0,         \
    .off   = 0,         \
    .imm   = IMM })

/* Memory load, dst_reg = *(uint *) (src_reg + off16) */

#define BPF_LDX_MEM(SIZE, DST, SRC, OFF)      \
  ((struct bpf_insn) {          \
    .code  = BPF_LDX | BPF_SIZE(SIZE) | BPF_MEM,  \
    .dst_reg = DST,         \
    .src_reg = SRC,         \
    .off   = OFF,         \
    .imm   = 0 })

/* Memory store, *(uint *) (dst_reg + off16) = src_reg */

#define BPF_STX_MEM(SIZE, DST, SRC, OFF)      \
  ((struct bpf_insn) {          \
    .code  = BPF_STX | BPF_SIZE(SIZE) | BPF_MEM,  \
    .dst_reg = DST,         \
    .src_reg = SRC,         \
    .off   = OFF,         \
    .imm   = 0 })

/* Memory store, *(uint *) (dst_reg + off16) = imm32 */

#define BPF_ST_MEM(SIZE, DST, OFF, IMM)       \
  ((struct bpf_insn) {          \
    .code  = BPF_ST | BPF_SIZE(SIZE) | BPF_MEM, \
    .dst_reg = DST,         \
    .src_reg = 0,         \
    .off   = OFF,         \
    .imm   = IMM })

/* Conditional jumps against registers, if (dst_reg 'op' src_reg) goto pc + off16 */

#define BPF_JMP_REG(OP, DST, SRC, OFF)        \
  ((struct bpf_insn) {          \
    .code  = BPF_JMP | BPF_OP(OP) | BPF_X,    \
    .dst_reg = DST,         \
    .src_reg = SRC,         \
    .off   = OFF,         \
    .imm   = 0 })

/* Conditional jumps against immediates, if (dst_reg 'op' imm32) goto pc + off16 */

#define BPF_JMP_IMM(OP, DST, IMM, OFF)        \
  ((struct bpf_insn) {          \
    .code  = BPF_JMP | BPF_OP(OP) | BPF_K,    \
    .dst_reg = DST,         \
    .src_reg = 0,         \
    .off   = OFF,         \
    .imm   = IMM })

/* Raw code statement block */

#define BPF_RAW_INSN(CODE, DST, SRC, OFF, IMM)      \
  ((struct bpf_insn) {          \
    .code  = CODE,          \
    .dst_reg = DST,         \
    .src_reg = SRC,         \
    .off   = OFF,         \
    .imm   = IMM })

/* Program exit */

#define BPF_EXIT_INSN()           \
  ((struct bpf_insn) {          \
    .code  = BPF_JMP | BPF_EXIT,      \
    .dst_reg = 0,         \
    .src_reg = 0,         \
    .off   = 0,         \
    .imm   = 0 })

struct perf_event_attr;
int ebpf_perf_event_open(struct perf_event_attr *attr, int pid, int cpu,
        int group_fd, unsigned long flags);
#endif
