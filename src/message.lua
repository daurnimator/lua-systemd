--[[
This module gives aliases for some common well known message ids.
The aliases are taken from `sd_message.h`.

Information about these message ids can be found in the system catalog in /usr/lib/systemd/catalog/
]]

local id128 = require "systemd.id128"

local messages = {}

messages.JOURNAL_START         = id128.from_string "f77379a8490b408bbe5f6940505a777b"
messages.JOURNAL_STOP          = id128.from_string "d93fb3c9c24d451a97cea615ce59c00b"
messages.JOURNAL_DROPPED       = id128.from_string "a596d6fe7bfa4994828e72309e95d61e"
messages.JOURNAL_MISSED        = id128.from_string "e9bf28e6e834481bb6f48f548ad13606"
messages.JOURNAL_USAGE         = id128.from_string "ec387f577b844b8fa948f33cad9a75e6"

messages.COREDUMP              = id128.from_string "fc2e22bc6ee647b6b90729ab34a250b1"

messages.SESSION_START         = id128.from_string "8d45620c1a4348dbb17410da57c60c66"
messages.SESSION_STOP          = id128.from_string "3354939424b4456d9802ca8333ed424a"
messages.SEAT_START            = id128.from_string "fcbefc5da23d428093f97c82a9290f7b"
messages.SEAT_STOP             = id128.from_string "e7852bfe46784ed0accde04bc864c2d5"
messages.MACHINE_START         = id128.from_string "24d8d4452573402496068381a6312df2"
messages.MACHINE_STOP          = id128.from_string "58432bd3bace477cb514b56381b8a758"

messages.TIME_CHANGE           = id128.from_string "c7a787079b354eaaa9e77b371893cd27"
messages.TIMEZONE_CHANGE       = id128.from_string "45f82f4aef7a4bbf942ce861d1f20990"

messages.STARTUP_FINISHED      = id128.from_string "b07a249cd024414a82dd00cd181378ff"

messages.SLEEP_START           = id128.from_string "6bbd95ee977941e497c48be27c254128"
messages.SLEEP_STOP            = id128.from_string "8811e6df2a8e40f58a94cea26f8ebf14"

messages.SHUTDOWN              = id128.from_string "98268866d1d54a499c4e98921d93bc40"

messages.UNIT_STARTING         = id128.from_string "7d4958e842da4a758f6c1cdc7b36dcc5"
messages.UNIT_STARTED          = id128.from_string "39f53479d3a045ac8e11786248231fbf"
messages.UNIT_STOPPING         = id128.from_string "de5b426a63be47a7b6ac3eaac82e2f6f"
messages.UNIT_STOPPED          = id128.from_string "9d1aaa27d60140bd96365438aad20286"
messages.UNIT_FAILED           = id128.from_string "be02cf6855d2428ba40df7e9d022f03d"
messages.UNIT_RELOADING        = id128.from_string "d34d037fff1847e6ae669a370e694725"
messages.UNIT_RELOADED         = id128.from_string "7b05ebc668384222baa8881179cfda54"

messages.SPAWN_FAILED          = id128.from_string "641257651c1b4ec9a8624d7a40a9e1e7"

messages.FORWARD_SYSLOG_MISSED = id128.from_string "0027229ca0644181a76c4e92458afa2e"

messages.OVERMOUNTING          = id128.from_string "1dee0369c7fc4736b7099b38ecb46ee7"

messages.LID_OPENED            = id128.from_string "b72ea4a2881545a0b50e200e55b9b06f"
messages.LID_CLOSED            = id128.from_string "b72ea4a2881545a0b50e200e55b9b070"
messages.SYSTEM_DOCKED         = id128.from_string "f5f416b862074b28927a48c3ba7d51ff"
messages.SYSTEM_UNDOCKED       = id128.from_string "51e171bd585248568110144c517cca53"
messages.POWER_KEY             = id128.from_string "b72ea4a2881545a0b50e200e55b9b071"
messages.SUSPEND_KEY           = id128.from_string "b72ea4a2881545a0b50e200e55b9b072"
messages.HIBERNATE_KEY         = id128.from_string "b72ea4a2881545a0b50e200e55b9b073"

messages.CONFIG_ERROR          = id128.from_string "c772d24e9a884cbeb9ea12625c306c01"

messages.BOOTCHART             = id128.from_string "9f26aa562cf440c2b16c773d0479b518"

return messages
