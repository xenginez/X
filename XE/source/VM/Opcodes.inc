#define CONTROL_OPCODES(visit_op)                                                \
	visit_op(0x0002, block              , "block"                            )   \
	visit_op(0x0003, loop               , "loop"                             )   \
	visit_op(0x0004, if_                , "if"                               )   \
	visit_op(0x0005, else_              , "else"                             )   \
	visit_op(0x000b, end                , "end"                              )   \
	visit_op(0xfb02, try_               , "try"                              )   \
	visit_op(0xfb03, catch_             , "catch"                            )   \
	visit_op(0xfb04, catch_all          , "catch_all"                        )

#define PARAMETRIC_OPCODES(visit_op)                                             \
/* Control flow                                                               */ \
	visit_op(0x0000, unreachable        , "unreachable"                      )   \
	visit_op(0x000c, br                 , "br"                               )   \
	visit_op(0x000d, br_if              , "br_if"                            )   \
	visit_op(0x000e, br_table           , "br_table"                         )   \
	visit_op(0x000f, return_            , "return"                           )   \
	visit_op(0x0010, call               , "call"                             )   \
	visit_op(0x0011, call_indirect      , "call_indirect"                    )   \
/* Stack manipulation                                                         */ \
	visit_op(0x001a, drop               , "drop"                             )   \
/* Variables                                                                  */ \
	visit_op(0x0020, local_get          , "local.get"                        )   \
	visit_op(0x0021, local_set          , "local.set"                        )   \
	visit_op(0x0022, local_tee          , "local.tee"                        )   \
	visit_op(0x0023, global_get         , "global.get"                       )   \
	visit_op(0x0024, global_set         , "global.set"                       )   \
/* Table access                                                               */ \
	visit_op(0x0025, table_get          , "table.get"                        )   \
	visit_op(0x0026, table_set          , "table.set"                        )   \
	visit_op(0xfc0f, table_grow         , "table.grow"                       )   \
	visit_op(0xfc11, table_fill         , "table.fill"                       )   \
/* Exceptions                                                                 */ \
	visit_op(0xfb00, throw_             , "throw"                            )   \
	visit_op(0xfb01, rethrow            , "rethrow"                          )   \
/* References                                                                 */ \
	visit_op(0x00d0, ref_null           , "ref.null"                         )   \
	visit_op(0x00d1, ref_is_null        , "ref.is_null"                      )

#define OVERLOADED_OPCODES(visit_op)                                             \
/*  visit_op(0x001b, select             , "select"                           )*/ \
	visit_op(0x001c, select             , "select"                           )

#define INDEX_POLYMORPHIC_OPCODES(visit_op)                                      \
/* Scalar load/store instructions                                             */ \
	visit_op(0x0028, i32_load                  , "i32.load"                  )   \
	visit_op(0x0029, i64_load                  , "i64.load"                  )   \
	visit_op(0x002a, f32_load                  , "f32.load"                  )   \
	visit_op(0x002b, f64_load                  , "f64.load"                  )   \
	visit_op(0x002c, i32_load8_s               , "i32.load8_s"               )   \
	visit_op(0x002d, i32_load8_u               , "i32.load8_u"               )   \
	visit_op(0x002e, i32_load16_s              , "i32.load16_s"              )   \
	visit_op(0x002f, i32_load16_u              , "i32.load16_u"              )   \
	visit_op(0x0030, i64_load8_s               , "i64.load8_s"               )   \
	visit_op(0x0031, i64_load8_u               , "i64.load8_u"               )   \
	visit_op(0x0032, i64_load16_s              , "i64.load16_s"              )   \
	visit_op(0x0033, i64_load16_u              , "i64.load16_u"              )   \
	visit_op(0x0034, i64_load32_s              , "i64.load32_s"              )   \
	visit_op(0x0035, i64_load32_u              , "i64.load32_u"              )   \
	visit_op(0x0036, i32_store                 , "i32.store"                 )   \
	visit_op(0x0037, i64_store                 , "i64.store"                 )   \
	visit_op(0x0038, f32_store                 , "f32.store"                 )   \
	visit_op(0x0039, f64_store                 , "f64.store"                 )   \
	visit_op(0x003a, i32_store8                , "i32.store8"                )   \
	visit_op(0x003b, i32_store16               , "i32.store16"               )   \
	visit_op(0x003c, i64_store8                , "i64.store8"                )   \
	visit_op(0x003d, i64_store16               , "i64.store16"               )   \
	visit_op(0x003e, i64_store32               , "i64.store32"               )   \
/* Memory size                                                                */ \
	visit_op(0x003f, memory_size               , "memory.size"               )   \
	visit_op(0x0040, memory_grow               , "memory.grow"               )   \
/* Bulk memory/table operators                                                */ \
	visit_op(0xfc08, memory_init               , "memory.init"               )   \
	visit_op(0xfc0a, memory_copy               , "memory.copy"               )   \
	visit_op(0xfc0b, memory_fill               , "memory.fill"               )   \
	visit_op(0xfc0c, table_init                , "table.init"                )   \
	visit_op(0xfc0e, table_copy                , "table.copy"                )   \
	visit_op(0xfc10, table_size                , "table.size"                )   \
/* v128 load/store                                                            */ \
	visit_op(0xfd00, v128_load                 , "v128.load"                 )   \
	visit_op(0xfd01, v128_load8x8_s            , "v128.load8x8_s"            )   \
	visit_op(0xfd02, v128_load8x8_u            , "v128.load8x8_u"            )   \
	visit_op(0xfd03, v128_load16x4_s           , "v128.load16x4_s"           )   \
	visit_op(0xfd04, v128_load16x4_u           , "v128.load16x4_u"           )   \
	visit_op(0xfd05, v128_load32x2_s           , "v128.load32x2_s"           )   \
	visit_op(0xfd06, v128_load32x2_u           , "v128.load32x2_u"           )   \
	visit_op(0xfd07, v128_load8_splat          , "v128.load8_splat"          )   \
	visit_op(0xfd08, v128_load16_splat         , "v128.load16_splat"         )   \
	visit_op(0xfd09, v128_load32_splat         , "v128.load32_splat"         )   \
	visit_op(0xfd0a, v128_load64_splat         , "v128.load64_splat"         )   \
	visit_op(0xfd0b, v128_store                , "v128.store"                )   \
	visit_op(0xfd54, v128_load8_lane           , "v128.load8_lane"           )   \
	visit_op(0xfd55, v128_load16_lane          , "v128.load16_lane"          )   \
	visit_op(0xfd56, v128_load32_lane          , "v128.load32_lane"          )   \
	visit_op(0xfd57, v128_load64_lane          , "v128.load64_lane"          )   \
	visit_op(0xfd58, v128_store8_lane          , "v128.store8_lane"          )   \
	visit_op(0xfd59, v128_store16_lane         , "v128.store16_lane"         )   \
	visit_op(0xfd5a, v128_store32_lane         , "v128.store32_lane"         )   \
	visit_op(0xfd5b, v128_store64_lane         , "v128.store64_lane"         )   \
	visit_op(0xfd5c, v128_load32_zero          , "v128.load32_zero"          )   \
	visit_op(0xfd5d, v128_load64_zero          , "v128.load64_zero"          )   \
/* v128 interleaved load/store instructions                                   */ \
	visit_op(0xff00, v8x16_load_interleaved_2  , "v8x16.load_interleaved_2"  )   \
	visit_op(0xff01, v8x16_load_interleaved_3  , "v8x16.load_interleaved_3"  )   \
	visit_op(0xff02, v8x16_load_interleaved_4  , "v8x16.load_interleaved_4"  )   \
	visit_op(0xff03, v16x8_load_interleaved_2  , "v16x8.load_interleaved_2"  )   \
	visit_op(0xff04, v16x8_load_interleaved_3  , "v16x8.load_interleaved_3"  )   \
	visit_op(0xff05, v16x8_load_interleaved_4  , "v16x8.load_interleaved_4"  )   \
	visit_op(0xff06, v32x4_load_interleaved_2  , "v32x4.load_interleaved_2"  )   \
	visit_op(0xff07, v32x4_load_interleaved_3  , "v32x4.load_interleaved_3"  )   \
	visit_op(0xff08, v32x4_load_interleaved_4  , "v32x4.load_interleaved_4"  )   \
	visit_op(0xff09, v64x2_load_interleaved_2  , "v64x2.load_interleaved_2"  )   \
	visit_op(0xff0a, v64x2_load_interleaved_3  , "v64x2.load_interleaved_3"  )   \
	visit_op(0xff0b, v64x2_load_interleaved_4  , "v64x2.load_interleaved_4"  )   \
	visit_op(0xff0c, v8x16_store_interleaved_2 , "v8x16.store_interleaved_2" )   \
	visit_op(0xff0d, v8x16_store_interleaved_3 , "v8x16.store_interleaved_3" )   \
	visit_op(0xff0e, v8x16_store_interleaved_4 , "v8x16.store_interleaved_4" )   \
	visit_op(0xff0f, v16x8_store_interleaved_2 , "v16x8.store_interleaved_2" )   \
	visit_op(0xff10, v16x8_store_interleaved_3 , "v16x8.store_interleaved_3" )   \
	visit_op(0xff11, v16x8_store_interleaved_4 , "v16x8.store_interleaved_4" )   \
	visit_op(0xff12, v32x4_store_interleaved_2 , "v32x4.store_interleaved_2" )   \
	visit_op(0xff13, v32x4_store_interleaved_3 , "v32x4.store_interleaved_3" )   \
	visit_op(0xff14, v32x4_store_interleaved_4 , "v32x4.store_interleaved_4" )   \
	visit_op(0xff15, v64x2_store_interleaved_2 , "v64x2.store_interleaved_2" )   \
	visit_op(0xff16, v64x2_store_interleaved_3 , "v64x2.store_interleaved_3" )   \
	visit_op(0xff17, v64x2_store_interleaved_4 , "v64x2.store_interleaved_4" )   \
/* Atomic wait/wake                                                           */ \
	visit_op(0xfe00, memory_atomic_notify      , "memory.atomic.notify"      )   \
	visit_op(0xfe01, memory_atomic_wait32      , "memory.atomic.wait32"      )   \
	visit_op(0xfe02, memory_atomic_wait64      , "memory.atomic.wait64"      )   \
/* Atomic load/store                                                          */ \
	visit_op(0xfe10, i32_atomic_load           , "i32.atomic.load"           )   \
	visit_op(0xfe11, i64_atomic_load           , "i64.atomic.load"           )   \
	visit_op(0xfe12, i32_atomic_load8_u        , "i32.atomic.load8_u"        )   \
	visit_op(0xfe13, i32_atomic_load16_u       , "i32.atomic.load16_u"       )   \
	visit_op(0xfe14, i64_atomic_load8_u        , "i64.atomic.load8_u"        )   \
	visit_op(0xfe15, i64_atomic_load16_u       , "i64.atomic.load16_u"       )   \
	visit_op(0xfe16, i64_atomic_load32_u       , "i64.atomic.load32_u"       )   \
	visit_op(0xfe17, i32_atomic_store          , "i32.atomic.store"          )   \
	visit_op(0xfe18, i64_atomic_store          , "i64.atomic.store"          )   \
	visit_op(0xfe19, i32_atomic_store8         , "i32.atomic.store8"         )   \
	visit_op(0xfe1a, i32_atomic_store16        , "i32.atomic.store16"        )   \
	visit_op(0xfe1b, i64_atomic_store8         , "i64.atomic.store8"         )   \
	visit_op(0xfe1c, i64_atomic_store16        , "i64.atomic.store16"        )   \
	visit_op(0xfe1d, i64_atomic_store32        , "i64.atomic.store32"        )   \
/* Atomic read-modify-write                                                   */ \
	visit_op(0xfe1e, i32_atomic_rmw_add        , "i32.atomic.rmw.add"        )   \
	visit_op(0xfe1f, i64_atomic_rmw_add        , "i64.atomic.rmw.add"        )   \
	visit_op(0xfe20, i32_atomic_rmw8_add_u     , "i32.atomic.rmw8.add_u"     )   \
	visit_op(0xfe21, i32_atomic_rmw16_add_u    , "i32.atomic.rmw16.add_u"    )   \
	visit_op(0xfe22, i64_atomic_rmw8_add_u     , "i64.atomic.rmw8.add_u"     )   \
	visit_op(0xfe23, i64_atomic_rmw16_add_u    , "i64.atomic.rmw16.add_u"    )   \
	visit_op(0xfe24, i64_atomic_rmw32_add_u    , "i64.atomic.rmw32.add_u"    )   \
	visit_op(0xfe25, i32_atomic_rmw_sub        , "i32.atomic.rmw.sub"        )   \
	visit_op(0xfe26, i64_atomic_rmw_sub        , "i64.atomic.rmw.sub"        )   \
	visit_op(0xfe27, i32_atomic_rmw8_sub_u     , "i32.atomic.rmw8.sub_u"     )   \
	visit_op(0xfe28, i32_atomic_rmw16_sub_u    , "i32.atomic.rmw16.sub_u"    )   \
	visit_op(0xfe29, i64_atomic_rmw8_sub_u     , "i64.atomic.rmw8.sub_u"     )   \
	visit_op(0xfe2a, i64_atomic_rmw16_sub_u    , "i64.atomic.rmw16.sub_u"    )   \
	visit_op(0xfe2b, i64_atomic_rmw32_sub_u    , "i64.atomic.rmw32.sub_u"    )   \
	visit_op(0xfe2c, i32_atomic_rmw_and        , "i32.atomic.rmw.and"        )   \
	visit_op(0xfe2d, i64_atomic_rmw_and        , "i64.atomic.rmw.and"        )   \
	visit_op(0xfe2e, i32_atomic_rmw8_and_u     , "i32.atomic.rmw8.and_u"     )   \
	visit_op(0xfe2f, i32_atomic_rmw16_and_u    , "i32.atomic.rmw16.and_u"    )   \
	visit_op(0xfe30, i64_atomic_rmw8_and_u     , "i64.atomic.rmw8.and_u"     )   \
	visit_op(0xfe31, i64_atomic_rmw16_and_u    , "i64.atomic.rmw16.and_u"    )   \
	visit_op(0xfe32, i64_atomic_rmw32_and_u    , "i64.atomic.rmw32.and_u"    )   \
	visit_op(0xfe33, i32_atomic_rmw_or         , "i32.atomic.rmw.or"         )   \
	visit_op(0xfe34, i64_atomic_rmw_or         , "i64.atomic.rmw.or"         )   \
	visit_op(0xfe35, i32_atomic_rmw8_or_u      , "i32.atomic.rmw8.or_u"      )   \
	visit_op(0xfe36, i32_atomic_rmw16_or_u     , "i32.atomic.rmw16.or_u"     )   \
	visit_op(0xfe37, i64_atomic_rmw8_or_u      , "i64.atomic.rmw8.or_u"      )   \
	visit_op(0xfe38, i64_atomic_rmw16_or_u     , "i64.atomic.rmw16.or_u"     )   \
	visit_op(0xfe39, i64_atomic_rmw32_or_u     , "i64.atomic.rmw32.or_u"     )   \
	visit_op(0xfe3a, i32_atomic_rmw_xor        , "i32.atomic.rmw.xor"        )   \
	visit_op(0xfe3b, i64_atomic_rmw_xor        , "i64.atomic.rmw.xor"        )   \
	visit_op(0xfe3c, i32_atomic_rmw8_xor_u     , "i32.atomic.rmw8.xor_u"     )   \
	visit_op(0xfe3d, i32_atomic_rmw16_xor_u    , "i32.atomic.rmw16.xor_u"    )   \
	visit_op(0xfe3e, i64_atomic_rmw8_xor_u     , "i64.atomic.rmw8.xor_u"     )   \
	visit_op(0xfe3f, i64_atomic_rmw16_xor_u    , "i64.atomic.rmw16.xor_u"    )   \
	visit_op(0xfe40, i64_atomic_rmw32_xor_u    , "i64.atomic.rmw32.xor_u"    )   \
	visit_op(0xfe41, i32_atomic_rmw_xchg       , "i32.atomic.rmw.xchg"       )   \
	visit_op(0xfe42, i64_atomic_rmw_xchg       , "i64.atomic.rmw.xchg"       )   \
	visit_op(0xfe43, i32_atomic_rmw8_xchg_u    , "i32.atomic.rmw8.xchg_u"    )   \
	visit_op(0xfe44, i32_atomic_rmw16_xchg_u   , "i32.atomic.rmw16.xchg_u"   )   \
	visit_op(0xfe45, i64_atomic_rmw8_xchg_u    , "i64.atomic.rmw8.xchg_u"    )   \
	visit_op(0xfe46, i64_atomic_rmw16_xchg_u   , "i64.atomic.rmw16.xchg_u"   )   \
	visit_op(0xfe47, i64_atomic_rmw32_xchg_u   , "i64.atomic.rmw32.xchg_u"   )   \
	visit_op(0xfe48, i32_atomic_rmw_cmpxchg    , "i32.atomic.rmw.cmpxchg"    )   \
	visit_op(0xfe49, i64_atomic_rmw_cmpxchg    , "i64.atomic.rmw.cmpxchg"    )   \
	visit_op(0xfe4a, i32_atomic_rmw8_cmpxchg_u , "i32.atomic.rmw8.cmpxchg_u" )   \
	visit_op(0xfe4b, i32_atomic_rmw16_cmpxchg_u, "i32.atomic.rmw16.cmpxchg_u")   \
	visit_op(0xfe4c, i64_atomic_rmw8_cmpxchg_u , "i64.atomic.rmw8.cmpxchg_u" )   \
	visit_op(0xfe4d, i64_atomic_rmw16_cmpxchg_u, "i64.atomic.rmw16.cmpxchg_u")   \
	visit_op(0xfe4e, i64_atomic_rmw32_cmpxchg_u, "i64.atomic.rmw32.cmpxchg_u")

#define NONCONTROL_NONPARAMETRIC_OPCODES(visit_op)                                       \
	visit_op(0x0001, nop                           , "nop"                           )   \
/* Literals                                                                           */ \
	visit_op(0x0041, i32_const                     , "i32.const"                     )   \
	visit_op(0x0042, i64_const                     , "i64.const"                     )   \
	visit_op(0x0043, f32_const                     , "f32.const"                     )   \
	visit_op(0x0044, f64_const                     , "f64.const"                     )   \
/* Comparisons                                                                        */ \
	visit_op(0x0045, i32_eqz                       , "i32.eqz"                       )   \
	visit_op(0x0046, i32_eq                        , "i32.eq"                        )   \
	visit_op(0x0047, i32_ne                        , "i32.ne"                        )   \
	visit_op(0x0048, i32_lt_s                      , "i32.lt_s"                      )   \
	visit_op(0x0049, i32_lt_u                      , "i32.lt_u"                      )   \
	visit_op(0x004a, i32_gt_s                      , "i32.gt_s"                      )   \
	visit_op(0x004b, i32_gt_u                      , "i32.gt_u"                      )   \
	visit_op(0x004c, i32_le_s                      , "i32.le_s"                      )   \
	visit_op(0x004d, i32_le_u                      , "i32.le_u"                      )   \
	visit_op(0x004e, i32_ge_s                      , "i32.ge_s"                      )   \
	visit_op(0x004f, i32_ge_u                      , "i32.ge_u"                      )   \
	visit_op(0x0050, i64_eqz                       , "i64.eqz"                       )   \
	visit_op(0x0051, i64_eq                        , "i64.eq"                        )   \
	visit_op(0x0052, i64_ne                        , "i64.ne"                        )   \
	visit_op(0x0053, i64_lt_s                      , "i64.lt_s"                      )   \
	visit_op(0x0054, i64_lt_u                      , "i64.lt_u"                      )   \
	visit_op(0x0055, i64_gt_s                      , "i64.gt_s"                      )   \
	visit_op(0x0056, i64_gt_u                      , "i64.gt_u"                      )   \
	visit_op(0x0057, i64_le_s                      , "i64.le_s"                      )   \
	visit_op(0x0058, i64_le_u                      , "i64.le_u"                      )   \
	visit_op(0x0059, i64_ge_s                      , "i64.ge_s"                      )   \
	visit_op(0x005a, i64_ge_u                      , "i64.ge_u"                      )   \
	visit_op(0x005b, f32_eq                        , "f32.eq"                        )   \
	visit_op(0x005c, f32_ne                        , "f32.ne"                        )   \
	visit_op(0x005d, f32_lt                        , "f32.lt"                        )   \
	visit_op(0x005e, f32_gt                        , "f32.gt"                        )   \
	visit_op(0x005f, f32_le                        , "f32.le"                        )   \
	visit_op(0x0060, f32_ge                        , "f32.ge"                        )   \
	visit_op(0x0061, f64_eq                        , "f64.eq"                        )   \
	visit_op(0x0062, f64_ne                        , "f64.ne"                        )   \
	visit_op(0x0063, f64_lt                        , "f64.lt"                        )   \
	visit_op(0x0064, f64_gt                        , "f64.gt"                        )   \
	visit_op(0x0065, f64_le                        , "f64.le"                        )   \
	visit_op(0x0066, f64_ge                        , "f64.ge"                        )   \
/* i32 arithmetic                                                                     */ \
	visit_op(0x0067, i32_clz                       , "i32.clz"                       )   \
	visit_op(0x0068, i32_ctz                       , "i32.ctz"                       )   \
	visit_op(0x0069, i32_popcnt                    , "i32.popcnt"                    )   \
	visit_op(0x006a, i32_add                       , "i32.add"                       )   \
	visit_op(0x006b, i32_sub                       , "i32.sub"                       )   \
	visit_op(0x006c, i32_mul                       , "i32.mul"                       )   \
	visit_op(0x006d, i32_div_s                     , "i32.div_s"                     )   \
	visit_op(0x006e, i32_div_u                     , "i32.div_u"                     )   \
	visit_op(0x006f, i32_rem_s                     , "i32.rem_s"                     )   \
	visit_op(0x0070, i32_rem_u                     , "i32.rem_u"                     )   \
	visit_op(0x0071, i32_and_                      , "i32.and"                       )   \
	visit_op(0x0072, i32_or_                       , "i32.or"                        )   \
	visit_op(0x0073, i32_xor_                      , "i32.xor"                       )   \
	visit_op(0x0074, i32_shl                       , "i32.shl"                       )   \
	visit_op(0x0075, i32_shr_s                     , "i32.shr_s"                     )   \
	visit_op(0x0076, i32_shr_u                     , "i32.shr_u"                     )   \
	visit_op(0x0077, i32_rotl                      , "i32.rotl"                      )   \
	visit_op(0x0078, i32_rotr                      , "i32.rotr"                      )   \
/* i64 arithmetic                                                                     */ \
	visit_op(0x0079, i64_clz                       , "i64.clz"                       )   \
	visit_op(0x007a, i64_ctz                       , "i64.ctz"                       )   \
	visit_op(0x007b, i64_popcnt                    , "i64.popcnt"                    )   \
	visit_op(0x007c, i64_add                       , "i64.add"                       )   \
	visit_op(0x007d, i64_sub                       , "i64.sub"                       )   \
	visit_op(0x007e, i64_mul                       , "i64.mul"                       )   \
	visit_op(0x007f, i64_div_s                     , "i64.div_s"                     )   \
	visit_op(0x0080, i64_div_u                     , "i64.div_u"                     )   \
	visit_op(0x0081, i64_rem_s                     , "i64.rem_s"                     )   \
	visit_op(0x0082, i64_rem_u                     , "i64.rem_u"                     )   \
	visit_op(0x0083, i64_and_                      , "i64.and"                       )   \
	visit_op(0x0084, i64_or_                       , "i64.or"                        )   \
	visit_op(0x0085, i64_xor_                      , "i64.xor"                       )   \
	visit_op(0x0086, i64_shl                       , "i64.shl"                       )   \
	visit_op(0x0087, i64_shr_s                     , "i64.shr_s"                     )   \
	visit_op(0x0088, i64_shr_u                     , "i64.shr_u"                     )   \
	visit_op(0x0089, i64_rotl                      , "i64.rotl"                      )   \
	visit_op(0x008a, i64_rotr                      , "i64.rotr"                      )   \
/* f32 arithmetic                                                                     */ \
	visit_op(0x008b, f32_abs                       , "f32.abs"                       )   \
	visit_op(0x008c, f32_neg                       , "f32.neg"                       )   \
	visit_op(0x008d, f32_ceil                      , "f32.ceil"                      )   \
	visit_op(0x008e, f32_floor                     , "f32.floor"                     )   \
	visit_op(0x008f, f32_trunc                     , "f32.trunc"                     )   \
	visit_op(0x0090, f32_nearest                   , "f32.nearest"                   )   \
	visit_op(0x0091, f32_sqrt                      , "f32.sqrt"                      )   \
	visit_op(0x0092, f32_add                       , "f32.add"                       )   \
	visit_op(0x0093, f32_sub                       , "f32.sub"                       )   \
	visit_op(0x0094, f32_mul                       , "f32.mul"                       )   \
	visit_op(0x0095, f32_div                       , "f32.div"                       )   \
	visit_op(0x0096, f32_min                       , "f32.min"                       )   \
	visit_op(0x0097, f32_max                       , "f32.max"                       )   \
	visit_op(0x0098, f32_copysign                  , "f32.copysign"                  )   \
/* f64 arithmetic                                                                     */ \
	visit_op(0x0099, f64_abs                       , "f64.abs"                       )   \
	visit_op(0x009a, f64_neg                       , "f64.neg"                       )   \
	visit_op(0x009b, f64_ceil                      , "f64.ceil"                      )   \
	visit_op(0x009c, f64_floor                     , "f64.floor"                     )   \
	visit_op(0x009d, f64_trunc                     , "f64.trunc"                     )   \
	visit_op(0x009e, f64_nearest                   , "f64.nearest"                   )   \
	visit_op(0x009f, f64_sqrt                      , "f64.sqrt"                      )   \
	visit_op(0x00a0, f64_add                       , "f64.add"                       )   \
	visit_op(0x00a1, f64_sub                       , "f64.sub"                       )   \
	visit_op(0x00a2, f64_mul                       , "f64.mul"                       )   \
	visit_op(0x00a3, f64_div                       , "f64.div"                       )   \
	visit_op(0x00a4, f64_min                       , "f64.min"                       )   \
	visit_op(0x00a5, f64_max                       , "f64.max"                       )   \
	visit_op(0x00a6, f64_copysign                  , "f64.copysign"                  )   \
/* Conversions                                                                        */ \
	visit_op(0x00a7, i32_wrap_i64                  , "i32.wrap_i64"                  )   \
	visit_op(0x00a8, i32_trunc_f32_s               , "i32.trunc_f32_s"               )   \
	visit_op(0x00a9, i32_trunc_f32_u               , "i32.trunc_f32_u"               )   \
	visit_op(0x00aa, i32_trunc_f64_s               , "i32.trunc_f64_s"               )   \
	visit_op(0x00ab, i32_trunc_f64_u               , "i32.trunc_f64_u"               )   \
	visit_op(0x00ac, i64_extend_i32_s              , "i64.extend_i32_s"              )   \
	visit_op(0x00ad, i64_extend_i32_u              , "i64.extend_i32_u"              )   \
	visit_op(0x00ae, i64_trunc_f32_s               , "i64.trunc_f32_s"               )   \
	visit_op(0x00af, i64_trunc_f32_u               , "i64.trunc_f32_u"               )   \
	visit_op(0x00b0, i64_trunc_f64_s               , "i64.trunc_f64_s"               )   \
	visit_op(0x00b1, i64_trunc_f64_u               , "i64.trunc_f64_u"               )   \
	visit_op(0x00b2, f32_convert_i32_s             , "f32.convert_i32_s"             )   \
	visit_op(0x00b3, f32_convert_i32_u             , "f32.convert_i32_u"             )   \
	visit_op(0x00b4, f32_convert_i64_s             , "f32.convert_i64_s"             )   \
	visit_op(0x00b5, f32_convert_i64_u             , "f32.convert_i64_u"             )   \
	visit_op(0x00b6, f32_demote_f64                , "f32.demote_f64"                )   \
	visit_op(0x00b7, f64_convert_i32_s             , "f64.convert_i32_s"             )   \
	visit_op(0x00b8, f64_convert_i32_u             , "f64.convert_i32_u"             )   \
	visit_op(0x00b9, f64_convert_i64_s             , "f64.convert_i64_s"             )   \
	visit_op(0x00ba, f64_convert_i64_u             , "f64.convert_i64_u"             )   \
	visit_op(0x00bb, f64_promote_f32               , "f64.promote_f32"               )   \
	visit_op(0x00bc, i32_reinterpret_f32           , "i32.reinterpret_f32"           )   \
	visit_op(0x00bd, i64_reinterpret_f64           , "i64.reinterpret_f64"           )   \
	visit_op(0x00be, f32_reinterpret_i32           , "f32.reinterpret_i32"           )   \
	visit_op(0x00bf, f64_reinterpret_i64           , "f64.reinterpret_i64"           )   \
/* 8- and 16-bit sign extension operators                                             */ \
	visit_op(0x00c0, i32_extend8_s                 , "i32.extend8_s"                 )   \
	visit_op(0x00c1, i32_extend16_s                , "i32.extend16_s"                )   \
	visit_op(0x00c2, i64_extend8_s                 , "i64.extend8_s"                 )   \
	visit_op(0x00c3, i64_extend16_s                , "i64.extend16_s"                )   \
	visit_op(0x00c4, i64_extend32_s                , "i64.extend32_s"                )   \
/* Reference type operators                                                           */ \
	visit_op(0x00d2, ref_func                      , "ref.func"                      )   \
/* Saturating float->int truncation operators                                         */ \
	visit_op(0xfc00, i32_trunc_sat_f32_s           , "i32.trunc_sat_f32_s"           )   \
	visit_op(0xfc01, i32_trunc_sat_f32_u           , "i32.trunc_sat_f32_u"           )   \
	visit_op(0xfc02, i32_trunc_sat_f64_s           , "i32.trunc_sat_f64_s"           )   \
	visit_op(0xfc03, i32_trunc_sat_f64_u           , "i32.trunc_sat_f64_u"           )   \
	visit_op(0xfc04, i64_trunc_sat_f32_s           , "i64.trunc_sat_f32_s"           )   \
	visit_op(0xfc05, i64_trunc_sat_f32_u           , "i64.trunc_sat_f32_u"           )   \
	visit_op(0xfc06, i64_trunc_sat_f64_s           , "i64.trunc_sat_f64_s"           )   \
	visit_op(0xfc07, i64_trunc_sat_f64_u           , "i64.trunc_sat_f64_u"           )   \
/* Bulk memory/table operators                                                        */ \
	visit_op(0xfc09, data_drop                     , "data.drop"                     )   \
	visit_op(0xfc0d, elem_drop                     , "elem.drop"                     )   \
/* v128 constant                                                                      */ \
	visit_op(0xfd0c, v128_const                    , "v128.const"                    )   \
/* v128 lane manipulation                                                             */ \
	visit_op(0xfd0d, i8x16_shuffle                 , "i8x16.shuffle"                 )   \
	visit_op(0xfd0e, i8x16_swizzle                 , "i8x16.swizzle"                 )   \
	visit_op(0xfd0f, i8x16_splat                   , "i8x16.splat"                   )   \
	visit_op(0xfd10, i16x8_splat                   , "i16x8.splat"                   )   \
	visit_op(0xfd11, i32x4_splat                   , "i32x4.splat"                   )   \
	visit_op(0xfd12, i64x2_splat                   , "i64x2.splat"                   )   \
	visit_op(0xfd13, f32x4_splat                   , "f32x4.splat"                   )   \
	visit_op(0xfd14, f64x2_splat                   , "f64x2.splat"                   )   \
	visit_op(0xfd15, i8x16_extract_lane_s          , "i8x16.extract_lane_s"          )   \
	visit_op(0xfd16, i8x16_extract_lane_u          , "i8x16.extract_lane_u"          )   \
	visit_op(0xfd17, i8x16_replace_lane            , "i8x16.replace_lane"            )   \
	visit_op(0xfd18, i16x8_extract_lane_s          , "i16x8.extract_lane_s"          )   \
	visit_op(0xfd19, i16x8_extract_lane_u          , "i16x8.extract_lane_u"          )   \
	visit_op(0xfd1a, i16x8_replace_lane            , "i16x8.replace_lane"            )   \
	visit_op(0xfd1b, i32x4_extract_lane            , "i32x4.extract_lane"            )   \
	visit_op(0xfd1c, i32x4_replace_lane            , "i32x4.replace_lane"            )   \
	visit_op(0xfd1d, i64x2_extract_lane            , "i64x2.extract_lane"            )   \
	visit_op(0xfd1e, i64x2_replace_lane            , "i64x2.replace_lane"            )   \
	visit_op(0xfd1f, f32x4_extract_lane            , "f32x4.extract_lane"            )   \
	visit_op(0xfd20, f32x4_replace_lane            , "f32x4.replace_lane"            )   \
	visit_op(0xfd21, f64x2_extract_lane            , "f64x2.extract_lane"            )   \
	visit_op(0xfd22, f64x2_replace_lane            , "f64x2.replace_lane"            )   \
/* v128 comparisons                                                                   */ \
	visit_op(0xfd23, i8x16_eq                      , "i8x16.eq"                      )   \
	visit_op(0xfd24, i8x16_ne                      , "i8x16.ne"                      )   \
	visit_op(0xfd25, i8x16_lt_s                    , "i8x16.lt_s"                    )   \
	visit_op(0xfd26, i8x16_lt_u                    , "i8x16.lt_u"                    )   \
	visit_op(0xfd27, i8x16_gt_s                    , "i8x16.gt_s"                    )   \
	visit_op(0xfd28, i8x16_gt_u                    , "i8x16.gt_u"                    )   \
	visit_op(0xfd29, i8x16_le_s                    , "i8x16.le_s"                    )   \
	visit_op(0xfd2a, i8x16_le_u                    , "i8x16.le_u"                    )   \
	visit_op(0xfd2b, i8x16_ge_s                    , "i8x16.ge_s"                    )   \
	visit_op(0xfd2c, i8x16_ge_u                    , "i8x16.ge_u"                    )   \
	visit_op(0xfd2d, i16x8_eq                      , "i16x8.eq"                      )   \
	visit_op(0xfd2e, i16x8_ne                      , "i16x8.ne"                      )   \
	visit_op(0xfd2f, i16x8_lt_s                    , "i16x8.lt_s"                    )   \
	visit_op(0xfd30, i16x8_lt_u                    , "i16x8.lt_u"                    )   \
	visit_op(0xfd31, i16x8_gt_s                    , "i16x8.gt_s"                    )   \
	visit_op(0xfd32, i16x8_gt_u                    , "i16x8.gt_u"                    )   \
	visit_op(0xfd33, i16x8_le_s                    , "i16x8.le_s"                    )   \
	visit_op(0xfd34, i16x8_le_u                    , "i16x8.le_u"                    )   \
	visit_op(0xfd35, i16x8_ge_s                    , "i16x8.ge_s"                    )   \
	visit_op(0xfd36, i16x8_ge_u                    , "i16x8.ge_u"                    )   \
	visit_op(0xfd37, i32x4_eq                      , "i32x4.eq"                      )   \
	visit_op(0xfd38, i32x4_ne                      , "i32x4.ne"                      )   \
	visit_op(0xfd39, i32x4_lt_s                    , "i32x4.lt_s"                    )   \
	visit_op(0xfd3a, i32x4_lt_u                    , "i32x4.lt_u"                    )   \
	visit_op(0xfd3b, i32x4_gt_s                    , "i32x4.gt_s"                    )   \
	visit_op(0xfd3c, i32x4_gt_u                    , "i32x4.gt_u"                    )   \
	visit_op(0xfd3d, i32x4_le_s                    , "i32x4.le_s"                    )   \
	visit_op(0xfd3e, i32x4_le_u                    , "i32x4.le_u"                    )   \
	visit_op(0xfd3f, i32x4_ge_s                    , "i32x4.ge_s"                    )   \
	visit_op(0xfd40, i32x4_ge_u                    , "i32x4.ge_u"                    )   \
	visit_op(0xfd41, f32x4_eq                      , "f32x4.eq"                      )   \
	visit_op(0xfd42, f32x4_ne                      , "f32x4.ne"                      )   \
	visit_op(0xfd43, f32x4_lt                      , "f32x4.lt"                      )   \
	visit_op(0xfd44, f32x4_gt                      , "f32x4.gt"                      )   \
	visit_op(0xfd45, f32x4_le                      , "f32x4.le"                      )   \
	visit_op(0xfd46, f32x4_ge                      , "f32x4.ge"                      )   \
	visit_op(0xfd47, f64x2_eq                      , "f64x2.eq"                      )   \
	visit_op(0xfd48, f64x2_ne                      , "f64x2.ne"                      )   \
	visit_op(0xfd49, f64x2_lt                      , "f64x2.lt"                      )   \
	visit_op(0xfd4a, f64x2_gt                      , "f64x2.gt"                      )   \
	visit_op(0xfd4b, f64x2_le                      , "f64x2.le"                      )   \
	visit_op(0xfd4c, f64x2_ge                      , "f64x2.ge"                      )   \
/* v128 bitwise                                                                       */ \
	visit_op(0xfd4d, v128_not                      , "v128.not"                      )   \
	visit_op(0xfd4e, v128_and                      , "v128.and"                      )   \
	visit_op(0xfd4f, v128_andnot                   , "v128.andnot"                   )   \
	visit_op(0xfd50, v128_or                       , "v128.or"                       )   \
	visit_op(0xfd51, v128_xor                      , "v128.xor"                      )   \
	visit_op(0xfd52, v128_bitselect                , "v128.bitselect"                )   \
	visit_op(0xfd53, v128_any_true                 , "v128.any_true"                 )   \
	visit_op(0xfd5e, f32x4_demote_f64x2_zero       , "f32x4.demote_f64x2_zero"       )   \
	visit_op(0xfd5f, f64x2_promote_low_f32x4       , "f64x2.promote_low_f32x4"       )   \
/* v128 integer arithmetic                                                            */ \
	visit_op(0xfd60, i8x16_abs                     , "i8x16.abs"                     )   \
	visit_op(0xfd61, i8x16_neg                     , "i8x16.neg"                     )   \
	visit_op(0xfd62, i8x16_popcnt                  , "i8x16.popcnt"                  )   \
	visit_op(0xfd63, i8x16_all_true                , "i8x16.all_true"                )   \
	visit_op(0xfd64, i8x16_bitmask                 , "i8x16.bitmask"                 )   \
	visit_op(0xfd65, i8x16_narrow_i16x8_s          , "i8x16.narrow_i16x8_s"          )   \
	visit_op(0xfd66, i8x16_narrow_i16x8_u          , "i8x16.narrow_i16x8_u"          )   \
	visit_op(0xfd67, f32x4_ceil                    , "f32x4.ceil"                    )   \
	visit_op(0xfd68, f32x4_floor                   , "f32x4.floor"                   )   \
	visit_op(0xfd69, f32x4_trunc                   , "f32x4.trunc"                   )   \
	visit_op(0xfd6a, f32x4_nearest                 , "f32x4.nearest"                 )   \
	visit_op(0xfd6b, i8x16_shl                     , "i8x16.shl"                     )   \
	visit_op(0xfd6c, i8x16_shr_s                   , "i8x16.shr_s"                   )   \
	visit_op(0xfd6d, i8x16_shr_u                   , "i8x16.shr_u"                   )   \
	visit_op(0xfd6e, i8x16_add                     , "i8x16.add"                     )   \
	visit_op(0xfd6f, i8x16_add_sat_s               , "i8x16.add_sat_s"               )   \
	visit_op(0xfd70, i8x16_add_sat_u               , "i8x16.add_sat_u"               )   \
	visit_op(0xfd71, i8x16_sub                     , "i8x16.sub"                     )   \
	visit_op(0xfd72, i8x16_sub_sat_s               , "i8x16.sub_sat_s"               )   \
	visit_op(0xfd73, i8x16_sub_sat_u               , "i8x16.sub_sat_u"               )   \
	visit_op(0xfd74, f64x2_ceil                    , "f64x2.ceil"                    )   \
	visit_op(0xfd75, f64x2_floor                   , "f64x2.floor"                   )   \
	visit_op(0xfd76, i8x16_min_s                   , "i8x16.min_s"                   )   \
	visit_op(0xfd77, i8x16_min_u                   , "i8x16.min_u"                   )   \
	visit_op(0xfd78, i8x16_max_s                   , "i8x16.max_s"                   )   \
	visit_op(0xfd79, i8x16_max_u                   , "i8x16.max_u"                   )   \
	visit_op(0xfd7a, f64x2_trunc                   , "f64x2.trunc"                   )   \
  	visit_op(0xfd7b, i8x16_avgr_u                  , "i8x16.avgr_u"                  )   \
  	visit_op(0xfd7c, i16x8_extadd_pairwise_i8x16_s , "i16x8.extadd_pairwise_i8x16_s" )   \
	visit_op(0xfd7d, i16x8_extadd_pairwise_i8x16_u , "i16x8.extadd_pairwise_i8x16_u" )   \
	visit_op(0xfd7e, i32x4_extadd_pairwise_i16x8_s , "i32x4.extadd_pairwise_i16x8_s" )   \
	visit_op(0xfd7f, i32x4_extadd_pairwise_i16x8_u , "i32x4.extadd_pairwise_i16x8_u" )   \
	visit_op(0xfd80, i16x8_abs                     , "i16x8.abs"                     )   \
	visit_op(0xfd81, i16x8_neg                     , "i16x8.neg"                     )   \
	visit_op(0xfd82, i16x8_q15mulr_sat_s           , "i16x8.q15mulr_sat_s"           )   \
	visit_op(0xfd83, i16x8_all_true                , "i16x8.all_true"                )   \
	visit_op(0xfd84, i16x8_bitmask                 , "i16x8.bitmask"                 )   \
	visit_op(0xfd85, i16x8_narrow_i32x4_s          , "i16x8.narrow_i32x4_s"          )   \
	visit_op(0xfd86, i16x8_narrow_i32x4_u          , "i16x8.narrow_i32x4_u"          )   \
	visit_op(0xfd87, i16x8_extend_low_i8x16_s      , "i16x8.extend_low_i8x16_s"      )   \
	visit_op(0xfd88, i16x8_extend_high_i8x16_s     , "i16x8.extend_high_i8x16_s"     )   \
	visit_op(0xfd89, i16x8_extend_low_i8x16_u      , "i16x8.extend_low_i8x16_u"      )   \
	visit_op(0xfd8a, i16x8_extend_high_i8x16_u     , "i16x8.extend_high_i8x16_u"     )   \
	visit_op(0xfd8b, i16x8_shl                     , "i16x8.shl"                     )   \
	visit_op(0xfd8c, i16x8_shr_s                   , "i16x8.shr_s"                   )   \
	visit_op(0xfd8d, i16x8_shr_u                   , "i16x8.shr_u"                   )   \
	visit_op(0xfd8e, i16x8_add                     , "i16x8.add"                     )   \
	visit_op(0xfd8f, i16x8_add_sat_s               , "i16x8.add_sat_s"               )   \
	visit_op(0xfd90, i16x8_add_sat_u               , "i16x8.add_sat_u"               )   \
	visit_op(0xfd91, i16x8_sub                     , "i16x8.sub"                     )   \
	visit_op(0xfd92, i16x8_sub_sat_s               , "i16x8.sub_sat_s"               )   \
	visit_op(0xfd93, i16x8_sub_sat_u               , "i16x8.sub_sat_u"               )   \
	visit_op(0xfd94, f64x2_nearest                 , "f64x2.nearest"                 )   \
	visit_op(0xfd95, i16x8_mul                     , "i16x8.mul"                     )   \
	visit_op(0xfd96, i16x8_min_s                   , "i16x8.min_s"                   )   \
	visit_op(0xfd97, i16x8_min_u                   , "i16x8.min_u"                   )   \
	visit_op(0xfd98, i16x8_max_s                   , "i16x8.max_s"                   )   \
	visit_op(0xfd99, i16x8_max_u                   , "i16x8.max_u"                   )   \
/*  visit_op(0xfd9a, i16x8_avgr_s                  , "i16x8.avgr_s"                  )*/ \
	visit_op(0xfd9b, i16x8_avgr_u                  , "i16x8.avgr_u"                  )   \
	visit_op(0xfd9c, i16x8_extmul_low_i8x16_s      , "i16x8.extmul_low_i8x16_s"      )   \
	visit_op(0xfd9d, i16x8_extmul_high_i8x16_s     , "i16x8.extmul_high_i8x16_s"     )   \
	visit_op(0xfd9e, i16x8_extmul_low_i8x16_u      , "i16x8.extmul_low_i8x16_u"      )   \
	visit_op(0xfd9f, i16x8_extmul_high_i8x16_u     , "i16x8.extmul_high_i8x16_u"     )   \
	visit_op(0xfda0, i32x4_abs                     , "i32x4.abs"                     )   \
	visit_op(0xfda1, i32x4_neg                     , "i32x4.neg"                     )   \
/*  visit_op(0xfda2, ...                           , ...                             )*/ \
	visit_op(0xfda3, i32x4_all_true                , "i32x4.all_true"                )   \
	visit_op(0xfda4, i32x4_bitmask                 , "i32x4.bitmask"                 )   \
/*  visit_op(0xfda5, i32x4_narrow_i64x2_s          , "i32x4.narrow_i64x2_s"          )*/ \
/*  visit_op(0xfda6, i32x4_narrow_i64x2_u          , "i32x4.narrow_i64x2_u"          )*/ \
	visit_op(0xfda7, i32x4_extend_low_i16x8_s      , "i32x4.extend_low_i16x8_s"      )   \
	visit_op(0xfda8, i32x4_extend_high_i16x8_s     , "i32x4.extend_high_i16x8_s"     )   \
	visit_op(0xfda9, i32x4_extend_low_i16x8_u      , "i32x4.extend_low_i16x8_u"      )   \
	visit_op(0xfdaa, i32x4_extend_high_i16x8_u     , "i32x4.extend_high_i16x8_u"     )   \
	visit_op(0xfdab, i32x4_shl                     , "i32x4.shl"                     )   \
	visit_op(0xfdac, i32x4_shr_s                   , "i32x4.shr_s"                   )   \
	visit_op(0xfdad, i32x4_shr_u                   , "i32x4.shr_u"                   )   \
	visit_op(0xfdae, i32x4_add                     , "i32x4.add"                     )   \
/*	visit_op(0xfdaf, i32x4_add_sat_s               , "i32x4.add_sat_s"               )*/ \
/*	visit_op(0xfdb0, i32x4_add_sat_u               , "i32x4.add_sat_u"               )*/ \
	visit_op(0xfdb1, i32x4_sub                     , "i32x4.sub"                     )   \
/*	visit_op(0xfdb2, i32x4_sub_sat_s               , "i32x4.sub_sat_s"               )*/ \
/*	visit_op(0xfdb3, i32x4_sub_sat_u               , "i32x4.sub_sat_u"               )*/ \
/*  visit_op(0xfdb4, i32x4_dot_i16x8_s             , "i32x4.dot_i16x8_s"             )*/ \
	visit_op(0xfdb5, i32x4_mul                     , "i32x4.mul"                     )   \
	visit_op(0xfdb6, i32x4_min_s                   , "i32x4.min_s"                   )   \
	visit_op(0xfdb7, i32x4_min_u                   , "i32x4.min_u"                   )   \
	visit_op(0xfdb8, i32x4_max_s                   , "i32x4.max_s"                   )   \
	visit_op(0xfdb9, i32x4_max_u                   , "i32x4.max_u"                   )   \
	visit_op(0xfdba, i32x4_dot_i16x8_s             , "i32x4.dot_i16x8_s"             )   \
/*  visit_op(0xfdbb, ...                           , ...                             )*/ \
	visit_op(0xfdbc, i32x4_extmul_low_i16x8_s      , "i32x4.extmul_low_i16x8_s"      )   \
	visit_op(0xfdbd, i32x4_extmul_high_i16x8_s     , "i32x4.extmul_high_i16x8_s"     )   \
	visit_op(0xfdbe, i32x4_extmul_low_i16x8_u      , "i32x4.extmul_low_i16x8_u"      )   \
	visit_op(0xfdbf, i32x4_extmul_high_i16x8_u     , "i32x4.extmul_high_i16x8_u"     )   \
	visit_op(0xfdc0, i64x2_abs                     , "i64x2.abs"                     )   \
	visit_op(0xfdc1, i64x2_neg                     , "i64x2.neg"                     )   \
/*  visit_op(0xfdc2, ...                           , ...                             )*/ \
	visit_op(0xfdc3, i64x2_all_true                , "i64x2.all_true"                )   \
	visit_op(0xfdc4, i64x2_bitmask                 , "i64x2.bitmask"                 )   \
/*  visit_op(0xfdc5, i64x2_narrow_i128x1_s         , "i32x4.narrow_i128x1_s"         )*/ \
/*  visit_op(0xfdc6, i64x2_narrow_i128x1_u         , "i32x4.narrow_i128x1_u"         )*/ \
	visit_op(0xfdc7, i64x2_extend_low_i32x4_s      , "i64x2.extend_low_i32x4_s"      )   \
	visit_op(0xfdc8, i64x2_extend_high_i32x4_s     , "i64x2.extend_high_i32x4_s"     )   \
	visit_op(0xfdc9, i64x2_extend_low_i32x4_u      , "i64x2.extend_low_i32x4_u"      )   \
	visit_op(0xfdca, i64x2_extend_high_i32x4_u     , "i64x2.extend_high_i32x4_u"     )   \
	visit_op(0xfdcb, i64x2_shl                     , "i64x2.shl"                     )   \
	visit_op(0xfdcc, i64x2_shr_s                   , "i64x2.shr_s"                   )   \
	visit_op(0xfdcd, i64x2_shr_u                   , "i64x2.shr_u"                   )   \
	visit_op(0xfdce, i64x2_add                     , "i64x2.add"                     )   \
/*	visit_op(0xfdcf, i64x2_add_sat_s               , "i64x2.add_sat_s"               )*/ \
/*	visit_op(0xfdd0, i64x2_add_sat_u               , "i64x2.add_sat_u"               )*/ \
	visit_op(0xfdd1, i64x2_sub                     , "i64x2.sub"                     )   \
/*	visit_op(0xfdd2, i64x2_sub_sat_s               , "i64x2.sub_sat_s"               )*/ \
/*	visit_op(0xfdd3, i64x2_sub_sat_u               , "i64x2.sub_sat_u"               )*/ \
	visit_op(0xfdd5, i64x2_mul                     , "i64x2.mul"                     )   \
	visit_op(0xfdd6, i64x2_eq                      , "i64x2.eq"                      )   \
	visit_op(0xfdd7, i64x2_ne                      , "i64x2.ne"                      )   \
	visit_op(0xfdd8, i64x2_lt_s                    , "i64x2.lt_s"                    )   \
	visit_op(0xfdd9, i64x2_gt_s                    , "i64x2.gt_s"                    )   \
	visit_op(0xfdda, i64x2_le_s                    , "i64x2.le_s"                    )   \
	visit_op(0xfddb, i64x2_ge_s                    , "i64x2.ge_s"                    )   \
	visit_op(0xfddc, i64x2_extmul_low_i32x4_s      , "i64x2.extmul_low_i32x4_s"      )   \
	visit_op(0xfddd, i64x2_extmul_high_i32x4_s     , "i64x2.extmul_high_i32x4_s"     )   \
	visit_op(0xfdde, i64x2_extmul_low_i32x4_u      , "i64x2.extmul_low_i32x4_u"      )   \
	visit_op(0xfddf, i64x2_extmul_high_i32x4_u     , "i64x2.extmul_high_i32x4_u"     )   \
/* v128 floating-point arithmetic                                                     */ \
	visit_op(0xfde0, f32x4_abs                     , "f32x4.abs"                     )   \
	visit_op(0xfde1, f32x4_neg                     , "f32x4.neg"                     )   \
/*  visit_op(0xfde2, f32x4_round                   , "f32x4.round"                   )*/ \
	visit_op(0xfde3, f32x4_sqrt                    , "f32x4.sqrt"                    )   \
	visit_op(0xfde4, f32x4_add                     , "f32x4.add"                     )   \
	visit_op(0xfde5, f32x4_sub                     , "f32x4.sub"                     )   \
	visit_op(0xfde6, f32x4_mul                     , "f32x4.mul"                     )   \
	visit_op(0xfde7, f32x4_div                     , "f32x4.div"                     )   \
	visit_op(0xfde8, f32x4_min                     , "f32x4.min"                     )   \
	visit_op(0xfde9, f32x4_max                     , "f32x4.max"                     )   \
	visit_op(0xfdea, f32x4_pmin                    , "f32x4.pmin"                    )   \
	visit_op(0xfdeb, f32x4_pmax                    , "f32x4.pmax"                    )   \
	visit_op(0xfdec, f64x2_abs                     , "f64x2.abs"                     )   \
	visit_op(0xfded, f64x2_neg                     , "f64x2.neg"                     )   \
/*  visit_op(0xfdee, f64x2_round                   , "f64x2.round"                   )*/ \
	visit_op(0xfdef, f64x2_sqrt                    , "f64x2.sqrt"                    )   \
	visit_op(0xfdf0, f64x2_add                     , "f64x2.add"                     )   \
	visit_op(0xfdf1, f64x2_sub                     , "f64x2.sub"                     )   \
	visit_op(0xfdf2, f64x2_mul                     , "f64x2.mul"                     )   \
	visit_op(0xfdf3, f64x2_div                     , "f64x2.div"                     )   \
	visit_op(0xfdf4, f64x2_min                     , "f64x2.min"                     )   \
	visit_op(0xfdf5, f64x2_max                     , "f64x2.max"                     )   \
	visit_op(0xfdf6, f64x2_pmin                    , "f64x2.pmin"                    )   \
	visit_op(0xfdf7, f64x2_pmax                    , "f64x2.pmax"                    )   \
/* v128 conversions                                                                   */ \
	visit_op(0xfdf8, i32x4_trunc_sat_f32x4_s       , "i32x4.trunc_sat_f32x4_s"       )   \
	visit_op(0xfdf9, i32x4_trunc_sat_f32x4_u       , "i32x4.trunc_sat_f32x4_u"       )   \
	visit_op(0xfdfa, f32x4_convert_i32x4_s         , "f32x4.convert_i32x4_s"         )   \
	visit_op(0xfdfb, f32x4_convert_i32x4_u         , "f32x4.convert_i32x4_u"         )   \
	visit_op(0xfdfc, i32x4_trunc_sat_f64x2_s_zero  , "i32x4.trunc_sat_f64x2_s_zero"  )   \
	visit_op(0xfdfd, i32x4_trunc_sat_f64x2_u_zero  , "i32x4.trunc_sat_f64x2_u_zero"  )   \
	visit_op(0xfdfe, f64x2_convert_low_i32x4_s     , "f64x2.convert_low_i32x4_s"     )   \
	visit_op(0xfdff, f64x2_convert_low_i32x4_u     , "f64x2.convert_low_i32x4_u"     )   \
/* Atomic fence                                                                       */ \
	visit_op(0xfe03, atomic_fence                  , "atomic.fence"                  )