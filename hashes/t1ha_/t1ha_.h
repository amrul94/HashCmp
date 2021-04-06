#ifndef THESIS_WORK_T1HA__H
#define THESIS_WORK_T1HA__H

/* The little-endian variant for 32-bit CPU. */
uint64_t t1ha0_32le(const void *data, size_t length, uint64_t seed);
/* The big-endian variant for 32-bit CPU. */
uint64_t t1ha0_32be(const void *data, size_t length, uint64_t seed);

#endif //THESIS_WORK_T1HA__H
