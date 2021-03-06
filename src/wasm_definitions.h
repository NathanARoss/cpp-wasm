typedef signed char i8;
typedef unsigned char u8;
typedef signed short i16;
typedef unsigned short u16;
typedef signed int i32;
typedef unsigned int u32;
typedef signed long long i64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

struct wasm
{
    static u32 varint(u8 *writePos, i32 value)
    {
        u8 *c = writePos;

        u8 byte;

        do
        {
            byte = value & 0x7F;
            value >>= 7;

            /* sign bit of byte is second high order bit (0x40) */
            if ((value != 0 && (byte & 0x40) == 0) || (value != -1 && (byte & 0x40)))
            {
                byte |= 0x80;
            }

            *c++ = byte;
        } while (byte & 0x80);

        return c - writePos;
    }

    static u32 varuint(u8 *writePos, u32 value)
    {
        u8 *c = writePos;

        do
        {
            u8 byte = value & 0x7F;
            value >>= 7;

            if (value != 0)
            { /* more bytes to come */
                byte |= 0x80;
            }

            *c++ = byte;
        } while (value != 0);

        return c - writePos;
    }

    struct section
    {
        enum
        {
            UserDefined,
            Type,
            Import,
            Function,
            Table,
            Memory,
            Global,
            Export,
            Start,
            Element,
            Code,
            Data,
        };
    };

    enum opcodes
    {
        unreachable,
        nop,
        block,
        loop,
        _if,
        _else,
        // undefined,
        // undefined,
        // undefined,
        // undefined,
        // undefined,
        end = _else + 6,
        br,
        br_if,
        br_table,
        _return,
        call, //0x10
        call_indirect,
        // undefined,
        // undefined,
        // undefined,
        // undefined,
        // undefined,
        // undefined,
        // undefined,
        // undefined,
        drop = call_indirect + 9,
        select,
        // undefined,
        // undefined,
        // undefined,
        // undefined,
        get_local = select + 5, //0x20
        set_local,
        tee_local,
        get_global,
        set_global,
        // undefined,
        // undefined,
        // undefined,
        i32_load = set_global + 4,
        i64_load,
        f32_load,
        f64_load,
        i32_load8_s,
        i32_load8_u,
        i32_load16_s,
        i32_load16_u,
        i64_load8_s, //0x30
        i64_load8_u,
        i64_load16_s,
        i64_load16_u,
        i64_load32_s,
        i64_load32_u,
        i32_store,
        i64_store,
        f32_store,
        f64_store,
        i32_store8,
        i32_store16,
        i64_store8,
        i64_store16,
        i64_store32,
        memory_size,
        memory_grow, //0x40
        i32_const,
        i64_const,
        f32_const,
        f64_const,
        i32_eqz,
        i32_eq,
        i32_ne,
        i32_lt_s,
        i32_lt_u,
        i32_gt_s,
        i32_gt_u,
        i32_le_s,
        i32_le_u,
        i32_ge_s,
        i32_ge_u,
        i64_eqz, //0x50
        i64_eq,
        i64_ne,
        i64_lt_s,
        i64_lt_u,
        i64_gt_s,
        i64_gt_u,
        i64_le_s,
        i64_le_u,
        i64_ge_s,
        i64_ge_u,
        f32_eq,
        f32_ne,
        f32_lt,
        f32_gt,
        f32_le,
        f32_ge, //0x60
        f64_eq,
        f64_ne,
        f64_lt,
        f64_gt,
        f64_le,
        f64_ge,
        i32_clz,
        i32_ctz,
        i32_popcnt,
        i32_add,
        i32_sub,
        i32_mul,
        i32_div_s,
        i32_div_u,
        i32_rem_s,
        i32_rem_u, //0x70
        i32_and,
        i32_or,
        i32_xor,
        i32_shl,
        i32_shr_s,
        i32_shr_u,
        i32_rotl,
        i32_rotr,
        i64_clz,
        i64_ctz,
        i64_popcnt,
        i64_add,
        i64_sub,
        i64_mul,
        i64_div_s,
        i64_div_u, //0x80
        i64_rem_s,
        i64_rem_u,
        i64_and,
        i64_or,
        i64_xor,
        i64_shl,
        i64_shr_s,
        i64_shr_u,
        i64_rotl,
        i64_rotr,
        f32_abs,
        f32_neg,
        f32_ceil,
        f32_floor,
        f32_trunc,
        f32_nearest, //0x90
        f32_sqrt,
        f32_add,
        f32_sub,
        f32_mul,
        f32_div,
        f32_min,
        f32_max,
        f32_copysign,
        f64_abs,
        f64_neg,
        f64_ceil,
        f64_floor,
        f64_trunc,
        f64_nearest,
        f64_sqrt,
        f64_add, //0xa0
        f64_sub,
        f64_mul,
        f64_div,
        f64_min,
        f64_max,
        f64_copysign,
        i32_wrap_from_i64,
        i32_trunc_s_from_f32,
        i32_trunc_u_from_f32,
        i32_trunc_s_from_f64,
        i32_trunc_u_from_f64,
        i64_extend_s_from_i32,
        i64_extend_u_from_i32,
        i64_trunc_s_from_f32,
        i64_trunc_u_from_f32,
        i64_trunc_s_from_f64, //0xb0
        i64_trunc_u_from_f64,
        f32_convert_s_from_i32,
        f32_convert_u_from_i32,
        f32_convert_s_from_i64,
        f32_convert_u_from_i64,
        f32_demote_from_f64,
        f64_convert_s_from_i32,
        f64_convert_u_from_i32,
        f64_convert_s_from_i64,
        f64_convert_u_from_i64,
        f64_promote_from_f32,
        i32_reinterpret_from_f32,
        i64_reinterpret_from_f64,
        f32_reinterpret_from_i32,
        f64_reinterpret_from_i64,
    };

    struct type
    {
        enum
        {
            i32 = 0x7F,
            i64 = 0x7E,
            f32 = 0x7D,
            f64 = 0x7C,
            anyFunc = 0x70,
            func = 0x60,
            _void = 0x40,
        };
    };

    struct external
    {
        enum kind
        {
            Function,
            Table,
            Memory,
            Global,
        };
    };
};