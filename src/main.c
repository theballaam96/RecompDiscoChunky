#include "modding.h"
#include "ultra64.h"
#include "enums.h"
#include "common_structs.h"

typedef struct {
    Actor* unk0;
    s32 unk4;
} GlobalASMStruct53;

extern u16 D_global_asm_807FBB34;
extern GlobalASMStruct53 D_global_asm_807FB930[];
extern s32 D_global_asm_807FBB68;
extern Maps current_map;
s16 playSoundAtPosition(f32 x, f32 y, f32 z, s16 arg3, u8 arg4, s16 arg5, u8 arg6, u8 arg7, f32 arg8, u8 arg9);
void playSong(MUSIC_E arg0, f32 arg1);

RECOMP_PATCH void func_global_asm_806CF2EC(s32 *arg0, s32 *arg1) {
    s32 var_v0;
    u16 temp_v1;

    *arg0 = 2;
    *arg1 = 3;
}

RECOMP_PATCH void func_global_asm_806F1250(Actor *arg0) {
    PlayerAdditionalActorData *PaaD = arg0->PaaD;
    if (arg0->unk58 == ACTOR_CHUNKY) {
        // func_global_asm_80613194(arg0, 0x10);
        playSoundAtPosition(arg0->x_position, arg0->y_position, arg0->z_position, 0x2E2, 0xFF, 0xA0, 0, 0x32, 0.0f, 0);
        if ((D_global_asm_807FBB68 & 2) == 0) {
            // Gorilla Gone
            playSong(MUSIC_108_GORILLA_GONE, 1.0f);
        }
    }
    PaaD->unk1F0 |= 0x40;
}

void applyGorillaGoneMask(Actor *kong) {
    PlayerAdditionalActorData *PaaD;

    if ((kong->interactable & 1) == 0) return;
    PaaD = kong->PaaD;
    if ((PaaD->unk1F0 & 0x40) == 0) return;
    if (kong->unk58 != ACTOR_CHUNKY) return;
    kong->object_properties_bitfield &= ~0x8000;
    kong->shadow_opacity -= 4;
    if (kong->shadow_opacity < 100) {
        kong->shadow_opacity = 100;
    }
}

typedef struct {
    EnemySpawner *unk0;
    Actor *unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u16 unk18; // used
} AAD_global_asm_80729B00;

RECOMP_CALLBACK("*", dk64recomp_every_frame)
void discoEveryFrame(void) {
    s32 i;
    Actor *ac;
    AAD_global_asm_80729B00 *aad;

    for (i = 0; i < D_global_asm_807FBB34; i++) {
        ac = D_global_asm_807FB930[i].unk0;
        if (ac) {
            applyGorillaGoneMask(ac);
            if ((ac->unk58 == ACTOR_CUTSCENE_CHUNKY) && (current_map == MAP_DK_RAP)) {
                aad = ac->AAD_as_array[0];
                if (aad->unk0) {
                    aad->unk0->properties_bitfield &= ~0x1000;
                }
            }
        }
    }
}

extern u16 size_instrument_disco;
extern u8 instrument_disco[];
extern u16 size_regular_disco;
extern u8 regular_disco[];
extern u16 size_regular_chunky;
extern u8 regular_chunky[];

RECOMP_CALLBACK("*", recomp_on_asset_file_load_override)
void discoLoader(s32 tableIndex, s32 fileIndex, u8 **outputFile, s32 *file_size) {
    if (tableIndex != 5) return;
    if (fileIndex == 0xB) {
        // Base outfit
        *outputFile = (u8*)&regular_disco[0];
        *file_size = size_regular_disco;
    } else if (fileIndex == 0xC) {
        // Instrument
        *outputFile = (u8*)&instrument_disco[0];
        *file_size = size_instrument_disco;
    } else if (fileIndex == 0xD) {
        // Alternate
        *outputFile = (u8*)&regular_chunky[0];
        *file_size = size_regular_chunky;
    }
}