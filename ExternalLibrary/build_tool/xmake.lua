target("brotli")
    set_kind("static")
    
    add_files("brotli/c/common/**.c")
    add_files("brotli/c/dec/**.c")
    add_files("brotli/c/enc/**.c")

    add_includedirs("brotli/c/include", {public = true})

target("cJSON")
    set_kind("static")

    add_files("cJSON/**.c")

    add_includedirs("cJSON/**.h")