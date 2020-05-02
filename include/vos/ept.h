//
// Created by x7cc on 2020/4/29.
//

#ifndef VOS_EPT_H
#define VOS_EPT_H

#include "vos/types.h"
#include "vos/assert.h"

typedef union
{
  // Table 28-1. Format of an EPT PML4 Entry (PML4E) that References an EPT Page-Directory-Pointer Table
  struct
  {
    /**
     * 0
     * Read access; indicates whether reads are allowed from the 512-GByte region controlled by this entry
     */
    uint64 read_access : 1;

    /**
     * 1
     * Write access; indicates whether writes are allowed from the 512-GByte region controlled by this entry
     */
    uint64 write_access : 1;

    /**
     * 2
     * If the “mode-based execute control for EPT” VM-execution control is 0, execute access; indicates whether instruction
     * fetches are allowed from the 512-GByte region controlled by this entry
     * If that control is 1, execute access for supervisor-mode linear addresses; indicates whether instruction fetches are
     * allowed from supervisor-mode linear addresses in the 512-GByte region controlled by this entry
     */
    uint64 execute_access : 1;

    /**
     * 7:3
     * Reserved (must be 0)
     */
    uint64 reversed : 5;

    /**
     * 8
     * If bit 6 of EPTP is 1, accessed flag for EPT; indicates whether software has accessed the 512-GByte region
     * controlled by this entry (see Section 28.2.4). Ignored if bit 6 of EPTP is 0
     */
    uint64 __ : 1;

    /**
     * 9
     * Ignored
     */
    uint64 ignored1 : 1;

    /**
     * 10
     * Execute access for user-mode linear addresses. If the “mode-based execute control for EPT” VM-execution control is
     * 1, indicates whether instruction fetches are allowed from user-mode linear addresses in the 512-GByte region
     * controlled by this entry. If that control is 0, this bit is ignored.
     */
    uint64 execute_access_for_user_mode_linear_address : 1;

    /**
     * 11
     * Ignored
     */
    uint64 ignored2 : 1;

    /**
     * (N–1):12
     * Physical address of 4-KByte aligned EPT page-directory-pointer table referenced by this entry
     */
    uint64 pdpt_page_PA : 36;

    /**
     * 51:N
     * Reserved (must be 0)
     */
    uint64 reverse2 : 4;

    /**
     * 63:52
     * Ignored
     */
    uint64 ignored4 : 12;
  };
  uint64 bits;
} ept_PML4E_t;

AssertCompileSize (ept_PML4E_t, 8);

typedef union
{
  // Table 28-3. Format of an EPT Page-Directory-Pointer-Table Entry (PDPTE) that References an EPT Page Directory
  struct
  {
    /**
     * 0
     * Read access; indicates whether reads are allowed from the 1-GByte region controlled by this entry
     */
    uint64 read_access : 1;

    /**
     * 1
     * Write access; indicates whether writes are allowed from the 1-GByte region controlled by this entry
     */
    uint64 write_access : 1;

    /**
     * 2
     * If the “mode-based execute control for EPT” VM-execution control is 0, execute access; indicates whether instruction
     * fetches are allowed from the 1-GByte region controlled by this entry
     * If that control is 1, execute access for supervisor-mode linear addresses; indicates whether instruction fetches are
     * allowed from supervisor-mode linear addresses in the 1-GByte region controlled by this entry
     */
    uint64 execute_access : 1;

    /**
     * 7:3
     * Reserved (must be 0)
     */
    uint64 reversed : 5;

    /**
     * 8
     * If bit 6 of EPTP is 1, accessed flag for EPT; indicates whether software has accessed the 1-GByte region controlled
     * by this entry (see Section 28.2.4). Ignored if bit 6 of EPTP is 0
     */
    uint64 __ : 1;

    /**
     * 9
     * Ignored
     */
    uint64 ignored1 : 1;

    /**
     * 10
     * Execute access for user-mode linear addresses. If the “mode-based execute control for EPT” VM-execution control is
     * 1, indicates whether instruction fetches are allowed from user-mode linear addresses in the 1-GByte region
     * controlled by this entry. If that control is 0, this bit is ignored.
     */
    uint64 execute_access_for_user_mode_linear_address : 1;

    /**
     * 11
     * Ignored
     */
    uint64 ignored2 : 1;

    /**
     * (N–1):12
     * Physical address of 4-KByte aligned EPT page directory referenced by this entry
     */
    uint64 pd_page_PA : 36;

    /**
     * 51:N
     * Reserved (must be 0)
     */
    uint64 reverse2 : 4;

    /**
     * 63:52
     * Ignored
     */
    uint64 ignored4 : 12;
  };
  uint64 bits;
} ept_PDPTE_t;

AssertCompileSize (ept_PDPTE_t, 8);

typedef union
{
  // Table 28-5. Format of an EPT Page-Directory Entry (PDE) that References an EPT Page Table
  struct
  {
    /**
     * 0
     * Read access; indicates whether reads are allowed from the 2-MByte region controlled by this entry
     */
    uint64 read_access : 1;

    /**
     * 1
     * Write access; indicates whether writes are allowed from the 2-MByte region controlled by this entry
     */
    uint64 write_access : 1;

    /**
     * 2
     * If the “mode-based execute control for EPT” VM-execution control is 0, execute access; indicates whether instruction
     * fetches are allowed from the 2-MByte region controlled by this entry
     * If that control is 1, execute access for supervisor-mode linear addresses; indicates whether instruction fetches are
     * allowed from supervisor-mode linear addresses in the 2-MByte region controlled by this entry
     */
    uint64 execute_access : 1;

    /**
     * 6:3
     * Reserved (must be 0)
     */
    uint64 reversed : 4;

    /**
     * 7
     * Must be 0 (otherwise, this entry maps a 2-MByte page)
     */
    uint64 must_be_zero : 1;

    /**
     * 8
     * If bit 6 of EPTP is 1, accessed flag for EPT; indicates whether software has accessed the 2-MByte region controlled
     * by this entry (see Section 28.2.4). Ignored if bit 6 of EPTP is 0
     */
    uint64 __ : 1;

    /**
     * 9
     * Ignored
     */
    uint64 ignored1 : 1;

    /**
     * 10
     * Execute access for user-mode linear addresses. If the “mode-based execute control for EPT” VM-execution control is
     * 1, indicates whether instruction fetches are allowed from user-mode linear addresses in the 2-MByte region
     * controlled by this entry. If that control is 0, this bit is ignored.
     */
    uint64 execute_access_for_user_mode_linear_address : 1;

    /**
     * 11
     * Ignored
     */
    uint64 ignored2 : 1;

    /**
     * (N–1):12
     * Physical address of 4-KByte aligned EPT page table referenced by this entry
     */
    uint64 pt_page_PA : 36;

    /**
     * 51:N
     * Reserved (must be 0)
     */
    uint64 reverse2 : 4;

    /**
     * 63:52
     * Ignored
     */
    uint64 ignored4 : 12;
  };
  uint64 bits;
} ept_PDE_t;

AssertCompileSize (ept_PDE_t, 8);

typedef union
{
  // Table 28-6. Format of an EPT Page-Table Entry that Maps a 4-KByte Page
  struct
  {
    /**
     * 0
     * Read access; indicates whether reads are allowed from the 4-KByte page referenced by this entry
     */
    uint64 read_access : 1;

    /**
     * 1
     * Write access; indicates whether writes are allowed from the 4-KByte page referenced by this entry
     */
    uint64 write_access : 1;

    /**
     * 2
     * If the “mode-based execute control for EPT” VM-execution control is 0, execute access; indicates whether
     * instruction fetches are allowed from the 4-KByte page controlled by this entry
     * If that control is 1, execute access for supervisor-mode linear addresses; indicates whether instruction fetches are
     * allowed from supervisor-mode linear addresses in the 4-KByte page controlled by this entry
     */
    uint64 execute_access : 1;

    /**
     * 5:3
     * EPT memory type for this 4-KByte page (see Section 28.2.6)
     */
    uint64 ept_memory_type : 3;

    /**
     * 6
     * Ignore PAT memory type for this 4-KByte page (see Section 28.2.6)
     */
    uint64 __ : 1;

    /**
     * 7
     * Ignored
     */
    uint64 ignored : 1;

    /**
     * 8
     * If bit 6 of EPTP is 1, accessed flag for EPT; indicates whether software has accessed the 4-KByte page referenced
     * by this entry (see Section 28.2.4). Ignored if bit 6 of EPTP is 0
     */
    uint64 ___ : 1;

    /**
     * 9
     * If bit 6 of EPTP is 1, dirty flag for EPT; indicates whether software has written to the 4-KByte page referenced by
     * this entry (see Section 28.2.4). Ignored if bit 6 of EPTP is 0
     */
    uint64 ignored1 : 1;

    /**
     * 10
     * Execute access for user-mode linear addresses. If the “mode-based execute control for EPT” VM-execution control is
     * 1, indicates whether instruction fetches are allowed from user-mode linear addresses in the 4-KByte page controlled
     * by this entry. If that control is 0, this bit is ignored.
     */
    uint64 execute_access_for_user_mode_linear_address : 1;

    /**
     * 11
     * Ignored
     */
    uint64 ignored2 : 1;

    /**
     * (N–1):12
     * Physical address of the 4-KByte page referenced by this entry1
     */
    uint64 page_PA : 36;

    /**
     * 51:N
     * Reserved (must be 0)
     */
    uint64 reverse2 : 4;

    /**
     * 62:52
     * Ignored
     */
    uint64 ignored4 : 11;

    /**
     * 63
     * Suppress #VE. If the “EPT-violation #VE” VM-execution control is 1, EPT violations caused by accesses to this page
     * are convertible to virtualization exceptions only if this bit is 0 (see Section 25.5.6.1). If “EPT-violation #VE” VMexecution
     * control is 0, this bit is ignored.
     */
    uint64 suppress : 1;
  };
  uint64 bits;
} ept_PTE_t;

AssertCompileSize (ept_PTE_t, 8);

/// See: Extended-Page-Table Pointer (EPTP)
typedef union
{
  struct
  {
    uint64 memory_type : 3;                     //!< [0:2]
    uint64 page_walk_length : 3;                //!< [3:5]
    uint64 enable_accessed_and_dirty_flags : 1; //!< [6]
    uint64 reserved1 : 5;                       //!< [7:11]
    uint64 pml4_address : 36;                   //!< [12:48-1]
    uint64 reserved2 : 16;                      //!< [48:63]
  };
  uint64 bits;
} EptPointer;
AssertCompileSize (EptPointer, 8);

uint setup_vmx_ept_pt (ept_PML4E_t* pml, uint host_PA, uint guest_PA);
uint make_vmx_ept (uint page_count);

#endif //VOS_EPT_H
