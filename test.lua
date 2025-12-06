local secp = require("nostr_schnorr")

--{
--  "kind": 1,
--  "id": "008d31f83bc62331d7c23917d58df548c7cbe95df5744115b6c7a355f31a85c9",
--  "pubkey": "79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798",
--  "created_at": 1765020302,
--  "tags": [],
--  "content": "テスト",
--  "sig": "395b7be5b2b29c6e5b20b2bc394d26199cbdca539379a0dfbd83ebcef34431c0bb124af62abd0e6c135a5ea5f870b2e998fb59008ac04c978cd4fecc00ac5c93"
--}

local sig = "395b7be5b2b29c6e5b20b2bc394d26199cbdca539379a0dfbd83ebcef34431c0bb124af62abd0e6c135a5ea5f870b2e998fb59008ac04c978cd4fecc00ac5c93"
local id  = "008d31f83bc62331d7c23917d58df548c7cbe95df5744115b6c7a355f31a85c9"
local pub = "79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"

local ok = secp.verify(sig, id, pub)
print("verify:", ok)
