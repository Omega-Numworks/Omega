SFLAGS += -Idiscord-rpc/discord-rpc/include -Idiscord-rpc/discord-rpc/src -Idiscord-rpc -Idiscord-rpc/rapidjson/include

discord_rpc_src += $(addprefix discord-rpc/discord-rpc/src/, \
	discord_rpc.cpp \
	rpc_connection.cpp \
	serialization.cpp \
)

ifeq ($(HOST),windows)
discord_rpc_src += discord-rpc/discord-rpc/src/connection_win.cpp
else
discord_rpc_src += discord-rpc/discord-rpc/src/connection_unix.cpp
endif

ifeq ($(HOST),windows)
discord_rpc_src += discord-rpc/discord-rpc/src/discord_register_win.cpp
else
ifeq ($(HOST),macos)
discord_rpc_src += discord-rpc/discord-rpc/src/discord_register_osx.macos
else
discord_rpc_src += discord-rpc/discord-rpc/src/discord_register_linux.cpp
endif
endif

discord_rpc_src += $(addprefix discord-rpc/, \
	discord.cpp \
)
