target("brotli")
    set_kind("static")
    
    add_files("brotli/c/common/**.c")
    add_files("brotli/c/dec/**.c")
    add_files("brotli/c/enc/**.c")

    add_headerfiles("brotli/c/include/**.h", {public = true})

    add_includedirs("brotli/c/include", {public = true})