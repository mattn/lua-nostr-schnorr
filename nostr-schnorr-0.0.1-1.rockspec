package = "nostr-schnorr"
version = "0.0.1-1"
source = {
   url = "git+https://github.com/mattn/lua-nostr-schnorr.git",
   tag = "v0.0.1",
}
description = {
   summary = "Nostr Schnorr signature verification using libsecp256k1",
   detailed = "A minimal wrapper for libsecp256k1 schnorrsig_verify (BIP-340).",
   license = "MIT",
}
dependencies = {
   "lua >= 5.1",
}
build = {
   type = "builtin",
   modules = {
      nostr_schnorr = {
         sources = {
            "src/nostr_schnorr.c",
         },
         libraries = {"secp256k1"},
         incdirs = {"/usr/include", "/usr/local/include"},
         libdirs = {"/usr/lib", "/usr/local/lib"},
      }
   }
}
