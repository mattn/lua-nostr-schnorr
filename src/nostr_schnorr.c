// nostr_schnorr.c
// Fast Nostr Schnorr signature verifier for Lua/LuaJIT
// Build: gcc -O3 -fPIC -shared -o nostr_schnorr.so nostr_schnorr.c -lsecp256k1

#include <lua.h>
#include <lauxlib.h>
#include <secp256k1.h>
#include <secp256k1_schnorrsig.h>

static secp256k1_context *ctx = NULL;

/* Create context on first use (verify-only, no private keys needed) */
static void ensure_context(void) {
    if (!ctx) {
        ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);
    }
}

/* Fast hex → binary conversion without sscanf */
static inline int hex_to_byte(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static int hex2bin(const char *hex, size_t hexlen, unsigned char *out, size_t outlen) {
    if (hexlen != outlen * 2) return 0;  // Wrong length

    for (size_t i = 0; i < outlen; i++) {
        int hi = hex_to_byte(hex[i*2]);
        int lo = hex_to_byte(hex[i*2 + 1]);
        if (hi < 0 || lo < 0) return 0;   // Invalid hex char
        out[i] = (hi << 4) | lo;
    }
    return 1;
}

/* Lua: verify(sig_hex: string, msg_hex: string, pubkey_hex: string) -> boolean */
static int l_verify(lua_State *L) {
    size_t sig_len, msg_len, pub_len;
    const char *sig_hex = luaL_checklstring(L, 1, &sig_len);   // 128 hex chars
    const char *msg_hex = luaL_checklstring(L, 2, &msg_len);   // 64 hex chars (event.id)
    const char *pub_hex = luaL_checklstring(L, 3, &pub_len);   // 64 hex chars (x-only pubkey)

    unsigned char sig[64], msg[32], pub[32];

    // Convert hex strings to binary
    if (!hex2bin(sig_hex, sig_len, sig, 64) ||
        !hex2bin(msg_hex, msg_len, msg, 32) ||
        !hex2bin(pub_hex, pub_len, pub, 32)) {
        lua_pushboolean(L, 0);
        return 1;
    }

    ensure_context();

    // Parse x-only public key (32-byte raw format used in Nostr)
    secp256k1_xonly_pubkey xonly_pk;
    if (!secp256k1_xonly_pubkey_parse(ctx, &xonly_pk, pub)) {
        lua_pushboolean(L, 0);
        return 1;
    }

    // Verify BIP-340 Schnorr signature
    int verified = secp256k1_schnorrsig_verify(ctx, sig, msg, sizeof(msg), &xonly_pk);

    lua_pushboolean(L, verified);
    return 1;
}

static const struct luaL_Reg module_funcs[] = {
    {"verify", l_verify},
    {NULL, NULL}
};

/* Module entry point */
int luaopen_nostr_schnorr(lua_State *L) {
    ensure_context();
    luaL_newlib(L, module_funcs);
    return 1;
}
