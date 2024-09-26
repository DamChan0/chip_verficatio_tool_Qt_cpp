#ifndef REGISTER_H
#define REGISTER_H
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

typedef struct IP_INFO
{
    uint32_t ip_name;
    uint8_t ip_version_patch;
    uint8_t ip_version_minor;
    uint8_t ip_version_major;
    uint8_t reserved;
} IP_INFO_t;

typedef struct INTERRUPT
{
    uint8_t status; // INTERRUPT_STATUS
    uint8_t mask;   // INTERRUPT_MASK
    uint8_t clear;  // INTERRUPT_CLEAR
} INTERRUPT_t;

typedef struct OP_SEQUENCE
{
    uint32_t op_start; // OP_START
    struct
    {
        uint32_t op_seq_0 : 5;
        uint32_t op_seq_1 : 5;
        uint32_t op_seq_2 : 5;
        uint32_t op_seq_3 : 5;
        uint32_t op_seq_4 : 5;
        uint32_t op_seq_5 : 5;
        uint32_t reserved : 2;
    } OP_SEQ_0;

    struct
    {
        uint32_t op_seq_6 : 5;
        uint32_t op_seq_7 : 5;
        uint32_t op_seq_8 : 5;
        uint32_t op_seq_9 : 5;
        uint32_t op_seq_10 : 5;
        uint32_t op_seq_11 : 5;
        uint32_t reserved : 2;
    } OP_SEQ_1;
} OP_SEQUENCE_t;
typedef struct ERRORS
{
    uint8_t op_error_code : 5;
    uint16_t pced_error_code : 12;
    uint8_t voxel_error_code : 2;
    uint16_t reserved : 13;
} ERRORS_t;

typedef struct PC_INFO
{
    struct
    {
        uint32_t width : 24; // Width or length of the point cloud data
        uint32_t reserved : 8;
    } info_1;
    struct
    {
        uint16_t height;        // Height of the point cloud data
        uint8_t point_type : 1; // Point type: 0 = PC_XYZI (Cartesian),
                                // 1 = PC_SPHERE (Spherical)
        uint8_t reserved1 : 3;
        uint8_t
            pos_ang_seq : 4; // Position and angle sequence (0-13, see coordinate alignment table)
        uint8_t pos_ang_type : 3; // Position and angle type:
                                  // 0: P_SF16 (float16 standard)
                                  // 1: P_BF16 (bfloat16)
                                  // 2: P_NF16 (nfloat16, <E6,M9>)
                                  // 3: P_SF32 (float32 standard)
                                  // 4: P_FX24_10 (fixed24, <M24,110>)
                                  // 5: P_IS32 (int32, scaling)
        uint8_t reserved2 : 1;
        uint8_t intens_type : 3; // Intensity type:
                                 // 0: I_SF16 (float16 standard)
                                 // 1: I_BF16 (bfloat16)
                                 // 2: I_NF16 (nfloat16, <E6,M9>)
                                 // 3: I_SF32 (float32 standard)
                                 // 4: I_UI16 (uint16)
                                 // 5: I_UI32 (uint32, scaling)
        uint8_t reserved3 : 1;
    } info_2;

    struct
    {
        uint32_t address_l; // Lower 32 bits of the address for point cloud data
    } info_3;
    struct
    {
        uint8_t address_h : 6; // Higher 6 bits of the address for point cloud data
        uint32_t reserved : 26;
    } info_4;
    struct
    {
        uint32_t capacity; // Capacity (total number of points) in the point cloud data
    } info_5;
} PC_INFO_t;

typedef struct PC_CONCAT_INFO
{
    struct
    {
        uint32_t width : 24; // width or length of pc_concat
        uint32_t reserved : 8;
    } info_1;

    struct
    {
        uint16_t height;        // height of pc_concat
        uint8_t point_type : 1; // point_type of pc_concat
        uint8_t reserved1 : 3;
        uint8_t pos_ang_seq : 4;  // position and angle sequence of pc_concat
        uint8_t pos_ang_type : 3; // position and angle type of pc_concat
        uint8_t reserved2 : 1;
        uint8_t intens_type : 3; // intensity type of pc_concat
        uint8_t reserved3 : 1;
    } info_2;

    uint32_t address_l; // lower address of pc_concat

    struct
    {
        uint8_t address_h : 6; // higher address of pc_concat
        uint32_t reserved : 26;
    } info_4;

    uint32_t capacity; // capacity of pc_concat
} PC_CONCAT_INFO_t;

typedef struct PC_SOR_DATA_INFO
{
    struct
    {
        uint32_t width : 24; // width or length of pc_sor_data
        uint32_t reserved : 8;
    } info_1;
    struct
    {
        uint16_t height;        // height of pc_sor_data
        uint8_t point_type : 1; // point_type of pc_sor_data
        uint8_t reserved1 : 3;
        uint8_t pos_ang_seq : 4;  // position and angle sequence of pc_sor_data
        uint8_t pos_ang_type : 3; // position and angle type of pc_sor_data
        uint8_t reserved2 : 1;
        uint8_t intens_type : 3; // intensity type of pc_sor_data
        uint8_t reserved3 : 1;
    } info_2;

    uint32_t address_l; // lower address of pc_sor_data
    struct
    {
        uint8_t address_h : 6; // higher address of pc_sor_data
        uint32_t reserved : 26;
    } info_4;

    uint32_t capacity; // capacity of pc_sor_data
} PC_SOR_DATA_INFO_t;

typedef struct DMA_PARAMS_1
{
    uint8_t dma_num_pc_src : 3; // number of pc_src
    uint16_t reserved1 : 13;
    uint8_t dma_chk_cmd_err_en : 1; // command error checking function enable
    uint8_t reserved2 : 7;
    uint8_t dma_sel_inval_mark : 3; // selecting how to mark invalid point for floating point types
    uint8_t reserved3 : 5;
} DMA_PARAMS_1_t;

typedef struct PC_SRC_SCALING_AND_OFFSET
{
    struct
    {
        uint32_t azm_scl_fac : 17; // Scale factor for azimuth angle [U1F16]
        uint32_t reserved : 15;
    } azm_scl_fac;
    struct
    {
        uint32_t elv_scl_fac : 17; // Scale factor for elevation angle [U1F16]
        uint32_t reserved : 15;
    } elv_scl_fac;
    struct
    {
        uint32_t pos_scl_fac : 17; // Scale factor for position or range [U1F16]
        uint32_t reserved : 15;
    } pos_scl_fac;
    struct
    {
        uint32_t inten_scl_fac : 17; // Scale factor for intensity [U1F16]
        uint32_t reserved : 15;
    } inten_scl_fac;
    uint32_t azm_offset; // Offset for azimuth angle [U32]
    uint32_t elv_offset; // Offset for elevation angle [U32]
    uint32_t pos_offset; // Offset for position [U32]
} PC_SRC_SCALING_AND_OFFSET_t;

typedef struct DMA_MONITORING
{
    struct
    {
        uint32_t dma_rd_pcnt : 24;     // Number of read points
        uint8_t dma_sel_radsor : 1;    // sel_radsor signal
        uint8_t dma_pc_src_id : 3;     // pc_src_id signal
        uint8_t dma_sordat_rphase : 1; // SORdata reading phase indicator
        uint8_t dma_rd_run : 1;        // Running signal of reading operation
        uint8_t reserved : 2;
    } read_state_mon;

    uint32_t wreq_addr_l; // Lower 32 bits of write request address

    struct
    {
        uint16_t dma_wreq_addr_h : 6; // Higher 6 bits of write request address
        uint16_t reserved : 10;
        uint16_t dma_wreq_size; // Size of write request
    } wreq_mon_h;

    struct
    {
        uint32_t dma_wr_pcnt : 24;     // Number of written points
        uint8_t dma_wstate : 3;        // Write state signal
        uint8_t dma_sordat_wphase : 1; // SORdata writing phase indicator
        uint8_t dma_wr_run : 1;        // Running signal of writing operation
        uint8_t reserved : 3;
    } write_state_mon;

    struct
    {
        uint16_t dma_wreq_cnt;  // Number of writing requests
        uint16_t dma_wresp_cnt; // Number of writing responses
    } wresp_state_mon;

    struct
    {
        uint8_t sync_sel_interrupt_mode : 1; // Interrupt mode selection , 1 is sync interrupt
        uint32_t reserved : 31;
    } sync_sel_mode;
} DMA_MONITORING_t;

#endif // REGISTER_H